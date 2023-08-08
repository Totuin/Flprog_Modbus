#pragma once
#include "base/flprogModbusData.h"
#include "flprogUart.h"
#include "tcp/tcpDevice/flprogTcpDevice.h"

class ModbusBridge
{
public:
    virtual void pool();
    void setTCPDevice(FLProgTcpDevice *device);
    void setRTUDevice(FLProgUartBasic *device);
    void setTCPPort(int port);
    void setTCPRemoteIp(byte newIpFirst, byte newIpSecond, byte newIpThird, byte newIpFourth);
    void setRtuPortSpeed(byte speed);
    byte getRtuPortSpeed() { return rtuDevice->getPortSpeed(); };
    void setRtuPortDataBits(byte dataBits);
    byte getRtuPortDataBits() { return rtuDevice->getPortDataBits(); };
    void setRtuPortStopBits(byte stopBits);
    byte getRtuPortStopBits() { return rtuDevice->getPortStopBits(); };
    void setRtuPortParity(byte stopBits);
    byte getRtuPortParity() { return rtuDevice->getPortParity(); };
    void setDeviceName(String name) { rtuDevice->setDeviceName(name); };
    void setPinPeDe(byte pin);
    void byServer();
    void byClient();
    virtual void begin();

protected:
    long t35TimeForSpeed();
    long timeForSendbytes(byte bufferSize);
    virtual void tcpPool(){};
    virtual void rtuPool();
    void sendRTUBuffer();
    void getRTURxBuffer();
    virtual void sendTCPBuffer(){};
    void onPeDePin();
    void offPeDePin();
    FLProgTcpDevice *tcpDevice;
    FLProgUartBasic *rtuDevice;
    bool isServer = true;
    uint8_t pinPeDe = 200;
    uint8_t bufferSize = 0;
    uint8_t buffer[64];
    uint8_t lastRec = 0;
    uint8_t workStatus = FLPROG_MODBUS_READY;
    unsigned long startT35;
    unsigned long startSendTime;
    int timeOfSend;
    bool compareRemoteIp(byte newIpFirst, byte newIpSecond, byte newIpThird, byte newIpFourth);
    uint8_t ipFirst = 0;
    uint8_t ipSecond = 0;
    uint8_t ipThird = 0;
    uint8_t ipFourth = 0;

private:
};

class ModbusTcpBridge : public ModbusBridge
{
protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
    uint8_t mbapBuffer[6];
};

class ModbusRtuOverTcpBridge : public ModbusBridge
{
protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
};

class ModbusKasCadaCloudTcpBridge : public ModbusBridge
{
public:
    virtual void pool();
    void setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    void setKaScadaCloudPort(int port);
    void setKaScadaCloudDevceId(String id);
    virtual void begin();

protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
    uint8_t mbapBuffer[6];
    String deniceId;
    unsigned long kaScadaCloudTimeStartTime;
};