#include "flprogModbusSlaveTCP.h"

ModbusSlaveTCP::ModbusSlaveTCP(FLProgAbstractTcpInterface *sourse)
{
    _interface = sourse;
    _server.setSourse(sourse);
    _server.setPort(_port);
}

void ModbusSlaveTCP::setMode(uint8_t mode)
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

void ModbusSlaveTCP::setTcpPort(int port)
{
    _port = port;
    _isInit = false;
}

void ModbusSlaveTCP::begin()
{
    _server.setPort(_port);
    _isInit = true;
}

void ModbusSlaveTCP::pool()
{
    if (!_isInit)
    {
        begin();
    }
    if (!_server.connected())
    {
        return;
    }
    if (_mode == FLPROG_TCP_MODBUS)

    {
        getRxBuffer();
        if (_bufferSize == 0)
        {
            return;
        }
    }
    else
    {
        if (_mode == FLPROG_RTU_OVER_TCP_MODBUS)
        {

            if (_status == FLPROG_MODBUS_WAITING_SENDING)
            {
                if ((flprog::isTimer(_startSendTime, _timeOfSend)))
                {
                    _status = FLPROG_MODBUS_READY;
                }
                else
                {
                    return;
                }
            }
            if (_server.available() == 0)
            {
                return;
            }
            if (rxBuffer() < 7)
            {
                return;
            }
        }
        else
        {
            // Описать режим каскада cloud
            return;
        }
    }
    executeSlaveReqest(mainData(), _slaveAddres);
}

void ModbusSlaveTCP::getRxBuffer()
{
    uint8_t currentByteIndex = 0;
    uint8_t readByte;
    _bufferSize = 0;
    while (_server.available())
    {
        readByte = _server.read();
        if (currentByteIndex < 6)
        {
            _mbapBuffer[currentByteIndex] = readByte;
        }
        else
        {
            if (_bufferSize < 64)
            {
                _buffer[_bufferSize] = readByte;
                _bufferSize++;
            }
            else
            {
                _server.read();
            }
        }
        currentByteIndex++;
    }
}

void ModbusSlaveTCP::sendTxBuffer()
{
    if (_buffer[0] == 0)
    {
        _bufferSize = 0;
        return;
    }
    if (_mode == FLPROG_TCP_MODBUS)
    {
        _mbapBuffer[4] = highByte(_bufferSize);
        _mbapBuffer[5] = lowByte(_bufferSize);
        _server.write(_mbapBuffer, 6);
    }
    if (_mode == FLPROG_RTU_OVER_TCP_MODBUS)
    {
        int crc = flprogModus::modbusCalcCRC(_bufferSize, _buffer);
        _buffer[_bufferSize] = crc >> 8;
        _bufferSize++;
        _buffer[_bufferSize] = crc & 0x00ff;
        _bufferSize++;
    }
    _server.write(_buffer, _bufferSize);
    if (_mode == FLPROG_RTU_OVER_TCP_MODBUS)
    {
        _status = FLPROG_MODBUS_READY;
    }
    _bufferSize = 0;
}

ModbusMainData *ModbusSlaveTCP::mainData()
{
    if (_data == 0)
    {
        _data = new ModbusMainData();
    }
    return _data;
}

uint8_t ModbusSlaveTCP::validateRequest()
{
    if (_mode == FLPROG_RTU_OVER_TCP_MODBUS)
    {
        int msgCRC =
            ((_buffer[_bufferSize - 2] << 8) | _buffer[_bufferSize - 1]);
        if (flprogModus::modbusCalcCRC(_bufferSize - 2, _buffer) != msgCRC)
        {
            return 255;
        }
    }
    return validateSlaveReqest(mainData());
}

uint8_t ModbusSlaveTCP::rxBuffer()
{
    bool bBuffOverflow = false;
    _bufferSize = 0;
    while (_server.available())
    {
        _buffer[_bufferSize] = _server.read();
        _bufferSize++;
        if (_bufferSize >= 64)
            bBuffOverflow = true;
    }
    if (bBuffOverflow)
    {
        return -3;
    }
    return _bufferSize;
}

//---------------------------------ModbusKaScadaCloud--------------------------------
ModbusKaScadaCloud::ModbusKaScadaCloud(FLProgAbstractTcpInterface *sourse)
{
    _interface = sourse;
    _tcpClient.setSourse(sourse);
}

void ModbusKaScadaCloud::setKaScadaCloudIp(uint8_t newFirst_octet, uint8_t newSecond_octet, uint8_t newThird_octet, uint8_t newFourth_octet)
{
    if ((IPAddress(newFirst_octet, newSecond_octet, newThird_octet, newFourth_octet)) == _cloudIp)
    {

        return;
    }
    _cloudIp[0] = newFirst_octet;
    _cloudIp[1] = newFirst_octet;
    _cloudIp[2] = newFirst_octet;
    _cloudIp[3] = newFirst_octet;
    _tcpClient.stop();
}

void ModbusKaScadaCloud::setKaScadaCloudPort(int newPort)
{
    if (_port == newPort)

    {
        return;
    }
    _cloudPort = newPort;
    _tcpClient.stop();
}

void ModbusKaScadaCloud::setKaScadaCloudDevceId(String id)
{
    if (_deniceId.equals(id))

    {
        return;
    }
    _deniceId = id;
    _tcpClient.stop();
}

void ModbusKaScadaCloud::begin()
{
    _isInit = true;
    _kaScadaCloudTimeOutStartTime = millis() + 5000;
}

void ModbusKaScadaCloud::begin(uint8_t address)
{
    _slaveAddres = address;
    begin();
}

void ModbusKaScadaCloud::pool()
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
        return;
    }
    if (_status == FLPROG_MODBUS_WAITING_CONNECT_CLIENT)
    {
        connect();
        return;
    }
    if (flprog::isTimer(_kaScadaCloudTimeOutStartTime, 5000))
    {
        if (_tcpClient.connected())
        {
            _tcpClient.print(_deniceId);
            _kaScadaCloudTimeOutStartTime = millis();
            return;
        }
        else
        {
            connect();
            return;
        }
    }
    if (!_tcpClient.connected())
    {
        connect();
        return;
    }
    getRxBuffer();
    if (_bufferSize == 0)
    {
        return;
    }
    executeSlaveReqest(mainData(), _slaveAddres);
    _kaScadaCloudTimeOutStartTime = millis();
}

void ModbusKaScadaCloud::connect()
{
    if (_tcpClient.connected())
    {
        _status = FLPROG_MODBUS_READY;
        return;
    }
    uint8_t result = _tcpClient.connect(_cloudIp, _cloudPort);
    if (result == FLPROG_WITE)
    {
        _status = FLPROG_MODBUS_WAITING_CONNECT_CLIENT;
        return;
    }
    if (result == FLPROG_ERROR)
    {
        _status = FLPROG_MODBUS_READY;
        return;
    }
    _status = FLPROG_MODBUS_READY;
}

void ModbusKaScadaCloud::sendTxBuffer()
{
    if (!_tcpClient.connected())
    {
        return;
    }
    if (_buffer[0] == 0)
    {
        return;
    }
    _mbapBuffer[4] = highByte(_bufferSize);
    _mbapBuffer[5] = lowByte(_bufferSize);
    _tcpClient.write(_mbapBuffer, 6);
    _tcpClient.write(_buffer, _bufferSize);
    _bufferSize = 0;
}

void ModbusKaScadaCloud::getRxBuffer()
{
    uint8_t currentByte = 0;
    uint8_t currentByteIndex = 0;
    _bufferSize = 0;
    if (!_tcpClient.connected())
    {
        return;
    }
    while (_tcpClient.available())
    {
        currentByte = _tcpClient.read();
        if (currentByteIndex < 6)

        {
            _mbapBuffer[currentByteIndex] = currentByte;
        }
        else
        {
            if (_bufferSize < 64)
            {
                _buffer[_bufferSize] = currentByte;
                _bufferSize++;
            }
        }
        currentByteIndex++;
    }
}
