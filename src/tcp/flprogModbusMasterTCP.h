#pragma once
#include "flprogModbusTCP.h"

class ModbusTCPSlaveServer
{
public:
    ModbusTCPSlaveServer(){};
    void setSlaves(ModbusSlaveInMaster *_table, uint8_t size);
    void setSlavesSize(uint8_t size);

    void setPort(int16_t serverPort) { port = serverPort; };
    void setIpAdress(IPAddress ip) { serverIp = ip; };
    void setSlaveAddress(uint8_t slaveIndex, uint16_t addr);
    bool hasSlave(uint8_t slaveIndex);
    IPAddress getIp() { return serverIp; };
    ModbusSlaveInMaster *slaveOnIndex(uint8_t slaveIndex);
    ModbusSlaveInMaster *slave(uint8_t slaveAddr, bool isIndex = false);

    void setDataTable(uint8_t slaveAdr, ModbusTable *table, bool isIndex = false);
    void setDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, int *_adresses, bool isIndex = false);

    void configDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, bool isIndex = false);
    void configDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, uint16_t _startAdr, bool isIndex = false);

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
    int getPort() { return port; };

protected:
    int port = 502;
    IPAddress serverIp;
    uint8_t slavesSize = 5;
    ModbusSlaveInMaster *slaves = 0;
};

class ModbusMasterTCP : public ModbusTCP
{
public:
    ModbusMasterTCP(FLProgAbstractTcpInterface *sourse, uint8_t size);
   
    ModbusTCPSlaveServer *server(uint8_t serverIndex);

    void setServerSlavesSize(uint8_t serverIndex, uint8_t size);
    void setServerPort(uint8_t serverIndex, int16_t port);
    void setServerIpAdress(uint8_t serverIndex, IPAddress ip);
    void setSlaveAddress(uint8_t serverIndex, uint8_t slaveIndex, uint16_t addr);

    void setDataTable(uint8_t serverIndex, uint8_t slaveAdr, ModbusTable *table, bool isIndex = false);
    void setDataTable(uint8_t serverIndex, uint8_t slaveAdr, uint8_t _table, int16_t dataSize, int *_adresses, bool isIndex = false);

    void configDataTable(uint8_t serverIndex, uint8_t slaveAdr, uint8_t _table, int16_t dataSize, bool isIndex = false);
    void configDataTable(uint8_t serverIndex, uint8_t slaveAdr, uint8_t _table, int16_t dataSize, uint16_t _startAdr, bool isIndex = false);

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

    virtual void connect(ModbusTCPSlaveServer *server);
    virtual uint8_t available() { return client()->available(); };
    virtual uint8_t read() { return client()->read(); };
    virtual void stop() { client()->stop(); };
    virtual void write(ModbusTCPSlaveServer *server, uint8_t *buffer, uint8_t buferSize);
    virtual void begin();
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
    Client *client();

    ModbusTCPSlaveServer *telegrammServer;
    ModbusSlaveInMaster *telegrammSlave;
    ModbusTable *telegrammTable;
    uint8_t telegrammFunction;
    int telegrammStartAddres;
    int telegrammNumbeRegs;
    int telegrammAnswerId = 1;
    ModbusTCPSlaveServer *currentServer;
    ModbusSlaveInMaster *currentSlave;
    ModbusTable *currentSlaveTable;
    int currentSlaveStartAddress = -1;
    int currentSlaveLastAddress = -1;
    int serversSize = 1;
    ModbusTCPSlaveServer *servs = 0;
    Client *tcpClient = 0;

};

class ModbusMasterRTUoverTCP : public ModbusMasterTCP
{
public:
    using ModbusMasterTCP::ModbusMasterTCP;

protected:
    virtual void checkAnswer();
    virtual uint8_t validateRequest();
    virtual void getRxBuffer();
    virtual void sendTxBuffer();

private:
};
