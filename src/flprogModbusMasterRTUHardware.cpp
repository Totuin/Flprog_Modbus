#include "flprogModbusMasterRTUHardware.h"

ModbusMasterRTUHardware::ModbusMasterRTUHardware(ModbusSlaveInMaster table[], int size)
{
    slavesSize = size;
    slaves = table;
}

void ModbusMasterRTUHardware::restartPort()
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

void ModbusMasterRTUHardware::begin(HardwareSerial *port)
{
    this->port = port;
    this->port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
}

#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
void ModbusMasterRTUHardware::begin(USBSerial *port)
{
    this->usbPort = port;
    isUSB = true;
    this->usbPort->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
}
#endif

byte ModbusMasterRTUHardware::portAvailable()
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

byte ModbusMasterRTUHardware::portRead()
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

byte ModbusMasterRTUHardware::portWrite(byte *buffer, byte size)
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

bool ModbusMasterRTUHardware::hasPort()
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