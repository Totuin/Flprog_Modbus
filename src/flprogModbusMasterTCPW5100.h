#pragma once
#include "Arduino.h"
#include "flprogModbusMasterTCP.h"
#include "Ethernet.h"
#include "SPI.h"


class ModbusTCPW5100SlaveServer : public ModbusTCPSlaveServer
{
public:
    ModbusTCPW5100SlaveServer(){};
    virtual void connect();
    virtual byte available();
    virtual byte read();
    virtual void stop();
    virtual void write(byte buffer[], byte buferSize);

private:
    EthernetClient w5100Client;
};

class ModbusMasterTCPW5100 : public ModbusMasterTCP
{
public:
    ModbusMasterTCPW5100(ModbusTCPSlaveServer table[], int size);
    ModbusMasterTCPW5100(){};

protected:
private:
};
