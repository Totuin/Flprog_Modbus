#pragma once
#include "base/flprogModbus.h"
#include "flprogUartBase.h"

class ModbusRTU : public Modbus
{
public:
    void setUart(uint8_t uartPort) { _uartPortNumber = uartPort; };
    void setSpeedUart(uint32_t speed) { _executor->setSpeedUart(speed, _uartPortNumber); };
    void setDataBitUart(uint8_t value) { _executor->setDataBitUart(value, _uartPortNumber); };
    void setStopBitUart(uint8_t value) { _executor->setStopBitUart(value, _uartPortNumber); };
    void setParityUart(uint8_t value) { _executor->setParityUart(value, _uartPortNumber); };
    void setPinRxUart(uint8_t pin) { _executor->setPinRxUart(pin, _uartPortNumber); };
    void setPinTxUart(uint8_t pin) { _executor->setPinTxUart(pin, _uartPortNumber); };
    void setPinPeDe(uint8_t pin) { _pinPeDe = pin; };

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
    FlprogAbstractUartExecutor *_executor = 0;
};



#include "rtu/flprogModbusSlaveRTU.h"
#include "rtu/flprogModbusMasterRTU.h"