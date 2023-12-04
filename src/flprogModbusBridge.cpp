#include "flprogModbusBridge.h"

ModbusBridge::ModbusBridge(uint8_t portNumber, FLProgAbstractTcpInterface *sourse)
{
    _interface = sourse;
    _uart = portNumber;
    _server.setSourse(sourse);
    _tcpClient.setSourse(sourse);
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
   
    if (!_isServer)
    {
        if (!_tcpClient.connected())
        {
            connect();
            return;
        }
    }
    if (_isServer)
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
    if (newIp == _ip)
    {
        return;
    }
    _ip = newIp;
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

void ModbusBridge::begin()
{
    _isInit = true;
    _status = FLPROG_MODBUS_READY;
    RT_HW_Base.uartBegin(_uart);
    if (_pinPeDe >= 0)
    {
        pinMode(_pinPeDe, OUTPUT);
        digitalWrite(_pinPeDe, LOW);
    }
    if (_isServer)
    {
        _server.setPort(_port);
    }
    else
    {
        _tcpClient.stop();
    }
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
    uint8_t avalibleBytes = RT_HW_Base.uartAvailable(_uart);
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
    if (!(flprog::isTimer(_startT35, (flprogModus::t35TimeForSpeed(RT_HW_Base.uartGetSpeed(_uart))))))
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
    while (RT_HW_Base.uartAvailable(_uart))
    {
        _buffer[_bufferSize] = RT_HW_Base.uartRead(_uart);
        _bufferSize++;
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
    for (uint8_t i = 0; i < _bufferSize; i++)
    {
        RT_HW_Base.uartWrite(_buffer[i], _uart);
    }
    uint8_t dataBits = 8;
    uint8_t stopBits = 1;
    uint8_t portParity = 0;
    uint16_t portSpeed = RT_HW_Base.uartGetSpeed(_uart);
    _timeOfSend = flprogModus::timeForSendBytes(dataBits, stopBits, portParity, portSpeed, _bufferSize);
    _startSendTime = millis();
    _status = FLPROG_MODBUS_WAITING_SENDING;
    _bufferSize = 0;
}

void ModbusBridge::connect()
{
    if (_tcpClient.connected())
    {

        _status = FLPROG_MODBUS_READY;
        return;
    }
    uint8_t result = _tcpClient.connect(_ip, _port);
    if (result == FLPROG_WITE)
    {
        _status = FLPROG_MODBUS_WAITING_CONNECT_CLIENT;
        return;
    }
    _status = FLPROG_MODBUS_READY;
}

//------------------------------------ModbusTcpBridge---------------------------------------------

void ModbusTcpBridge::tcpPool()
{
    if (_isServer)
    {
        if (!_server.connected())
        {
            return;
        }
        _bufferSize = 0;
        uint8_t byteIndex = 0;
        if (_server.available())
        {
            while (_server.available())
            {
                if (byteIndex < 6)
                {
                    _mbapBuffer[byteIndex] = _server.read();
                    byteIndex++;
                }
                else
                {
                    _buffer[_bufferSize] = _server.read();
                    _bufferSize++;
                }
            }
            sendRTUBuffer();
        }
        return;
    }

    _bufferSize = 0;
    uint8_t byteIndex = 0;
    if (_tcpClient.available())
    {
        while (_tcpClient.available())
        {
            if (byteIndex < 6)
            {
                _mbapBuffer[byteIndex] = _tcpClient.read();
                byteIndex++;
            }
            else
            {
                _buffer[_bufferSize] = _tcpClient.read();
                _bufferSize++;
            }
        }
        sendRTUBuffer();
    }
}

void ModbusTcpBridge::sendTCPBuffer()
{
    if (_isServer)
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
        _bufferSize = 0;
        return;
    }

    if (!_tcpClient.connected())
    {
        connect();
        return;
    }
    _bufferSize -= 2;
    _mbapBuffer[4] = highByte(_bufferSize);
    _mbapBuffer[5] = lowByte(_bufferSize);
    _tcpClient.write(_mbapBuffer, 6);
    _tcpClient.write(_buffer, _bufferSize);
    _bufferSize = 0;
}

// --------------------------------ModbusRtuOverTcpBridge------------------------------------
void ModbusRtuOverTcpBridge::tcpPool()
{
    /*
    if (!_isServer)
    {
        if (!client()->connected())
        {
            client()->connect(_ip, _port);
        }
    }
    if (!client()->connected())
    {
        return;
    }
    if (client()->available())
    {
        _bufferSize = 0;
        while (client()->available())
        {
            if (_bufferSize < 64)
            {
                _buffer[_bufferSize] = client()->read();
                _bufferSize++;
            }
            else
            {
                client()->read();
            }
        }
        sendRTUBuffer();
    }
    */
}

void ModbusRtuOverTcpBridge::sendTCPBuffer()
{
    /*
    if (!_isServer)
    {
        if (!client()->connected())
        {
            client()->connect(_ip, _port);
        }
    }
    if (!client()->connected())
    {
        return;
    }
    client()->write(_buffer, _bufferSize);
    _bufferSize = 0;
    */
}

//-----------ModbusKasCadaCloudTcpBridge------------------------------

ModbusKasCadaCloudTcpBridge::ModbusKasCadaCloudTcpBridge()
{
    _port = 25000;
    _ip = IPAddress(94, 250, 249, 225);
}

ModbusKasCadaCloudTcpBridge::ModbusKasCadaCloudTcpBridge(uint8_t portNumber, FLProgAbstractTcpInterface *sourse)
{
    _interface = sourse;
    _uart = portNumber;
    _port = 25000;
    _ip = IPAddress(94, 250, 249, 225);
}

void ModbusKasCadaCloudTcpBridge::pool()
{
    /*
    if (!_isInit)
    {
        begin();
        return;
    }
    if (flprog::isTimer(_kaScadaCloudTimeStartTime, 5000))
    {
        if (client()->connected())
        {
            client()->print(_deniceId);
            _kaScadaCloudTimeStartTime = millis();
        }
        else
        {
            client()->connect(_ip, _port);
            return;
        }
    }
    if (!client()->connected())
    {
        return;
    }
    tcpPool();
    rtuPool();
    */
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudIp(IPAddress newIp)
{
    _ip = newIp;
    _isInit = false;
}
void ModbusKasCadaCloudTcpBridge::setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    setKaScadaCloudIp(IPAddress(first_octet, second_octet, third_octet, fourth_octet));
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudPort(int port)
{
    _port = port;
    _isInit = false;
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudDevceId(String id)
{
    _deniceId = id;
}
void ModbusKasCadaCloudTcpBridge::begin()
{
    _isInit = true;
    RT_HW_Base.uartBegin(_uart);
    if (_pinPeDe >= 0)
    {
        pinMode(_pinPeDe, OUTPUT);
        digitalWrite(_pinPeDe, LOW);
    }
    _kaScadaCloudTimeStartTime = flprog::timeBack(5000);
}
void ModbusKasCadaCloudTcpBridge::tcpPool()
{
    /*
    if (!client()->connected())
    {
        return;
    }
    uint8_t readingByte;
    uint8_t byteIndex = 0;
    if (client()->available())
    {
        _bufferSize = 0;
        while (client()->available())
        {
            readingByte = client()->read();
            if (byteIndex < 6)
            {
                _mbapBuffer[byteIndex] = readingByte;
            }
            else
            {
                _buffer[_bufferSize] = readingByte;
                _bufferSize++;
            }
            byteIndex++;
        }
        sendRTUBuffer();
    }
    */
}

void ModbusKasCadaCloudTcpBridge::sendTCPBuffer()
{
    /*
    if (!client()->connected())
    {
        return;
    }
    _bufferSize -= 2;
    _mbapBuffer[4] = highByte(_bufferSize);
    _mbapBuffer[5] = lowByte(_bufferSize);
    client()->write(_mbapBuffer, 6);
    client()->write(_buffer, _bufferSize);
    _bufferSize = 0;
    */
}
