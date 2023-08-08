#include "flprogModbusBridge.h"

bool ModbusBridge::compareRemoteIp(byte newIpFirst, byte newIpSecond, byte newIpThird, byte newIpFourth)
{
    if (this->ipFirst != newIpFirst)
    {
        return false;
    }
    if (this->ipSecond != newIpSecond)
    {
        return false;
    }
    if (this->ipThird != newIpThird)
    {
        return false;
    }
    if (this->ipFourth != newIpFourth)
    {
        return false;
    }
    return true;
}

void ModbusBridge::setTCPDevice(FLProgTcpDevice *device)
{
    tcpDevice = device;
}

void ModbusBridge::setRTUDevice(FLProgUartBasic *device)
{
    rtuDevice = device;
}

void ModbusBridge::pool()
{
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

void ModbusBridge::getRTURxBuffer()
{
    bufferSize = 0;
    while (rtuDevice->available())
    {
        buffer[bufferSize] = rtuDevice->read();

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
    rtuDevice->write(buffer, bufferSize);
    timeOfSend = timeForSendbytes(bufferSize);
    startSendTime = millis();
    workStatus = FLPROG_MODBUS_WAITING_SENDING;
    bufferSize = 0;
}

long ModbusBridge::timeForSendbytes(byte bufferSize)
{
    return flprogModus::timeForSendBytes((rtuDevice->getPortDataBits()), (rtuDevice->getPortStopBits()), (rtuDevice->getPortParity()), (rtuDevice->getPortSpeed()), bufferSize);
}

void ModbusBridge::setPinPeDe(byte pin)
{
    pinPeDe = pin;
}

void ModbusBridge::onPeDePin()
{
    if (pinPeDe == 200)
    {
        return;
    }
    digitalWrite(pinPeDe, HIGH);
}

void ModbusBridge::offPeDePin()
{
    if (pinPeDe == 200)
    {
        return;
    }
    digitalWrite(pinPeDe, LOW);
}

void ModbusBridge::begin()
{
    rtuDevice->begin();
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
    if (isServer)
    {
        tcpDevice->beServer();
    }
    else
    {
        tcpDevice->beClient();
    }
    tcpDevice->begin();
}

void ModbusBridge::setTCPPort(int port)
{
    tcpDevice->setPort(port);
}

void ModbusBridge::setRtuPortSpeed(byte speed)
{
    rtuDevice->setPortSpeed(speed);
}

void ModbusBridge::setRtuPortDataBits(byte dataBits)
{
    rtuDevice->setPortDataBits(dataBits);
}

void ModbusBridge::setRtuPortStopBits(byte stopBits)
{
    rtuDevice->setPortStopBits(stopBits);
}

void ModbusBridge::setRtuPortParity(byte stopBits)
{
    rtuDevice->setPortParity(stopBits);
}

void ModbusBridge::byServer()
{
    if (isServer == true)
    {
        return;
    }
    isServer = true;
    tcpDevice->beServer();
}

void ModbusBridge::byClient()
{
    if (isServer == false)
    {
        return;
    }
    isServer = false;
    tcpDevice->beClient();
}

void ModbusBridge::setTCPRemoteIp(byte newIpFirst, byte newIpSecond, byte newIpThird, byte newIpFourth)
{
    if (compareRemoteIp(newIpFirst, newIpSecond, newIpThird, newIpFourth))
    {
        return;
    }
    ipFirst = newIpFirst;
    ipSecond = newIpSecond;
    ipThird = newIpThird;
    ipFourth = newIpFourth;
    tcpDevice->restart();
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
    byte avalibleBytes = rtuDevice->available();
    if (avalibleBytes == 0)
        return;
    if (avalibleBytes != lastRec)
    {
        lastRec = avalibleBytes;
        startT35 = millis();
        return;
    }
    if (!(flprog::isTimer(startT35, (t35TimeForSpeed()))))
        return;
    lastRec = 0;
    getRTURxBuffer();
    if (bufferSize < 5)
    {
        return;
    }
    sendTCPBuffer();
}

long ModbusBridge::t35TimeForSpeed()
{
    return flprogModus::t35TimeForSpeed(rtuDevice->getPortSpeed());
}

// ModbusTcpBridge******
void ModbusTcpBridge::tcpPool()
{
    tcpDevice->connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth));

    if (!tcpDevice->connected())
    {
        return;
    }
    bufferSize = 0;
    byte byteIndex = 0;
    if (tcpDevice->available())
    {
        while (tcpDevice->available())
        {
            if (byteIndex < 6)
            {
                mbapBuffer[byteIndex] = tcpDevice->read();
                byteIndex++;
            }
            else
            {
                buffer[bufferSize] = tcpDevice->read();
                bufferSize++;
            }
        }
        sendRTUBuffer();
    }
}

void ModbusTcpBridge::sendTCPBuffer()
{
    tcpDevice->connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth));
    if (!tcpDevice->connected())
    {
        return;
    }
    bufferSize -= 2;
    mbapBuffer[4] = highByte(bufferSize);
    mbapBuffer[5] = lowByte(bufferSize);
    tcpDevice->write(mbapBuffer, 6);
    tcpDevice->write(buffer, bufferSize);
    bufferSize = 0;
}

// ModbusRtuOverTcpBridge*****************
void ModbusRtuOverTcpBridge::tcpPool()
{
    tcpDevice->connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth));

    if (!tcpDevice->connected())
    {
        return;
    }
    if (tcpDevice->available())
    {
        bufferSize = 0;
        while (tcpDevice->available())
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
        sendRTUBuffer();
    }
}

void ModbusRtuOverTcpBridge::sendTCPBuffer()
{
    tcpDevice->connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth));
    if (!tcpDevice->connected())
    {
        return;
    }
    tcpDevice->write(buffer, bufferSize);
    bufferSize = 0;
}

// ModbusKasCadaCloudTcpBridge*************
void ModbusKasCadaCloudTcpBridge::pool()
{
    isServer = false;
    if (flprog::isTimer(kaScadaCloudTimeStartTime, 5000))
    {
        if (tcpDevice->connected())
        {
            tcpDevice->print(deniceId);
            kaScadaCloudTimeStartTime = millis();
        }
        else
        {
            tcpDevice->connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth));
            return;
        }
    }
    if (!tcpDevice->connected())
    {
        return;
    }
    tcpPool();
    rtuPool();
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    isServer = false;
    tcpDevice->beClient();
    if (compareRemoteIp(first_octet, second_octet, third_octet, fourth_octet))
    {
        return;
    }
    ipFirst = first_octet;
    ipSecond = second_octet;
    ipThird = third_octet;
    ipFourth = fourth_octet;
    tcpDevice->restart();
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudPort(int port)
{
    isServer = false;
    tcpDevice->beClient();
    tcpDevice->setPort(port);
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudDevceId(String id)
{
    isServer = false;
    tcpDevice->beClient();
    if (deniceId.equals(id))

    {
        return;
    }
    deniceId = id;
    tcpDevice->stop();
}

void ModbusKasCadaCloudTcpBridge::tcpPool()
{
    isServer = false;
    tcpDevice->beClient();
    tcpDevice->connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth));

    if (!tcpDevice->connected())
    {
        return;
    }
    byte readingByte;
    byte byteIndex = 0;
    if (tcpDevice->available())
    {
        bufferSize = 0;
        while (tcpDevice->available())
        {
            readingByte = tcpDevice->read();
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
    isServer = false;
    tcpDevice->beClient();
    tcpDevice->connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth));
    if (!tcpDevice->connected())
    {
        return;
    }
    bufferSize -= 2;
    mbapBuffer[4] = highByte(bufferSize);
    mbapBuffer[5] = lowByte(bufferSize);
    tcpDevice->write(mbapBuffer, 6);
    tcpDevice->write(buffer, bufferSize);
    bufferSize = 0;
}

void ModbusKasCadaCloudTcpBridge::begin()
{
    isServer = false;
    tcpDevice->beClient();
    rtuDevice->begin();
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
    tcpDevice->begin();
}