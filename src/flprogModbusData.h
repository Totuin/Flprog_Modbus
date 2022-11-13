#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"

#define COIL 0
#define DISCRETE_INPUT 1
#define INPUT_REGISTR 3
#define HOLDING_REGISTR 4
#define AB_ORDER 1
#define BA_ORDER 2
#define ABCD_ORDER 1
#define CDAB_ORDER 2
#define BADC_ORDER 3
#define DCBA_ORDER 4

class ModbusTable
{
public:
    byte table;
    int tableSize;
    int *adresses;
    bool *sendRegisters;
    int indexForAddres(int addr);
    int firstWriteAddress();
    int writeRegsSize(int startAddres);
    int readRegsSize(int startAddres);
    int findNextStartAddres(int result);
    int getMinAdress();
    int getMaxAdress();
    void resetWriteFlag(int addres);
    bool hasAdress(int address);
    virtual void setData(int address, bool value){};
    virtual void setData(int address, uint16_t value){};
    virtual void writeRegister(int addres, bool value){};
    virtual void writeRegister(int addres, int value){};
    virtual int readWorldRegister(int addres) { return 0; };
    virtual bool readBoolRegister(int addres) { return false; };

private:
protected:
    int minAdress = -1;
    int maxAdress = -1;
    int recursiveWriteRegsSize(int startAddres, int result);
    int recursiveReadRegsSize(int startAddres, int result);
};

class ModbusWorldTable : public ModbusTable
{
public:
    ModbusWorldTable(byte table, int data[], int adresses[], int tableSize);
    ModbusWorldTable(byte table, int data[], int adresses[], bool sendRegisters[], int tableSize);
    virtual void setData(int index, uint16_t value);
    virtual void writeRegister(int addres, int value);
    virtual int readWorldRegister(int addres);
    int *data;

private:
};

class ModbusBoolTable : public ModbusTable
{
public:
    ModbusBoolTable(byte table, bool data[], int adresses[], int tableSize);
    ModbusBoolTable(byte table, bool data[], int adresses[], bool sendRegisters[], int tableSize);
    virtual void setData(int index, bool value);
    virtual void writeRegister(int addres, bool value);
    virtual bool readBoolRegister(int addres);
    bool *data;

private:
};

class ModbusMainData
{
public:
    void setDataTable(ModbusWorldTable *table);
    void setDataTable(ModbusBoolTable *table);
    void setIntOrder(byte order);
    void setLongOrder(byte order);
    void setFloatOrder(byte order);
    void setUnsignedlongOrder(byte order);
    int indexForAddres(int addr, byte table);
    bool checkModbusAddres(int addr, byte table);
    bool checkModbusRange(int startAddr, int addrNumber, byte table);
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
    bool isSupportFunction(byte function);
    ModbusTable *tableForType(byte table);
    bool hasTable(byte table);
    bool existsTables[4] = {0, 0, 0, 0};

protected:
    ModbusBoolTable *table_0;
    ModbusBoolTable *table_1;
    ModbusWorldTable *table_3;
    ModbusWorldTable *table_4;
    byte intOrder = 1;
    byte longOrder = 1;
    byte floatOrder = 1;
    byte unsignedlongOrder = 1;
    bool supported[8];
    void modbusOrderAray(byte orderArray[], byte order);
    void sortTwoWordRegistors(byte orderArray[], byte order, byte data[], uint16_t w1, uint16_t w2);
    virtual bool canSaveTable(byte table);
};

class ModbusSlaveInMaster : public ModbusMainData
{
public:
    ModbusSlaveInMaster(){};
    bool isActive = true;
    unsigned long lastReqestTime;
    bool initFlag = false;
    byte slaveAddres;
    byte getLastError();
    void status(bool status);
    bool status() { return isActive; };
    void setPollingPeriod(unsigned long period);
    void setTimeOutTime(unsigned long time);
    unsigned long getTimeOutTime();
    bool hasWriteRegisters();
    void setLastError(byte error);
    ModbusTable *firstWriteTable();
    ModbusTable *firstTabe();
    int nextTable(int table);
    ModbusTable *nextTable(ModbusTable *table);
    bool isReady();
    void setSlaveAddress(byte address);

private:
protected:
    byte lastError;
    unsigned long pollingPeriod = 1000;
    unsigned long timeOutTime = 1000;
    virtual bool canSaveTable(byte table);
};
