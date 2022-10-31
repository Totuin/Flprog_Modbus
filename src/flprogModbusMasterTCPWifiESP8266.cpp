#include "flprogModbusMasterTCPWifiESP8266.h"


#ifdef ESP8266
// ModbusTCPWifiESP8266SlaveServer*****************
void ModbusTCPWifiESP8266SlaveServer::connect()
{
    if (client.connected())
    {
        return;
    }
    client.connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth), port);
}

void ModbusTCPWifiESP8266SlaveServer::write(byte buffer[], byte buferSize)
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

void ModbusTCPWifiESP8266SlaveServer::stop()
{
    client.stop();
}

byte ModbusTCPWifiESP8266SlaveServer::read()
{
    return client.read();
}

byte ModbusTCPWifiESP8266SlaveServer::available()
{
    return client.available();
}

// ModbusMasterTCPW5100********************
ModbusMasterTCPWifiESP8266::ModbusMasterTCPWifiESP8266(ModbusTCPSlaveServer table[], int size)
{
    serversSize = size;
    servers = table;
}

#endif
