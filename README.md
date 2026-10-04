# IPC - Inter-Platform Communication

A lightweight, lock-free IPC driver for dual-core MCU. It enables high-throughput data exchange between cores using shared memory and interrupt notification.

## How It Works

Each direction uses an independent single-producer/single-consumer (SPSC)
ring buffer in shared memory:

```text
Core 1  --- queue[0] --> Core 2
Core 1  <-- queue[1] --- Core 2

Each shared memory region contains two flags and queues:
+----------+----------+----------+----------+
| ready[0] | ready[1] | queue[0] | queue[1] |
+----------+----------+----------+----------+
                      /          \

Each queue contains two indexes and a fixed number of slots:
+-------------+------------+----------------------+
| write_index | read_index | slot[IPC_SLOT_COUNT] |
+-------------+------------+----------------------+
                           /                      \

Each slot contains a payload length and a fixed-size data buffer:
+-----+---------------------+
| len | data[IPC_SLOT_SIZE] |
+-----+---------------------+
```

The queue uses separate read and write indexes. Data is exchanged without
locks; memory barriers ensure that payloads are visible before indexes are
updated.

`ipc_notify()` only notifies the remote core that data may be available. It does not
carry the payload. The receiver must process all available entries.

## Configuration

The message size is configured by `IPC_SLOT_SIZE`:

```c
#define IPC_SLOT_COUNT 8U
#define IPC_SLOT_SIZE  32U
```

Change `IPC_SLOT_SIZE` to select the maximum payload size of each message.
The same configuration must be used by both cores, followed by a clean rebuild
of both images.

`IPC_SLOT_COUNT` controls the number of slots. One slot is reserved to
distinguish between full and empty states.

## Usage

The following example demonstrates bidirectional communication between two cores using shared memory.

### Core 1: Send Data
```c
ipc_driver_t ipcd;
ipc_slot_t *slot;

/* 1. Initialize and wait for remote core */
ipc_init(&ipcd, &ipc_config_core1);
while (!ipc_is_ready(&ipcd)) {}

/* 2. Allocate slot directly in shared memory */
if (ipc_slot_allocate(&ipcd, &slot) == true) {
    /* 3. Write data directly to the slot */
    memcpy(slot->data, tx_data, TX_DATA_SIZE);
    slot->len = TX_DATA_SIZE;
    
    /* 4. Commit and notify remote core */
    ipc_slot_send(&ipcd, slot);
    ipc_notify(&ipcd);
}
```

### Core 2: Receive Data
```c
ipc_driver_t ipcd;
ipc_slot_t *slot;

/* 1. Initialize and wait for remote core */
ipc_init(&ipcd, &ipc_config_core2);
while (!ipc_is_ready(&ipcd)) {}

/* 2. Process all available slots (typically in an IRQ handler or task loop) */
while (ipc_slot_receive(&ipcd, &slot) == true) {
    /* 3. Read data from shared memory */
    process_data(slot->data, slot->len);
    
    /* 4. IMPORTANT: Release the slot back to the pool */
    ipc_slot_release(&ipcd, slot);
}
```

## Important Notes

- The shared memory must be accessible by both cores and mapped to `.ipc_shm`.
- Shared memory should be `non-cacheable`.
- Call `ipc_notify()` only after committing data to the queue.
- The driver is non-blocking: send fails when full, receive fails when empty.
- Always check API return values and release every acquired zero-copy slot.

## Limitations

The driver does not provide multi-producer/multi-consumer synchronization,
message priorities, retransmission, timeouts, or flow control.
