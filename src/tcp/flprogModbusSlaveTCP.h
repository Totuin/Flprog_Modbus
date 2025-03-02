#pragma once
#include "flprogEthernet.h"
#include "../base/flprogModbus.h"

class ModbusSlaveTCP : public Modbus
{
public:
    ModbusSlaveTCP(FLProgAbstractTcpInterface *sourse);

    virtual void pool();

    void setSlaveAddress(uint8_t adr) { _slaveAddres = adr; };
    uint8_t slaveAddress() { return _slaveAddres; };

    void setTcpPort(int _port);
    int tcpPort() { return _port; };
    void setKaScadaCloudIp(uint8_t newFirst_octet, uint8_t newSecond_octet, uint8_t newThird_octet, uint8_t newFourth_octet);
    void setKaScadaCloudIp(IPAddress ip);
    void setKaScadaCloudIpNmber(uint32_t ip) { setKaScadaCloudIp(flprog::numberToIp(ip)); };
    IPAddress kaScadaCloudIp() { return _cloudIp; };
    void setKaScadaCloudHost(String host);
    void setKaScadaCloudPort(int newPort);
    int kaScadaCloudPort() { return _cloudPort; };
    void setKaScadaCloudDevceId(String id);
    String kaScadaCloudDevceId() { return _deniceId; };
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

    void setMode(uint8_t mode);
    void byTcp() { setMode(FLPROG_TCP_MODBUS); };
    void byRtuOverTcp() { setMode(FLPROG_RTU_OVER_TCP_MODBUS); };
    void byKasCadaCloud() { setMode(FLPROG_KASCADA_CLOUD_MODBUS); };
    uint8_t mode() { return _mode; };

    ModbusMainData *mainData();

    void setLongOrder(uint8_t order) { mainData()->setLongOrder(order); };
    void setFloatOrder(uint8_t order) { mainData()->setFloatOrder(order); };
    void setUnsignedlongOrder(uint8_t order) { mainData()->setUnsignedlongOrder(order); };
    void setIntOrder(uint8_t order) { mainData()->setIntOrder(order); };

    uint8_t intOrder() { return mainData()->intOrder(); };
    uint8_t longOrder() { return mainData()->longOrder(); };
    uint8_t floatOrder() { return mainData()->floatOrder(); };
    uint8_t unsignedlongOrder() { return mainData()->unsignedlongOrder(); };

    void setCallBack(FLProgModbusNewDataCallback func);

protected:
    virtual uint8_t validateRequest();
    virtual void begin();
    virtual void getRxBuffer();
    virtual void sendTxBuffer();
    uint8_t rxBuffer();
    void connect();

    uint8_t _mode = FLPROG_TCP_MODBUS;
    uint8_t _slaveAddres = 1;
    FLProgEthernetServer _server;
    int _port = 502;
    ModbusMainData *_data = 0;

    uint8_t _mbapBuffer[6];
    FLProgAbstractTcpInterface *_interface = 0;

    FLProgEthernetClient _tcpClient;
    int _cloudPort = 25000;
    String _deniceId;
    IPAddress _cloudIp = IPAddress(94, 250, 249, 225);
    char _cloudHost[FLPROG_HOST_NAME_LENGTH] = "";
    bool _cloudAdressAsHost = false;
    uint32_t _kaScadaCloudTimeOutStartTime;
};
