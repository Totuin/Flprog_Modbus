#pragma once
#include "Arduino.h"
#ifdef ESP32
#include "BluetoothSerial.h"
#include "flprogModbusSlaveRTU.h"

class ModbusSlaveRTUESP32Bluetooth : public ModbusSlaveRTU
{
public:
    virtual void begin(byte addres);
    void setDeviceName(String name);
    void setPartnerName(String name);
    void beBluetoothMaster();
    void beBluetoothSlave();

protected:
    BluetoothSerial port;
    String deviceName;
    String partnerName;
    bool isMaster = false;
    virtual void restartPort();
    virtual byte portAvailable();
    virtual byte portRead();
    virtual byte portWrite(byte *buffer, byte size);
    virtual bool hasPort();
};

#endif