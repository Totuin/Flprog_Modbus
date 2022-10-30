#pragma once
#include "Arduino.h"
#include "flprogModbusMasterRTU.h"

class ModbusMasterRTUHardware : public ModbusMasterRTU
{
public:
    ModbusMasterRTUHardware(ModbusSlaveInMaster table[], int size);
    virtual void begin(HardwareSerial *port);

protected:
    HardwareSerial *port;
    virtual void restartPort();
    virtual byte portAvailable();
    virtual byte portRead();
    virtual byte portWrite(byte *buffer, byte size);
    virtual bool hasPort();
    virtual void getRxBuffer();
    virtual void sendTxBuffer();
};