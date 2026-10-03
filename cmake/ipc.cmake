# Define source files for the IPC module
set(IPC_SOURCES
    ${CMAKE_SOURCE_DIR}/source/ipc.c
    ${CMAKE_SOURCE_DIR}/source/ipc_cfg.c
    ${CMAKE_SOURCE_DIR}/source/ipc_port.c
)

# Add the defined source files to the ipc library target
target_sources(ipc PRIVATE ${IPC_SOURCES})
