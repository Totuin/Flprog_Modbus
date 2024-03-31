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
    if (_eventsCount < _skippingEvents)
    {
        _eventsCount++;
        return;
    }
    _eventsCount = 0;
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
    if (!(checkAvaliblePacage()))
    {
        return;
    }

    if (rxBuffer() < 7)
    {
        setLastError(2);
        buildException(2);
        sendTxBuffer();
        return;
    }
    executeSlaveReqest(mainData(), _slaveAddres);
}

uint8_t ModbusSlaveRTU::validateRequest()
{

    int msgCRC =
        ((_buffer[_bufferSize - 2] << 8) | _buffer[_bufferSize - 1]);
    if (flprogModus::modbusCalcCRC(_bufferSize - 2, _buffer) != msgCRC)
    {
        return 255;
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
