#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>

#define CF_LOGGER_PROCESSOR_NUM_MAX			(15)

typedef enum
{
    LOG_TYPE_NONE = 0,
    LOG_TYPE_FILE = 1,
    LOG_TYPE_CONSOLE = 2,
    LOG_TYPE_HTTP = 4,
    LOG_TYPE_All = 0xFF
} log_type_t;

typedef enum
{
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_VERBOSE
} log_level_t;

typedef struct
{
    uint8_t unused;
} console_log_config_t;

typedef struct
{
    const char* log_dir;
} file_log_config_t;

typedef struct
{
    log_type_t type;
    union
    {
        console_log_config_t  console;
        file_log_config_t     file;
    } cfgs;
} log_config_t;

typedef struct
{
    log_level_t   level;
    log_config_t  configs[CF_LOGGER_PROCESSOR_NUM_MAX];
} log_configs_t;

class Logger
{
public:
    Logger(uint8_t type);
    virtual ~Logger();
    virtual int					init(log_config_t* config) = 0;
    virtual void				deinit() = 0;
    virtual int					log(char* text) = 0;
    uint8_t						logType(void);
protected:
    std::mutex					log_Locker;
    uint8_t						log_Type;
};

#endif
