#pragma once
#include "Arduino.h"
#include "flprogModbusSlaveRTU.h"
#include "HardwareSerial.h"

class ModbusSlaveRTUHardware : public ModbusSlaveRTU
{
public:
    virtual void begin(byte addres, HardwareSerial *port);
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    virtual void begin(byte addres, USBSerial *port);
#endif

protected:
    HardwareSerial *port;
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    bool isUSB = false;
    USBSerial *usbPort;
#endif
    virtual void restartPort();
    virtual byte portAvailable();
    virtual byte portRead();
    virtual byte portWrite(byte *buffer, byte size);
    virtual bool hasPort();
};