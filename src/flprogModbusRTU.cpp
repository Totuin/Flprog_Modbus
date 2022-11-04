#include "flprogModbusRTU.h"

void ModbusRTU::setPortSpeed(byte speed)
{
    if (portSpeed == speed)
    {
        return;
    }
    portSpeed = speed;
    if (!hasPort())
    {
        return;
    }
    restartPort();
}

void ModbusRTU::setPinPeDe(byte pin)
{
    pinPeDe = pin;
}

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

void ModbusRTU::setPortDataBits(byte dataBits)
{
    if ((dataBits < 5) || (dataBits > 8))
    {
        return;
    }
    if (portDataBits == dataBits)
    {
        return;
    }
    portDataBits = dataBits;
    if (!hasPort())
    {
        return;
    }
    restartPort();
}

void ModbusRTU::setPortStopBits(byte stopBits)
{
    if ((stopBits < 1) || (stopBits > 2))
    {
        return;
    }

    if (portStopBits == stopBits)
    {
        return;
    }
    portStopBits = stopBits;
    if (!hasPort())
    {
        return;
    }
    restartPort();
}

void ModbusRTU::setPortParity(byte parity)
{
    if ((parity < 0) || (parity > 2))
    {
        return;
    }

    if (portParity == parity)
    {
        return;
    }
    portParity = parity;
    if (!hasPort())
    {
        return;
    }
    restartPort();
}

byte ModbusRTU::rxBuffer()
{
    boolean bBuffOverflow = false;
    bufferSize = 0;
    while (portAvailable())
    {
        buffer[bufferSize] = portRead();
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
    portWrite(buffer, bufferSize);
    timeOfSend = flprogModus::timeForSendBytes(portDataBits, portStopBits, portParity, portSpeed, bufferSize);
    startSendTime = millis();
    workStatus = MODBUS_WAITING_SENDING;
    bufferSize = 0;
}

bool ModbusRTU::checkAvalibleBytes()
{
    byte avalibleBytes = portAvailable();
    if (avalibleBytes == 0)
        return false;
    if (avalibleBytes != lastRec)
    {
        lastRec = avalibleBytes;
        time = millis();
        return false;
    }
    if (!(flprog::isTimer(time, flprogModus::t35TimeForSpeed(portSpeed))))
        return false;
    lastRec = 0;
    return true;
}






