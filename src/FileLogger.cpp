#include "FileLogger.h"
#include <iostream>

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

int	FileLogger::log(char* text)
{
    int ret = -1;
    FILE* logFile = NULL;
    do
    {
        std::lock_guard<std::mutex> lock(this->log_Locker);
        fopen_s(&logFile, this->log_Path, "a+");
        if (logFile == NULL)
        {
            std::cout << "[FILE INI] error while opening file\n";
            break;
        }
        if (fprintf(logFile, text) < 0)
        {
            std::cout << "[FILE WRITING] error while writing data to file\n";
            ret = -1;
            break;
        }
        std::cout << "[FILE WRITING] write to file successfully\n";
        ret = 0;
    } while (0);
    if (logFile != NULL)
    {
        fclose(logFile);
    }
    return ret;
}

int FileLogger::init(log_config_t* config)
{
    int ret = -1;
    std::time_t curTime;
    struct tm curTm;
    size_t size = FILE_PATH_LEN_MAX;
    size_t len = 0;
    int tmpLen = 0;
    struct stat info;
    FILE* logFile = NULL;
    do
    {
        tmpLen = snprintf(this->log_Path, size, "%s", (config->cfgs.file.log_dir == NULL ? "./" : config->cfgs.file.log_dir));
        if (tmpLen <= 0)
        {
            break;
        }
        ret = stat(this->log_Path, &info);
        if ((ret != 0)
            || ((info.st_mode & S_IFDIR) == 0))
        {
            std::cout << "[FILE init] directory is not existing\n";
            break;
        }
        len += tmpLen;
        curTime = std::time(nullptr);
        localtime_s(&curTm, &curTime);
        len += std::strftime(this->log_Path + len, size - len, "%Y%m%d.txt", &curTm);
        fopen_s(&logFile, this->log_Path, "a");
        if (logFile == NULL)
        {
            std::cout << "[ini] error while opening file\n";
            break;
        }
        ret = 0;
    } while (0);
    if (logFile != NULL)
    {
        fclose(logFile);
    }
    if (ret != 0)
    {
        std::cout << "[ini] error ret=%d\n";
        this->deinit();
    }
    return ret;
}
