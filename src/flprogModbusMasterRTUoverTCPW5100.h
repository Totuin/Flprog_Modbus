#pragma once
#include "Arduino.h"
#include "flprogModbusMasterTCPW5100.h"

class ModbusMasterRTUoverTCPW5100 : public ModbusMasterTCPW5100
{
public:
    ModbusMasterRTUoverTCPW5100(ModbusTCPSlaveServer table[], int size);

protected:
    virtual void checkAnswer();
    virtual byte validateRequest();
    virtual void getRxBuffer();
    virtual void sendTxBuffer();
};