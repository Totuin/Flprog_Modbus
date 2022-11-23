#include "flprogModbusMasterTCPW5100.h"

#ifndef ESP32
// ModbusTCPW5100Client*****************
void ModbusTCPW5100SlaveServer::connect()
{
    
    if (w5100Client.connected())
    {
        
        return;
    }
    w5100Client.connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth), port);
}

void ModbusTCPW5100SlaveServer::write(byte buffer[], byte buferSize)
{
    if (!w5100Client.connected())
    {
        connect();
    }
    if (w5100Client.connected())
    {
        w5100Client.write(buffer, buferSize);
       
    }
}

void ModbusTCPW5100SlaveServer::stop()
{
    w5100Client.stop();
  
}

byte ModbusTCPW5100SlaveServer::read()
{
    return w5100Client.read();
}

byte ModbusTCPW5100SlaveServer::available()
{
    return w5100Client.available();
}

// ModbusMasterTCPW5100********************
ModbusMasterTCPW5100::ModbusMasterTCPW5100(ModbusTCPSlaveServer table[], int size)
{
    serversSize = size;
    servers = table;
}

#endif
