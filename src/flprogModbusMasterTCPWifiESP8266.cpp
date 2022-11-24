#include "flprogModbusMasterTCPWifiESP8266.h"

#ifdef ESP8266

void ModbusMasterTCPWifiESP8266::connect(ModbusTCPSlaveServer *server)
{
    if (client.connected())
    {
        return;
    }
    client.connect(IPAddress(server->getIp(0), server->getIp(1), server->getIp(2), server->getIp(3)), server->getPort());
}

void ModbusMasterTCPWifiESP8266::write(ModbusTCPSlaveServer *server, byte buffer[], byte buferSize)
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

void ModbusMasterTCPWifiESP8266::stop()
{
    client.stop();
}

byte ModbusMasterTCPWifiESP8266::read()
{
    return client.read();
}

byte ModbusMasterTCPWifiESP8266::available()
{
    return client.available();
}

ModbusMasterTCPWifiESP8266::ModbusMasterTCPWifiESP8266(ModbusTCPSlaveServer table[], int size)
{
    serversSize = size;
    servers = table;
}

#endif
