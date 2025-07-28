#ifndef _DATA_LINKED_LIST_H
#define _DATA_LINKED_LIST_H
/////////////////////////////////////////////////
// INCLUDE
#include <stdint.h>
#include <mutex>
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
struct data_item_t
{
    void clear()
    {
        buff = NULL;
        size = 0;
        len = 0;
    }

    bool isValid(void)
    {
        return (this->buff != NULL);
    }

    uint8_t* buff;
    uint8_t size;
    uint8_t len;
};

struct LinkedNode
{
    data_item_t data;
    LinkedNode* next;
};
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class DataLinkedList
{
public:
    DataLinkedList(void);
    virtual		        ~DataLinkedList(void);
    int			        init(uint8_t queueSize, uint8_t itemSize);
    void		        deinit(void);
    bool		        isValid();
    bool		        isEmpty();
    uint8_t		        size();
    int			        deQueue(data_item_t* data);
    int			        enQueue(data_item_t* data);
    int			        remove(data_item_t* data);
    bool		        isFull();

protected:
    LinkedNode*         getAvailableSlot(void);

protected:
    std::mutex	        data_Locker;

    uint8_t             queue_Size;
    uint8_t             queue_Count;

    LinkedNode*         head;
    LinkedNode* 	    tail;
    LinkedNode* 	    data_Item;
};

#endif
