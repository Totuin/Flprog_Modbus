#pragma once
#include "Arduino.h"

#ifdef ESP8266
#include "ESP8266WiFi.h"
#include "flprogModbusMasterTCP.h"

class ModbusMasterTCPWifiESP8266 : public ModbusMasterTCP
{
public:
    ModbusMasterTCPWifiESP8266(ModbusTCPSlaveServer table[], int size);
    ModbusMasterTCPWifiESP8266(){};

    virtual void connect(ModbusTCPSlaveServer *server);
    virtual byte available();
    virtual byte read();
    virtual void stop();
    virtual void write(ModbusTCPSlaveServer *server, byte buffer[], byte buferSize);

protected:
    WiFiClient client;

private:
};

#endif
