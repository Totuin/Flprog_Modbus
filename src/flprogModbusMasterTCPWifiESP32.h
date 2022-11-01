#pragma once
#include "Arduino.h"


#ifdef ESP32
#include "WiFi.h"
#include "flprogModbusMasterTCP.h"

class ModbusTCPWifiESP32SlaveServer : public ModbusTCPSlaveServer
{
public:
    ModbusTCPWifiESP32SlaveServer(){};
    virtual void connect();
    virtual byte available();
    virtual byte read();
    virtual void stop();
    virtual void write(byte buffer[], byte buferSize);

private:
    WiFiClient client;
};

class ModbusMasterTCPWifiESP32 : public ModbusMasterTCP
{
public:
    ModbusMasterTCPWifiESP32(ModbusTCPSlaveServer table[], int size);
    ModbusMasterTCPWifiESP32(){};

protected:
private:
};

#endif
