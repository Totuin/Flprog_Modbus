#pragma once
#include "Arduino.h"
#ifndef FLPROG_MODBUS_BUFER_SIZE
#define FLPROG_MODBUS_BUFER_SIZE 64
#endif
#define FLPROG_WAIT 0
#define FLPROG_ERROR 255
#define FLPROG_SPEED_300 0
#define FLPROG_SPEED_600 1
#define FLPROG_SPEED_1200 2
#define FLPROG_SPEED_2400 3
#define FLPROG_SPEED_4800 4
#define FLPROG_SPEED_9600 5
#define FLPROG_SPEED_14400 6
#define FLPROG_SPEED_19200 7
#define FLPROG_SPEED_28800 8
#define FLPROG_SPEED_38400 9
class AbstractFLProgClass {protected: uint8_t _status=0; void setFlags() {} public: virtual ~AbstractFLProgClass() {} };
namespace flprog {
inline uint16_t codeFromSpeed(uint16_t v) {return v==9600?FLPROG_SPEED_9600:v==19200?FLPROG_SPEED_19200:FLPROG_SPEED_38400;}
inline uint16_t speedFromCode(uint16_t v) {return v==FLPROG_SPEED_9600?9600:v==FLPROG_SPEED_19200?19200:38400;}
inline bool isTimer(uint32_t a, uint32_t b) {return uint32_t(millis()-a)>=b;}
}
