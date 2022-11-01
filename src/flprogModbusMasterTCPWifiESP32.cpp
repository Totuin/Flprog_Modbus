#include "flprogModbusMasterTCPWifiESP32.h"

#ifdef ESP32
// ModbusTCPWifiESP32SlaveServer*****************
void ModbusTCPWifiESP32SlaveServer::connect()
{
    if (client.connected())
    {
        return;
    }
    client.connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth), port);
}

void ModbusTCPWifiESP32SlaveServer::write(byte buffer[], byte buferSize)
{
    if (!client.connected())
    {
        connect();
    }
    if (client.connected())
    {
        client.write(buffer, buferSize);
    }
}

void ModbusTCPWifiESP32SlaveServer::stop()
{
    client.stop();
}

byte ModbusTCPWifiESP32SlaveServer::read()
{
    return client.read();
}

byte ModbusTCPWifiESP32SlaveServer::available()
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
