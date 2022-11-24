#pragma once
#include "Arduino.h"

#ifdef ESP32
#include "WiFi.h"
#include "flprogModbusMasterTCP.h"

class ModbusMasterTCPWifiESP32 : public ModbusMasterTCP
{
public:
    ModbusMasterTCPWifiESP32(ModbusTCPSlaveServer table[], int size);
    ModbusMasterTCPWifiESP32(){};
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
