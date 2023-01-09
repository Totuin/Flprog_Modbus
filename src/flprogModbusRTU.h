#pragma once
#include "Arduino.h"
#include "flprogModbus.h"

class ModbusRTU : public Modbus
{
public:
    void setPortSpeed(byte speed) { uart->setPortSpeed(speed); };
    void setPinPeDe(byte pin) { pinPeDe = pin; };
    void setPortDataBits(byte dataBits) { uart->setPortDataBits(dataBits); };
    void setPortStopBits(byte stopBits) { uart->setPortStopBits(stopBits); };
    void setPortParity(byte parity) { uart->setPortParity(parity); };
    byte getPortSpeed() { return uart->getPortSpeed(); };
    byte getPortDataBits() { return uart->getPortDataBits(); };
    byte getPortStopBits() { return uart->getPortStopBits(); };
    byte getPortParity() { return uart->getPortParity(); };
    void setUart(FLProgUart *uartPort) { uart = uartPort; };

protected:
    FLProgUart *uart;
    void onPeDePin();
    void offPeDePin();
    byte rxBuffer();
    virtual void sendTxBuffer();
    bool checkAvalibleBytes();
    byte pinPeDe = 200;
    byte lastRec = 0;
    unsigned long time;
    int timeOfSend;
};