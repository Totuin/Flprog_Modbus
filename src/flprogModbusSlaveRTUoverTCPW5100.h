#pragma once
#include "Arduino.h"
#ifndef ESP32
#include "flprogModbusSlaveRTU.h"
#include "Ethernet.h"
#include "SPI.h"


class ModbusSlaveRTUoverTCPW5100 : public ModbusSlaveRTU
{

    public:
    ModbusSlaveRTUoverTCPW5100(EthernetServer *server);
    virtual void begin();
    virtual void pool();
protected:

    virtual byte portAvailable(){return client.available();};
    virtual byte portRead(){return client.read();};
    virtual byte portWrite(byte *buffer, byte size){return client.write(buffer,size);};
    virtual bool hasPort(){return true;};


    private:
    EthernetServer *server;
    EthernetClient client;

};
#endif