# LoggerModule

## 📁 Project Structure

``` bash
LoggerModule/
├── src/
│   ├── ConsoleLogger.cpp
│   └── ConsoleLogger.h
│   └── FileLogger.cpp
│   └── FileLogger.h
│   └── LogTask.cpp
│   └── LogTask.h
│   └── Logger.cpp
│   └── Logger.h
│   └── LoggerGenerate.cpp
│   └── LoggerGenerate.h
│   └── LoggerManager.cpp
│   └── LoggerManager.h
│   └── main.cpp
├── tools/
│   ├── DataManager/
│        ├── DataLinkedList.cpp
│        └── DataLinkedList.h
│        └── DataManager.cpp
│        └── DataManager.h
│   ├── EventManager/
│        ├── EventManager.cpp
│        └── EventManager.h
│   ├── TaskManager/
│        ├── TaskManager.cpp
│        └── TaskManager.h
```

# 1. Build Instruction (CMake)
- git clone https://github.com/kien271220/LoggerModule.git
- cd LoggerModule
- mkdir build && cd build
- cmake ..
- cmake --build .

# 2. Run
- For Windows: Navigate to LoggerModule/build/Debug/ and run main.exe
- For Linux: Navigate to build/Debug/ and run main
