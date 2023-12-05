#pragma once
#include "flprogEthernet.h"
#include "../base/flprogModbus.h"
#include "../base/flprogModbusTCPSlaveServer.h"

class ModbusMasterTCP : public Modbus
{
public:
    ModbusMasterTCP(FLProgAbstractTcpInterface *sourse, uint8_t size);

    void setServerSlavesSize(uint8_t serverIndex, uint8_t size);
    void setServerPort(uint8_t serverIndex, int16_t port);
    void setServerIpAdress(uint8_t serverIndex, IPAddress ip);
    void setSlaveAddress(uint8_t serverIndex, uint8_t slaveIndex, uint16_t addr);

    void setSlaveMode(uint8_t serverIndex, uint8_t mode);
    void setSlaveAsTcp(uint8_t serverIndex) { setSlaveMode(serverIndex, FLPROG_TCP_MODBUS); };
    void setSlaveAsRtuOverTcp(uint8_t serverIndex) { setSlaveMode(serverIndex, FLPROG_RTU_OVER_TCP_MODBUS); };
    uint8_t slaveMode(uint8_t serverIndex);

    void setDataTable(uint8_t serverIndex, uint8_t slaveAdr, ModbusTable *table);
    void setDataTable(uint8_t serverIndex, uint8_t slaveAdr, uint8_t _table, int16_t dataSize, int *_adresses);

    void configDataTable(uint8_t serverIndex, uint8_t slaveAdr, uint8_t _table, int16_t dataSize);
    void configDataTable(uint8_t serverIndex, uint8_t slaveAdr, uint8_t _table, int16_t dataSize, uint16_t _startAdr);

    void setPollingPeriod(uint8_t serverIndex, uint8_t slaveAdr, uint32_t period, bool isIndex = false);
    void setTimeOutTime(uint8_t serverIndex, uint8_t slaveAdr, uint32_t time, bool isIndex = false);

    void setLongOrder(uint8_t serverIndex, uint8_t slaveAdr, uint8_t order, bool isIndex = false);
    void setFloatOrder(uint8_t serverIndex, uint8_t slaveAdr, uint8_t order, bool isIndex = false);
    void setUnsignedlongOrder(uint8_t serverIndex, uint8_t slaveAdr, uint8_t order, bool isIndex = false);
    void setIntOrder(uint8_t serverIndex, uint8_t slaveAdr, uint8_t order, bool isIndex = false);

    void saveLong(uint8_t serverIndex, uint8_t slaveAdr, int32_t value, uint8_t table, int startAddres, bool isIndex = false);
    void saveUnsignedLong(uint8_t serverIndex, uint8_t slaveAdr, uint32_t value, uint8_t table, int startAddres, bool isIndex = false);
    void saveFloat(uint8_t serverIndex, uint8_t slaveAdr, float value, uint8_t table, int startAddres, bool isIndex = false);
    void saveInteger(uint8_t serverIndex, uint8_t slaveAdr, int value, uint8_t table, int startAddres, bool isIndex = false);
    void saveByte(uint8_t serverIndex, uint8_t slaveAdr, uint8_t value, uint8_t table, int startAddres, bool isIndex = false);
    void saveBool(uint8_t serverIndex, uint8_t slaveAdr, bool value, uint8_t table, int startAddres, bool isIndex = false);

    uint8_t readByte(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    int readInteger(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    float readFloat(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    int32_t readLong(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    uint32_t readUnsignedLong(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);
    bool readBool(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex = false);

    uint8_t getLastError(uint8_t serverIndex, uint8_t slaveAdr, bool isIndex = false);
    void status(uint8_t serverIndex, uint8_t slaveAdr, bool status, bool isIndex = false);
    void setSlavesToServer(uint8_t serverIndex, ModbusSlaveInMaster *table, int size);
    ModbusTCPSlaveServer *servers();
    virtual void pool();

protected:
    virtual void checkAnswer();
    void nextQuery();
    bool createNewTelegramm();
    bool createReadTelegram();
    void sendQuery();
    bool hasServer(uint8_t serverIndex);
    bool createWriteTelegramm(ModbusTCPSlaveServer *writeServer);
    virtual uint8_t validateRequest();
    ModbusTCPSlaveServer *firstWriteServer();
    virtual void sendTxBuffer();
    ModbusTCPSlaveServer *nextReadyServer(ModbusTCPSlaveServer *oldServer);
    bool hasReadyServer();
    bool nextStep();
    bool nextServer();
    bool nextSlave();
    bool nextTable();
    bool nextRegistor();
    ModbusTCPSlaveServer *firstReadyServer();
    virtual void getRxBuffer();
    virtual void begin();
    ModbusTCPSlaveServer *server(uint8_t serverIndex);
    virtual void connect(ModbusTCPSlaveServer *server);

    ModbusTCPSlaveServer *_telegrammServer;
    ModbusSlaveInMaster *_telegrammSlave;
    ModbusTable *_telegrammTable;
    uint8_t _telegrammFunction;
    int _telegrammStartAddres;
    int _telegrammNumbeRegs;
    int _telegrammAnswerId = 1;
    ModbusTCPSlaveServer *_currentServer;
    ModbusTCPSlaveServer *_tempCurrentServer;
    ModbusSlaveInMaster *_currentSlave;
    ModbusTable *_currentSlaveTable;
    int _currentSlaveStartAddress = -1;
    int _currentSlaveLastAddress = -1;
    int _serversSize = 1;
    ModbusTCPSlaveServer *_servs = 0;
    FLProgEthernetClient _tcpClient = 0;
    uint8_t _mbapBuffer[6];
    FLProgAbstractTcpInterface *_interface = 0;
};
