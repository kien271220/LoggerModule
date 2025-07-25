/////////////////////////////////////////////////
// INCLUDE
#include "LogTask.h"
#include "LoggerManager.h"
#include <iostream>
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
LogTask::LogTask() :
    logger_List(NULL)
{

}

LogTask::~LogTask()
{
    this->deinit();
}

bool LogTask::isValid()
{
    return (this->logger_List != NULL);
}

int LogTask::init(Logger** logger, log_task_config_t* taskConfig)
{
    int ret = -1;
    do
    {
        if (this->isValid() != false)
        {
            ret = 0;
            break;
        }

        this->logger_List = logger;

        // if no thread is configured, break the loop
        if (taskConfig->use_thread == false)
        {
            ret = 0;
            break;
        }
        // else init the task
        ret = TaskManager::init(taskConfig->event_count, taskConfig->event_size);
        if (ret != 0)
        {
            break;
        }
        ret = 0;
    } while (0);

    if (ret != 0)
    {
        this->deinit();
        std::cout << " [ini] error ret\n";
    }
    return ret;
}

void LogTask::deinit()
{
    TaskManager::deinit();
    this->logger_List = NULL;
}

void LogTask::stop()
{
    if (this->isTaskValid() == false)
    {
        return;
    }
    this->stopTask();
}

int LogTask::start()
{
    int ret = -1;
    do
    {
        if (this->isTaskValid() == false)
        {
            ret = 0;
            break;
        }
        if (this->isTaskStopped() == false)
        {
            ret = 0;
            break;
        }
        ret = TaskManager::startTask(5000);
        if (ret != 0)
        {
            break;
        }

        ret = 0;
    } while (0);
    if (ret != 0)
    {
        this->stop();
    }
    return ret;
}

int	LogTask::log(uint8_t logType, data_item_t* dataItem)
{
    int ret = -1;
    event_item_t eventItem = event_item_t();
    log_task_log_data_t* logData = NULL;

    do
    {
        if (this->isTaskValid() == false)
        {
            ret = this->onLog(logType, dataItem);
            break;
        }
        eventItem = this->event_Mgr.get();
        if (eventItem.isValid() == false)
        {
            break;
        }

        // event_item_t buffer: ID + LogType + padding + data.

        logData = (log_task_log_data_t*)eventItem.data();
        if (eventItem.size() < sizeof(log_task_log_data_t))
        {
            break;
        }

        logData->log_type = logType;
        memcpy(&logData->data_item, dataItem, sizeof(data_item_t));
        ret = this->notifyTask(EID_LOG, &eventItem);
        if (ret != 0)
        {
            break;
        }
        eventItem.clear();
        dataItem->clear();
        ret = 0;
    } while (0);
    if (ret != 0)
    {
        this->event_Mgr.release(&eventItem);
    }
    return ret;
}

int LogTask::onLog(uint8_t logType, data_item_t* dataItem)
{
    int ret = -1;
    int idx = 0;
    Logger* logger = NULL;
    do
    {
        for (idx = 0; idx < CF_LOGGER_PROCESSOR_NUM_MAX; idx++)
        {
            logger = this->logger_List[idx];
            if (logger == NULL)
            {
                break;
            }
            if ((logger->logType() & logType) == 0)
            {
                continue;
            }
            logger->log((char*)dataItem->buff);
        }
        ret = 0;
    } while (0);
    LoggerManager::getInstance().releaseData(dataItem);
    return ret;
}

void LogTask::proc()
{
    std::cout << "[proc] starting\n";
    event_item_t eventItem = event_item_t();
    bool running = true;
    int ret = 0;
    // wait for start event
    if (this->prepare() != 0)
    {                                                             /* if something wrong, jump into infinity loop                        */
        goto proc_end;
    }
    this->task_State = task_state_t::TASK_STS_READY;
    this->task_State = task_state_t::TASK_STS_OP;
    std::cout << "[proc] proc is running\n";
    while (running)
    {
        eventItem = this->event_Mgr.waitForItem(1);
        if (eventItem.isValid() == false)
        {
            continue;
        }
        std::cout << "[proc] waite with eventItem num is " << (int)eventItem.id() << " - " << toString(eventItem.id()) << std::endl;
        switch (eventItem.id())
        {
        case EID_TASK_STOP:
        {
            this->task_State = task_state_t::TASK_STS_STOP_REQ;
            running = false;
            std::cout << "[proc] proc stops operating\n";
            break;
        }
        case EID_LOG:
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << "[proc] proc logging data\n";
            log_task_log_data_t* logData = (log_task_log_data_t*)eventItem.data();
            this->onLog(logData->log_type, &logData->data_item);
            break;
        }
        default:
            break;
        }
        this->event_Mgr.release(&eventItem);
    }
    this->event_Mgr.release(&eventItem);
proc_end:
    std::cout << "[proc] ends\n";
    this->task_State = task_state_t::TASK_STS_STOPPED;
    this->task_Hdl = NULL;
}
