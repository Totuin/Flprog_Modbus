#include "flprogModbusSlaveRTUoverTCPW5100.h"

ModbusSlaveRTUoverTCPW5100::ModbusSlaveRTUoverTCPW5100(EthernetServer *server)
{
    this->server = server;
}

void ModbusSlaveRTUoverTCPW5100::begin()
{
    server->begin();
}

void ModbusSlaveRTUoverTCPW5100::pool()
{
    if (!client.connected())
    {
        client = server->available();
    }
    if (! client)
    {
        return;
    }
    ModbusSlaveRTU::pool();
}


