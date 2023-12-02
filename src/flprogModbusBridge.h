#pragma once
#include "base/flprogModbusUtilites.h"
#include "flprogEthernet.h"


class ModbusBridge
{
public:
    ModbusBridge(){};
    ModbusBridge(uint8_t portNumber, FLProgAbstractTcpInterface *sourse);
    virtual void pool();
    void setRTUDevice(uint8_t device) { _uart = device; };
    void setTCPPort(int _port);
    void setTCPRemoteIp(uint8_t newIpFirst, uint8_t newIpSecond, uint8_t newIpThird, uint8_t newIpFourtiph);
    void setTCPRemoteIp(IPAddress newIp);
    void setPinPeDe(uint8_t pin) { _pinPeDe = pin; };

    virtual void byServer();
    virtual void byClient();

    virtual void begin();

protected:
    void onPeDePin();
    void offPeDePin();

    virtual void tcpPool() = 0;
    virtual void rtuPool();

    void getRTURxBuffer();
    void sendRTUBuffer();
    virtual void sendTCPBuffer() = 0;
    FLProgEthernetClient *client();

    uint8_t _uart = 0;
    int _port = 502;
    bool _isServer = false;
    int _pinPeDe = -1;
    uint8_t _bufferSize = 0;
    uint8_t _buffer[64];
    uint8_t _lastRec = 0;
    uint8_t _status = FLPROG_MODBUS_READY;
    unsigned long _startT35;
    unsigned long _startSendTime;
    int _timeOfSend;
    IPAddress _ip = FLPROG_INADDR_NONE;
    bool _isInit = false;
    FLProgAbstractTcpInterface *_interface;
    FLProgEthernetServer *_server = 0;
    FLProgEthernetClient *_tcpClient = 0;
};

class ModbusTcpBridge : public ModbusBridge
{
public:
    using ModbusBridge::ModbusBridge;

protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
    uint8_t _mbapBuffer[6];
};

class ModbusRtuOverTcpBridge : public ModbusBridge
{
public:
    using ModbusBridge::ModbusBridge;

protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();
};

class ModbusKasCadaCloudTcpBridge : public ModbusBridge
{
public:
    ModbusKasCadaCloudTcpBridge();
    ModbusKasCadaCloudTcpBridge(uint8_t portNumber, FLProgAbstractTcpInterface *sourse);

    virtual void pool();
    void setKaScadaCloudIp(IPAddress newIp);
    void setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    void setKaScadaCloudPort(int port);
    void setKaScadaCloudDevceId(String id);
    virtual void byServer(){};
    virtual void byClient(){};
    virtual void begin();

protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
    uint8_t _mbapBuffer[6];
    String _deniceId;
    unsigned long _kaScadaCloudTimeStartTime;
};