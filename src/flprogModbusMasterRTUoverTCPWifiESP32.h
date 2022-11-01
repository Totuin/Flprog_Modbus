#pragma once
#include "Arduino.h"
#ifdef ESP32
#include "WiFi.h"
#include "flprogModbusMasterTCPWifiESP32.h"

class ModbusMasterRTUoverTCPWifiESP32 : public ModbusMasterTCPWifiESP32
{
public:
    ModbusMasterRTUoverTCPWifiESP32(ModbusTCPSlaveServer table[], int size);

protected:
    virtual void checkAnswer();
    virtual byte validateRequest();
    virtual void getRxBuffer();
    virtual void sendTxBuffer();
};
#endif