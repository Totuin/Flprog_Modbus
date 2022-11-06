#include "flprogModbusSlaveTCPW5100.h"

#ifndef ESP32
ModbusSlaveTCPW5100::ModbusSlaveTCPW5100(EthernetServer *server)
{
    this->server = server;
}

void ModbusSlaveTCPW5100::begin()
{
    server->begin();
}

void ModbusSlaveTCPW5100::pool()
{
    if (!client.connected())
    {
        client = server->available();
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

void ModbusSlaveTCPW5100::getRxBuffer()
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

void ModbusSlaveTCPW5100::sendTxBuffer()
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

#endif