#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
////////////////////////////////////////////////
// INCLUDE
#include "DataLinkedList.h"
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
// CLASS DEFINITION
class DataManager
{
public:
    DataManager(void);
    virtual ~DataManager(void);
    virtual bool                                            isValid(void);
    virtual int                                             init(uint8_t queueSize, uint8_t itemSize);
    virtual void                                            deinit(void);
    int                                                     releaseAll(void);
    data_item_t                                             get(void);
    int                                                     release(data_item_t* item);
protected:
    DataLinkedList                                          unused_Queue;
    DataLinkedList                                          used_Queue;
};
#endif
