#include "Logger.h"
#include <iostream>

Logger::Logger(uint8_t type) :
    log_Locker()
    , log_Type(type)
{

}

Logger::~Logger()
{

}

uint8_t	Logger::logType()
{
    return this->log_Type;
}
