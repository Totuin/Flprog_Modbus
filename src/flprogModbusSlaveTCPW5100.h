#pragma once
#include "Arduino.h"
#ifndef ESP32
#include "flprogModbusSlaveTCP.h"
#include "Ethernet.h"
#include "SPI.h"

class ModbusSlaveTCPW5100 : public ModbusSlaveTCP
{
public:
    ModbusSlaveTCPW5100(EthernetServer *server);
    virtual void begin();
    virtual void pool();

protected:
    void getRxBuffer();
    virtual void sendTxBuffer();
    
   

private:
    EthernetServer *server;
    EthernetClient client;
};

#endif