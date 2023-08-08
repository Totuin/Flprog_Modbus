#include "flprogModbusRTU.h"

FLProgUartBasic *ModbusRTU::uardDevice()
{
    if (uart == 0)
    {
        uart = new FLProgUart();
    }
    return uart;
}

void ModbusRTU::onPeDePin()
{
    if (pinPeDe < 0)
    {
        return;
    }
    digitalWrite(pinPeDe, HIGH);
}

void ModbusRTU::offPeDePin()
{
    if (pinPeDe < 0)
    {
        return;
    }
    digitalWrite(pinPeDe, LOW);
}

uint8_t ModbusRTU::rxBuffer()
{
    bool bBuffOverflow = false;
    bufferSize = 0;
    while (uardDevice()->available())
    {
        if (bufferSize < 64)
        {
            buffer[bufferSize] = uardDevice()->read();
        }
        else
        {
            uardDevice()->read();
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
    uardDevice()->write(buffer, bufferSize);
    timeOfSend = flprogModus::timeForSendBytes((uardDevice()->getPortDataBits()), (uardDevice()->getPortStopBits()), (uardDevice()->getPortParity()), (uardDevice()->getPortSpeed()), bufferSize);
    startSendTime = millis();
    workStatus = FLPROG_MODBUS_WAITING_SENDING;
    bufferSize = 0;
}

bool ModbusRTU::checkAvaliblePacage()
{
    uint8_t avalibleBytes = uardDevice()->available();
    if (avalibleBytes == 0)
    {
        return false;
    }

    if (avalibleBytes != lastRec)
    {
        lastRec = avalibleBytes;
        time = millis();
        {
            return false;
        }
    }
    if (!(flprog::isTimer(time, flprogModus::t35TimeForSpeed(uardDevice()->getPortSpeed()))))
    {
        return false;
    }
    lastRec = 0;
    return true;
}
