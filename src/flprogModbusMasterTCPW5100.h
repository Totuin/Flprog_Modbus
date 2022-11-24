#pragma once
#include "Arduino.h"
#ifndef ESP32
#include "flprogModbusMasterTCP.h"
#include "Ethernet.h"
#include "SPI.h"




class ModbusMasterTCPW5100 : public ModbusMasterTCP
{
public:
    ModbusMasterTCPW5100(ModbusTCPSlaveServer table[], int size);
    ModbusMasterTCPW5100(){};
    virtual void connect(ModbusTCPSlaveServer *server);
    virtual byte available();
    virtual byte read();
    virtual void stop();
    virtual void write(ModbusTCPSlaveServer *server, byte buffer[], byte buferSize);

protected:
EthernetClient w5100Client;
private:
};

#endif