#pragma once
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "flprogModbusMasterTCPWifiESP8266.h"

class ModbusMasterRTUoverTCPWifiESP8266 : public ModbusMasterTCPWifiESP8266
{
public:
    ModbusMasterRTUoverTCPWifiESP8266(ModbusTCPSlaveServer table[], int size);

protected:
    virtual void checkAnswer();
    virtual byte validateRequest();
    virtual void getRxBuffer();
    virtual void sendTxBuffer();
};