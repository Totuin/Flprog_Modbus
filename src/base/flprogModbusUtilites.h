#include "Arduino.h"
#include "flprogUtilites.h"
#pragma once
#define FLPROG_MODBUS_READY 0
#define FLPROG_MODBUS_WAITING_SENDING 1
#define FLPROG_MODBUS_WAITING_ANSWER 2
#define FLPROG_MODBUS_WAITING_CONNECT_CLIENT 2

#define FLPROG_COIL 0
#define FLPROG_DISCRETE_INPUT 1
#define FLPROG_INPUT_REGISTR 3
#define FLPROG_HOLDING_REGISTR 4
#define FLPROG_AB_ORDER 1
#define FLPROG_BA_ORDER 2
#define FLPROG_ABCD_ORDER 1
#define FLPROG_CDAB_ORDER 2
#define FLPROG_BADC_ORDER 3
#define FLPROG_DCBA_ORDER 4

namespace flprogModus
{
    int modbusCalcCRC(byte length, byte bufferArray[]);
    byte t35TimeForSpeed(int portSpeed);
    int timeForSendBytes(byte portDataBits, byte portStopBits, byte portParity, int portSpeed, int dataSize);
};
