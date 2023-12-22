#include "flprogModbusBridge.h"

ModbusBridge::ModbusBridge(uint8_t portNumber, FLProgAbstractTcpInterface *sourse)
{
    _interface = sourse;
    _uart = portNumber;
    _server.setSourse(sourse);
    _tcpClient.setSourse(sourse);
}

void ModbusBridge::setMode(uint8_t mode)
{
    if (mode > 2)
    {
        return;
    }
    if (_mode == mode)
    {
        return;
    }
    _mode = mode;
    _isInit = false;
}

void ModbusBridge::setTCPPort(int port)
{
    if (_port == port)
    {
        return;
    }
    _port = port;
    _isInit = false;
}

void ModbusBridge::setTCPRemoteIp(uint8_t newIpFirst, uint8_t newIpSecond, uint8_t newIpThird, uint8_t newIpFourth)
{
    setTCPRemoteIp(IPAddress(newIpFirst, newIpSecond, newIpThird, newIpFourth));
}

void ModbusBridge::setTCPRemoteIp(IPAddress newIp)
{
    _remoteServerAsHost = false;
    if (newIp == _ip)
    {
        return;
    }
    _ip = newIp;
    _isInit = false;
}

void ModbusBridge::setTCPRemoteHost(String host)
{
    if (host.length() == 0)
    {
        return;
    }
    _remoteServerAsHost = true;
    if (host.equals(String(_remoteServerHost)))
    {
        return;
    }
    host.toCharArray(_remoteServerHost, FLPROG_HOST_NAME_LENGTH);
    _isInit = false;
}

void ModbusBridge::byServer()
{
    if (_isServer)
    {
        return;
    }
    _isServer = true;
    _isInit = false;
}

void ModbusBridge::byClient()
{
    if (!_isServer)
    {
        return;
    }
    _isServer = false;
    _isInit = false;
}

void ModbusBridge::setKaScadaCloudIp(IPAddress newIp)
{
    _cloudAdressAsHost = false;
    if (newIp == _kasCadaCloudIP)
    {
        return;
    }
    _kasCadaCloudIP = newIp;
    _isInit = false;
}

void ModbusBridge::setKaScadaCloudHost(String host)
{
    if (host.length() == 0)
    {
        return;
    }
    _cloudAdressAsHost = true;
    if (host.equals(String(_cloudHost)))
    {
        return;
    }
    host.toCharArray(_cloudHost, FLPROG_HOST_NAME_LENGTH);
    _isInit = false;
}

void ModbusBridge::setKaScadaCloudPort(int port)
{
    if (port == _kasCadaCloudPort)
    {
        return;
    }
    _kasCadaCloudPort = port;
    _isInit = false;
}

void ModbusBridge::setKaScadaCloudDevceId(String id)
{
    _deniceId = id;
}

void ModbusBridge::setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    setKaScadaCloudIp(IPAddress(first_octet, second_octet, third_octet, fourth_octet));
}

void ModbusBridge::onPeDePin()
{
    if (_pinPeDe < 0)
    {
        return;
    }
    digitalWrite(_pinPeDe, HIGH);
}

void ModbusBridge::offPeDePin()
{
    if (_pinPeDe < 0)
    {
        return;
    }
    digitalWrite(_pinPeDe, LOW);
}

void ModbusBridge::connect()
{
    if (_tcpClient.connected())
    {

        _status = FLPROG_MODBUS_READY;
        return;
    }
    uint8_t result;
    if (_mode == FLPROG_KASCADA_CLOUD_MODBUS)
    {
        if (_cloudAdressAsHost)
        {
            result = _tcpClient.connect(_cloudHost, _kasCadaCloudPort);
        }
        else
        {
            result = _tcpClient.connect(_kasCadaCloudIP, _kasCadaCloudPort);
        }
    }
    else
    {
        if (_remoteServerAsHost)
        {
            result = _tcpClient.connect(_remoteServerHost, _port);
        }
        else
        {
            result = _tcpClient.connect(_ip, _port);
        }
    }
    if (result == FLPROG_WAIT)
    {
        _status = FLPROG_MODBUS_WAITING_CONNECT_CLIENT;
        return;
    }
    _status = FLPROG_MODBUS_READY;
}

void ModbusBridge::pool()
{
    if (_interface == 0)
    {
        return;
    }
    if (!_interface->isReady())
    {
        return;
    }
    if (!_isInit)
    {
        begin();
    }
    if (_status == FLPROG_MODBUS_WAITING_CONNECT_CLIENT)
    {
        connect();
        return;
    }
    if ((!_isServer) || (_mode == FLPROG_KASCADA_CLOUD_MODBUS))
    {
        if (!_tcpClient.connected())
        {
            connect();
            return;
        }
    }
    if (_mode == FLPROG_KASCADA_CLOUD_MODBUS)
    {
        if (flprog::isTimer(_kaScadaCloudTimeStartTime, 5000))
        {

            _tcpClient.print(_deniceId);
            _kaScadaCloudTimeStartTime = millis();
        }
    }
    if (_isServer || (_mode != FLPROG_KASCADA_CLOUD_MODBUS))
    {
        tcpPool();
        rtuPool();
    }
    else
    {
        rtuPool();
        tcpPool();
    }
}

void ModbusBridge::begin()
{
    _isInit = true;
    _status = FLPROG_MODBUS_READY;
    flprog::beginUart(_uart);
    _tcpClient.stop();
    _server.stop();
    if (_pinPeDe >= 0)
    {
        pinMode(_pinPeDe, OUTPUT);
        digitalWrite(_pinPeDe, LOW);
    }
    if (_isServer && (_mode != FLPROG_KASCADA_CLOUD_MODBUS))
    {
        _server.setPort(_port);
    }
    _kaScadaCloudTimeStartTime = flprog::timeBack(5000);
}

void ModbusBridge::rtuPool()
{
    if (_status == FLPROG_MODBUS_WAITING_SENDING)
    {
        if ((flprog::isTimer(_startSendTime, _timeOfSend)))
        {
            _status = FLPROG_MODBUS_READY;
            offPeDePin();
        }
        else
        {
            return;
        }
    }
    uint8_t avalibleBytes = flprog::availableUart(_uart);
    if (avalibleBytes == 0)
    {
        return;
    }
    if (avalibleBytes != _lastRec)
    {
        _lastRec = avalibleBytes;
        _startT35 = millis();
        return;
    }
    if (!(flprog::isTimer(_startT35, (flprogModus::t35TimeForSpeed(flprog::getSpeedUart(_uart))))))
    {
        return;
    }
    _lastRec = 0;
    getRTURxBuffer();
    if (_bufferSize < 5)
    {
        return;
    }
    sendTCPBuffer();
}

void ModbusBridge::getRTURxBuffer()
{
    _bufferSize = 0;
    while (flprog::availableUart(_uart))
    {
        if (_bufferSize < FLPROG_MODBUS_BUFER_SIZE)
        {
            _buffer[_bufferSize] = flprog::readUart(_uart);
            _bufferSize++;
        }
        else
        {
            flprog::readUart(_uart);
        }
    }
}

void ModbusBridge::sendRTUBuffer()
{
    onPeDePin();
    int crc = flprogModus::modbusCalcCRC(_bufferSize, _buffer);
    _buffer[_bufferSize] = crc >> 8;
    _bufferSize++;
    _buffer[_bufferSize] = crc & 0x00ff;
    _bufferSize++;
    flprog::writeUart(_buffer, _bufferSize, _uart);
    uint8_t dataBits = 8;
    uint8_t stopBits = 1;
    uint8_t portParity = 0;
    uint16_t portSpeed = flprog::getSpeedUart(_uart);
    _timeOfSend = flprogModus::timeForSendBytes(dataBits, stopBits, portParity, portSpeed, _bufferSize);
    _startSendTime = millis();
    _status = FLPROG_MODBUS_WAITING_SENDING;
    _bufferSize = 0;
}

void ModbusBridge::tcpPool()
{
    if (_mode == FLPROG_RTU_OVER_TCP_MODBUS)
    {
        rtuOverTspModeTcpPool();
        return;
    }
    tspModeTcpPool();
}

void ModbusBridge::tspModeTcpPool()
{
    _bufferSize = 0;

    if (_isServer && (_mode != FLPROG_KASCADA_CLOUD_MODBUS))
    {
        tspModeAsServerTcpPool();
    }
    else
    {
        tspModeAsClientTcpPool();
    }
    if (_bufferSize > 0)
    {
        sendRTUBuffer();
    }
}

void ModbusBridge::rtuOverTspModeTcpPool()
{
    _bufferSize = 0;

    if (_isServer)
    {
        rtuOverTspModeAsServerTcpPool();
    }
    else
    {
        rtuOverTspModeAsClientTcpPool();
    }
    if (_bufferSize > 0)
    {
        sendRTUBuffer();
    }
}

void ModbusBridge::tspModeAsServerTcpPool()
{
    if (!_server.connected())
    {
        return;
    }

    if (!_server.available())
    {
        return;
    }
    uint8_t byteIndex = 0;
    while (_server.available())
    {
        if (byteIndex < 6)
        {
            _mbapBuffer[byteIndex] = _server.read();
            byteIndex++;
        }
        else
        {
            if (_bufferSize < FLPROG_MODBUS_BUFER_SIZE)
            {
                _buffer[_bufferSize] = _server.read();
                _bufferSize++;
            }
            else
            {
                _server.read();
            }
        }
    }
}

void ModbusBridge::rtuOverTspModeAsServerTcpPool()
{
    if (!_server.connected())
    {
        return;
    }

    if (!_server.available())
    {
        return;
    }
    while (_server.available())
    {
        if (_bufferSize < FLPROG_MODBUS_BUFER_SIZE)
        {
            _buffer[_bufferSize] = _server.read();
            _bufferSize++;
        }
        else
        {
            _server.read();
        }
    }
}

void ModbusBridge::tspModeAsClientTcpPool()
{
    if (!_tcpClient.connected())
    {
        return;
    }
    if (!_tcpClient.available())
    {
        return;
    }
    uint8_t byteIndex = 0;
    while (_tcpClient.available())
    {
        if (byteIndex < 6)
        {
            _mbapBuffer[byteIndex] = _tcpClient.read();
            byteIndex++;
        }
        else
        {
            if (_bufferSize < FLPROG_MODBUS_BUFER_SIZE)
            {

                _buffer[_bufferSize] = _tcpClient.read();
                _bufferSize++;
            }
            else
            {
                _tcpClient.read();
            }
        }
    }
}

void ModbusBridge::rtuOverTspModeAsClientTcpPool()
{
    if (!_tcpClient.connected())
    {
        return;
    }
    if (!_tcpClient.available())
    {
        return;
    }
    while (_tcpClient.available())
    {
        if (_bufferSize < FLPROG_MODBUS_BUFER_SIZE)
        {
            _buffer[_bufferSize] = _tcpClient.read();
            _bufferSize++;
        }
        else
        {
            _tcpClient.read();
        }
    }
}

void ModbusBridge::sendTCPBuffer()
{
    if (_mode == FLPROG_RTU_OVER_TCP_MODBUS)
    {
        rtuOverTspModeSendTCPBuffer();
        return;
    }
    tspModeSendTCPBuffer();
}

void ModbusBridge::tspModeSendTCPBuffer()
{
    if (_isServer && (_mode != FLPROG_KASCADA_CLOUD_MODBUS))
    {
        tspModeSendAsServerTCPBuffer();
    }
    else
    {
        tspModeSendAsClientTCPBuffer();
    }
    _bufferSize = 0;
}

void ModbusBridge::rtuOverTspModeSendTCPBuffer()
{
    if (_isServer)
    {
        rtuOverTspModeSendAsServerTCPBuffer();
    }
    else
    {
        rtuOverTspModeSendAsClientTCPBuffer();
    }
    _bufferSize = 0;
}

void ModbusBridge::tspModeSendAsServerTCPBuffer()
{
    if (!_server.connected())
    {
        return;
    }
    _bufferSize -= 2;
    _mbapBuffer[4] = highByte(_bufferSize);
    _mbapBuffer[5] = lowByte(_bufferSize);
    _server.write(_mbapBuffer, 6);
    _server.write(_buffer, _bufferSize);
}

void ModbusBridge::rtuOverTspModeSendAsServerTCPBuffer()
{
    if (!_server.connected())
    {
        return;
    }
    _server.write(_buffer, _bufferSize);
}

void ModbusBridge::tspModeSendAsClientTCPBuffer()
{
    if (!_tcpClient.connected())
    {
        return;
    }
    _bufferSize -= 2;
    _mbapBuffer[4] = highByte(_bufferSize);
    _mbapBuffer[5] = lowByte(_bufferSize);
    _tcpClient.write(_mbapBuffer, 6);
    _tcpClient.write(_buffer, _bufferSize);
}

void ModbusBridge::rtuOverTspModeSendAsClientTCPBuffer()
{
    if (!_tcpClient.connected())
    {
        return;
    }
    _tcpClient.write(_buffer, _bufferSize);
}
