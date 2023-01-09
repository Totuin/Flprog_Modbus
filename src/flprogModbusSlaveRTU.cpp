#include "flprogModbusSlaveRTU.h"

void ModbusSlaveRTU::begin(byte addres){
    uart->begin();
    slaveAddres = addres;
    bufferSize = 0;
    lastRec = 0;
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
}

void ModbusSlaveRTU::pool()
{
    if (workStatus == MODBUS_WAITING_SENDING)
    {
        if ((flprog::isTimer(startSendTime, timeOfSend)))
        {
            workStatus = MODBUS_READY;
            offPeDePin();
        }
        else
        {
            return;
        }
    }
    if (!(checkAvalibleBytes()))
        return;
    byte state = rxBuffer();
    if (state < 7)
    {
        return;
    }
    executeSlaveReqest(data);
}

byte ModbusSlaveRTU::validateRequest()
{

    int msgCRC =
        ((buffer[bufferSize - 2] << 8) | buffer[bufferSize - 1]);
    if (flprogModus::modbusCalcCRC(bufferSize - 2, buffer) != msgCRC)
    {
        return 255;
    }
    return validateSlaveReqest(data);
}

void ModbusSlaveRTU::setSlaveAddress(byte address)
{
    slaveAddres = address;
}

void ModbusSlaveRTU::setData(ModbusMainData *data)
{
    this->data = data;
}

void ModbusSlaveRTU::saveLong(long value, byte table, int startAddres)
{
    data->saveLong(value, table, startAddres);
}

void ModbusSlaveRTU::saveUnsignedLong(unsigned long value, byte table, int startAddres)
{
    data->saveUnsignedLong(value, table, startAddres);
}

void ModbusSlaveRTU::saveFloat(float value, byte table, int startAddres)
{
    data->saveFloat(value, table, startAddres);
}

void ModbusSlaveRTU::saveInteger(int value, byte table, int startAddres)
{
    data->saveInteger(value, table, startAddres);
}

void ModbusSlaveRTU::saveByte(byte value, byte table, int startAddres)
{
    data->saveByte(value, table, startAddres);
}

void ModbusSlaveRTU::saveBool(bool value, byte table, int startAddres)
{
    data->saveBool(value, table, startAddres);
}

byte ModbusSlaveRTU::readByte(byte table, int startAddres)
{
    return data->readByte(table, startAddres);
}

int ModbusSlaveRTU::readInteger(byte table, int startAddres)
{
    return data->readInteger(table, startAddres);
}

float ModbusSlaveRTU::readFloat(byte table, int startAddres)
{
    return data->readFloat(table, startAddres);
}

long ModbusSlaveRTU::readLong(byte table, int startAddres)
{
    return data->readLong(table, startAddres);
}

unsigned long ModbusSlaveRTU::readUnsignedLong(byte table, int startAddres)
{
    return data->readUnsignedLong(table, startAddres);
}

bool ModbusSlaveRTU::readBool(byte table, int startAddres)
{
    return data->readBool(table, startAddres);
}

byte ModbusSlaveRTU::getLastError()
{
    return lastError;
}