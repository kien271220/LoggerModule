cmake_minimum_required(VERSION 3.10)
project(LoggerModule)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include all headers in src/ and tools/

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/tools/DataManager
    ${PROJECT_SOURCE_DIR}/tools/EventManager
    ${PROJECT_SOURCE_DIR}/tools/TaskManager
)

# Define all source files
set(SOURCES
    # src/
    src/ConsoleLogger.cpp
    src/FileLogger.cpp
    src/LogTask.cpp
    src/Logger.cpp
    src/LoggerGenerate.cpp
    src/LoggerManager.cpp
    src/main.cpp

    # tools/DataManager/
    tools/DataManager/DataLinkedList.cpp
    tools/DataManager/DataManager.cpp

    # tools/EventManager/
    tools/EventManager/EventManager.cpp

    # tools/TaskManager/
    tools/TaskManager/TaskManager.cpp
)

# Create executable
add_executable(main ${SOURCES})
