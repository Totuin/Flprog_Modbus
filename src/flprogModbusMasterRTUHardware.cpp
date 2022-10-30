#include "flprogModbusMasterRTUHardware.h"

ModbusMasterRTUHardware::ModbusMasterRTUHardware(ModbusSlaveInMaster table[], int size)
{
    slavesSize = size;
    slaves = table;
}

void ModbusMasterRTUHardware::restartPort()
{
    port->end();
    port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
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

byte ModbusMasterRTUHardware::portAvailable()
{
    return port->available();
}

byte ModbusMasterRTUHardware::portRead()
{
    return port->read();
}

byte ModbusMasterRTUHardware::portWrite(byte *buffer, byte size)
{
    return port->write(buffer, size);
}

bool ModbusMasterRTUHardware::hasPort()
{
    return !(port == 0);
}