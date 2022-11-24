#include "flprogModbusMasterTCPWifiESP32.h"

#ifdef ESP32
// ModbusTCPWifiESP32SlaveServer*****************
void ModbusMasterTCPWifiESP32::connect(ModbusTCPSlaveServer *server)
{
    if (client.connected())
    {
        return;
    }
    client.connect(IPAddress(server->getIp(0), server->getIp(1), server->getIp(2), server->getIp(3)), server->getPort());
}

void ModbusMasterTCPWifiESP32::write(ModbusTCPSlaveServer *server, byte buffer[], byte buferSize)
{
    if (!client.connected())
    {
        connect(server);
    }
    if (client.connected())
    {
        client.write(buffer, buferSize);
    }
}

void ModbusMasterTCPWifiESP32::stop()
{
    client.stop();
}

byte ModbusMasterTCPWifiESP32::read()
{
    return client.read();
}

byte ModbusMasterTCPWifiESP32::available()
{
    return client.available();
}

// ModbusMasterTCPWifiESP32********************
ModbusMasterTCPWifiESP32::ModbusMasterTCPWifiESP32(ModbusTCPSlaveServer table[], int size)
{
    serversSize = size;
    servers = table;
}

#endif
