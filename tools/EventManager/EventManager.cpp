/////////////////////////////////////////////////
// INCLUDE
#include "EventManager.h"
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
EventManager::EventManager()
    : DataManager()
    , event_Key()
    , event_Locker()
{

}

EventManager::~EventManager()
{
    this->deinit();
}

void EventManager::deinit()
{
    DataManager::deinit();
}

bool EventManager::isValid()
{
    return DataManager::isValid();
}

int EventManager::init(event_count_t queueSize, event_size_t eventSize)
{
    return DataManager::init(queueSize, eventSize + sizeof(event_id_t));
}

event_item_t EventManager::get()
{
    event_item_t eventItem = event_item_t();
    while (this->unused_Queue.isEmpty() == true);
    this->unused_Queue.deQueue(&eventItem);
    return eventItem;
}

int EventManager::release(event_item_t* item)
{
    int ret = -1;
    do
    {
        if (item->isValid() == false)
        {
            break;
        }
        ret = this->unused_Queue.enQueue((data_item_t*)item);
        if (ret != 0)
        {
            break;
        }
        item->clear();
        ret = 0;
    } while (0);
    return ret;
}

event_item_t EventManager::waitForItem(unsigned int waitMs)
{
    event_item_t eventItem = event_item_t();
    std::unique_lock <std::mutex> uLock(this->event_Key);
    do
    {
        this->event_Locker.wait_for(uLock, std::chrono::milliseconds(0)); // lock if it is unlock
        if (this->used_Queue.isEmpty() != false)
        {
            this->event_Locker.wait_for(uLock, std::chrono::milliseconds(waitMs)); // lock and wait for specified time
        }

        if (this->used_Queue.isEmpty() != false)
        {
            break;
        }

        this->used_Queue.deQueue((data_item_t*)&eventItem);
    } while (0);
    return eventItem;
}

int EventManager::notify(event_id_t eventId, event_item_t* eventItem)
{
    int ret = -1;
    do
    {
        if (this->isValid() == false)
        {
            break;
        }
        eventItem->id(eventId);
        ret = this->used_Queue.enQueue((data_item_t*)eventItem);
        if (ret != 0)
        {
            break;
        }

        this->event_Locker.notify_one();
        eventItem->clear();
        ret = 0;
    } while (0);
    if (ret != 0)
    {
        this->release(eventItem);
    }
    return ret;
}

int EventManager::notify(event_id_t eventId, event_size_t eventSize, unsigned char* eventData)
{
    int ret = -1;
    event_item_t eventItem = event_item_t();
    do
    {
        eventItem = this->get();
        if (eventItem.isValid() == false)
        {
            break;
        }
        if (eventSize > eventItem.size())
        {
            break;
        }
        if (eventSize > 0)
        {
            memcpy(eventItem.data(), eventData, eventSize);
        }
        eventItem.id(eventId);
        eventItem.len(eventSize);
        ret = this->notify(eventId, &eventItem);
    } while (0);
    if (ret != 0)
    {
        this->release(&eventItem);
    }
    return ret;
}
