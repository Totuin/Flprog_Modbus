#include "flprogModbusSlaveTCP.h"

void ModbusSlaveTCP::pool(){

}

void ModbusSlaveTCP::setSlaveAddress(byte address)
{
    slaveAddres = address;
}

void ModbusSlaveTCP::setData(ModbusMainData *data)
{
    this->data = data;
}

void ModbusSlaveTCP::saveLong(long value, byte table, int startAddres)
{
    data->saveLong(value, table, startAddres);
}

void ModbusSlaveTCP::saveUnsignedLong(unsigned long value, byte table, int startAddres)
{
    data->saveUnsignedLong(value, table, startAddres);
}

void ModbusSlaveTCP::saveFloat(float value, byte table, int startAddres)
{
    data->saveFloat(value, table, startAddres);
}

void ModbusSlaveTCP::saveInteger(int value, byte table, int startAddres)
{
    data->saveInteger(value, table, startAddres);
}

void ModbusSlaveTCP::saveByte(byte value, byte table, int startAddres)
{
    data->saveByte(value, table, startAddres);
}

void ModbusSlaveTCP::saveBool(bool value, byte table, int startAddres)
{
    data->saveBool(value, table, startAddres);
}

byte ModbusSlaveTCP::readByte(byte table, int startAddres)
{
    return data->readByte(table, startAddres);
}

int ModbusSlaveTCP::readInteger(byte table, int startAddres)
{
    return data->readInteger(table, startAddres);
}

float ModbusSlaveTCP::readFloat(byte table, int startAddres)
{
    return data->readFloat(table, startAddres);
}

long ModbusSlaveTCP::readLong(byte table, int startAddres)
{
    return data->readLong(table, startAddres);
}

unsigned long ModbusSlaveTCP::readUnsignedLong(byte table, int startAddres)
{
    return data->readUnsignedLong(table, startAddres);
}

bool ModbusSlaveTCP::readBool(byte table, int startAddres)
{
    return data->readBool(table, startAddres);
}

byte ModbusSlaveTCP::getLastError()
{
    return lastError;
}