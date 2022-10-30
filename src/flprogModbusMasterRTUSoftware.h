#pragma once
#include "Arduino.h"
#include "flprogModbusMasterRTU.h"
#include "SoftwareSerial.h"

class ModbusMasterRTUSoftware : public ModbusMasterRTU
{
public:
    ModbusMasterRTUSoftware(ModbusSlaveInMaster table[], int size);
    virtual void begin(uint8_t receivePin, uint8_t transmitPin);

protected:
    SoftwareSerial *port;
    virtual void restartPort();
    virtual byte portAvailable();
    virtual byte portRead();
    virtual byte portWrite(byte *buffer, byte size);
    virtual bool hasPort();
};