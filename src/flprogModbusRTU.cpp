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
    bufferSize = 0;
    while (RT_HW_Base.uartAvailable(_uartPortNumber))
    {
        if (bufferSize < 64)
        {
            buffer[bufferSize] = RT_HW_Base.uartRead(_uartPortNumber);
        }
        else
        {
            RT_HW_Base.uartRead(_uartPortNumber);
            bBuffOverflow = true;
        }
        bufferSize++;
    }
    if (bBuffOverflow)
    {
        return -3;
    }

    return bufferSize;
}

void ModbusRTU::sendTxBuffer()
{
    if (buffer[0] == 0)
    {
        bufferSize = 0;
        return;
    }
    onPeDePin();
    int crc = flprogModus::modbusCalcCRC(bufferSize, buffer);
    buffer[bufferSize] = crc >> 8;
    bufferSize++;
    buffer[bufferSize] = crc & 0x00ff;
    bufferSize++;
    for (uint8_t i = 0; i < bufferSize; i++)
    {
        RT_HW_Base.uartWrite(buffer[i], _uartPortNumber);
    }
    uint8_t dataBits = 8;
    uint8_t stopBits = 1;
    uint8_t portParity = 0;
    uint16_t portSpeed = RT_HW_Base.uartGetSpeed(_uartPortNumber);
    timeOfSend = flprogModus::timeForSendBytes(dataBits, stopBits, portParity, portSpeed, bufferSize);
    startSendTime = millis();
    workStatus = FLPROG_MODBUS_WAITING_SENDING;
    bufferSize = 0;
}

bool ModbusRTU::checkAvaliblePacage()
{
     uint16_t avalibleBytes = RT_HW_Base.uartAvailable(_uartPortNumber);
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
    if (!(flprog::isTimer(_time, flprogModus::t35TimeForSpeed(RT_HW_Base.uartGetSpeed(_uartPortNumber)))))
    {
        return false;
    }
    _lastRec = 0;
    return true;
}
