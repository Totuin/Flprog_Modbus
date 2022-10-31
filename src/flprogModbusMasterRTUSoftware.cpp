#include "flprogModbusMasterRTUSoftware.h"

#ifndef ESP32

ModbusMasterRTUSoftware::ModbusMasterRTUSoftware(ModbusSlaveInMaster table[], int size)
{
    slavesSize = size;
    slaves = table;
}

void ModbusMasterRTUSoftware::begin(uint8_t receivePin, uint8_t transmitPin)
{
    port = new SoftwareSerial(receivePin, transmitPin);
    port->begin(flprog::speedFromCode(portSpeed));
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
}

void ModbusMasterRTUSoftware::restartPort()
{
    port->end();
    port->begin(flprog::speedFromCode(portSpeed));
}

byte ModbusMasterRTUSoftware::portAvailable()
{
    return port->available();
}

byte ModbusMasterRTUSoftware::portRead()
{
    return port->read();
}

byte ModbusMasterRTUSoftware::portWrite(byte *buffer, byte size)
{
    return port->write(buffer, size);
}

bool ModbusMasterRTUSoftware::hasPort()
{
    return !(port == 0);
}

#endif