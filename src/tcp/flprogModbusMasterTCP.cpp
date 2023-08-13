#include "flprogModbusMasterTCP.h"

// ModbusTCPSlaveServer************

void ModbusTCPSlaveServer::setSlaves(ModbusSlaveInMaster *_table, uint8_t size)
{
    slaves = _table;
    slavesSize = size;
}

void ModbusTCPSlaveServer::setSlavesSize(uint8_t size)
{
    slavesSize = size;
    slaves = new ModbusSlaveInMaster[slavesSize];
}

bool ModbusTCPSlaveServer::hasSlave(uint8_t slaveIndex)
{
    return slaveIndex < slavesSize;
}

ModbusSlaveInMaster *ModbusTCPSlaveServer::slaveOnIndex(uint8_t slaveIndex)
{
    if (slaves == 0)
    {
        slaves = new ModbusSlaveInMaster[slavesSize];
    }
    if (hasSlave(slaveIndex))
    {
        return &slaves[slaveIndex];
    }
    return 0;
}

ModbusSlaveInMaster *ModbusTCPSlaveServer::slave(uint8_t slaveAddr, bool isIndex)
{
    if (isIndex)
    {
        return slaveOnIndex(slaveAddr);
    }
    if (slaves == 0)
    {
        slaves = new ModbusSlaveInMaster[slavesSize];
    }

    for (uint8_t i = 0; i < slavesSize; i++)
    {
        if (slaves[i].slaveAddres() == slaveAddr)
        {
            return &slaves[i];
        }
    }
    return 0;
}

void ModbusTCPSlaveServer::setDataTable(uint8_t slaveAdr, ModbusTable *table, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setDataTable(table);
}

void ModbusTCPSlaveServer::setDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, int *_adresses, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setDataTable(_table, dataSize, _adresses);
}

void ModbusTCPSlaveServer::configDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->configDataTable(_table, dataSize);
}

void ModbusTCPSlaveServer::configDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, uint16_t _startAdr, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->configDataTable(_table, dataSize, _startAdr);
}

void ModbusTCPSlaveServer::setSlaveAddress(uint8_t slaveIndex, uint16_t addr)
{
    ModbusSlaveInMaster *sl = slave(slaveIndex, true);
    if (sl == 0)
    {
        return;
    }
    sl->setSlaveAddress(addr);
}

void ModbusTCPSlaveServer::setPollingPeriod(uint8_t slaveAdr, uint32_t period, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setPollingPeriod(period);
}

void ModbusTCPSlaveServer::setTimeOutTime(uint8_t slaveAdr, uint32_t time, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setTimeOutTime(time);
}

void ModbusTCPSlaveServer::setLongOrder(uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setLongOrder(order);
}

void ModbusTCPSlaveServer::setFloatOrder(uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setFloatOrder(order);
}

void ModbusTCPSlaveServer::setUnsignedlongOrder(uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setUnsignedlongOrder(order);
}

void ModbusTCPSlaveServer::setIntOrder(uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setIntOrder(order);
}

void ModbusTCPSlaveServer::saveLong(uint8_t slaveAdr, int32_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveLong(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveUnsignedLong(uint8_t slaveAdr, uint32_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveUnsignedLong(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveFloat(uint8_t slaveAdr, float value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveFloat(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveInteger(uint8_t slaveAdr, int value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveInteger(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveByte(uint8_t slaveAdr, uint8_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveByte(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveBool(uint8_t slaveAdr, bool value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveBool(value, table, startAddres);
}

uint8_t ModbusTCPSlaveServer::readByte(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readByte(table, startAddres);
}

int ModbusTCPSlaveServer::readInteger(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readInteger(table, startAddres);
}

float ModbusTCPSlaveServer::readFloat(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readFloat(table, startAddres);
}

int32_t ModbusTCPSlaveServer::readLong(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readLong(table, startAddres);
}

uint32_t ModbusTCPSlaveServer::readUnsignedLong(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readUnsignedLong(table, startAddres);
}

bool ModbusTCPSlaveServer::readBool(uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return false;
    }
    return sl->readBool(table, startAddres);
}

uint8_t ModbusTCPSlaveServer::getLastError(uint8_t slaveAdr, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return 70;
    }
    return sl->getLastError();
}

void ModbusTCPSlaveServer::status(uint8_t slaveAdr, bool status, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAdr, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->status(status);
}

bool ModbusTCPSlaveServer::hasWriteRegisters()
{
    return !(firstWriteSlave() == 0);
}

ModbusSlaveInMaster *ModbusTCPSlaveServer::firstWriteSlave()
{
    for (int i = 0; i < slavesSize; i++)
    {
        if ((slaves[i].status()) && (slaves[i].hasWriteRegisters()))
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

// ModbusMasterTCP******************

ModbusMasterTCP::ModbusMasterTCP(FlprogAbstractEthernet *sourse, uint8_t size)
{
    serversSize = size;
    servs = new ModbusTCPSlaveServer[serversSize];
    tcpDevice = new FLProgTcpDevice(sourse);
}

ModbusMasterTCP::ModbusMasterTCP(FLProgAbstracttWiFiInterface *sourse, uint8_t size)
{
    serversSize = size;
    servs = new ModbusTCPSlaveServer[serversSize];
    tcpDevice = new FLProgTcpDevice(sourse);
}

ModbusTCPSlaveServer *ModbusMasterTCP::servers()
{
    if (servs == 0)
    {
        servs = new ModbusTCPSlaveServer[serversSize];
    }
    return servs;
}

ModbusTCPSlaveServer *ModbusMasterTCP::server(uint8_t serverIndex)
{
    if (hasServer(serverIndex))
    {
        return &servers()[serverIndex];
    }
    return 0;
}

void ModbusMasterTCP::setServerSlavesSize(uint8_t serverIndex, uint8_t size)
{
    if (hasServer(serverIndex))
    {
        server(serverIndex)->setSlavesSize(size);
    }
}

void ModbusMasterTCP::setServerPort(uint8_t serverIndex, int16_t port)
{
    if (hasServer(serverIndex))
    {
        server(serverIndex)->setPort(port);
    }
}

void ModbusMasterTCP::setServerIpAdress(uint8_t serverIndex, IPAddress ip)
{
    if (hasServer(serverIndex))
    {
        server(serverIndex)->setIpAdress(ip);
    }
}

void ModbusMasterTCP::setSlaveAddress(uint8_t serverIndex, uint8_t slaveIndex, uint16_t addr)
{
    if (hasServer(serverIndex))
    {
        server(serverIndex)->setSlaveAddress(slaveIndex, addr);
    }
}

void ModbusMasterTCP::setDataTable(uint8_t serverIndex, uint8_t slaveAdr, ModbusTable *table, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setDataTable(slaveAdr, table, isIndex);
}

void ModbusMasterTCP::setDataTable(uint8_t serverIndex, uint8_t slaveAdr, uint8_t _table, int16_t dataSize, int *_adresses, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setDataTable(slaveAdr, _table, dataSize, _adresses, isIndex);
}

void ModbusMasterTCP::configDataTable(uint8_t serverIndex, uint8_t slaveAdr, uint8_t _table, int16_t dataSize, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->configDataTable(slaveAdr, _table, dataSize, isIndex);
}

void ModbusMasterTCP::configDataTable(uint8_t serverIndex, uint8_t slaveAdr, uint8_t _table, int16_t dataSize, uint16_t _startAdr, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->configDataTable(slaveAdr, _table, dataSize, _startAdr, isIndex);
}

void ModbusMasterTCP::setPollingPeriod(uint8_t serverIndex, uint8_t slaveAdr, uint32_t period, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setPollingPeriod(slaveAdr, period, isIndex);
}

void ModbusMasterTCP::setTimeOutTime(uint8_t serverIndex, uint8_t slaveAdr, uint32_t time, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setTimeOutTime(slaveAdr, time, isIndex);
}

void ModbusMasterTCP::setLongOrder(uint8_t serverIndex, uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setLongOrder(slaveAdr, order, isIndex);
}

void ModbusMasterTCP::setFloatOrder(uint8_t serverIndex, uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setFloatOrder(slaveAdr, order, isIndex);
}

void ModbusMasterTCP::setUnsignedlongOrder(uint8_t serverIndex, uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setUnsignedlongOrder(slaveAdr, order, isIndex);
}

void ModbusMasterTCP::setIntOrder(uint8_t serverIndex, uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setIntOrder(slaveAdr, order, isIndex);
}

void ModbusMasterTCP::saveLong(uint8_t serverIndex, uint8_t slaveAdr, int32_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveLong(slaveAdr, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveUnsignedLong(uint8_t serverIndex, uint8_t slaveAdr, uint32_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveUnsignedLong(slaveAdr, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveFloat(uint8_t serverIndex, uint8_t slaveAdr, float value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveFloat(slaveAdr, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveInteger(uint8_t serverIndex, uint8_t slaveAdr, int value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveInteger(slaveAdr, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveByte(uint8_t serverIndex, uint8_t slaveAdr, uint8_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveByte(slaveAdr, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveBool(uint8_t serverIndex, uint8_t slaveAdr, bool value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveBool(slaveAdr, value, table, startAddres, isIndex);
}

uint8_t ModbusMasterTCP::readByte(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readByte(slaveAdr, table, startAddres, isIndex);
}

int ModbusMasterTCP::readInteger(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readInteger(slaveAdr, table, startAddres, isIndex);
}

float ModbusMasterTCP::readFloat(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readFloat(slaveAdr, table, startAddres, isIndex);
}

int32_t ModbusMasterTCP::readLong(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readLong(slaveAdr, table, startAddres, isIndex);
}

uint32_t ModbusMasterTCP::readUnsignedLong(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readUnsignedLong(slaveAdr, table, startAddres, isIndex);
}

bool ModbusMasterTCP::readBool(uint8_t serverIndex, uint8_t slaveAdr, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return false;
    }
    return serv->readBool(slaveAdr, table, startAddres, isIndex);
}

uint8_t ModbusMasterTCP::getLastError(uint8_t serverIndex, uint8_t slaveAdr, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 60;
    }
    return serv->getLastError(slaveAdr, isIndex);
}

void ModbusMasterTCP::status(uint8_t serverIndex, uint8_t slaveAdr, bool status, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    return serv->status(slaveAdr, status, isIndex);
}

void ModbusMasterTCP::connect(ModbusTCPSlaveServer *server)
{
    tcpDevice->connect(server->getIp(), server->getPort());
}

void ModbusMasterTCP::write(ModbusTCPSlaveServer *server, uint8_t *buffer, uint8_t buferSize)
{
    if (!tcpDevice->connected())
    {
        connect(server);
    }
    if (tcpDevice->connected())
    {
        tcpDevice->write(buffer, buferSize);
    }
}

bool ModbusMasterTCP::hasServer(uint8_t serverIndex)
{
    return serverIndex < serversSize;
}

void ModbusMasterTCP::pool()
{
    if (!isInit)
    {
        begin();
    }
    if (workStatus == FLPROG_MODBUS_WAITING_ANSWER)
    {
        checkAnswer();
    }
    if (workStatus == FLPROG_MODBUS_READY)
    {
        nextQuery();
    }
}

void ModbusMasterTCP::checkAnswer()
{

    if (flprog::isTimer(startSendTime, (telegrammSlave->getTimeOutTime())))
    {
        telegrammSlave->setLastError(244);
        workStatus = FLPROG_MODBUS_READY;
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
    uint8_t exception = validateRequest();
    if (exception > 0)
    {
        telegrammSlave->setLastError(exception);
        workStatus = FLPROG_MODBUS_READY;
        return;
    }
    telegrammSlave->setLastError(0);
    writeMaserData(telegrammTable, telegrammStartAddres, telegrammNumbeRegs);
}

void ModbusMasterTCP::getRxBuffer()
{
    uint8_t currentByte = 0;

    uint8_t currentByteIndex = 0;
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
            if (bufferSize < 64)
            {
                buffer[bufferSize] = currentByte;
                bufferSize++;
            }
        }
        currentByteIndex++;
    }
}

uint8_t ModbusMasterTCP::validateRequest()
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
    ModbusTCPSlaveServer *writeServer = firstWriteServer();
    if (writeServer != 0)
    {
        return createWriteTelegramm(writeServer);
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

    if (currentSlaveTable->tableType() == FLPROG_COIL)
    {
        telegrammFunction = 1;
    }
    if (currentSlaveTable->tableType() == FLPROG_DISCRETE_INPUT)
    {
        telegrammFunction = 2;
    }
    if (currentSlaveTable->tableType() == FLPROG_INPUT_REGISTR)
    {
        telegrammFunction = 4;
    }
    if (currentSlaveTable->tableType() == FLPROG_HOLDING_REGISTR)
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
        currentSlave->lastReqest(millis());
        currentSlave->setInit();
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
        if (servers()[i].isReady())
        {
            return &servers()[i];
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
            if (server(i)->isReady())
            {
                return server(i);
            }
        }
        else
        {
            if (oldServer == server(i))
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
        if (server(i)->isReady())
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
    buffer[0] = telegrammSlave->slaveAddres();
    buffer[1] = telegrammFunction;
    create_PDU(telegrammTable, telegrammStartAddres, telegrammNumbeRegs);
    startSendTime = millis();
    workStatus = FLPROG_MODBUS_WAITING_ANSWER;
}

bool ModbusMasterTCP::createWriteTelegramm(ModbusTCPSlaveServer *writeServer)
{
    telegrammServer = writeServer;
    telegrammSlave = telegrammServer->firstWriteSlave();
    telegrammTable = telegrammSlave->firstWriteTable();
    telegrammStartAddres = telegrammTable->firstWriteAddress();
    telegrammNumbeRegs = telegrammTable->writeRegsSize(telegrammStartAddres);
    if (telegrammTable->tableType() == FLPROG_HOLDING_REGISTR)
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
    for (uint8_t i = 0; i < telegrammNumbeRegs; i++)
    {
        telegrammTable->resetWriteFlag(telegrammStartAddres + i);
    }
    return true;
}

ModbusTCPSlaveServer *ModbusMasterTCP::firstWriteServer()
{
    for (int i = 0; i < serversSize; i++)
    {
        if (server(i)->hasWriteRegisters())
        {
            return server(i);
        }
    }
    return 0;
}

void ModbusMasterTCP::sendTxBuffer()
{
    uint8_t resultBuffer[70];
    uint8_t resultBufferSize = 0;
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

void ModbusMasterTCP::setSlavesToServer(uint8_t serverIndex, ModbusSlaveInMaster table[], int size)
{
    if (hasServer(serverIndex))
    {
        server(serverIndex)->setSlaves(table, size);
    }
}

void ModbusMasterTCP::begin()
{

    tcpDevice->beClient();
    tcpDevice->begin();
    isInit = true;
}

//---------------------ModbusMasterRTUoverTCP-------------------------------------

void ModbusMasterRTUoverTCP::checkAnswer()
{
    if (flprog::isTimer(startSendTime, (telegrammSlave->getTimeOutTime())))
    {
        telegrammSlave->setLastError(244);
        workStatus = FLPROG_MODBUS_READY;
        return;
    }
    getRxBuffer();
    if (bufferSize == 0)
    {
        return;
    }
    uint8_t exception = validateRequest();
    if (exception > 0)
    {
        telegrammSlave->setLastError(exception);
        workStatus = FLPROG_MODBUS_READY;
        return;
    }
    telegrammSlave->setLastError(0);
    writeMaserData(telegrammTable, telegrammStartAddres, telegrammNumbeRegs);
}

uint8_t ModbusMasterRTUoverTCP::validateRequest()
{
    int16_t msgCRC = ((buffer[bufferSize - 2] << 8) | buffer[bufferSize - 1]);
    if (flprogModus::modbusCalcCRC(bufferSize - 2, buffer) != msgCRC)
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

void ModbusMasterRTUoverTCP::getRxBuffer()
{

    bufferSize = 0;
    while (available())
    {
        if (bufferSize < 64)
        {
            buffer[bufferSize] = read();
            bufferSize++;
        }
        else
        {
            read();
        }
    }
}

void ModbusMasterRTUoverTCP::sendTxBuffer()
{

    int crc = flprogModus::modbusCalcCRC(bufferSize, buffer);
    buffer[bufferSize] = crc >> 8;
    bufferSize++;
    buffer[bufferSize] = crc & 0x00ff;
    bufferSize++;
    connect(telegrammServer);
    write(telegrammServer, buffer, bufferSize);
    bufferSize = 0;
}