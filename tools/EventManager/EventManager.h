#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
/////////////////////////////////////////////////
// INCLUDE
#include "../DataManager/DataManager.h"
#include <condition_variable>
#include <mutex>
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO
#define EVENT_MANAGER_MAX_WAIT_TIME             (0xFFFFFFFF)
#define TIMEOUT	
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// TYPE DEF
typedef uint8_t		event_id_t, event_count_t;
typedef uint8_t		event_size_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
struct event_item_t : data_item_t {
    bool isValid()
    {
        return (this->buff != NULL);
    }

    event_id_t id(void)
    {
        return *((event_id_t*)this->buff);
    }

    void id(event_id_t eventId)
    {
        *((event_id_t*)this->buff) = eventId;
    }

    uint8_t* data()
    {
        return this->buff + sizeof(event_id_t);
    }

    event_size_t len()
    {
        return (event_size_t)data_item_t::len;
    }

    void len(event_size_t dataLen)
    {
        data_item_t::len = dataLen;
    }

    event_size_t size()
    {
        return (event_size_t)data_item_t::size;
    }

    void clear()
    {
        data_item_t::clear();
    }
};
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class EventManager : protected DataManager
{
public:
    EventManager();
    virtual ~EventManager();
    bool                        isValid(void);
    virtual int                 init(event_count_t queueSize, event_size_t eventSize);
    void                        deinit(void) override;
    event_item_t                get(void);
    int                         release(event_item_t* item);
    int                         notify(event_id_t eventId, event_item_t* eventItem);
    event_item_t                waitForItem(unsigned int waitMs = EVENT_MANAGER_MAX_WAIT_TIME); // ms
    int                         notify(event_id_t eventId, event_size_t eventSize, unsigned char* eventData);

protected:
    std::mutex			        event_Key;
    std::condition_variable     event_Locker;
};
#endif

