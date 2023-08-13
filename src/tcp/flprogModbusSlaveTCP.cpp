#include "flprogModbusSlaveTCP.h"

ModbusSlaveTCP::ModbusSlaveTCP(FlprogAbstractEthernet *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
}

ModbusSlaveTCP::ModbusSlaveTCP(FLProgAbstracttWiFiInterface *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
}

void ModbusSlaveTCP::begin()
{
    tcpDevice->beServer();
    tcpDevice->begin();
    isInit = true;
}

void ModbusSlaveTCP::begin(uint8_t address)
{
    slaveAddres = address;
    begin();
}

void ModbusSlaveTCP::pool()
{
    if (!isInit)
    {
        begin();
    }

    tcpDevice->connect();
    if (tcpDevice->hasClient())
    {
        getRxBuffer();
    }
    else
    {
        return;
    }
    if (bufferSize == 0)
    {
        return;
    }
    executeSlaveReqest(mainData(), slaveAddres);
}

void ModbusSlaveTCP::getRxBuffer()
{
    uint8_t currentByteIndex = 0;
    bufferSize = 0;
    while (tcpDevice->available())
    {
        if (currentByteIndex < 6)

        {
            mbapBuffer[currentByteIndex] = tcpDevice->read();
        }
        else
        {
            if (bufferSize < 64)
            {
                buffer[bufferSize] = tcpDevice->read();
                bufferSize++;
            }
            else
            {
                tcpDevice->read();
            }
        }
        currentByteIndex++;
    }
}

void ModbusSlaveTCP::sendTxBuffer()
{
    if (buffer[0] == 0)
    {
        return;
    }
    mbapBuffer[4] = highByte(bufferSize);
    mbapBuffer[5] = lowByte(bufferSize);
    tcpDevice->write(mbapBuffer, 6);
    tcpDevice->write(buffer, bufferSize);
    bufferSize = 0;
}

ModbusMainData *ModbusSlaveTCP::mainData()
{
    if (data == 0)
    {
        data = new ModbusMainData();
    }
    return data;
}

//---------------------------------ModbusSlaveRTUoverTCP--------------------------------

void ModbusSlaveRTUoverTCP::pool()
{
    if (!isInit)
    {
        begin();
    }
    tcpDevice->connect();
    if (!tcpDevice->hasClient())
    {
        return;
    }
    if (workStatus == FLPROG_MODBUS_WAITING_SENDING)
    {
        if ((flprog::isTimer(startSendTime, timeOfSend)))
        {
            workStatus = FLPROG_MODBUS_READY;
        }
        else
        {
            return;
        }
    }
    if (!(checkAvaliblePacage()))
        return;
    uint8_t state = rxBuffer();
    if (state < 7)
    {
        return;
    }
    executeSlaveReqest(mainData(), slaveAddres);
}

uint8_t ModbusSlaveRTUoverTCP::validateRequest()
{

    int msgCRC =
        ((buffer[bufferSize - 2] << 8) | buffer[bufferSize - 1]);
    if (flprogModus::modbusCalcCRC(bufferSize - 2, buffer) != msgCRC)
    {
        return 255;
    }
    return validateSlaveReqest(mainData());
}

bool ModbusSlaveRTUoverTCP::checkAvaliblePacage()
{
    uint8_t avalibleBytes = tcpDevice->available();
    if (avalibleBytes == 0)
        return false;
    return true;
}

uint8_t ModbusSlaveRTUoverTCP::rxBuffer()
{
    bool bBuffOverflow = false;
    bufferSize = 0;
    while (tcpDevice->available())
    {
        buffer[bufferSize] = tcpDevice->read();
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

void ModbusSlaveRTUoverTCP::sendTxBuffer()
{
    if (buffer[0] == 0)
    {
        bufferSize = 0;
        return;
    }
    int crc = flprogModus::modbusCalcCRC(bufferSize, buffer);
    buffer[bufferSize] = crc >> 8;
    bufferSize++;
    buffer[bufferSize] = crc & 0x00ff;
    bufferSize++;
    tcpDevice->write(buffer, bufferSize);
    workStatus = FLPROG_MODBUS_READY;
    bufferSize = 0;
}

//---------------------------------ModbusKaScadaCloud--------------------------------
void ModbusKaScadaCloud::setKaScadaCloudIp(uint8_t newFirst_octet, uint8_t newSecond_octet, uint8_t newThird_octet, uint8_t newFourth_octet)
{
    if ((IPAddress(newFirst_octet, newSecond_octet, newThird_octet, newFourth_octet)) == cloudIp)
    {

        return;
    }
    cloudIp[0] = newFirst_octet;
    cloudIp[1] = newFirst_octet;
    cloudIp[2] = newFirst_octet;
    cloudIp[3] = newFirst_octet;
    tcpDevice->stop();
}

void ModbusKaScadaCloud::setKaScadaCloudPort(int newPort)
{
    if (port == newPort)

    {
        return;
    }
    port = newPort;
    tcpDevice->stop();
}

void ModbusKaScadaCloud::setKaScadaCloudDevceId(String id)
{
    if (deniceId.equals(id))

    {
        return;
    }
    deniceId = id;
    tcpDevice->stop();
}

void ModbusKaScadaCloud::begin()
{
    isInit = true;
    kaScadaCloudTimeOutStartTime = millis() + 5000;
    tcpDevice->beClient();
    tcpDevice->begin();
}

void ModbusKaScadaCloud::begin(uint8_t address)
{
    slaveAddres = address;
    begin();
}

void ModbusKaScadaCloud::pool()
{
    if (!isInit)
    {
        begin();
    }
    if (flprog::isTimer(kaScadaCloudTimeOutStartTime, 5000))
    {
        if (tcpDevice->connected())
        {
            tcpDevice->print(deniceId);
            kaScadaCloudTimeOutStartTime = millis();
            return;
        }
        else
        {
            tcpDevice->connect(cloudIp, port);
            return;
        }
    }
    if (!tcpDevice->connected())
    {
        return;
    }

    if (tcpDevice->hasClient())
    {
        getRxBuffer();
    }
    else
    {
        return;
    }

    if (bufferSize == 0)
    {
        return;
    }
    executeSlaveReqest(mainData(), slaveAddres);
    kaScadaCloudTimeOutStartTime = millis();
}

void ModbusKaScadaCloud::sendTxBuffer()
{
    if (buffer[0] == 0)
    {
        return;
    }
    String stringBuffer = "";
    mbapBuffer[4] = highByte(bufferSize);
    mbapBuffer[5] = lowByte(bufferSize);
    tcpDevice->write(mbapBuffer, 6);
    tcpDevice->write(buffer, bufferSize);
    bufferSize = 0;
}

void ModbusKaScadaCloud::getRxBuffer()
{
    uint8_t currentByte = 0;
    uint8_t currentByteIndex = 0;
    bufferSize = 0;
    while (tcpDevice->available())
    {
        currentByte = tcpDevice->read();
        if (currentByteIndex < 6)

        {
            mbapBuffer[currentByteIndex] = currentByte;
        }
        else
        {
            if (bufferSize < 64)
            {
                buffer[bufferSize] = currentByte;
                bufferSize++;
            }
        }
        currentByteIndex++;
    }
}
