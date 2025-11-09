#include "flprogModbusSlaveTCP.h"

ModbusSlaveTCP::ModbusSlaveTCP(FLProgAbstractTcpInterface *sourse)
{
  _interface = sourse;
  _tcpClient.setSourse(sourse);
  _server.setSourse(sourse);
  _status = FLPROG_MODBUS_READY;
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

void ModbusSlaveTCP::setTcpPort(int16_t port)
{
  if (_port == port)
  {
    return;
  }
  _port = port;
  _isInit = false;
}

void ModbusSlaveTCP::begin()
{
  _server.stop();
  _tcpClient.stop();
  if (_mode != FLPROG_KASCADA_CLOUD_MODBUS)
  {
    _server.setPort(_port);
    _server.begin();
  }
  _kaScadaCloudTimeOutStartTime = flprog::timeBack(5000);
  _isInit = true;
}

void ModbusSlaveTCP::pool()
{
  if(!_enable)
  {
    return;
  }
  if (_interface == 0)
  {
    return;
  }
  if (_eventsCount < _skippingEvents)
  {
    _eventsCount++;
    return;
  }
  _eventsCount = 0;

  if (!_interface->isReady())
  {
    _isInit = false;
    return;
  }
  if (!_isInit)
  {
    begin();
    return;
  }
  if (_mode == FLPROG_KASCADA_CLOUD_MODBUS)
  {
    if (_status == FLPROG_MODBUS_WAITING_CONNECT_CLIENT)
    {
      connect();
      return;
    }
    if (!_tcpClient.connected())
    {
      connect();
      return;
    }
    if (flprog::isTimer(_kaScadaCloudTimeOutStartTime, 5000))
    {

      _tcpClient.print(_deniceId);
      _kaScadaCloudTimeOutStartTime = millis();
      return;
    }
    getRxBuffer();
    if (_bufferSize == 0)
    {
      return;
    }
    executeSlaveReqest(mainData(), _slaveAddres);
    return;
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
    executeSlaveReqest(mainData(), _slaveAddres);
    return;
  }
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
  executeSlaveReqest(mainData(), _slaveAddres);
  return;
}

void ModbusSlaveTCP::getRxBuffer()
{
  uint8_t currentByteIndex = 0;
  uint8_t readByte;
  _bufferSize = 0;
  if (_mode == FLPROG_KASCADA_CLOUD_MODBUS)
  {
    if (!_tcpClient.connected())
    {
      return;
    }
  }
  else
  {
    if (!_server.connected())
    {
      return;
    }
  }

  if (_mode == FLPROG_KASCADA_CLOUD_MODBUS)
  {
    while (_tcpClient.available())
    {
      readByte = _tcpClient.read();
      if (currentByteIndex < 6)

      {
        _mbapBuffer[currentByteIndex] = readByte;
      }
      else
      {
        if (_bufferSize < FLPROG_MODBUS_BUFER_SIZE)
        {
          _buffer[_bufferSize] = readByte;
          _bufferSize++;
        }
      }
      currentByteIndex++;
    }
  }
  else
  {
    while (_server.available())
    {
      readByte = _server.read();
      if (currentByteIndex < 6)
      {
        _mbapBuffer[currentByteIndex] = readByte;
      }
      else
      {
        if (_bufferSize < FLPROG_MODBUS_BUFER_SIZE)
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
}

void ModbusSlaveTCP::sendTxBuffer()
{
  if (_mode == FLPROG_KASCADA_CLOUD_MODBUS)
  {
    if (!_tcpClient.connected())
    {
      return;
    }
  }
  else
  {
    if (!_server.connected())
    {
      _server.stopConnection();
      _bufferSize = 0;
      return;
    }
  }
  if (_buffer[0] == 0)
  {
    _bufferSize = 0;
    return;
  }
  if ((_mode == FLPROG_TCP_MODBUS) || (_mode == FLPROG_KASCADA_CLOUD_MODBUS))
  {
    _mbapBuffer[4] = highByte(_bufferSize);
    _mbapBuffer[5] = lowByte(_bufferSize);
    if (_mode == FLPROG_KASCADA_CLOUD_MODBUS)
    {
      _tcpClient.write(_mbapBuffer, 6);
    }
    else
    {
      _server.write(_mbapBuffer, 6);
    }
  }
  if (_mode == FLPROG_RTU_OVER_TCP_MODBUS)
  {
    uint16_t crc = flprogModus::modbusCalcCRC(_bufferSize, _buffer);
    _buffer[_bufferSize] = lowByte(crc);
    _bufferSize++;
    _buffer[_bufferSize] = highByte(crc);
    _bufferSize++;
  }
  if (_mode == FLPROG_KASCADA_CLOUD_MODBUS)
  {
    _tcpClient.write(_buffer, _bufferSize);
  }
  else
  {
    _server.write(_buffer, _bufferSize);
  }
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

void ModbusSlaveTCP::setCallBack(FLProgModbusNewDataCallback func)
{
  mainData()->setCallBack(func);
}

uint8_t ModbusSlaveTCP::validateRequest()
{
  uint16_t pacadgeSize;
  if (_mode == FLPROG_RTU_OVER_TCP_MODBUS)
  {
    pacadgeSize = flprogModus::slaveRTUPacadgeSize(_bufferSize, _buffer);
    if (pacadgeSize == 0)
    {
      return 253;
    }
    if (pacadgeSize > _bufferSize)
    {
      return 253;
    }
    if (!(flprogModus::checkCRCOnBuffer(pacadgeSize, _buffer)))
    {
      return 252;
    }
  }
  else
  {
    pacadgeSize = (uint16_t)word(_mbapBuffer[4], _mbapBuffer[5]);
    if (pacadgeSize == 0)
    {
      return 253;
    }
    if (pacadgeSize != _bufferSize)
    {
      return 253;
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
    if (_bufferSize < FLPROG_MODBUS_BUFER_SIZE)
    {
      _buffer[_bufferSize] = _server.read();
      _bufferSize++;
    }
    else
    {
      _server.read();
      bBuffOverflow = true;
    }
  }
  if (bBuffOverflow)
  {
    return -3;
  }
  return _bufferSize;
}

void ModbusSlaveTCP::setKaScadaCloudIp(uint8_t newFirst_octet, uint8_t newSecond_octet, uint8_t newThird_octet, uint8_t newFourth_octet)
{
  setKaScadaCloudIp(IPAddress(newFirst_octet, newSecond_octet, newThird_octet, newFourth_octet));
}

void ModbusSlaveTCP::setKaScadaCloudIp(IPAddress ip)
{
  _cloudAdressAsHost = false;
  if (_cloudIp == ip)
  {
    return;
  }
  _cloudIp == ip;
  _isInit = false;
}

void ModbusSlaveTCP::setKaScadaCloudHost(String host)
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

void ModbusSlaveTCP::setKaScadaCloudPort(int16_t newPort)
{
  if (_cloudPort == newPort)

  {
    return;
  }
  _cloudPort = newPort;
  _isInit = false;
}

void ModbusSlaveTCP::setKaScadaCloudDevceId(String id)
{
  if (_deniceId.equals(id))

  {
    return;
  }
  _deniceId = id;
  _isInit = false;
}

void ModbusSlaveTCP::connect()
{
  if (_tcpClient.connected())
  {
    _status = FLPROG_MODBUS_READY;
    // return; ander
  }
  uint8_t result;
  if (_cloudAdressAsHost)
  {
    result = _tcpClient.connect(_cloudHost, _cloudPort);
  }
  else
  {
    result = _tcpClient.connect(_cloudIp, _cloudPort);
  }

  if (result == FLPROG_WAIT)
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
