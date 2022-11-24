#pragma once
#include "Arduino.h"
#include "flprogModbusTCP.h"

class ModbusTCPSlaveServer
{
public:
    ModbusTCPSlaveServer(){};
    void setSlaves(ModbusSlaveInMaster table[], int size);
    ModbusSlaveInMaster *firstWriteSlave();
    ModbusSlaveInMaster *firstReadySlave();
    ModbusSlaveInMaster *nextSlave(ModbusSlaveInMaster *currentSlave);
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
    bool hasWriteRegisters();
    bool isReady();
    void setIpAdress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    void setPort(int serverPort);
    int getPort() { return port; };
    void setSlaveAddress(int slave, byte address);
    void setPollingPeriod(int slave, unsigned long period);
    void setLongOrder(int slave, byte order);
    void setFloatOrder(int slave, byte order);
    void setUnsignedlongOrder(int slave, byte order);
    void setIntOrder(int slave, byte order);
    void setDataTable(int slave, ModbusWorldTable *table);
    void setDataTable(int slave, ModbusBoolTable *table);
    uint8_t getIp(byte index);
    byte getLastError(int slave);
    void status(int slave, bool status);



protected:
    int port = 502;
    uint8_t ipFirst;
    uint8_t ipSecond;
    uint8_t ipThird;
    uint8_t ipFourth;
    bool compareWithIpAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    int slavesSize;
    ModbusSlaveInMaster *slaves;
};

class ModbusMasterTCP : public ModbusTCP
{
public:
    virtual void pool();
    void saveLong(int server, int slave, long value, byte table, int startAddres);
    void saveUnsignedLong(int server, int slave, unsigned long value, byte table, int startAddres);
    void saveFloat(int server, int slave, float value, byte table, int startAddres);
    void saveInteger(int server, int slave, int value, byte table, int startAddres);
    void saveByte(int server, int slave, byte value, byte table, int startAddres);
    void saveBool(int server, int slave, bool value, byte table, int startAddres);
    byte readByte(int server, int slave, byte table, int startAddres);
    int readInteger(int server, int slave, byte table, int startAddres);
    float readFloat(int server, int slave, byte table, int startAddres);
    long readLong(int server, int slave, byte table, int startAddres);
    unsigned long readUnsignedLong(int server, int slave, byte table, int startAddres);
    bool readBool(int server, int slave, byte table, int startAddres);
    void setServerPort(int server, int serverPort);
    void setSlavesToServer(int server, ModbusSlaveInMaster table[], int size);
    void setServerIP(int server, uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    void setSlaveAddress(int server, int slave, byte address);
    void setPollingPeriod(int server, int slave, unsigned long period);
    void setLongOrder(int server, int slave, byte order);
    void setFloatOrder(int server, int slave, byte order);
    void setUnsignedlongOrder(int server, int slave, byte order);
    void setIntOrder(int server, int slave, byte order);
    void setDataTable(int server, int slave, ModbusWorldTable *table);
    void setDataTable(int server, int slave, ModbusBoolTable *table);
    byte getLastError(int server, int slave);
    void status(int server, int slave, bool status);


    virtual void connect(ModbusTCPSlaveServer *server){};
    virtual byte available(){};
    virtual byte read(){};
    virtual void stop(){};
    virtual void write(ModbusTCPSlaveServer *server, byte buffer[], byte buferSize){};

protected:
    byte mbapBuffer[6];
    ModbusTCPSlaveServer *servers;
    int serversSize;
    ModbusTCPSlaveServer *telegrammServer;
    ModbusSlaveInMaster *telegrammSlave;
    ModbusTable *telegrammTable;
    byte telegrammFunction;
    int telegrammStartAddres;
    int telegrammNumbeRegs;
    int telegrammAnswerId = 1;
    ModbusTCPSlaveServer *currentServer;
    ModbusSlaveInMaster *currentSlave;
    ModbusTable *currentSlaveTable;
    int currentSlaveStartAddress = -1;
    int currentSlaveLastAddress = -1;
    virtual void checkAnswer();
    void nextQuery();
    bool createNewTelegramm();
    bool createReadTelegram();
    void sendQuery();
    bool hasWriteRegisters();
    bool hasServer(int serverIndex);
    bool createWriteTelegramm();
    virtual byte validateRequest();
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
};
