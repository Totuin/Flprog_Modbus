#pragma once
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "flprogModbusSlaveRTU.h"

class ModbusSlaveRTUoverTCPWifiESP8266 : public ModbusSlaveRTU
{

public:
    ModbusSlaveRTUoverTCPWifiESP8266(WiFiServer *server);
    virtual void begin();
    virtual void pool();

protected:
    virtual byte portAvailable() { return client.available(); };
    virtual byte portRead() { return client.read(); };
    virtual byte portWrite(byte *buffer, byte size) { return client.write(buffer, size); };
    virtual bool hasPort() { return true; };

private:
    WiFiServer *server;
    WiFiClient client;
};