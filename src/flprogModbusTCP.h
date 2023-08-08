#pragma once

#include "tcp/tcpDevice/flprogTcpDevice.h"
#include "base/flprogModbus.h"

class ModbusTCP : public Modbus
{

protected:
    uint8_t mbapBuffer[6];
    FLProgTcpDevice *tcpDevice;
};

#include "tcp/flprogModbusSlaveTCP.h"
#include "tcp/flprogModbusMasterTCP.h"
