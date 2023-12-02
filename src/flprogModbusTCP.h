#pragma once
#include "flprogEthernet.h"
#include "base/flprogModbus.h"

class ModbusTCP : public Modbus
{

protected:
    uint8_t _mbapBuffer[6];
    FLProgAbstractTcpInterface *_interface = 0;
};

#include "tcp/flprogModbusSlaveTCP.h"
#include "tcp/flprogModbusMasterTCP.h"
