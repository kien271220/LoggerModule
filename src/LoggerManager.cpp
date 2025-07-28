/////////////////////////////////////////////////
// INCLUDE
#include "LoggerManager.h"
#include <iostream>
#include <cstdarg>
#include <cstring>
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

LoggerManager& LoggerManager::getInstance()
{
    static LoggerManager instance;
    return instance;
}

LoggerManager::LoggerManager() :
    logger_List(),
    log_Task(),
    data_Mgr(),
    log_Level(LOG_LEVEL_INFO)
{

}

LoggerManager::~LoggerManager()
{
    this->deinit();
    std::cout << "[Main] Main thread stops\n";
}


int LoggerManager::init(log_mgr_configs_t* configData)
{
    std::cout << "[Main] Main thread is starting\n";

    int ret = -1;
    uint8_t idx = 0;
    log_configs_t* logCfgs = &configData->log_cfgs;
    do {
        if (this->isValid() != false)
        {
            ret = 0;
            break;
        }
        this->log_Level = configData->log_cfgs.level;
        ret = this->data_Mgr.init(configData->data_count, configData->data_size);
        if (ret != 0)
        {
            ret = 1;
            break;
        }

        memset(this->logger_List, 0, sizeof(Logger*) * CF_LOGGER_PROCESSOR_NUM_MAX);
        for (idx = 0; idx < CF_LOGGER_PROCESSOR_NUM_MAX; idx++)
        {
            if (configData->log_cfgs.configs[idx].type == log_type_t::LOG_TYPE_NONE)
            {
                continue;
            }

            this->logger_List[idx] = generate(logCfgs->configs[idx].type);
            if (this->logger_List[idx] == NULL)
            {
                break;
            }
            ret = this->logger_List[idx]->init(&logCfgs->configs[idx]);
            if (ret != 0)
            {
                break;
            }
        }

        if (idx < CF_LOGGER_PROCESSOR_NUM_MAX)
        {
            break;
        }

        ret = this->log_Task.init(this->logger_List, &configData->task_cfg);
        if (ret != 0)
        {
            ret = 1;
            break;
        }

        ret = this->log_Task.start();
        if (ret != 0)
        {
            ret = 1;
            break;
        }
        ret = 0;
    } while (0);
    if (ret != 0)
    {
        this->deinit();
    }
    //SYS_LOGI(TAG, "[ini] ended ret=%d\n", ret);
    return ret;
}

void LoggerManager::deinit()
{
    this->log_Task.deinit();
    for (uint8_t idx = 0; idx < CF_LOGGER_PROCESSOR_NUM_MAX; idx++)
    {
        if (this->logger_List[idx] == NULL)
        {
            continue;
        }
        this->logger_List[idx]->deinit();
        destroy(&this->logger_List[idx]);
        this->logger_List[idx] = NULL;
    }
    memset(this->logger_List, 0, sizeof(Logger*) * CF_LOGGER_PROCESSOR_NUM_MAX);
    this->data_Mgr.deinit();
    this->log_Level = LOG_LEVEL_INFO;
    std::cout << "[Main] Main thread deinitializing\n";
}

int LoggerManager::log(log_level_t level, uint8_t logType, const char* fmt, ...)
{
    int ret = -1;
    std::va_list args;
    va_start(args, fmt);
    ret = this->log(level, logType, fmt, args);
    va_end(args);
    return ret;
}

int LoggerManager::logE(uint8_t logType, const char* fmt, ...)
{
    int ret = -1;
    std::va_list args;
    va_start(args, fmt);
    ret = this->log(LOG_LEVEL_ERROR, logType, fmt, args);
    va_end(args);
    return ret;
}

int LoggerManager::logW(uint8_t logType, const char* fmt, ...)
{
    int ret = -1;
    std::va_list args;
    va_start(args, fmt);
    ret = this->log(LOG_LEVEL_WARN, logType, fmt, args);
    va_end(args);
    return ret;
}

int LoggerManager::logI(uint8_t logType, const char* fmt, ...)
{
    int ret = -1;
    std::va_list args;
    va_start(args, fmt);
    ret = this->log(LOG_LEVEL_INFO, logType, fmt, args);
    va_end(args);
    return ret;
}

int LoggerManager::logD(uint8_t logType, const char* fmt, ...)
{
    int ret = -1;
    std::va_list args;
    va_start(args, fmt);
    ret = this->log(LOG_LEVEL_DEBUG, logType, fmt, args);
    va_end(args);
    return ret;
}

int LoggerManager::logV(uint8_t logType, const char* fmt, ...)
{
    int ret = -1;
    std::va_list args;
    va_start(args, fmt);
    ret = this->log(LOG_LEVEL_VERBOSE, logType, fmt, args);
    va_end(args);
    return ret;
}

int LoggerManager::log(log_level_t level, uint8_t logType, const char* fmt, va_list args)
{
    int ret = -1;

    //Start putting data into data_item_t
    data_item_t dataItem = data_item_t();
    std::time_t curTime;
    struct tm curTm;
    char* ptr = NULL;
    size_t sz = 0;
    size_t len = 0;
    int tmpLen = 0;
    do
    {
        if (level > this->log_Level)
        {
            break;
        }
        dataItem = this->data_Mgr.get(); // take container to store data
        if (dataItem.isValid() == false)
        {
            break;
        }
        ptr = (char*)dataItem.buff;
        sz = dataItem.size;
        curTime = std::time(nullptr);
        #ifdef _WIN32
            localtime_s(&curTm, &curTime);
        #else
            localtime_r(&curTime, &curTm);
        #endif
        len = std::strftime(ptr, sz, "%Y/%m/%d %H:%M:%S ", &curTm);
        tmpLen = snprintf(ptr + len, sz - len, "%s ", LoggerManager::logLevel2String(level));
        if (tmpLen <= 0)
        {
            break;
        }
        len += tmpLen;

        tmpLen = vsnprintf(ptr + len, sz - len, fmt, args);
        if (tmpLen <= 0)
        {
            break;
        }
        len += tmpLen;
        dataItem.len = (uint8_t)len;
        // putting data end

        ret = this->log_Task.log(logType, &dataItem);
        if (ret != 0)
        {
            break;
        }
        ret = 0;
    } while (0);

    if (ret != 0)
    {
        this->data_Mgr.release(&dataItem);
        std::cout << "[log] error when write log\n";
    }
    return ret;
}

const char* LoggerManager::logLevel2String(log_level_t level)
{
    const char* levelName = "";
    switch (level)
    {
    case log_level_t::LOG_LEVEL_NONE:
        levelName = "[NONE]";
        break;
    case log_level_t::LOG_LEVEL_ERROR:
        levelName = "[ERROR]";
        break;
    case log_level_t::LOG_LEVEL_WARN:
        levelName = "[WARN]";
        break;
    case log_level_t::LOG_LEVEL_DEBUG:
        levelName = "[DEBUG]";
        break;
    case log_level_t::LOG_LEVEL_INFO:
        levelName = "[INFOR]";
        break;
    case log_level_t::LOG_LEVEL_VERBOSE:
        levelName = "[VERB]";
        break;
    }
    return levelName;
}

bool LoggerManager::isValid(void)
{
    return this->data_Mgr.isValid();
}

int LoggerManager::releaseData(data_item_t* dataItem)
{
    return this->data_Mgr.release(dataItem);
}
