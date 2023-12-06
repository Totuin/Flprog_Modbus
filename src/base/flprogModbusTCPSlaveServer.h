#pragma once
#include "flprogModbusData.h"

class ModbusTCPSlaveServer
{
public:
    ModbusTCPSlaveServer(){};
    void setSlaves(ModbusSlaveInMaster *_table, uint8_t size);
    void setSlavesSize(uint8_t size);

    void setPort(int16_t serverPort) { _port = serverPort; };
    void setIpAdress(IPAddress ip);
    void setHost(String host);
    void setSlaveAddress(uint8_t slaveIndex, uint16_t addr);

    void setMode(uint8_t mode);
    uint8_t mode() { return _mode; };

    IPAddress getIp() { return _serverIp; };
    char *getHost() { return _serverHost; };
    bool serverAsHost() { return _serverAsHost; };

    bool hasSlave(uint8_t slaveIndex);

    ModbusSlaveInMaster *slaveOnIndex(uint8_t slaveIndex);
    ModbusSlaveInMaster *slave(uint8_t slaveAddr, bool isIndex = false);

    void setDataTable(uint8_t slaveAdr, ModbusTable *table);
    void setDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, int *_adresses);

    void configDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize);
    void configDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, uint16_t _startAdr);

    void setPollingPeriod(uint8_t slaveAdr, uint32_t period, bool isIndex = false);
    void setTimeOutTime(uint8_t slaveAdr, uint32_t time, bool isIndex = false);

    void setLongOrder(uint8_t slaveAdr, uint8_t order, bool isIndex = false);
    void setFloatOrder(uint8_t slaveAdr, uint8_t order, bool isIndex = false);
    void setUnsignedlongOrder(uint8_t slaveAdr, uint8_t order, bool isIndex = false);
    void setIntOrder(uint8_t slaveAdr, uint8_t order, bool isIndex = false);

    void saveLong(uint8_t slaveAdr, int32_t value, uint8_t table, int startAddres, bool isIndex = false);
    void saveUnsignedLong(uint8_t slaveAdr, uint32_t value, uint8_t table, int startAddres, bool isIndex = false);
    void saveFloat(uint8_t slaveAdr, float value, uint8_t table, int startAddres, bool isIndex = false);
    void saveInteger(uint8_t slaveAdr, int value, uint8_t table, int startAddres, bool isIndex = false);
    void saveByte(uint8_t slaveAdr, uint8_t value, uint8_t table, int startAddres, bool isIndex = false);
    void saveBool(uint8_t slaveAdr, bool value, uint8_t table, int startAddres, bool isIndex = false);

    uint8_t readByte(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    int readInteger(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    float readFloat(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    int32_t readLong(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    uint32_t readUnsignedLong(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    bool readBool(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);

    uint8_t getLastError(uint8_t slaveAdr, bool isIndex = false);
    void status(uint8_t slaveAdr, bool status, bool isIndex = false);

    ModbusSlaveInMaster *firstWriteSlave();
    ModbusSlaveInMaster *firstReadySlave();
    ModbusSlaveInMaster *nextSlave(ModbusSlaveInMaster *currentSlave);

    bool hasWriteRegisters();
    bool isReady();
    int getPort() { return _port; };

protected:
    int _port = 502;
    IPAddress _serverIp;
    char _serverHost[FLPROG_HOST_NAME_LENGTH] = "";
    bool _serverAsHost = false;
    uint8_t slavesSize = 5;
    ModbusSlaveInMaster *slaves = 0;
    uint8_t _mode = FLPROG_TCP_MODBUS;
};