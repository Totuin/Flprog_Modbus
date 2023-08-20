#include "flprogModbusBridge.h"

ModbusBridge::ModbusBridge(uint8_t portNumber, FLProgAbstractTcpInterface *sourse)
{
    interface = sourse;
    uart = new FLProgUart(portNumber);
}

ModbusBridge::ModbusBridge(uint8_t rxPin, uint8_t txPin, FLProgAbstractTcpInterface *sourse)
{
    interface = sourse;
    uart = new FLProgSoftwareUart(rxPin, txPin);
}

void ModbusBridge::pool()
{
    if (!isInit)
    {
        begin();
    }
    if (isServer)
    {
        tcpPool();
        rtuPool();
    }
    else
    {
        rtuPool();
        tcpPool();
    }
}

void ModbusBridge::setTCPPort(int _port)
{
    if (port == _port)
    {
        return;
    }
    port = _port;
    isInit = false;
}

void ModbusBridge::setTCPRemoteIp(uint8_t newIpFirst, uint8_t newIpSecond, uint8_t newIpThird, uint8_t newIpFourth)
{
    setTCPRemoteIp(IPAddress(newIpFirst, newIpSecond, newIpThird, newIpFourth));
}

void ModbusBridge::setTCPRemoteIp(IPAddress newIp)
{
    if (newIp == ip)
    {
        return;
    }
    ip = newIp;
    isInit = false;
}

void ModbusBridge::byServer()
{
    if (isServer)
    {
        return;
    }
    isServer = true;
    isInit = false;
}

void ModbusBridge::byClient()
{
    if (!isServer)
    {
        return;
    }
    isServer = false;
    isInit = false;
}

void ModbusBridge::begin()
{
    isInit = true;
    rtuDevice()->begin();
    if (pinPeDe >= 0)
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
    if (isServer)
    {
        server = interface->getServer(port);
    }
    else
    {
        tcpClient = interface->getClient();
    }
}

FLProgUartBasic *ModbusBridge::rtuDevice()
{
    if (uart == 0)
    {
        uart = new FLProgUart();
    }
    return uart;
}

void ModbusBridge::onPeDePin()
{
    if (pinPeDe < 0)
    {
        return;
    }
    digitalWrite(pinPeDe, HIGH);
}

void ModbusBridge::offPeDePin()
{
    if (pinPeDe < 0)
    {
        return;
    }
    digitalWrite(pinPeDe, LOW);
}

void ModbusBridge::rtuPool()
{

    if (workStatus == FLPROG_MODBUS_WAITING_SENDING)
    {
        if ((flprog::isTimer(startSendTime, timeOfSend)))
        {
            workStatus = FLPROG_MODBUS_READY;
            offPeDePin();
        }
        else
        {
            return;
        }
    }
    uint8_t avalibleBytes = rtuDevice()->available();
    if (avalibleBytes == 0)
        return;
    if (avalibleBytes != lastRec)
    {
        lastRec = avalibleBytes;
        startT35 = millis();
        return;
    }
    if (!(flprog::isTimer(startT35, (flprogModus::t35TimeForSpeed(rtuDevice()->getPortSpeed())))))
        return;
    lastRec = 0;
    getRTURxBuffer();
    if (bufferSize < 5)
    {
        return;
    }
    sendTCPBuffer();
}

void ModbusBridge::getRTURxBuffer()
{
    bufferSize = 0;
    while (rtuDevice()->available())
    {
        buffer[bufferSize] = rtuDevice()->read();
        bufferSize++;
    }
}

void ModbusBridge::sendRTUBuffer()
{
    onPeDePin();
    int crc = flprogModus::modbusCalcCRC(bufferSize, buffer);
    buffer[bufferSize] = crc >> 8;
    bufferSize++;
    buffer[bufferSize] = crc & 0x00ff;
    bufferSize++;
    rtuDevice()->write(buffer, bufferSize);
    timeOfSend = flprogModus::timeForSendBytes((rtuDevice()->getPortDataBits()), (rtuDevice()->getPortStopBits()), (rtuDevice()->getPortParity()), (rtuDevice()->getPortSpeed()), bufferSize);
    startSendTime = millis();
    workStatus = FLPROG_MODBUS_WAITING_SENDING;
    bufferSize = 0;
}

Client *ModbusBridge::client()
{
    if (tcpClient != 0)
    {
        if (tcpClient->connected())
        {
            return tcpClient;
        }
    }
    if (isServer)
    {
        if (server == 0)
        {
            server = interface->getServer(port);
        }
        server->setClient();
        tcpClient = server->client();
    }
    if (tcpClient == 0)
    {
        tcpClient = interface->getClient();
    }
    return tcpClient;
}

//------------------------------------ModbusTcpBridge---------------------------------------------

void ModbusTcpBridge::tcpPool()
{
    if (!isServer)
    {
        if (!client()->connected())
        {
            client()->connect(ip, port);
        }
    }

    if (!client()->connected())
    {
        return;
    }
    bufferSize = 0;
    uint8_t byteIndex = 0;
    if (client()->available())
    {
        while (client()->available())
        {
            if (byteIndex < 6)
            {
                mbapBuffer[byteIndex] = client()->read();
                byteIndex++;
            }
            else
            {
                buffer[bufferSize] = client()->read();
                bufferSize++;
            }
        }
        sendRTUBuffer();
    }
}

void ModbusTcpBridge::sendTCPBuffer()
{
    if (!isServer)
    {
        if (!client()->connected())
        {
            client()->connect(ip, port);
        }
    }
    if (!client()->connected())
    {
        return;
    }
    bufferSize -= 2;
    mbapBuffer[4] = highByte(bufferSize);
    mbapBuffer[5] = lowByte(bufferSize);
    client()->write(mbapBuffer, 6);
    client()->write(buffer, bufferSize);
    bufferSize = 0;
}

// --------------------------------ModbusRtuOverTcpBridge------------------------------------
void ModbusRtuOverTcpBridge::tcpPool()
{
    if (!isServer)
    {
        if (!client()->connected())
        {
            client()->connect(ip, port);
        }
    }
    if (!client()->connected())
    {
        return;
    }
    if (client()->available())
    {
        bufferSize = 0;
        while (client()->available())
        {
            if (bufferSize < 64)
            {
                buffer[bufferSize] = client()->read();
                bufferSize++;
            }
            else
            {
                client()->read();
            }
        }
        sendRTUBuffer();
    }
}

void ModbusRtuOverTcpBridge::sendTCPBuffer()
{
    if (!isServer)
    {
        if (!client()->connected())
        {
            client()->connect(ip, port);
        }
    }
    if (!client()->connected())
    {
        return;
    }
    client()->write(buffer, bufferSize);
    bufferSize = 0;
}

//-----------ModbusKasCadaCloudTcpBridge------------------------------

ModbusKasCadaCloudTcpBridge::ModbusKasCadaCloudTcpBridge()
{
    port = 25000;
    ip = IPAddress(94, 250, 249, 225);
}

ModbusKasCadaCloudTcpBridge::ModbusKasCadaCloudTcpBridge(uint8_t portNumber, FLProgAbstractTcpInterface *sourse)
{
    interface = sourse;
    uart = new FLProgUart(portNumber);
    port = 25000;
    ip = IPAddress(94, 250, 249, 225);
}

ModbusKasCadaCloudTcpBridge::ModbusKasCadaCloudTcpBridge(uint8_t rxPin, uint8_t txPin, FLProgAbstractTcpInterface *sourse)
{
    interface = sourse;
    uart = new FLProgSoftwareUart(rxPin, txPin);
    port = 25000;
    ip = IPAddress(94, 250, 249, 225);
}

void ModbusKasCadaCloudTcpBridge::pool()
{
    if (!isInit)
    {
        begin();
        return;
    }
    if (flprog::isTimer(kaScadaCloudTimeStartTime, 5000))
    {
        if (client()->connected())
        {
            client()->print(deniceId);
            kaScadaCloudTimeStartTime = millis();
        }
        else
        {
            client()->connect(ip, port);
            return;
        }
    }
    if (!client()->connected())
    {
        return;
    }
    tcpPool();
    rtuPool();
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudIp(IPAddress newIp)
{
    ip = newIp;
    isInit = false;
}
void ModbusKasCadaCloudTcpBridge::setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    setKaScadaCloudIp(IPAddress(first_octet, second_octet, third_octet, fourth_octet));
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudPort(int port)
{
    port = port;
    isInit = false;
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudDevceId(String id)
{
    deniceId = id;
}
void ModbusKasCadaCloudTcpBridge::begin()
{
    isInit = true;
    rtuDevice()->begin();
    if (pinPeDe >= 0)
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
    kaScadaCloudTimeStartTime = flprog::timeBack(5000);
}
void ModbusKasCadaCloudTcpBridge::tcpPool()
{
    if (!client()->connected())
    {
        return;
    }
    uint8_t readingByte;
    uint8_t byteIndex = 0;
    if (client()->available())
    {
        bufferSize = 0;
        while (client()->available())
        {
            readingByte = client()->read();
            if (byteIndex < 6)
            {
                mbapBuffer[byteIndex] = readingByte;
            }
            else
            {
                buffer[bufferSize] = readingByte;
                bufferSize++;
            }
            byteIndex++;
        }
        sendRTUBuffer();
    }
}

void ModbusKasCadaCloudTcpBridge::sendTCPBuffer()
{
    if (!client()->connected())
    {
        return;
    }
    bufferSize -= 2;
    mbapBuffer[4] = highByte(bufferSize);
    mbapBuffer[5] = lowByte(bufferSize);
    client()->write(mbapBuffer, 6);
    client()->write(buffer, bufferSize);
    bufferSize = 0;
}
