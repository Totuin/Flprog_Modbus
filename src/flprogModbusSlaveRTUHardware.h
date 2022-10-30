#pragma once
#include "Arduino.h"
#include "flprogModbusSlaveRTU.h"
#include "HardwareSerial.h"

class ModbusSlaveRTUHardware : public ModbusSlaveRTU
{
public:
    virtual void begin(byte addres, HardwareSerial *port);

protected:
    HardwareSerial *port;
    virtual void restartPort();
    virtual byte portAvailable();
    virtual byte portRead();
    virtual byte portWrite(byte *buffer, byte size);
    virtual bool hasPort();
};