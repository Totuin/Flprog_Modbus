#pragma once
#include "flprogModbusTCP.h"
#include "flprogEthernet.h"

class ModbusSlaveTCP : public ModbusTCP
{
public:
    ModbusSlaveTCP(){};
    ModbusSlaveTCP(FLProgAbstractTcpInterface *sourse);

    void setSlaveAddress(uint8_t adr) { _slaveAddres = adr; };
    void setData(ModbusMainData *data) { _data = data; };

    void setDataTable(ModbusTable *table) { mainData()->setDataTable(table); };
    void setDataTable(uint8_t _table, uint16_t dataSize, int *_adresses) { mainData()->setDataTable(_table, dataSize, _adresses); };
    void configDataTable(uint8_t _table, uint16_t dataSize, uint16_t _startAdr = 0) { mainData()->configDataTable(_table, dataSize, _startAdr); };

    void saveLong(int32_t val, uint8_t table, int adr) { mainData()->saveLong(val, table, adr); };
    void saveUnsignedLong(uint32_t val, uint8_t table, int adr) { mainData()->saveUnsignedLong(val, table, adr); };
    void saveFloat(float val, uint8_t table, int adr) { mainData()->saveFloat(val, table, adr); };
    void saveInteger(int val, uint8_t table, int adr) { mainData()->saveInteger(val, table, adr); };
    void saveByte(uint8_t val, uint8_t table, int adr) { mainData()->saveByte(val, table, adr); };
    void saveBool(bool val, uint8_t table, int adr) { mainData()->saveBool(val, table, adr); };

    uint8_t readByte(uint8_t table, int adr) { return mainData()->readByte(table, adr); };
    int readInteger(uint8_t table, int adr) { return mainData()->readInteger(table, adr); };
    float readFloat(uint8_t table, int adr) { return mainData()->readFloat(table, adr); };
    int32_t readLong(uint8_t table, int adr) { return mainData()->readLong(table, adr); };
    uint32_t readUnsignedLong(uint8_t table, int adr) { return mainData()->readUnsignedLong(table, adr); };
    bool readBool(uint8_t table, int adr) { return mainData()->readBool(table, adr); };

    virtual uint8_t validateRequest() { return validateSlaveReqest(mainData()); };
    virtual void begin();
    virtual void begin(uint8_t address);
    virtual void pool();
    void setTcpPort(int _port);
    ModbusMainData *mainData();

    void setLongOrder(uint8_t order) { mainData()->setLongOrder(order); };
    void setFloatOrder(uint8_t order) { mainData()->setFloatOrder(order); };
    void setUnsignedlongOrder(uint8_t order) { mainData()->setUnsignedlongOrder(order); };
    void setIntOrder(uint8_t order) { mainData()->setIntOrder(order); };

protected:
    uint8_t _slaveAddres = 1;
    virtual void getRxBuffer();
    virtual void sendTxBuffer();
    FLProgEthernetServer _server;
    int _port = 502;
    ModbusMainData *_data = 0;
};

class ModbusSlaveRTUoverTCP : public ModbusSlaveTCP
{

public:
    using ModbusSlaveTCP::ModbusSlaveTCP;
    virtual void pool();

protected:
    virtual bool checkAvaliblePacage();
    virtual uint8_t rxBuffer();
    virtual void sendTxBuffer();
    virtual uint8_t validateRequest();
};

class ModbusKaScadaCloud : public ModbusSlaveTCP
{
public:
    ModbusKaScadaCloud(FLProgAbstractTcpInterface *sourse);
    void setKaScadaCloudIp(uint8_t newFirst_octet, uint8_t newSecond_octet, uint8_t newThird_octet, uint8_t newFourth_octet);
    void setKaScadaCloudPort(int newPort);
    void setKaScadaCloudDevceId(String id);
    virtual void begin();
    virtual void begin(uint8_t address);
    virtual void pool();

protected:
    void getRxBuffer();
    virtual void sendTxBuffer();
    void connect();

private:
    FLProgEthernetClient _tcpClient;
    int _cloudPort = 25000;
    String _deniceId;
    IPAddress _cloudIp = IPAddress(94, 250, 249, 225);
    uint32_t _kaScadaCloudTimeOutStartTime;
};
