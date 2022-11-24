#include "flprogModbusMasterTCPW5100.h"

#ifndef ESP32

void ModbusMasterTCPW5100::connect(ModbusTCPSlaveServer *server)
{
    
    if (w5100Client.connected())
    {
        
        return;
    }
    w5100Client.connect(IPAddress(server->getIp(0), server->getIp(1), server->getIp(2), server->getIp(3)), server->getPort());
}

void ModbusMasterTCPW5100::write(ModbusTCPSlaveServer *server, byte buffer[], byte buferSize)
{
    if (!w5100Client.connected())
    {
        connect(server);
    }
    if (w5100Client.connected())
    {
        w5100Client.write(buffer, buferSize);
       
    }
}

void ModbusMasterTCPW5100::stop()
{
    w5100Client.stop();
  
}

byte ModbusMasterTCPW5100::read()
{
    return w5100Client.read();
}

byte ModbusMasterTCPW5100::available()
{
    return w5100Client.available();
}


ModbusMasterTCPW5100::ModbusMasterTCPW5100(ModbusTCPSlaveServer table[], int size)
{
    serversSize = size;
    servers = table;
}

#endif
