#ifndef LOGGER_MANAGER_H
#define LOGGER_MANAGER_H
/////////////////////////////////////////////////
// INCLUDE
#include "LogTask.h"
#include "../tools/TaskManager/TaskManager.h"
#include "LoggerGenerate.h"
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
typedef struct
{
    uint8_t  data_count;
    uint8_t  data_size;
    log_task_config_t task_cfg;
    log_configs_t log_cfgs;
} log_mgr_configs_t;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class LoggerManager
{
public:
    static LoggerManager& getInstance(void);
    virtual ~LoggerManager();
    int						init(log_mgr_configs_t* configData);
    void					deinit();
    int                     releaseData(data_item_t* dataItem);
    int						log(log_level_t level, uint8_t logType, const char* fmt, ...);
    int						log(log_level_t level, uint8_t logType, const char* fmt, va_list args);
    int						logE(uint8_t logType, const char* fmt, ...);
    int						logW(uint8_t logType, const char* fmt, ...);
    int						logI(uint8_t logType, const char* fmt, ...);
    int						logD(uint8_t logType, const char* fmt, ...);
    int						logV(uint8_t logType, const char* fmt, ...);
    bool					isValid();
    const char*             logLevel2String(log_level_t level);
private:
    LoggerManager();
    LoggerManager(const LoggerManager&) = delete;            // Copy constructor
    LoggerManager& operator=(const LoggerManager&) = delete; // Copy assignment
    LoggerManager(LoggerManager&&) = delete;                 // Move constructor
    LoggerManager& operator=(LoggerManager&&) = delete;      // Move assignment
protected:
    Logger* logger_List[CF_LOGGER_PROCESSOR_NUM_MAX];
    DataManager             data_Mgr;
    LogTask                 log_Task;
    log_level_t             log_Level;
};

#endif
