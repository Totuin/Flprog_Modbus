#pragma once
#include "base/flprogModbusUtilites.h"
#include "flprogUart.h"
#include "tcp/tcpDevice/flprogTcpDevice.h"

class ModbusBridge
{
public:
    ModbusBridge(){};
    ModbusBridge(uint8_t portNumber, FlprogAbstractEthernet *sourse);
    ModbusBridge(uint8_t portNumber, FLProgAbstracttWiFiInterface *sourse);

    ModbusBridge(uint8_t rxpin, uint8_t txpin, FlprogAbstractEthernet *sourse);
    ModbusBridge(uint8_t rxpin, uint8_t txpin, FLProgAbstracttWiFiInterface *sourse);

    virtual void pool();
    virtual void setTCPDevice(FLProgTcpDevice *device) { tcpDevice = device; };
    void setRTUDevice(FLProgUartBasic *device) { uart = device; };

    void setTCPPort(int port);
    void setTCPRemoteIp(uint8_t newIpFirst, uint8_t newIpSecond, uint8_t newIpThird, uint8_t newIpFourth);
    void setTCPRemoteIp(IPAddress newIp);

    void setRtuPortSpeed(uint8_t speed) { rtuDevice()->setPortSpeed(speed); };
    uint8_t getRtuPortSpeed() { return rtuDevice()->getPortSpeed(); };
    void setRtuPortDataBits(uint8_t dataBits) { rtuDevice()->setPortDataBits(dataBits); };
    uint8_t getRtuPortDataBits() { return rtuDevice()->getPortDataBits(); };
    void setRtuPortStopBits(uint8_t stopBits) { rtuDevice()->setPortStopBits(stopBits); };
    uint8_t getRtuPortStopBits() { return rtuDevice()->getPortStopBits(); };
    void setRtuPortParity(uint8_t stopBits) { rtuDevice()->setPortParity(stopBits); };
    uint8_t getRtuPortParity() { return rtuDevice()->getPortParity(); };
    void setDeviceName(String name) { rtuDevice()->setDeviceName(name); };
    void setPinPeDe(uint8_t pin) { pinPeDe = pin; };

    virtual void byServer();
    virtual void byClient();

    virtual void begin();

    FLProgUartBasic *rtuDevice();

protected:
    void onPeDePin();
    void offPeDePin();

    virtual void tcpPool() = 0;
    virtual void rtuPool();

    void getRTURxBuffer();
    void sendRTUBuffer();
    virtual void sendTCPBuffer() = 0;

    FLProgTcpDevice *tcpDevice = 0;
    FLProgUartBasic *uart = 0;

    bool isServer = false;
    int pinPeDe = -1;
    uint8_t bufferSize = 0;
    uint8_t buffer[64];
    uint8_t lastRec = 0;
    uint8_t workStatus = FLPROG_MODBUS_READY;
    unsigned long startT35;
    unsigned long startSendTime;
    int timeOfSend;
    IPAddress ip = IPAddress(0, 0, 0, 0);
    bool isInit = false;
};

class ModbusTcpBridge : public ModbusBridge
{
public:
    using ModbusBridge::ModbusBridge;

protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
    uint8_t mbapBuffer[6];
};

class ModbusRtuOverTcpBridge : public ModbusBridge
{
public:
    using ModbusBridge::ModbusBridge;

protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
};

class ModbusKasCadaCloudTcpBridge : public ModbusBridge
{
public:
    ModbusKasCadaCloudTcpBridge(){};
    
    ModbusKasCadaCloudTcpBridge(uint8_t portNumber, FlprogAbstractEthernet *sourse);
    ModbusKasCadaCloudTcpBridge(uint8_t portNumber, FLProgAbstracttWiFiInterface *sourse);

    ModbusKasCadaCloudTcpBridge(uint8_t rxpin, uint8_t txpin, FlprogAbstractEthernet *sourse);
    ModbusKasCadaCloudTcpBridge(uint8_t rxpin, uint8_t txpin, FLProgAbstracttWiFiInterface *sourse);
    
    virtual void pool();
    void setKaScadaCloudIp(IPAddress newIp);
    void setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    void setKaScadaCloudPort(int port);
    void setKaScadaCloudDevceId(String id);
    virtual void setTCPDevice(FLProgTcpDevice *device);
    virtual void byServer(){};
    virtual void byClient(){};
    virtual void begin();

protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
    IPAddress cloudIp = IPAddress(94, 250, 249, 225);
    int cloudPort = 25000;
    uint8_t mbapBuffer[6];
    String deniceId;
    unsigned long kaScadaCloudTimeStartTime;
};