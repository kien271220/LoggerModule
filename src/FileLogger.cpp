/////////////////////////////////////////////////
// INCLUDE
#include "FileLogger.h"
#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include <ctime>
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS
inline void localtime_safe(const time_t* timep, struct tm* result)
{
#ifdef _WIN32
    localtime_s(result, timep);
#else
    localtime_r(timep, result);
#endif
}
/////////////////////////////////////////////////
// TYPE DEF

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTATION

FileLogger::FileLogger()
    : Logger(LOG_TYPE_FILE),
    log_Path()
{
    memset(this->log_Path, 0, FILE_PATH_LEN_MAX);
}

FileLogger::~FileLogger()
{
    this->deinit();
}

void FileLogger::deinit(void)
{
    memset(this->log_Path, 0, FILE_PATH_LEN_MAX);
}

int FileLogger::log(char* text)
{
    int ret = -1;
    FILE* logFile = nullptr;

    {
        std::lock_guard<std::mutex> lock(this->log_Locker);

        logFile = fopen(this->log_Path, "a");
        if (!logFile)
        {
            std::cerr << "[FILE INI] Error opening file: " << this->log_Path << "\n";
            return ret;
        }

        if (fprintf(logFile, "%s", text) < 0)
        {
            std::cerr << "[FILE WRITING] Error writing data to file\n";
        }
        else
        {
            std::cout << "[FILE WRITING] Write to file successfully\n";
            ret = 0;
        }

        fclose(logFile);
    }

    return ret;
}


int FileLogger::init(log_config_t* config)
{
    int ret = -1;
    std::time_t curTime;
    struct tm curTm {};
    size_t size = FILE_PATH_LEN_MAX;
    int tmpLen = 0;
    struct stat info {};
    FILE* logFile = nullptr;

    do
    {
        const char* dir = (config->cfgs.file.log_dir == nullptr) ? "./" : config->cfgs.file.log_dir;

        tmpLen = snprintf(this->log_Path, size, "%s", dir);
        if (tmpLen <= 0 || tmpLen >= FILE_PATH_LEN_MAX)
        {
            std::cerr << "[FILE init] Error: log_path too long or invalid\n";
            break;
        }

        ret = stat(this->log_Path, &info);
        if (ret != 0 || !(info.st_mode & S_IFDIR))
        {
            std::cerr << "[FILE init] Directory does not exist: " << this->log_Path << "\n";
            break;
        }

        size_t len = strlen(this->log_Path);
        if (this->log_Path[len - 1] != '/' && this->log_Path[len - 1] != '\\') {
            this->log_Path[len++] = '/';
        }

        curTime = std::time(nullptr);

        #ifdef _WIN32
            localtime_s(&curTm, &curTime);
        #else
            localtime_r(&curTime, &curTm);
        #endif
        
        size_t timeLen = std::strftime(this->log_Path + len, size - len, "%Y%m%d.txt", &curTm);

        if (timeLen == 0)
        {
            std::cerr << "[FILE init] Error formatting date\n";
            break;
        }

        logFile = fopen(this->log_Path, "a");
        if (!logFile)
        {
            std::cerr << "[init] Error opening file: " << this->log_Path << "\n";
            break;
        }

        ret = 0;

    } while (0);

    if (logFile != nullptr)
    {
        fclose(logFile);
    }

    if (ret != 0)
    {
        std::cerr << "[init] Initialization failed. Cleaning up...\n";
        this->deinit();
    }

    return ret;
}

