#pragma once
#include "Arduino.h"
#include "flprogModbusSlaveRTU.h"
#include "SoftwareSerial.h"

class ModbusSlaveRTUSoftware : public ModbusSlaveRTU
{
public:
    virtual void begin(byte addres, uint8_t receivePin, uint8_t transmitPin);

protected:
    SoftwareSerial *port;
    virtual void restartPort();
    virtual byte portAvailable();
    virtual byte portRead();
    virtual byte portWrite(byte *buffer, byte size);
    virtual bool hasPort();
};