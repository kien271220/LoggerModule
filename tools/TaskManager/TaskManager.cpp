/////////////////////////////////////////////////
// INCLUDE
#include "TaskManager.h"
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
std::string toString(event_id_t eventID)
{
    switch (eventID)
    {
    case EID_INVALID: return "EVENT_INVALID";
    case EID_TERMINATE: return "EVENT_TERMINATE";
    case EID_TASK_STOP: return "EVENT_STOP";
    case EID_TASK_OP: return "EVENT_OPERATE";
    case EID_LOG: return "EVENT_LOG";
    default: return "EVENT_INVALID";
    }
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
TaskManager::TaskManager()
    : event_Mgr()
    , task_Hdl(NULL)
    , task_State(task_state_t::TASK_STS_NOT_INI)
{

}

TaskManager::~TaskManager(void)
{

}

task_state_t TaskManager::taskState(void)
{
    return this->task_State;
}

EventManager* TaskManager::eventManager()
{
    return &this->event_Mgr;
}

bool TaskManager::isTaskValid()
{
    return (this->taskState() != task_state_t::TASK_STS_NOT_INI);
}

bool TaskManager::isTaskStopped(void)
{
    return (this->taskState() == task_state_t::TASK_STS_STOPPED);
}

bool TaskManager::isTaskReady(void)
{
    return (this->taskState() == task_state_t::TASK_STS_READY) || (this->taskState() == task_state_t::TASK_STS_OP);
}

int TaskManager::init(event_count_t eventCount, event_size_t eventSz)
{
    int ret = -1;
    do
    {
        if (this->isTaskValid() != false)
        {
            ret = 0;
            break;
        }
        ret = this->event_Mgr.init(eventCount, eventSz);
        if (ret != 0)
        {
            break;
        }

        this->task_State = task_state_t::TASK_STS_STOPPED;
        ret = 0;
    } while (0);
    if (ret != 0)
    {
        this->deinit();
    }
    return ret;
}

void TaskManager::deinit(void)
{
    this->stopTask();
    this->event_Mgr.deinit();
}

int TaskManager::prepare(void)
{
    return 0;
}

void TaskManager::stopTask(void)
{
    int ret = 0;
    do
    {
        if (this->isTaskStopped() != false)
        {
            break;
        }
        ret = this->notifyTask(EID_TASK_STOP);
        if (ret != 0)
        {
            break;
        }
        while (this->isTaskStopped() == false)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } while (0);
    this->task_Hdl = NULL;
}

int TaskManager::startTask(int32_t prepWaitMs)
{
    int ret = -1;
    do
    {
        if (this->isTaskValid() == false)
        {
            break;
        }
        if (this->isTaskStopped() == false)
        {
            break;
        }

        this->task_State = task_state_t::TASK_STS_STOPPED;
        this->task_Hdl = new std::thread(&TaskManager::proc, this);
        if (this->task_Hdl == NULL)
        {
            break;
        }

        while ((this->task_State == task_state_t::TASK_STS_STOPPED)
            && (prepWaitMs > 0))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            prepWaitMs -= 100;
        }

        if (this->task_State == task_state_t::TASK_STS_STOPPED)
        {
            break;
        }
        ret = 0;
    } while (0);
    return ret;
}

int TaskManager::startTaskOp(void)
{
    return this->event_Mgr.notify(EID_TASK_OP, 0, NULL);
}

int TaskManager::notifyTask(event_id_t eventId, event_item_t* eventItem)
{
    if (this->isTaskReady() == false)
    {
        return -1;
    }
    return this->event_Mgr.notify(eventId, eventItem);
}

int TaskManager::notifyTask(event_id_t eventId)
{
    if (this->isTaskValid() == false)
    {
        return -1;
    }
    return this->event_Mgr.notify(eventId, 0, NULL);
}

void TaskManager::waitTaskOp(void)
{
    event_item_t eventData = event_item_t();
    int ret = 0;
    unsigned long waitMs = 100;
    bool waiting = true;
    while (waiting)
    {
        this->event_Mgr.release(&eventData);
        eventData = this->event_Mgr.waitForItem(waitMs);         /* wait for incoming events                                   */
        if (eventData.isValid() == false)
        {
            continue;
        }

        switch (eventData.id())
        {
        case EID_TERMINATE:
        {
            this->task_State = task_state_t::TASK_STS_OP;
            waiting = false;
            break;
        }
        case EID_TASK_STOP:
        {
            this->task_State = task_state_t::TASK_STS_STOP_REQ;
            waiting = false;
            break;
        }
        }
    }
    this->event_Mgr.release(&eventData);
}
