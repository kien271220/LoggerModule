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
---

## ⚙️ Build Instructions (CMake)

### Requirements:
- CMake ≥ 3.5
- C++11 compatible compiler (e.g., GCC, Clang, MSVC)

### Steps:

```bash
git clone https://github.com/kien271220/LoggerModule.git
cd LoggerModule
mkdir build && cd build
cmake ..
cmake --build .

---

## 🚀 Run:
Navigate to LoggerModule/build/
Run main.exe

