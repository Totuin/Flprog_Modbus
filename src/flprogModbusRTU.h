#pragma once
#include "base/flprogModbus.h"
#include "flprogUart.h"

class ModbusRTU : public Modbus
{
public:
     void setUart(FLProgUartBasic *uartPort) { uart = uartPort; };
    FLProgUartBasic *uardDevice();

    void setPortSpeed(uint8_t speed) { uardDevice()->setPortSpeed(speed); };
    void setPinPeDe(uint8_t pin) { pinPeDe = pin; };
    void setPortDataBits(uint8_t dataBits) { uardDevice()->setPortDataBits(dataBits); };
    void setPortStopBits(uint8_t stopBits) { uardDevice()->setPortStopBits(stopBits); };
    void setPortParity(uint8_t parity) { uardDevice()->setPortParity(parity); };
    void setDeviceName(String name) { uardDevice()->setDeviceName(name); };

    uint8_t getPortSpeed() { return uardDevice()->getPortSpeed(); };
    uint8_t getPortDataBits() { return uardDevice()->getPortDataBits(); };
    uint8_t getPortStopBits() { return uardDevice()->getPortStopBits(); };
    uint8_t getPortParity() { return uardDevice()->getPortParity(); };

protected:
    void onPeDePin();
    void offPeDePin();
    virtual uint8_t rxBuffer();
    virtual void sendTxBuffer();
    virtual bool checkAvaliblePacage();
    int pinPeDe = -1;
    uint8_t lastRec = 0;
    unsigned long time;
    FLProgUartBasic *uart = 0;
};

#include "rtu/flprogModbusSlaveRTU.h"
#include "rtu/flprogModbusMasterRTU.h"