#pragma once
#include "Arduino.h"
#include "flprogModbusTCP.h"

class ModbusSlaveTCP : public ModbusTCP
{
public:
    virtual void pool();
    void setSlaveAddress(byte address);
    void setData(ModbusMainData *data);
    void saveLong(long value, byte table, int startAddres);
    void saveUnsignedLong(unsigned long value, byte table, int startAddres);
    void saveFloat(float value, byte table, int startAddres);
    void saveInteger(int value, byte table, int startAddres);
    void saveByte(byte value, byte table, int startAddres);
    void saveBool(bool value, byte table, int startAddres);
    byte readByte(byte table, int startAddres);
    int readInteger(byte table, int startAddres);
    float readFloat(byte table, int startAddres);
    long readLong(byte table, int startAddres);
    unsigned long readUnsignedLong(byte table, int startAddres);
    bool readBool(byte table, int startAddres);
    byte getLastError();
    virtual void setLastError(byte error) { lastError = error; };

protected:
    byte mbapBuffer[6];
    byte slaveAddres = 1;
    ModbusMainData *data;
    byte lastError = 0;
};