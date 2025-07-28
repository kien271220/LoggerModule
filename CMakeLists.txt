cmake_minimum_required(VERSION 3.10)
project(LoggerModule)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/tools/DataManager
    ${PROJECT_SOURCE_DIR}/tools/EventManager
    ${PROJECT_SOURCE_DIR}/tools/TaskManager
)

# Source files
set(SOURCES
    src/ConsoleLogger.cpp
    src/FileLogger.cpp
    src/LogTask.cpp
    src/Logger.cpp
    src/LoggerGenerate.cpp
    src/LoggerManager.cpp
    src/main.cpp

    tools/DataManager/DataLinkedList.cpp
    tools/DataManager/DataManager.cpp
    tools/EventManager/EventManager.cpp
    tools/TaskManager/TaskManager.cpp
)

# Determine platform
if(ANDROID)
    message(STATUS "Building for Android...")
    # Build as a shared library for Android
    add_library(logger SHARED ${SOURCES})
elseif(APPLE)
    message(STATUS "Building for macOS...")
    add_executable(main ${SOURCES})
elseif(UNIX)
    message(STATUS "Building for Linux...")
    add_executable(main ${SOURCES})
elseif(WIN32)
    message(STATUS "Building for Windows...")
    add_executable(main ${SOURCES})
else()
    message(FATAL_ERROR "Unknown or unsupported platform")
endif()
