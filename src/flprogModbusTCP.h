#pragma once
#include "flprogEthernet.h"
#include "flprogWiFi.h"
#include "base/flprogModbus.h"

class ModbusTCP : public Modbus
{

protected:
    uint8_t mbapBuffer[6];
    FLProgAbstractTcpInterface *interface = 0;
};

#include "tcp/flprogModbusSlaveTCP.h"
#include "tcp/flprogModbusMasterTCP.h"
