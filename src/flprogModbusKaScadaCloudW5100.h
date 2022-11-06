#pragma once
#include "Arduino.h"
#ifndef ESP32
#include "flprogModbusSlaveTCP.h"

#include "Ethernet.h"
#include "SPI.h"

class ModbusKaScadaCloudW5100 : public ModbusSlaveTCP
{
public:
    void setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    void setKaScadaCloudPort(int port);
    void setKaScadaCloudDevceId(String id);
    virtual void begin();
    virtual void pool();

protected:
    void getRxBuffer();
    virtual void sendTxBuffer();

private:
    int port = 25000;
    String deniceId;
    IPAddress kaScadaCloudIp = IPAddress(94, 250, 249, 225);
    unsigned long kaScadaCloudTimeOutStartTime;
    EthernetClient client;
};
#endif