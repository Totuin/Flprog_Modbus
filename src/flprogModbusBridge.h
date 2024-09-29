#pragma once
#include "base/flprogModbusUtilites.h"
#include "flprogEthernet.h"
#include "flprogUartBase.h"

class ModbusBridge
{
public:
    ModbusBridge(uint8_t portNumber, FLProgAbstractTcpInterface *sourse, FlprogAbstractUartExecutor *executor);
    void pool();

    void setUart(uint8_t portNumber) { _uart = portNumber; };
    void setSpeedUart(uint32_t speed) { _executor->setSpeedUart(speed, _uart); };
    void setDataBitUart(uint8_t value) { _executor->setDataBitUart(value, _uart); };
    void setStopBitUart(uint8_t value) { _executor->setStopBitUart(value, _uart); };
    void setParityUart(uint8_t value) { _executor->setParityUart(value, _uart); };
    void setPinRxUart(uint8_t pin) { _executor->setPinRxUart(pin, _uart); };
    void setPinTxUart(uint8_t pin) { _executor->setPinTxUart(pin, _uart); };

    void setTCPPort(int port);
    int tcpPort() { return _port; };

    void setTCPRemoteIp(IPAddress newIp);
    void setTCPRemoteIp(uint8_t newIpFirst, uint8_t newIpSecond, uint8_t newIpThird, uint8_t newIpFourtiph);
    IPAddress tcpRemoteIp() { return _ip; };

    void setTCPRemoteIpNumber(uint32_t newIp) { setTCPRemoteIp(flprog::numberToIp(newIp)); };
    void setTCPRemoteHost(String host);

    void setPinPeDe(uint8_t pin) { _pinPeDe = pin; };

    void setKaScadaCloudIp(IPAddress newIp);
    void setKaScadaCloudIpNumber(uint32_t newIp) { setKaScadaCloudIp(flprog::numberToIp(newIp)); };
    void setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress kaScadaCloudIp() { return _kasCadaCloudIP; };

    void setKaScadaCloudPort(int port);
    int kaScadaCloudPort() { return _kasCadaCloudPort; };

    void setKaScadaCloudDevceId(String id);
    String kaScadaCloudDevceId() { return _deniceId; };

    void setKaScadaCloudHost(String host);

    void byServer();
    void byClient();
    void setBridgeMode(bool mode);
    bool bridgeMode() { return _isServer; };

    void setMode(uint8_t mode);
    void byTcp() { setMode(FLPROG_TCP_MODBUS); };
    void byRtuOverTcp() { setMode(FLPROG_RTU_OVER_TCP_MODBUS); };
    void byKasCadaCloud() { setMode(FLPROG_KASCADA_CLOUD_MODBUS); };
    uint8_t mode() { return _mode; };

    uint16_t getSkippingEvents() { return _skippingEvents; };
    void setSkippingEvents(uint16_t value) { _skippingEvents = value; };
    void isPause(bool value) { _isPause = value; };
    bool isPause() { return _isPause; };

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

    FlprogAbstractUartExecutor *_executor = 0;

    uint16_t _skippingEvents = 0;
    uint16_t _eventsCount = 0;

    uint8_t _status = FLPROG_MODBUS_READY;
    uint8_t _uart = 0;
    int _port = 502;
    int _kasCadaCloudPort = 25000;
    IPAddress _kasCadaCloudIP = IPAddress(94, 250, 249, 225);
    char _cloudHost[FLPROG_HOST_NAME_LENGTH] = "";
    bool _cloudAdressAsHost = false;

    bool _isServer = false;
    int _pinPeDe = -1;
    uint8_t _bufferSize = 0;
    uint8_t _buffer[FLPROG_MODBUS_BUFER_SIZE];
    uint8_t _lastRec = 0;
    unsigned long _startT35;
    unsigned long _startSendTime;
    int _timeOfSend;
    IPAddress _ip = FLPROG_INADDR_NONE;
    char _remoteServerHost[FLPROG_HOST_NAME_LENGTH] = "";
    bool _remoteServerAsHost = false;

    bool _isPause = false;

    bool _isInit = false;

    FLProgAbstractTcpInterface *_interface;
    FLProgEthernetServer _server;
    FLProgEthernetClient _tcpClient;
    uint8_t _mode = FLPROG_TCP_MODBUS;
    uint8_t _mbapBuffer[6];

    String _deniceId;
    unsigned long _kaScadaCloudTimeStartTime;
};
