#include "flprogModbusSlaveRTUoverTCPWifiESP32.h"

#ifdef ESP32
ModbusSlaveRTUoverTCPWifiESP32::ModbusSlaveRTUoverTCPWifiESP32(WiFiServer *server)
{
    this->server = server;
}

void ModbusSlaveRTUoverTCPWifiESP32::begin()
{
    server->begin();
}

void ModbusSlaveRTUoverTCPWifiESP32::pool()
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
#endif