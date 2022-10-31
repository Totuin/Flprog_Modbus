#pragma once
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "flprogModbusMasterTCP.h"

class ModbusTCPWifiESP8266SlaveServer : public ModbusTCPSlaveServer
{
public:
    ModbusTCPWifiESP8266SlaveServer(){};
    virtual void connect();
    virtual byte available();
    virtual byte read();
    virtual void stop();
    virtual void write(byte buffer[], byte buferSize);

private:
    WiFiClient client;
};

class ModbusMasterTCPWifiESP8266 : public ModbusMasterTCP
{
public:
    ModbusMasterTCPWifiESP8266(ModbusTCPSlaveServer table[], int size);
    ModbusMasterTCPWifiESP8266(){};

protected:
private:
};
