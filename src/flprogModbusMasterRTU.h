
#pragma once
#include "Arduino.h"
#include "flprogModbusRTU.h"

class ModbusMasterRTU : public ModbusRTU
{
public:
    ModbusMasterRTU(ModbusSlaveInMaster table[], int size);
    void pool();
    void begin();
    void saveLong(int slave, long value, byte table, int startAddres);
    void saveUnsignedLong(int slave, unsigned long value, byte table, int startAddres);
    void saveFloat(int slave, float value, byte table, int startAddres);
    void saveInteger(int slave, int value, byte table, int startAddres);
    void saveByte(int slave, byte value, byte table, int startAddres);
    void saveBool(int slave, bool value, byte table, int startAddres);
    byte readByte(int slave, byte table, int startAddres);
    int readInteger(int slave, byte table, int startAddres);
    float readFloat(int slave, byte table, int startAddres);
    long readLong(int slave, byte table, int startAddres);
    unsigned long readUnsignedLong(int slave, byte table, int startAddres);
    bool readBool(int slave, byte table, int startAddres);
    bool hasSlave(int slaveIndex);
    void setDataTable(int slave, ModbusWorldTable *table);
    void setDataTable(int slave, ModbusBoolTable *table);
    void setSlaveAddress(int slave, byte address);
    void setPollingPeriod(int slave, unsigned long period);
    void setLongOrder(int slave, byte order);
    void setFloatOrder(int slave, byte order);
    void setUnsignedlongOrder(int slave, byte order);
    void setIntOrder(int slave, byte order);
    byte getLastError(int slave);
    void status(int slave, bool status);

protected:
    ModbusSlaveInMaster *slaves;
    int slavesSize;
    ModbusSlaveInMaster *currentSlave;
    ModbusTable *currentSlaveTable;
    ModbusSlaveInMaster *telegrammSlave;
    ModbusTable *telegrammTable;
    byte telegrammFunction;
    int currentSlaveStartAddress = -1;
    int currentSlaveLastAddress = -1;
    int telegrammStartAddres;
    int telegrammNumbeRegs;
    void checkAnswer();
    void nextQuery();
    byte validateRequest();
    bool createNewTelegramm();
    bool createWriteTelegramm();
    bool createReadTelegram();
    bool hasWriteRegisters();
    ModbusSlaveInMaster *firstWriteSlave();
    ModbusSlaveInMaster *slaveForAddress(int adress);
    bool nextRegistor();
    ModbusSlaveInMaster *firstReadySlave();
    ModbusSlaveInMaster *nextSlave(ModbusSlaveInMaster *currentTempSlave);
    bool hasReadySlave();
    bool nextSlave();
    bool nextTable();
    void sendQuery();
    bool nextStep();
};