#include "flprogModbusRTU.h"

void ModbusRTU::onPeDePin()
{
    if (pinPeDe == 200)
    {
        return;
    }
    digitalWrite(pinPeDe, HIGH);
}

void ModbusRTU::offPeDePin()
{
    if (pinPeDe == 200)
    {
        return;
    }
    digitalWrite(pinPeDe, LOW);
}

byte ModbusRTU::rxBuffer()
{
    boolean bBuffOverflow = false;
    bufferSize = 0;
    while (uart->available())
    {
        buffer[bufferSize] = uart->read();
        bufferSize++;
        if (bufferSize >= 64)
            bBuffOverflow = true;
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
    byte i = 0;
    onPeDePin();
    int crc = flprogModus::modbusCalcCRC(bufferSize, buffer);
    buffer[bufferSize] = crc >> 8;
    bufferSize++;
    buffer[bufferSize] = crc & 0x00ff;
    bufferSize++;
    uart->write(buffer, bufferSize);
    timeOfSend = flprogModus::timeForSendBytes((uart->getPortDataBits()), (uart->getPortStopBits()), (uart->getPortParity()),(uart->getPortSpeed()), bufferSize);
    startSendTime = millis();
    workStatus = MODBUS_WAITING_SENDING;
    bufferSize = 0;
}

bool ModbusRTU::checkAvalibleBytes()
{
    byte avalibleBytes = uart->available();
    if (avalibleBytes == 0)
        return false;
    if (avalibleBytes != lastRec)
    {
        lastRec = avalibleBytes;
        time = millis();
        return false;
    }
    if (!(flprog::isTimer(time, flprogModus::t35TimeForSpeed(uart->getPortSpeed()))))
        return false;
    lastRec = 0;
    return true;
}
