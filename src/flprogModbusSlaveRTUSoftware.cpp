#include "flprogModbusSlaveRTUSoftware.h"
#ifndef ESP32
void ModbusSlaveRTUSoftware::begin(byte addres, uint8_t receivePin, uint8_t transmitPin)
{
    port = new SoftwareSerial(receivePin, transmitPin);
    port->begin(flprog::speedFromCode(portSpeed));
    slaveAddres = addres;
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
}

void ModbusSlaveRTUSoftware::restartPort()
{
    port->end();
    port->begin(flprog::speedFromCode(portSpeed));
}

byte ModbusSlaveRTUSoftware::portAvailable()
{
    return port->available();
}

byte ModbusSlaveRTUSoftware::portRead()
{
    return port->read();
}

byte ModbusSlaveRTUSoftware::portWrite(byte *buffer, byte size)
{
    return port->write(buffer, size);
}

bool ModbusSlaveRTUSoftware::hasPort()
{
    return !(port == 0);
}
#endif