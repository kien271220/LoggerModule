/////////////////////////////////////////////////
// INCLUDE
#include "DataManager.h"
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
DataManager::DataManager(void)
    : unused_Queue()
    , used_Queue()
{

}

DataManager::~DataManager(void)
{
    this->deinit();
}


bool DataManager::isValid(void)
{
    if ((this->used_Queue.isEmpty() == false)
        || (this->unused_Queue.isEmpty() == false)
        )
    {
        return true;
    }

    return false;
}

int DataManager::init(uint8_t queueSize, uint8_t itemSize)
{
    int ret = -1;
    int idx = 0;
    data_item_t dataItem = data_item_t();
    do
    {
        if (this->isValid() != false)
        {
            ret = 0;
            std::cout << "[ini] already inited\n";
            break;
        }

        if ((queueSize == 0)
            || (itemSize == 0))
        {
            break;
        }

        ret = this->unused_Queue.init(queueSize, itemSize);
        if (ret != 0)
        {
            break;
        }

        ret = this->used_Queue.init(queueSize, itemSize);
        if (ret != 0)
        {
            break;
        }

        for (idx = 0; idx < queueSize; idx++)
        {
            dataItem.buff = new uint8_t[itemSize];
            if (dataItem.buff == NULL)
            {
                break;
            }
            dataItem.len = 0;
            dataItem.size = itemSize;
            ret = this->unused_Queue.enQueue(&dataItem);
            if (ret != 0)
            {
                break;
            }
        }
        if (idx < queueSize)
        {
            break;
        }

        ret = 0;
    } while (0);
    if (ret != 0)
    {
        this->deinit();
    }
    return ret;
}

void DataManager::deinit(void)
{
    int ret = -1;
    data_item_t dataItem = data_item_t();
    do
    {
        this->releaseAll();
        do
        {
            ret = this->unused_Queue.deQueue(&dataItem);
            if (ret != 0)
            {
                break;
            }
            if (dataItem.buff == NULL)
            {
                continue;
            }
            delete[] dataItem.buff;
            dataItem.buff = NULL;
        } while (ret == 0);
    } while (0);
    this->unused_Queue.deinit();
    this->used_Queue.deinit();
}

int DataManager::releaseAll(void)
{
    int ret = -1;
    data_item_t dataItem = data_item_t();
    do
    {
        do
        {
            ret = this->used_Queue.deQueue(&dataItem);
            if (ret != 0)
            {
                break;
            }
            //this->release(&dataItem);
            ret = this->unused_Queue.enQueue(&dataItem);
            if (ret != 0)
            {
                break;
            }
        } while (ret == 0);
        ret = 0;
    } while (0);
    return ret;
}

data_item_t DataManager::get(void)
{
    data_item_t dataItem = data_item_t();
    int ret = -1;
    do
    {
        ret = this->unused_Queue.deQueue(&dataItem);
        if (ret != 0)
        {
            std::cout << "[DataMgr] deQueue empty\n";
            break;
        }

        ret = this->used_Queue.enQueue(&dataItem);
        if (ret != 0)
        {
            this->unused_Queue.enQueue(&dataItem);
            break;
        }
    } while (0);
    return dataItem;
}

int DataManager::release(data_item_t* item)
{
    int ret = -1;
    do
    {
        if (item->buff == NULL)
        {
            break;
        }
        ret = this->used_Queue.remove(item);
        if (ret != 0)
        {
            break;
        }

        ret = this->unused_Queue.enQueue(item);
        if (ret != 0)
        {
            break;
        }
        memset(item, 0, sizeof(data_item_t));
        ret = 0;
    } while (0);
    return ret;
}
