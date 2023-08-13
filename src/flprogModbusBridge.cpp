#include "flprogModbusBridge.h"

ModbusBridge::ModbusBridge(uint8_t portNumber, FlprogAbstractEthernet *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
    uart = new FLProgUart(portNumber);
}

ModbusBridge::ModbusBridge(uint8_t portNumber, FLProgAbstracttWiFiInterface *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
    uart = new FLProgUart(portNumber);
}

ModbusBridge::ModbusBridge(uint8_t rxPin, uint8_t txPin, FlprogAbstractEthernet *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
    uart = new FLProgSoftwareUart(rxPin, txPin);
}

ModbusBridge::ModbusBridge(uint8_t rxPin, uint8_t txPin, FLProgAbstracttWiFiInterface *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
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

void ModbusBridge::setTCPPort(int port)
{
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->setPort(port);
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
    tcpDevice->restart();
}

void ModbusBridge::byServer()
{
    if (isServer == true)
    {
        return;
    }
    isServer = true;
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->beServer();
}

void ModbusBridge::byClient()
{
    if (isServer == false)
    {
        return;
    }
    isServer = false;
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->beClient();
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
    if (tcpDevice == 0)
    {
        return;
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

//------------------------------------ModbusTcpBridge---------------------------------------------

void ModbusTcpBridge::tcpPool()
{
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->connect(ip);

    if (!tcpDevice->connected())
    {
        return;
    }
    bufferSize = 0;
    uint8_t byteIndex = 0;
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
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->connect(ip);
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

// --------------------------------ModbusRtuOverTcpBridge------------------------------------
void ModbusRtuOverTcpBridge::tcpPool()
{
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->connect(ip);
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
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->connect(ip);
    if (!tcpDevice->connected())
    {
        return;
    }
    tcpDevice->write(buffer, bufferSize);
    bufferSize = 0;
}

//_______________-----------ModbusKasCadaCloudTcpBridge------------------------------

ModbusKasCadaCloudTcpBridge::ModbusKasCadaCloudTcpBridge(uint8_t portNumber, FlprogAbstractEthernet *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
    uart = new FLProgUart(portNumber);
    tcpDevice->beClient();
}
ModbusKasCadaCloudTcpBridge::ModbusKasCadaCloudTcpBridge(uint8_t portNumber, FLProgAbstracttWiFiInterface *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
    uart = new FLProgUart(portNumber);
    tcpDevice->beClient();
}

ModbusKasCadaCloudTcpBridge::ModbusKasCadaCloudTcpBridge(uint8_t rxPin, uint8_t txPin, FlprogAbstractEthernet *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
    uart = new FLProgSoftwareUart(rxPin, txPin);
    tcpDevice->beClient();
}

ModbusKasCadaCloudTcpBridge::ModbusKasCadaCloudTcpBridge(uint8_t rxPin, uint8_t txPin, FLProgAbstracttWiFiInterface *sourse)
{
    tcpDevice = new FLProgTcpDevice(sourse);
    uart = new FLProgSoftwareUart(rxPin, txPin);
    tcpDevice->beClient();
}

void ModbusKasCadaCloudTcpBridge::pool()
{
    if (!isInit)
    {
        begin();
        return;
    }
    if (tcpDevice == 0)
    {
        return;
    }
    if (flprog::isTimer(kaScadaCloudTimeStartTime, 5000))
    {
        if (tcpDevice->connected())
        {
            tcpDevice->print(deniceId);
            kaScadaCloudTimeStartTime = millis();
        }
        else
        {
            tcpDevice->connect(cloudIp, cloudPort);
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

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudIp(IPAddress newIp)
{
    cloudIp = newIp;
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->stop();
}
void ModbusKasCadaCloudTcpBridge::setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    setKaScadaCloudIp(IPAddress(first_octet, second_octet, third_octet, fourth_octet));
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudPort(int port)
{
    cloudPort = port;
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->stop();
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
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->begin();
}
void ModbusKasCadaCloudTcpBridge::tcpPool()
{
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->connect(cloudIp, cloudPort);
    if (!tcpDevice->connected())
    {
        return;
    }
    uint8_t readingByte;
    uint8_t byteIndex = 0;
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
    if (tcpDevice == 0)
    {
        return;
    }
    tcpDevice->connect(cloudIp, cloudPort);
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

void ModbusKasCadaCloudTcpBridge::setTCPDevice(FLProgTcpDevice *device)
{
    tcpDevice = device;
    tcpDevice->beClient();
}