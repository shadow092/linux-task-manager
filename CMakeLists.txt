cmake_minimum_required(VERSION 3.20)
project(SysMonProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add subdirectories
add_subdirectory(daemon)
add_subdirectory(widgit)

# Install KDE application launcher desktop file
install(FILES com.shadow.sysmon.desktop DESTINATION /usr/share/applications)