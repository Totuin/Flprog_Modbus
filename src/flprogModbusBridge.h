#pragma once
#include "base/flprogModbusUtilites.h"
#include "flprogEthernet.h"

class ModbusBridge
{
public:
    ModbusBridge(uint8_t portNumber, FLProgAbstractTcpInterface *sourse);
    void pool();
    void setRTUDevice(uint8_t device) { _uart = device; };
    void setTCPPort(int _port);
    void setTCPRemoteIp(uint8_t newIpFirst, uint8_t newIpSecond, uint8_t newIpThird, uint8_t newIpFourtiph);
    void setTCPRemoteIp(IPAddress newIp);
    void setPinPeDe(uint8_t pin) { _pinPeDe = pin; };

    void setKaScadaCloudIp(IPAddress newIp);
    void setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    void setKaScadaCloudPort(int port);
    void setKaScadaCloudDevceId(String id);

    void byServer();
    void byClient();

    void setMode(uint8_t mode);
    void byTcp() { setMode(FLPROG_TCP_MODBUS); };
    void byRtuOverTcp() { setMode(FLPROG_RTU_OVER_TCP_MODBUS); };
    void byKasCadaCloud() { setMode(FLPROG_KASCADA_CLOUD_MODBUS); };
    uint8_t mode() { return _mode; };

private:
    void onPeDePin();
    void offPeDePin();
    void begin();

    void tcpPool();
    void tspModeTcpPool();
    void tspModeAsServerTcpPool();
    void tspModeAsClientTcpPool();
    void rtuOverTspModeTcpPool();
    void rtuOverTspModeAsServerTcpPool();
    void rtuOverTspModeAsClientTcpPool();

    void sendTCPBuffer();
    void tspModeSendTCPBuffer();
    void tspModeSendAsServerTCPBuffer();
    void tspModeSendAsClientTCPBuffer();
    void rtuOverTspModeSendTCPBuffer();
    void rtuOverTspModeSendAsServerTCPBuffer();
    void rtuOverTspModeSendAsClientTCPBuffer();

    void rtuPool();

    void getRTURxBuffer();
    void sendRTUBuffer();

    void connect();

    uint8_t _status = FLPROG_MODBUS_READY;
    uint8_t _uart = 0;
    int _port = 502;
    int _kasCadaCloudPort = 25000;
    IPAddress _kasCadaCloudIP = IPAddress(94, 250, 249, 225);
    bool _isServer = false;
    int _pinPeDe = -1;
    uint8_t _bufferSize = 0;
    uint8_t _buffer[200];
    uint8_t _lastRec = 0;
    unsigned long _startT35;
    unsigned long _startSendTime;
    int _timeOfSend;
    IPAddress _ip = FLPROG_INADDR_NONE;
    bool _isInit = false;
    FLProgAbstractTcpInterface *_interface;
    FLProgEthernetServer _server;
    FLProgEthernetClient _tcpClient;
    uint8_t _mode = FLPROG_TCP_MODBUS;
    uint8_t _mbapBuffer[6];

    String _deniceId;
    unsigned long _kaScadaCloudTimeStartTime;
};
