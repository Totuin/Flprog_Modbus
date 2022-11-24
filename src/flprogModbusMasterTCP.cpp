#include "flprogModbusMasterTCP.h"

// ModbusTCPSlaveServer************

void ModbusTCPSlaveServer::setSlaves(ModbusSlaveInMaster table[], int size)
{
    this->slaves = table;
    this->slavesSize = size;
}

void ModbusTCPSlaveServer::setPort(int serverPort)
{
    if (serverPort == this->port)
    {
        return;
    }

    this->port = serverPort;
}

void ModbusTCPSlaveServer::setIpAdress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    if (compareWithIpAddress(first_octet, second_octet, third_octet, fourth_octet))
    {
        return;
    }
    ipFirst = first_octet;
    ipSecond = second_octet;
    ipThird = third_octet;
    ipFourth = fourth_octet;
}

bool ModbusTCPSlaveServer::compareWithIpAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    if (ipFirst != first_octet)
    {
        return false;
    }
    if (ipSecond != second_octet)
    {
        return false;
    }
    if (ipThird != third_octet)
    {
        return false;
    }
    if (ipFourth != fourth_octet)
    {
        return false;
    }
    return true;
}

bool ModbusTCPSlaveServer::hasSlave(int slaveIndex)
{
    if ((slaveIndex > -1) && (slaveIndex < slavesSize))
    {
        return true;
    }
    return false;
}

void ModbusTCPSlaveServer::saveLong(int slave, long value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveLong(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveUnsignedLong(int slave, unsigned long value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveUnsignedLong(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveFloat(int slave, float value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveFloat(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveInteger(int slave, int value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveInteger(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveByte(int slave, byte value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveByte(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveBool(int slave, bool value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveBool(value, table, startAddres);
}

byte ModbusTCPSlaveServer::readByte(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readByte(table, startAddres);
}

int ModbusTCPSlaveServer::readInteger(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readInteger(table, startAddres);
}

float ModbusTCPSlaveServer::readFloat(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readFloat(table, startAddres);
}

long ModbusTCPSlaveServer::readLong(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readLong(table, startAddres);
}

unsigned long ModbusTCPSlaveServer::readUnsignedLong(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readUnsignedLong(table, startAddres);
}

bool ModbusTCPSlaveServer::readBool(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return false;
    }
    return slaves[slave].readBool(table, startAddres);
}

bool ModbusTCPSlaveServer::hasWriteRegisters()
{
    return !(firstWriteSlave() == 0);
}

ModbusSlaveInMaster *ModbusTCPSlaveServer::firstWriteSlave()
{
    for (int i = 0; i < slavesSize; i++)
    {
        if ((slaves[i].isActive) && (slaves[i].hasWriteRegisters()))
        {
            return &slaves[i];
        }
    }
    return 0;
}

ModbusSlaveInMaster *ModbusTCPSlaveServer::firstReadySlave()
{

    for (int i = 0; i < slavesSize; i++)
    {
        if (slaves[i].isReady())
        {
            return &slaves[i];
        }
    }
    return 0;
}

ModbusSlaveInMaster *ModbusTCPSlaveServer::nextSlave(ModbusSlaveInMaster *currentSlave)
{
    bool isFindSlave = false;
    for (int i = 0; i < slavesSize; i++)
    {
        if (isFindSlave)
        {
            if (slaves[i].isReady())
            {
                return &slaves[i];
            }
        }
        else
        {
            if (currentSlave == &slaves[i])
            {
                isFindSlave = true;
            }
        }
    }
    return 0;
}

bool ModbusTCPSlaveServer::isReady()
{

    for (int i = 0; i < slavesSize; i++)
    {
        if (slaves[i].isReady())
        {
            return true;
        }
    }
    return false;
}

void ModbusTCPSlaveServer::setSlaveAddress(int slave, byte address)
{
    slaves[slave].setSlaveAddress(address);
}

void ModbusTCPSlaveServer::setPollingPeriod(int slave, unsigned long period)
{
    slaves[slave].setPollingPeriod(period);
}

void ModbusTCPSlaveServer::setLongOrder(int slave, byte order)
{
    slaves[slave].setLongOrder(order);
}

void ModbusTCPSlaveServer::setFloatOrder(int slave, byte order)
{
    slaves[slave].setFloatOrder(order);
}

void ModbusTCPSlaveServer::setUnsignedlongOrder(int slave, byte order)
{
    slaves[slave].setUnsignedlongOrder(order);
}

void ModbusTCPSlaveServer::setIntOrder(int slave, byte order)
{
    slaves[slave].setIntOrder(order);
}

void ModbusTCPSlaveServer::setDataTable(int slave, ModbusWorldTable *table)
{
    slaves[slave].setDataTable(table);
}

void ModbusTCPSlaveServer::setDataTable(int slave, ModbusBoolTable *table)
{
    slaves[slave].setDataTable(table);
}

uint8_t ModbusTCPSlaveServer::getIp(byte index)
{

    if (index == 0)
    {
        return ipFirst;
    }
    if (index == 1)
    {
        return ipSecond;
    }
    if (index == 2)
    {
        return ipThird;
    }
    if (index == 3)
    {
        return ipFourth;
    }
    return 0;
}

byte ModbusTCPSlaveServer::getLastError(int slave)
{
    return slaves[slave].getLastError();
}

void ModbusTCPSlaveServer::status(int slave, bool status)
{
    slaves[slave].status(status);
}

// ModbusMasterTCP******************

bool ModbusMasterTCP::hasServer(int serverIndex)
{
    if ((serverIndex > -1) && (serverIndex < serversSize))
    {
        return true;
    }
    return false;
}

void ModbusMasterTCP::saveLong(int server, int slave, long value, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return;
    }
    servers[server].saveLong(slave, value, table, startAddres);
}

void ModbusMasterTCP::saveUnsignedLong(int server, int slave, unsigned long value, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return;
    }
    servers[server].saveUnsignedLong(slave, value, table, startAddres);
}

void ModbusMasterTCP::saveFloat(int server, int slave, float value, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return;
    }
    servers[server].saveFloat(slave, value, table, startAddres);
}

void ModbusMasterTCP::saveInteger(int server, int slave, int value, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return;
    }
    servers[server].saveInteger(slave, value, table, startAddres);
}

void ModbusMasterTCP::saveByte(int server, int slave, byte value, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return;
    }
    servers[server].saveByte(slave, value, table, startAddres);
}

void ModbusMasterTCP::saveBool(int server, int slave, bool value, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return;
    }
    servers[server].saveBool(slave, value, table, startAddres);
}

byte ModbusMasterTCP::readByte(int server, int slave, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return 0;
    }
    return servers[server].readByte(slave, table, startAddres);
}

int ModbusMasterTCP::readInteger(int server, int slave, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return 0;
    }
    return servers[server].readInteger(slave, table, startAddres);
}

float ModbusMasterTCP::readFloat(int server, int slave, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return 0;
    }
    return servers[server].readFloat(slave, table, startAddres);
}

long ModbusMasterTCP::readLong(int server, int slave, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return 0;
    }
    return servers[server].readLong(slave, table, startAddres);
}

unsigned long ModbusMasterTCP::readUnsignedLong(int server, int slave, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return 0;
    }
    return servers[server].readUnsignedLong(slave, table, startAddres);
}

bool ModbusMasterTCP::readBool(int server, int slave, byte table, int startAddres)
{
    if (!hasServer(server))
    {
        return false;
    }
    return servers[server].readBool(slave, table, startAddres);
}

void ModbusMasterTCP::pool()
{

    if (workStatus == MODBUS_WAITING_ANSWER)
    {
        checkAnswer();
    }
    if (workStatus == MODBUS_READY)
    {
        nextQuery();
    }
}

void ModbusMasterTCP::checkAnswer()
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
    if (mbapBuffer[0] != highByte(telegrammAnswerId))
    {
        return;
    }
    if (mbapBuffer[1] != lowByte(telegrammAnswerId))
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

void ModbusMasterTCP::getRxBuffer()
{
    byte currentByte = 0;
    boolean buffOverflow = false;
    byte currentByteIndex = 0;
    bufferSize = 0;
    while (available())
    {
        currentByte = read();
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

byte ModbusMasterTCP::validateRequest()
{
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

void ModbusMasterTCP::nextQuery()
{
    if (!createNewTelegramm())
    {
        return;
    }
    sendQuery();
}

bool ModbusMasterTCP::createNewTelegramm()
{
    if (hasWriteRegisters())
    {
        return createWriteTelegramm();
    }
    if (!nextStep())
    {
        return false;
    }

    return createReadTelegram();
}

bool ModbusMasterTCP::createReadTelegram()
{
    telegrammServer = currentServer;
    telegrammSlave = currentSlave;
    telegrammTable = currentSlaveTable;
    telegrammStartAddres = currentSlaveStartAddress;
    telegrammNumbeRegs = currentSlaveTable->readRegsSize(telegrammStartAddres);
    currentSlaveLastAddress = telegrammStartAddres + telegrammNumbeRegs - 1;

    if (currentSlaveTable->table == COIL)
    {
        telegrammFunction = 1;
    }
    if (currentSlaveTable->table == DISCRETE_INPUT)
    {
        telegrammFunction = 2;
    }
    if (currentSlaveTable->table == INPUT_REGISTR)
    {
        telegrammFunction = 4;
    }
    if (currentSlaveTable->table == HOLDING_REGISTR)
    {
        telegrammFunction = 3;
    }
    return true;
}

bool ModbusMasterTCP::nextStep()
{
    if (!hasReadyServer())
    {
        return false;
    }
    if (currentServer == 0)
    {
        currentServer = firstReadyServer();
        currentSlave = currentServer->firstReadySlave();
        currentSlaveTable = currentSlave->firstTabe();
        currentSlaveStartAddress = currentSlaveTable->getMinAdress();
        return true;
    }
    return nextRegistor();
}

bool ModbusMasterTCP::nextRegistor()
{
    currentSlaveStartAddress = currentSlaveTable->findNextStartAddres(currentSlaveLastAddress);
    if (currentSlaveStartAddress == -1)
    {
        return nextTable();
    }
    return true;
}

bool ModbusMasterTCP::nextTable()
{
    currentSlaveTable = currentSlave->nextTable(currentSlaveTable);
    if (currentSlaveTable == 0)
    {
        return nextSlave();
    }

    currentSlaveStartAddress = currentSlaveTable->getMinAdress();
    return true;
}

bool ModbusMasterTCP::nextSlave()

{
    if (currentSlave != 0)
    {
        currentSlave->lastReqestTime = millis();
        currentSlave->initFlag = true;
    }
    currentSlave = currentServer->nextSlave(currentSlave);
    if (currentSlave == 0)
    {
        return nextServer();
    }
    currentSlaveTable = currentSlave->firstTabe();
    currentSlaveStartAddress = currentSlaveTable->getMinAdress();
    return true;
}

bool ModbusMasterTCP::nextServer()
{
    stop();
    currentServer = nextReadyServer(currentServer);

    if (currentServer == 0)
    {
        return false;
    }
    currentSlave = currentServer->firstReadySlave();
    currentSlaveTable = currentSlave->firstTabe();
    currentSlaveStartAddress = currentSlaveTable->getMinAdress();
    return true;
}

ModbusTCPSlaveServer *ModbusMasterTCP::firstReadyServer()
{
    for (int i = 0; i < serversSize; i++)
    {
        if (servers[i].isReady())
        {
            return &servers[i];
        }
    }
    return 0;
}

ModbusTCPSlaveServer *ModbusMasterTCP::nextReadyServer(ModbusTCPSlaveServer *oldServer)
{
    bool isFindServer = false;
    for (int i = 0; i < serversSize; i++)
    {
        if (isFindServer)
        {
            if (servers[i].isReady())
            {
                return &servers[i];
            }
        }
        else
        {
            if (oldServer == &servers[i])
            {
                isFindServer = true;
            }
        }
    }
    return 0;
}

bool ModbusMasterTCP::hasReadyServer()
{
    for (int i = 0; i < serversSize; i++)
    {
        if (servers[i].isReady())
        {
            return true;
        }
    }
    return false;
}

void ModbusMasterTCP::sendQuery()

{
    telegrammAnswerId++;
    mbapBuffer[0] = highByte(telegrammAnswerId);
    mbapBuffer[1] = lowByte(telegrammAnswerId);
    mbapBuffer[2] = 0;
    mbapBuffer[3] = 0;
    buffer[0] = telegrammSlave->slaveAddres;
    buffer[1] = telegrammFunction;
    create_PDU(telegrammTable, telegrammStartAddres, telegrammNumbeRegs);
    startSendTime = millis();
    workStatus = MODBUS_WAITING_ANSWER;
}

bool ModbusMasterTCP::hasWriteRegisters()
{
    return !(firstWriteServer() == 0);
}

bool ModbusMasterTCP::createWriteTelegramm()
{
    telegrammServer = firstWriteServer();
    telegrammSlave = telegrammServer->firstWriteSlave();
    telegrammTable = telegrammSlave->firstWriteTable();
    telegrammStartAddres = telegrammTable->firstWriteAddress();
    telegrammNumbeRegs = telegrammTable->writeRegsSize(telegrammStartAddres);
    if (telegrammTable->table == HOLDING_REGISTR)
    {
        if (telegrammNumbeRegs == 1)
        {
            telegrammFunction = 6;
        }
        else
        {
            telegrammFunction = 16;
        }
    }
    else
    {
        if (telegrammNumbeRegs == 1)
        {
            telegrammFunction = 5;
        }
        else
        {
            telegrammFunction = 15;
        }
    }
    for (byte i = 0; i < telegrammNumbeRegs; i++)
    {
        telegrammTable->resetWriteFlag(telegrammStartAddres + i);
    }
    return true;
}

ModbusTCPSlaveServer *ModbusMasterTCP::firstWriteServer()
{
    for (int i = 0; i < serversSize; i++)
    {
        if (servers[i].hasWriteRegisters())
        {
            return &servers[i];
        }
    }
    return 0;
}

void ModbusMasterTCP::sendTxBuffer()
{
    byte resultBuffer[70];
    byte resultBufferSize = 0;
    mbapBuffer[4] = highByte(bufferSize);
    mbapBuffer[5] = lowByte(bufferSize);
    for (int i = 0; i < 6; i++)
    {
        resultBuffer[resultBufferSize] = mbapBuffer[i];
        resultBufferSize++;
    }
    for (int i = 0; i < bufferSize; i++)
    {
        resultBuffer[resultBufferSize] = buffer[i];
        resultBufferSize++;
    }
    connect(telegrammServer);
    write(telegrammServer, resultBuffer, resultBufferSize);
    bufferSize = 0;
}

void ModbusMasterTCP::setSlavesToServer(int server, ModbusSlaveInMaster table[], int size)
{
    servers[server].setSlaves(table, size);
}

void ModbusMasterTCP::setServerPort(int server, int serverPort)
{
    servers[server].setPort(serverPort);
}

void ModbusMasterTCP::setServerIP(int server, uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    servers[server].setIpAdress(first_octet, second_octet, third_octet, fourth_octet);
}

void ModbusMasterTCP::setSlaveAddress(int server, int slave, byte address)
{
    servers[server].setSlaveAddress(slave, address);
}

void ModbusMasterTCP::setPollingPeriod(int server, int slave, unsigned long period)
{
    servers[server].setPollingPeriod(slave, period);
}

void ModbusMasterTCP::setLongOrder(int server, int slave, byte order)
{
    servers[server].setLongOrder(slave, order);
}

void ModbusMasterTCP::setFloatOrder(int server, int slave, byte order)
{
    servers[server].setFloatOrder(slave, order);
}

void ModbusMasterTCP::setUnsignedlongOrder(int server, int slave, byte order)
{
    servers[server].setUnsignedlongOrder(slave, order);
}

void ModbusMasterTCP::setIntOrder(int server, int slave, byte order)
{
    servers[server].setIntOrder(slave, order);
}

void ModbusMasterTCP::setDataTable(int server, int slave, ModbusWorldTable *table)
{
    servers[server].setDataTable(slave, table);
}

void ModbusMasterTCP::setDataTable(int server, int slave, ModbusBoolTable *table)
{
    servers[server].setDataTable(slave, table);
}

byte ModbusMasterTCP::getLastError(int server, int slave)
{
    servers[server].getLastError(slave);
}

void ModbusMasterTCP::status(int server, int slave, bool status)
{
    servers[server].status(slave, status);
}