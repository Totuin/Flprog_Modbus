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

void ModbusSlaveRTUHardware::restartPort()
{
    port->end();
    port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
}

byte ModbusSlaveRTUHardware::portAvailable()
{
    return port->available();
}

byte ModbusSlaveRTUHardware::portRead()
{
    return port->read();
}

byte ModbusSlaveRTUHardware::portWrite(byte *buffer, byte size)
{
    return port->write(buffer, size);
}

bool ModbusSlaveRTUHardware::hasPort()
{
    return !(port == 0);
}
