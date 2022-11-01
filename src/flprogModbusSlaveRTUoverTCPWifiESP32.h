#pragma once
#include "Arduino.h"

#ifdef ESP32
#include "WiFi.h"
#include "flprogModbusSlaveRTU.h"

class ModbusSlaveRTUoverTCPWifiESP32 : public ModbusSlaveRTU
{

public:
    ModbusSlaveRTUoverTCPWifiESP32(WiFiServer *server);
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
#endif