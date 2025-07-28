/////////////////////////////////////////////////
// INCLUDE
#include "DataLinkedList.h"
#include <iostream>
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
DataLinkedList::DataLinkedList(void) :
    data_Item(NULL),
    head(NULL),
    tail(NULL),
    queue_Size(0),
    queue_Count(0),
    data_Locker()
{

}

DataLinkedList::~DataLinkedList(void)
{
    this->deinit();
}

bool DataLinkedList::isValid()
{
    std::lock_guard<std::mutex> lock(this->data_Locker);
    return (this->queue_Size > 0);
}

uint8_t DataLinkedList::size()
{
    std::lock_guard<std::mutex> lock(this->data_Locker);
    return this->queue_Count;
}

int DataLinkedList::init(uint8_t queueSize, uint8_t itemSize)
{
    int ret = -1;
    do
    {
        if (this->isValid() != false)
        {
            ret = 0;
            std::cout << "LinkedList is already initialized!" << std::endl;
            break;
        }
        std::lock_guard<std::mutex> lock(this->data_Locker);

        if (queueSize == 0)
        {
            break;
        }

        this->queue_Size = queueSize;
        this->queue_Count = 0;
        this->data_Item = new LinkedNode[queue_Size];

        if (this->data_Item == NULL)
        {
            break;
        }

        memset(this->data_Item, 0, sizeof(LinkedNode) * this->queue_Size);

        this->head = this->data_Item;
        this->tail = this->data_Item;

        ret = 0;
    } while (0);

    if (ret != 0)
    {
        this->deinit();
    }
    return ret;
}

bool DataLinkedList::isEmpty()
{
    std::lock_guard<std::mutex> lock(this->data_Locker);
    return (this->queue_Count <= 0);
}

bool DataLinkedList::isFull()
{
    std::lock_guard<std::mutex> lock(this->data_Locker);
    return (this->queue_Count >= this->queue_Size);
}

void DataLinkedList::deinit(void)
{
    do
    {
        if (this->isValid() == false)
        {
            break;
        }
        std::lock_guard<std::mutex> lock(this->data_Locker);
        if (this->data_Item != NULL)
        {
            delete[] this->data_Item;
        }
        this->head = NULL;
        this->tail = NULL;
        this->data_Item = NULL;
        this->queue_Size = 0;
        this->queue_Count = 0;
    } while (0);
}

LinkedNode* DataLinkedList::getAvailableSlot()
{
    LinkedNode* candidateNode = NULL;
    do
    {
        if (this->queue_Count >= this->queue_Size)
        {
            break;
        }
        candidateNode = (LinkedNode*)(this->data_Item);
        while (candidateNode->next != NULL)
        {
            candidateNode++;
        }
        return candidateNode;
    } while (0);
    return NULL;
}

int DataLinkedList::enQueue(data_item_t* data)
{
    int ret = -1;
    do
    {
        if (this->isValid() == false)
        {
            break;
        }
        std::lock_guard<std::mutex> lock(this->data_Locker);
        LinkedNode* availableNode = this->getAvailableSlot();
        if (availableNode == NULL)
        {
            std::cout << "[DataLinkedList] LinkedList overloaded" << std::endl;
            break;
        }

        memcpy(&availableNode->data, data, sizeof(data_item_t));
        if (this->queue_Count == 0)
        {
            this->head = availableNode;
            this->tail = this->head;
        }
        // Example: head - A -> B -> C - tail
        // We want to add D to the list, we do:
        // C -> next = D, which is tail -> next = D;
        // update tail = D;
        // To prevent memory leak, we take data_Tail point to itself:
        // data_tail -> next = data_Tail;
        this->tail->next = availableNode;
        this->tail = availableNode;
        this->tail->next = this->tail;
        this->queue_Count++;
        ret = 0;
    } while (0);
    return ret;
}

int DataLinkedList::deQueue(data_item_t* data)
{
    int ret = -1;
    do
    {
        if (this->isValid() == false)
        {
            break;
        }
        std::lock_guard<std::mutex> lock(this->data_Locker);
        if (this->queue_Count == 0)
        {
            break;
        }
        if (&this->head->data == NULL)
        {
            break;
        }

        memcpy(data, &this->head->data, sizeof(data_item_t));
        LinkedNode* nextNode = this->head->next;
        this->head->next = NULL;
        this->head = nextNode;
        this->queue_Count--;
        ret = 0;
    } while (0);
    return ret;
}

int DataLinkedList::remove(data_item_t* data)
{
    int ret = -1;
    data_item_t tempData = data_item_t();
    LinkedNode* previousNode = this->head;
    LinkedNode* candidateNode = NULL;
    LinkedNode* foundNode = NULL;

    do
    {
        if (this->isValid() == false)
        {
            break;
        }
        std::lock_guard<std::mutex> lock(this->data_Locker);
        if (this->head->data.buff == data->buff)
        {
            this->head = this->head->next;
            candidateNode = previousNode;
            candidateNode->next = NULL;
            this->queue_Count--;
            ret = 0;
            break;
        }

        do
        {
            candidateNode = previousNode->next;
            if (candidateNode->data.buff == data->buff)
            {
                foundNode = candidateNode;
                break;
            }
            previousNode = candidateNode;
        } while (candidateNode != this->tail);

        if (foundNode == NULL)
        {
            break;
        }

        if (foundNode == this->tail)
        {
            previousNode->next = previousNode;
            this->tail = previousNode;
        }
        else
        {
            previousNode->next = candidateNode->next;
        }

        candidateNode->next = NULL;
        this->queue_Count--;
        ret = 0;

    } while (0);
    return ret;
}
