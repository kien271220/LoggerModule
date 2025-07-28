#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
/////////////////////////////////////////////////
// INCLUDE
#include "../EventManager/EventManager.h"
#include <thread>
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
std::string toString(event_id_t eventID);
/////////////////////////////////////////////////
// TYPE DEF
typedef enum
{
    EID_INVALID = 0,
    EID_TERMINATE,
    EID_TASK_STOP,
    EID_TASK_OP,
    EID_LOG
} _event_id_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)
enum task_state_t                                                       /* task state enum                                          */
{
    TASK_STS_NOT_INI = 0,                                               /* task is not initialized yet                              */
    TASK_STS_STOPPED,                                                   /* task is stopped                                          */
    TASK_STS_READY,                                                     /* task is running but not operating                        */
    TASK_STS_OP,                                                        /* task is operating                                        */
    TASK_STS_STOP_REQ,                                                  /* task is requested to stop                                */
};
/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class TaskManager
{
public:
    TaskManager();
    virtual ~TaskManager();
    task_state_t                taskState();
    bool					    isTaskValid();
    bool					    isTaskStopped();
    bool					    isTaskReady();
    EventManager*               eventManager();
    int                         init(event_count_t eventCount, event_size_t eventSz);
    virtual void			    deinit();
    int						    startTask(int32_t prepWaitMs);
    void					    stopTask();
    int						    startTaskOp(void);
    int						    notifyTask(event_id_t eventId, event_item_t* eventData);
    int                         notifyTask(event_id_t eventId);

protected:
    virtual void				waitTaskOp(void);
    virtual int					prepare(void);
    virtual void                proc(void) = 0;

protected:
    std::thread*                task_Hdl;
    task_state_t				task_State;
    EventManager				event_Mgr;
};

#endif
