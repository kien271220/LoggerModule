#ifndef LOG_TASK_H
#define LOG_TASK_H
/////////////////////////////////////////////////
// INCLUDE
#include "../tools/TaskManager/TaskManager.h"
#include "Logger.h"

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
    uint8_t log_type;
    data_item_t data_item;
} log_task_log_data_t;

typedef struct
{
    bool       use_thread;
    event_count_t event_count;
    event_size_t  event_size;
} log_task_config_t;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class LogTask : protected TaskManager
{
public:
    LogTask();
    virtual ~LogTask();
    bool				isValid();
    int					init(Logger** logger, log_task_config_t* taskConfig);
    void				deinit();
    int					start();
    void				stop();
    int					log(uint8_t logType, data_item_t* dataItem);				// for 2nd task
protected:
    void                proc(void) override;
    int                 onLog(uint8_t logType, data_item_t* dataItem);				// for only 1 main task
protected:
    Logger** logger_List;
};
#endif
