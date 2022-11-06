#include "flprogModbusKaScadaCloudW5100.h"

#ifndef ESP32
void ModbusKaScadaCloudW5100::setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    IPAddress tempIp = IPAddress(first_octet, second_octet, third_octet, fourth_octet);
    if (kaScadaCloudIp == tempIp)
        ;
    {
        return;
    }
    kaScadaCloudIp == tempIp;
    client.stop();
}

void ModbusKaScadaCloudW5100::setKaScadaCloudPort(int port)
{
    if (this->port == port)

    {
        return;
    }
    this->port = port;
    client.stop();
}

void ModbusKaScadaCloudW5100::setKaScadaCloudDevceId(String id)
{
    if (deniceId.equals(id))

    {
        return;
    }
    deniceId = id;
    client.stop();
}

void ModbusKaScadaCloudW5100::begin()
{
    kaScadaCloudTimeOutStartTime = millis() - 5000;
}

void ModbusKaScadaCloudW5100::pool()
{
    if (flprog::isTimer(kaScadaCloudTimeOutStartTime, 5000))
    {
        if (client.connected())
        {
            client.print(deniceId);
            kaScadaCloudTimeOutStartTime = millis();
        }
        else
        {
            client.connect(kaScadaCloudIp, port);
            return;
        }
    }
    if (!client.connected())
    {
        return;
    }
    if (client)
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
    executeSlaveReqest(data);
}

void ModbusKaScadaCloudW5100::sendTxBuffer()
{
    if (buffer[0] == 0)
    {
        return;
    }
    String stringBuffer = "";
    mbapBuffer[4] = highByte(bufferSize);
    mbapBuffer[5] = lowByte(bufferSize);
    for (int i = 0; i < 6; i++)
    {
        stringBuffer.concat(char(mbapBuffer[i]));
    }
    for (int i = 0; i < bufferSize; i++)
    {
        stringBuffer.concat(char(buffer[i]));
    }
    client.print(stringBuffer);
    bufferSize = 0;
}

void ModbusKaScadaCloudW5100::getRxBuffer()
{
    byte currentByte = 0;
    boolean buffOverflow = false;
    byte currentByteIndex = 0;
    bufferSize = 0;
    while (client.available())
    {
        currentByte = client.read();
        if (currentByteIndex < 6)

        {
            mbapBuffer[currentByteIndex] = currentByte;
        }
        else
        {
            buffer[bufferSize] = currentByte;
            bufferSize++;
            if (bufferSize >= 64)
                buffOverflow = true;
        }
        currentByteIndex++;
    }
}
#endif