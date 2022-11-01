#include "flprogModbusMasterRTUoverTCPWifiESP32.h"

#ifdef ESP32
ModbusMasterRTUoverTCPWifiESP32::ModbusMasterRTUoverTCPWifiESP32(ModbusTCPSlaveServer table[], int size)
{
    serversSize = size;
    servers = table;
}

void ModbusMasterRTUoverTCPWifiESP32::checkAnswer()
{
    if (flprog::isTimer(startSendTime, (telegrammSlave->getTimeOutTime())))
    {
        telegrammSlave->setLastError(255);
        workStatus = MODBUS_READY;
        return;
    }
    getRxBuffer();
    if (bufferSize == 0)
    {
        return;
    }
    byte exception = validateRequest();
    if (exception > 0)
    {
        telegrammSlave->setLastError(exception);
        workStatus = MODBUS_READY;
        return;
    }
    telegrammSlave->setLastError(0);
    writeMaserData(telegrammTable, telegrammStartAddres, telegrammNumbeRegs);
}

byte ModbusMasterRTUoverTCPWifiESP32::validateRequest()
{
    uint16_t msgCRC = ((buffer[bufferSize - 2] << 8) | buffer[bufferSize - 1]);
    if (modbusCalcCRC(bufferSize - 2, buffer) != msgCRC)
    {
        return 255;
    }
    if ((buffer[1] & 0x80) != 0)
    {
        return buffer[2];
    }
    if (!(telegrammSlave->isSupportFunction(buffer[1])))
    {
        return 1;
    }
    return 0;
}

void ModbusMasterRTUoverTCPWifiESP32::getRxBuffer()
{

    boolean buffOverflow = false;

    bufferSize = 0;
    while (telegrammServer->available())
    {
        buffer[bufferSize] = telegrammServer->read();
        bufferSize++;
        if (bufferSize >= 64)
        {
            buffOverflow = true;
        }
    }
}

void ModbusMasterRTUoverTCPWifiESP32::sendTxBuffer()
{
    byte i = 0;
    int crc = modbusCalcCRC(bufferSize, buffer);
    buffer[bufferSize] = crc >> 8;
    bufferSize++;
    buffer[bufferSize] = crc & 0x00ff;
    bufferSize++;
    telegrammServer->connect();
    telegrammServer->write(buffer, bufferSize);
    bufferSize = 0;
}

#endif