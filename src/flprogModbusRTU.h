#pragma once
#include "Arduino.h"
#include "flprogModbus.h"

class ModbusRTU : public Modbus
{
public:
    void setPortSpeed(byte speed);
    void setPinPeDe(byte pin);
    void setPortDataBits(byte dataBits);
    void setPortStopBits(byte stopBits);
    void setPortParity(byte stopBits);

protected:
    virtual void restartPort(){};
    virtual byte portAvailable() { return 0; };
    virtual byte portRead() { return 0; };
    virtual bool hasPort() { return false; };
    virtual byte portWrite(byte *buffer, byte size) { return 0; };
    void onPeDePin();
    void offPeDePin();
    byte rxBuffer();
    virtual void sendTxBuffer();
    bool checkAvalibleBytes();
    byte pinPeDe = 200;
    byte portSpeed = SPEED_9600;
    byte portDataBits = 8;
    byte portParity = 0;
    byte portStopBits = 1;
    byte lastRec = 0;
    unsigned long time;
    int timeOfSend;
};