#pragma once
#include "Arduino.h"
#ifdef ESP8266
#include "ESP8266WiFi.h"
#include "flprogModbusSlaveTCP.h"

class ModbusSlaveTCPWifiESP8266 : public ModbusSlaveTCP
{
public:
    ModbusSlaveTCPWifiESP8266(WiFiServer *server);
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
    