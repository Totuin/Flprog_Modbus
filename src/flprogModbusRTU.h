#pragma once
#include "base/flprogModbus.h"

class ModbusRTU : public Modbus
{
public:
    void setUart(uint8_t uartPort) { uartPortNumber = uartPort; };
    
protected:
    void onPeDePin();
    void offPeDePin();
    virtual uint8_t rxBuffer();
    virtual void sendTxBuffer();
    virtual bool checkAvaliblePacage();
    int pinPeDe = -1;
    uint8_t lastRec = 0;
    unsigned long time;
    uint8_t uartPortNumber = 0;
};

#include "rtu/flprogModbusSlaveRTU.h"
#include "rtu/flprogModbusMasterRTU.h"