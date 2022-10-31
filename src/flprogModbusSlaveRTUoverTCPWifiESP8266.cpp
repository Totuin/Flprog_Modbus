#include "flprogModbusSlaveRTUoverTCPWifiESP8266.h"


ModbusSlaveRTUoverTCPWifiESP8266::ModbusSlaveRTUoverTCPWifiESP8266(WiFiServer *server)
{
    this->server = server;
}

void ModbusSlaveRTUoverTCPWifiESP8266::begin()
{
    server->begin();
}

void ModbusSlaveRTUoverTCPWifiESP8266::pool()
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