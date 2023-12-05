#include "flprogModbusRTU.h"

void ModbusRTU::onPeDePin()
{
    if (_pinPeDe < 0)
    {
        return;
    }
    digitalWrite(_pinPeDe, HIGH);
}

void ModbusRTU::offPeDePin()
{
    if (_pinPeDe < 0)
    {
        return;
    }
    digitalWrite(_pinPeDe, LOW);
}

uint8_t ModbusRTU::rxBuffer()
{
    bool bBuffOverflow = false;
    _bufferSize = 0;
    while (flprog::availableUart(_uartPortNumber))
    {
        if (_bufferSize < 64)
        {
            _buffer[_bufferSize] = flprog::readUart(_uartPortNumber);
        }
        else
        {
            flprog::readUart(_uartPortNumber);
            bBuffOverflow = true;
        }
        _bufferSize++;
    }
    if (bBuffOverflow)
    {
        return -3;
    }

    return _bufferSize;
}

void ModbusRTU::sendTxBuffer()
{
    if (_buffer[0] == 0)
    {
        _bufferSize = 0;
        return;
    }
    onPeDePin();
    int crc = flprogModus::modbusCalcCRC(_bufferSize, _buffer);
    _buffer[_bufferSize] = crc >> 8;
    _bufferSize++;
    _buffer[_bufferSize] = crc & 0x00ff;
    _bufferSize++;
    flprog::writeUart(_buffer, _bufferSize, _uartPortNumber);
    uint8_t dataBits = 8;
    uint8_t stopBits = 1;
    uint8_t portParity = 0;
    uint16_t portSpeed = flprog::getSpeedUart(_uartPortNumber);
    _timeOfSend = flprogModus::timeForSendBytes(dataBits, stopBits, portParity, portSpeed, _bufferSize);
    _startSendTime = millis();
    _status = FLPROG_MODBUS_WAITING_SENDING;
    _bufferSize = 0;
}

bool ModbusRTU::checkAvaliblePacage()
{
    uint16_t avalibleBytes = flprog::availableUart(_uartPortNumber);
    if (avalibleBytes == 0)
    {
        return false;
    }

    if (avalibleBytes != _lastRec)
    {
        _lastRec = avalibleBytes;
        _time = millis();
        {
            return false;
        }
    }
    if (!(flprog::isTimer(_time, flprogModus::t35TimeForSpeed(flprog::getSpeedUart(_uartPortNumber)))))
    {
        return false;
    }
    _lastRec = 0;
    return true;
}
