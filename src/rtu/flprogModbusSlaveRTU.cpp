#include "flprogModbusSlaveRTU.h"

ModbusSlaveRTU::ModbusSlaveRTU(uint8_t number, FlprogAbstractUartExecutor *executor)
{
  _uartPortNumber = number;
  _executor = executor;
}

void ModbusSlaveRTU::begin()
{
  _executor->beginUart(_uartPortNumber);
  _bufferSize = 0;
  _lastRec = 0;
  if (_pinPeDe >= 0)
  {
    pinMode(_pinPeDe, OUTPUT);
    digitalWrite(_pinPeDe, LOW);
  }
  _isInit = true;
}

void ModbusSlaveRTU::pool()
{
  if(!_enable)
  {
    return;
  }
  if (_eventsCount < _skippingEvents)
  {
    _eventsCount++;
    return;
  }
  _eventsCount = 0;
  if (_isPause)
  {
    return;
  }
  if (!_isInit)
  {
    begin();
    return;
  }
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
  if (checkAvaliblePacage())
  {
    executeSlaveReqest(mainData(), _slaveAddres);
    return;
  }
  if (_bufferSize == 0)
  {
    return;
  }
  if (flprog::isTimer(_time, 50))
  {
    _bufferSize = 0;
  }
}

uint8_t ModbusSlaveRTU::validateRequest()
{
  if (!(flprogModus::checkCRCOnBuffer(_bufferSize, _buffer)))
  {
    return 252;
  }
  return validateSlaveReqest(mainData());
}

ModbusMainData *ModbusSlaveRTU::mainData()
{
  if (_data == 0)
  {
    _data = new ModbusMainData();
  }
  return _data;
}

void ModbusSlaveRTU::setCallBack(FLProgModbusNewDataCallback func)
{
  mainData()->setCallBack(func);
}

uint16_t ModbusSlaveRTU::rtuPacadgeSize(uint16_t length, uint8_t bufferArray[])
{
  return flprogModus::slaveRTUPacadgeSize(length, bufferArray);
}