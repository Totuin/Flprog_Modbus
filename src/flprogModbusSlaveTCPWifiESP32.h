#pragma once
#include "Arduino.h"
#ifdef ESP32
#include "WiFi.h"
#include "flprogModbusSlaveTCP.h"

class ModbusSlaveTCPWifiESP32 : public ModbusSlaveTCP
{
public:
    ModbusSlaveTCPWifiESP32(WiFiServer *server);
    virtual void begin();
    virtual void pool();

protected:
    void getRxBuffer();
    virtual void sendTxBuffer();
     
private:
    WiFiServer *server;
    WiFiClient client;
};

#endif
    