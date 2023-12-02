#pragma once
#include "flprogModbusUtilites.h"

class ModbusTable
{
public:
    ModbusTable(uint8_t _table, uint16_t dataSize, uint16_t startAddres = 0);
    ModbusTable(uint8_t _table, uint16_t dataSize, int *_adresses);

    void init(int16_t dataSize, uint16_t startAddres);

    void init(int16_t dataSize, int *_adresses);

    void init(int16_t dataSize);

    int16_t indexForAddres(int16_t addr);
    int16_t adressForIndex(int16_t index);

    int16_t firstWriteAddress();
    int16_t writeRegsSize(int16_t startAddres);
    int16_t readRegsSize(int16_t startAddres);
    int16_t findNextStartAddres(int16_t result);
    int16_t getMinAdress();
    int16_t getMaxAdress();
    void resetWriteFlag(int16_t addres);
    bool hasAdress(int16_t address);
    void setData(int16_t adress, bool value);
    void setData(int16_t adress, int16_t value);
    void writeRegister(int16_t adress, bool value);
    void writeRegister(int16_t adress, int16_t value);
    int16_t readWorldRegister(int16_t adress);
    bool readBoolRegister(int16_t adress);
    uint8_t tableType() { return _table; };

private:
    int16_t recursiveWriteRegsSize(int16_t startAddres, int16_t result);
    int16_t recursiveReadRegsSize(int16_t startAddres, int16_t result);

    int16_t _minAdress = -1;
    int16_t _maxAdress = -1;
    uint8_t _table = 0;
    int16_t _tableSize = 0;
    int16_t _startAddres = -1;
    int *_adresses = 0;
    int16_t *_worldData;
    bool *_boolData;
    bool *_sendRegisters;
};

class ModbusMainData
{
public:
    ModbusMainData(){};
    void setDataTable(ModbusTable *_table);
    void setDataTable(uint8_t _table, uint16_t dataSize, int *_adresses);
    void configDataTable(uint8_t _table, uint16_t dataSize, int16_t startAddres = 0);

    void setIntOrder(uint8_t order);
    void setLongOrder(uint8_t order);
    void setFloatOrder(uint8_t order);
    void setUnsignedlongOrder(uint8_t order);
    int16_t indexForAddres(int16_t addr, uint8_t table);
    bool checkModbusAddres(int16_t addr, uint8_t table);
    bool checkModbusRange(int16_t startAddr, int16_t addrNumber, uint8_t table);
    void saveLong(int32_t value, uint8_t table, int16_t startAddres);
    void saveUnsignedLong(uint32_t value, uint8_t table, int16_t startAddres);
    void saveFloat(float value, uint8_t table, int16_t startAddres);
    void saveInteger(int16_t value, uint8_t table, int16_t startAddres);
    void saveByte(uint8_t value, uint8_t table, int16_t startAddres);
    void saveBool(bool value, uint8_t table, int16_t startAddres);
    uint8_t readByte(uint8_t table, int16_t startAddres);
    int16_t readInteger(uint8_t table, int16_t startAddres);
    float readFloat(uint8_t table, int16_t startAddres);
    int32_t readLong(uint8_t table, int16_t startAddres);
    uint32_t readUnsignedLong(uint8_t table, int16_t startAddres);
    bool readBool(uint8_t table, int16_t startAddres);
    bool isSupportFunction(uint8_t function);
    ModbusTable *tableForType(uint8_t table);
    bool hasTable(uint8_t table);

protected:
    ModbusTable *_tableCoil = 0;
    ModbusTable *_tableDiscreteInput = 0;
    ModbusTable *_tableInputRegistr = 0;
    ModbusTable *_tableHoldingRegistr = 0;
    uint8_t _intOrder = 1;
    uint8_t _longOrder = 1;
    uint8_t _floatOrder = 1;
    uint8_t _unsignedlongOrder = 1;
    void saveForByteWithOrder(unsigned char *sourse, uint8_t table, int16_t startAddres, uint8_t order);
    void modbusOrderAray(uint8_t *orderArray, uint8_t order);
    void sortTwoWordRegistors(uint8_t *orderArray, uint8_t order, uint8_t *data, uint16_t w1, uint16_t w2);
    void readForByteWithOrder(unsigned char *sourse, uint8_t table, int16_t startAddres, uint8_t order);
    ModbusTable *tableForStartArddres(uint8_t table, int16_t startAddres, bool isTwoWord);
    virtual bool canSaveTable(uint8_t table);
};

class ModbusSlaveInMaster : public ModbusMainData
{
public:
    ModbusSlaveInMaster(){};
    ModbusSlaveInMaster(uint8_t addr) { setSlaveAddress(addr); };
    uint8_t getLastError();

    void status(bool status);
    bool status() { return _isActive; };

    void lastReqest(uint32_t time) { _lastReqestTime = time; };
    uint32_t lastReqest() { return _lastReqestTime; };

    bool isInit() { return _initFlag; };
    void setInit() { _initFlag = true; };
    void resetInit() { _initFlag = false; };

    void setPollingPeriod(uint32_t period);
    void setTimeOutTime(uint32_t time);
    uint32_t getTimeOutTime();
    bool hasWriteRegisters();
    void setLastError(uint8_t error);
    ModbusTable *firstWriteTable();
    ModbusTable *firstTabe();
    ModbusTable *lastTable();
    int16_t nextTable(int16_t table);
    ModbusTable *nextTable(ModbusTable *table);
    bool isReady();
    void setSlaveAddress(uint8_t adr);
    uint8_t slaveAddres() { return _slaveAddres; };

protected:
    bool _isActive = true;
    uint32_t _lastReqestTime;
    bool _initFlag = false;
    uint8_t _slaveAddres = 1;
    uint8_t _lastError;
    uint32_t _pollingPeriod = 1000;
    uint32_t _timeOutTime = 1000;
    virtual bool canSaveTable(uint8_t table);
};