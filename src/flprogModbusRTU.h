#pragma once
#include "base/flprogModbus.h"

class ModbusRTU : public Modbus
{
public:
    void setUart(uint8_t uartPort) { _uartPortNumber = uartPort; };
    
protected:
    void onPeDePin();
    void offPeDePin();
    virtual uint8_t rxBuffer();
    virtual void sendTxBuffer();
    virtual bool checkAvaliblePacage();
    int _pinPeDe = -1;
    uint8_t _lastRec = 0;
    unsigned long _time;
    uint8_t _uartPortNumber = 0;
};

#include "rtu/flprogModbusSlaveRTU.h"
#include "rtu/flprogModbusMasterRTU.h"