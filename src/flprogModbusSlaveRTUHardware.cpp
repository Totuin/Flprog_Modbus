#include "flprogModbusSlaveRTUHardware.h"

void ModbusSlaveRTUHardware::begin(byte addres, HardwareSerial *port)
{

    this->port = port;
    this->port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    slaveAddres = addres;
    bufferSize = 0;
    lastRec = 0;
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
}

#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
void ModbusSlaveRTUHardware::begin(byte addres, USBSerial *port)
{

    this->usbPort = port;
    isUSB = true;
    this->usbPort->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    slaveAddres = addres;
    bufferSize = 0;
    lastRec = 0;
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
}
#endif

void ModbusSlaveRTUHardware::restartPort()
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    if (isUSB)
    {
        usbPort->end();
        usbPort->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    }
    else
    {
        port->end();
        port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    }
#else
    port->end();
    port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
#endif
}

byte ModbusSlaveRTUHardware::portAvailable()
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    if (isUSB)
    {
        return usbPort->available();
    }
    else
    {
        return port->available();
    }
#else
    return port->available();
#endif
}

byte ModbusSlaveRTUHardware::portRead()
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    if (isUSB)
    {
        return usbPort->read();
    }
    else
    {
        return port->read();
    }
#else
    return port->read();
#endif
}

byte ModbusSlaveRTUHardware::portWrite(byte *buffer, byte size)
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    if (isUSB)
    {
        return usbPort->write(buffer, size);
    }
    else
    {
        return port->write(buffer, size);
    }
#else
    return port->write(buffer, size);
#endif
}

bool ModbusSlaveRTUHardware::hasPort()
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    if (isUSB)
    {
        return !(usbPort == 0);
    }
    else
    {
        return !(port == 0);
    }
#else
    return !(port == 0);
#endif
}
