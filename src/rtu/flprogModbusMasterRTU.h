
#pragma once
#include "flprogModbusRTU.h"

class ModbusMasterRTU : public ModbusRTU
{
public:
    ModbusMasterRTU(){};
    ModbusMasterRTU(uint8_t portNumber, uint8_t size);

    void pool();
    void begin();

    void setDataTable(uint8_t slaveAddres, ModbusTable *table);
    void setDataTable(uint8_t slaveAddres, uint8_t _table, int16_t dataSize, int *_adresses);
    void configDataTable(uint8_t slaveAddres, uint8_t _table, int16_t dataSize);
    void configDataTable(uint8_t slaveAddres, uint8_t _table, int16_t dataSize, int16_t _startAdr);

    void setSlaveAddress(uint8_t slaveIndex, uint8_t adr);
    bool hasSlaveOnIndex(uint8_t slaveIndex);
    ModbusSlaveInMaster *slaveOnIndex(uint8_t slaveIndex);
    ModbusSlaveInMaster *slave(uint8_t adr);

    void saveLong(uint8_t slaveAddres, int32_t val, uint8_t table, int16_t adr, bool isIndex = false);
    void saveUnsignedLong(uint8_t slaveAddres, uint32_t val, uint8_t table, int16_t adr, bool isIndex = false);
    void saveFloat(uint8_t slaveAddres, float val, uint8_t table, int16_t adr, bool isIndex = false);
    void saveInteger(uint8_t slaveAddres, int16_t val, uint8_t table, int16_t adr, bool isIndex = false);
    void saveByte(uint8_t slaveAddres, uint8_t val, uint8_t table, int16_t adr, bool isIndex = false);
    void saveBool(uint8_t slaveAddres, bool val, uint8_t table, int16_t adr, bool isIndex = false);

    uint8_t readByte(uint8_t slaveAddres, uint8_t table, int16_t adr, bool isIndex = false);
    int16_t readInteger(uint8_t slaveAddres, uint8_t table, int16_t adr, bool isIndex = false);
    float readFloat(uint8_t slaveAddres, uint8_t table, int16_t ad, bool isIndex = false);
    int32_t readLong(uint8_t slaveAddres, uint8_t table, int16_t adr, bool isIndex = false);
    uint32_t readUnsignedLong(uint8_t slaveAddres, uint8_t table, int16_t adr, bool isIndex = false);
    bool readBool(uint8_t slaveAddres, uint8_t table, int16_t adr, bool isIndex = false);

    void setPollingPeriod(uint8_t slaveAddres, uint32_t period, bool isIndex = false);
    void setTimeOutTime(uint8_t slaveAddres, uint32_t time, bool isIndex = false);
    void setLongOrder(uint8_t slaveAddres, uint8_t order, bool isIndex = false);
    void setFloatOrder(uint8_t slaveAddres, uint8_t order, bool isIndex = false);
    void setUnsignedlongOrder(uint8_t slaveAddres, uint8_t order, bool isIndex = false);
    void setIntOrder(uint8_t slaveAddres, uint8_t order, bool isIndex = false);

    uint8_t getLastError(uint8_t slaveAddres, bool isIndex = false);
    void status(uint8_t slaveAddres, bool status, bool isIndex = false);

protected:
    ModbusSlaveInMaster *slaves;
    uint8_t slavesSize = 5;
    ModbusSlaveInMaster *_currentSlave;
    ModbusTable *_currentSlaveTable;
    ModbusSlaveInMaster *_telegrammSlave;
    ModbusTable *_telegrammTable;
    uint8_t _telegrammFunction;
    int16_t _currentSlaveStartAddress = -1;
    int16_t _currentSlaveLastAddress = -1;
    int16_t _telegrammStartAddres;
    int16_t _telegrammNumbeRegs;
    void checkAnswer();
    void nextQuery();
    uint8_t validateRequest();
    bool createNewTelegramm();
    bool createWriteTelegramm();
    bool createReadTelegram();
    bool hasWriteRegisters();
    ModbusSlaveInMaster *firstWriteSlave();
    ModbusSlaveInMaster *slaveForAddress(int16_t adress);
    bool nextRegistor();
    ModbusSlaveInMaster *firstReadySlave();
    ModbusSlaveInMaster *nextSlave(ModbusSlaveInMaster *currentTempSlave);
    bool hasReadySlave();
    bool nextSlave();
    bool nextTable();
    void sendQuery();
    bool nextStep();
};