add_executable(daemon
    main.cpp
    src/SystemMonitor.cpp
    headers/SystemMonitor.h
)

target_include_directories(daemon PRIVATE headers)

# Install daemon binary as sysmond in /usr/local/bin
install(TARGETS daemon DESTINATION /usr/local/bin RENAME sysmond)