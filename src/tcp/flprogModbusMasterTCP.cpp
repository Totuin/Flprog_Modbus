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

void ModbusTCPSlaveServer::setDataTable(uint8_t slaveAddres, ModbusTable *table)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, false);
    if (sl == 0)
    {
        return;
    }
    sl->setDataTable(table);
}

void ModbusTCPSlaveServer::setDataTable(uint8_t slaveAddres, uint8_t _table, int16_t dataSize, int *_adresses)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, false);
    if (sl == 0)
    {
        return;
    }
    sl->setDataTable(_table, dataSize, _adresses);
}

void ModbusTCPSlaveServer::configDataTable(uint8_t slaveAddres, uint8_t _table, int16_t dataSize)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, false);
    if (sl == 0)
    {
        return;
    }
    sl->configDataTable(_table, dataSize);
}

void ModbusTCPSlaveServer::configDataTable(uint8_t slaveAddres, uint8_t _table, int16_t dataSize, uint16_t _startAdr)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, false);
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

void ModbusTCPSlaveServer::setPollingPeriod(uint8_t slaveAddres, uint32_t period, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setPollingPeriod(period);
}

void ModbusTCPSlaveServer::setTimeOutTime(uint8_t slaveAddres, uint32_t time, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setTimeOutTime(time);
}

void ModbusTCPSlaveServer::setLongOrder(uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setLongOrder(order);
}

void ModbusTCPSlaveServer::setFloatOrder(uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setFloatOrder(order);
}

void ModbusTCPSlaveServer::setUnsignedlongOrder(uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setUnsignedlongOrder(order);
}

void ModbusTCPSlaveServer::setIntOrder(uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->setIntOrder(order);
}

void ModbusTCPSlaveServer::saveLong(uint8_t slaveAddres, int32_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveLong(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveUnsignedLong(uint8_t slaveAddres, uint32_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveUnsignedLong(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveFloat(uint8_t slaveAddres, float value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveFloat(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveInteger(uint8_t slaveAddres, int value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveInteger(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveByte(uint8_t slaveAddres, uint8_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveByte(value, table, startAddres);
}

void ModbusTCPSlaveServer::saveBool(uint8_t slaveAddres, bool value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return;
    }
    sl->saveBool(value, table, startAddres);
}

uint8_t ModbusTCPSlaveServer::readByte(uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readByte(table, startAddres);
}

int ModbusTCPSlaveServer::readInteger(uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readInteger(table, startAddres);
}

float ModbusTCPSlaveServer::readFloat(uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readFloat(table, startAddres);
}

int32_t ModbusTCPSlaveServer::readLong(uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readLong(table, startAddres);
}

uint32_t ModbusTCPSlaveServer::readUnsignedLong(uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return 0;
    }
    return sl->readUnsignedLong(table, startAddres);
}

bool ModbusTCPSlaveServer::readBool(uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return false;
    }
    return sl->readBool(table, startAddres);
}

uint8_t ModbusTCPSlaveServer::getLastError(uint8_t slaveAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
    if (sl == 0)
    {
        return 70;
    }
    return sl->getLastError();
}

void ModbusTCPSlaveServer::status(uint8_t slaveAddres, bool status, bool isIndex)
{
    ModbusSlaveInMaster *sl = slave(slaveAddres, isIndex);
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

ModbusMasterTCP::ModbusMasterTCP(FLProgAbstractTcpInterface *sourse, uint8_t size)
{
    _serversSize = size;
    _servs = new ModbusTCPSlaveServer[_serversSize];
    _interface = sourse;
}

ModbusTCPSlaveServer *ModbusMasterTCP::servers()
{
    if (_servs == 0)
    {
        _servs = new ModbusTCPSlaveServer[_serversSize];
    }
    return _servs;
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

void ModbusMasterTCP::setDataTable(uint8_t serverIndex, uint8_t slaveAddres, ModbusTable *table)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setDataTable(slaveAddres, table);
}

void ModbusMasterTCP::setDataTable(uint8_t serverIndex, uint8_t slaveAddres, uint8_t _table, int16_t dataSize, int *_adresses)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setDataTable(slaveAddres, _table, dataSize, _adresses);
}

void ModbusMasterTCP::configDataTable(uint8_t serverIndex, uint8_t slaveAddres, uint8_t _table, int16_t dataSize)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->configDataTable(slaveAddres, _table, dataSize);
}

void ModbusMasterTCP::configDataTable(uint8_t serverIndex, uint8_t slaveAddres, uint8_t _table, int16_t dataSize, uint16_t _startAdr)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->configDataTable(slaveAddres, _table, dataSize, _startAdr);
}

void ModbusMasterTCP::setPollingPeriod(uint8_t serverIndex, uint8_t slaveAddres, uint32_t period, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setPollingPeriod(slaveAddres, period, isIndex);
}

void ModbusMasterTCP::setTimeOutTime(uint8_t serverIndex, uint8_t slaveAddres, uint32_t time, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setTimeOutTime(slaveAddres, time, isIndex);
}

void ModbusMasterTCP::setLongOrder(uint8_t serverIndex, uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setLongOrder(slaveAddres, order, isIndex);
}

void ModbusMasterTCP::setFloatOrder(uint8_t serverIndex, uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setFloatOrder(slaveAddres, order, isIndex);
}

void ModbusMasterTCP::setUnsignedlongOrder(uint8_t serverIndex, uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setUnsignedlongOrder(slaveAddres, order, isIndex);
}

void ModbusMasterTCP::setIntOrder(uint8_t serverIndex, uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setIntOrder(slaveAddres, order, isIndex);
}

void ModbusMasterTCP::saveLong(uint8_t serverIndex, uint8_t slaveAddres, int32_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveLong(slaveAddres, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveUnsignedLong(uint8_t serverIndex, uint8_t slaveAddres, uint32_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveUnsignedLong(slaveAddres, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveFloat(uint8_t serverIndex, uint8_t slaveAddres, float value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveFloat(slaveAddres, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveInteger(uint8_t serverIndex, uint8_t slaveAddres, int value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveInteger(slaveAddres, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveByte(uint8_t serverIndex, uint8_t slaveAddres, uint8_t value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveByte(slaveAddres, value, table, startAddres, isIndex);
}

void ModbusMasterTCP::saveBool(uint8_t serverIndex, uint8_t slaveAddres, bool value, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->saveBool(slaveAddres, value, table, startAddres, isIndex);
}

uint8_t ModbusMasterTCP::readByte(uint8_t serverIndex, uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readByte(slaveAddres, table, startAddres, isIndex);
}

int ModbusMasterTCP::readInteger(uint8_t serverIndex, uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readInteger(slaveAddres, table, startAddres, isIndex);
}

float ModbusMasterTCP::readFloat(uint8_t serverIndex, uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readFloat(slaveAddres, table, startAddres, isIndex);
}

int32_t ModbusMasterTCP::readLong(uint8_t serverIndex, uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readLong(slaveAddres, table, startAddres, isIndex);
}

uint32_t ModbusMasterTCP::readUnsignedLong(uint8_t serverIndex, uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->readUnsignedLong(slaveAddres, table, startAddres, isIndex);
}

bool ModbusMasterTCP::readBool(uint8_t serverIndex, uint8_t slaveAddres, uint8_t table, int startAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return false;
    }
    return serv->readBool(slaveAddres, table, startAddres, isIndex);
}

uint8_t ModbusMasterTCP::getLastError(uint8_t serverIndex, uint8_t slaveAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 60;
    }
    return serv->getLastError(slaveAddres, isIndex);
}

void ModbusMasterTCP::status(uint8_t serverIndex, uint8_t slaveAddres, bool status, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    return serv->status(slaveAddres, status, isIndex);
}

Client *ModbusMasterTCP::client()
{
    if (_tcpClient == 0)
    {
        _tcpClient = _interface->getClient();
    }
    return _tcpClient;
}
void ModbusMasterTCP::connect(ModbusTCPSlaveServer *server)
{
    client()->connect(server->getIp(), server->getPort());
}

void ModbusMasterTCP::write(ModbusTCPSlaveServer *server, uint8_t *buffer, uint8_t buferSize)
{
    if (!client()->connected())
    {
        connect(server);
    }
    if (client()->connected())
    {
        client()->write(_buffer, buferSize);
    }
}

bool ModbusMasterTCP::hasServer(uint8_t serverIndex)
{
    return serverIndex < _serversSize;
}

void ModbusMasterTCP::pool()
{
    if (!_isInit)
    {
        begin();
    }
    if (_status == FLPROG_MODBUS_WAITING_ANSWER)
    {
        checkAnswer();
    }
    if (_status == FLPROG_MODBUS_READY)
    {
        nextQuery();
    }
}

void ModbusMasterTCP::checkAnswer()
{

    if (flprog::isTimer(_startSendTime, (_telegrammSlave->getTimeOutTime())))
    {
        _telegrammSlave->setLastError(244);
        _status = FLPROG_MODBUS_READY;
        return;
    }
    getRxBuffer();
    if (_bufferSize == 0)
    {
        return;
    }
    if (_mbapBuffer[0] != highByte(_telegrammAnswerId))
    {
        return;
    }
    if (_mbapBuffer[1] != lowByte(_telegrammAnswerId))
    {
        return;
    }
    uint8_t exception = validateRequest();
    if (exception > 0)
    {
        _telegrammSlave->setLastError(exception);
        _status = FLPROG_MODBUS_READY;
        return;
    }
    _telegrammSlave->setLastError(0);
    writeMaserData(_telegrammTable, _telegrammStartAddres, _telegrammNumbeRegs);
}

void ModbusMasterTCP::getRxBuffer()
{
    uint8_t currentByte = 0;

    uint8_t currentByteIndex = 0;
    _bufferSize = 0;
    while (available())
    {
        currentByte = read();
        if (currentByteIndex < 6)

        {
            _mbapBuffer[currentByteIndex] = currentByte;
        }
        else
        {
            if (_bufferSize < 64)
            {
                _buffer[_bufferSize] = currentByte;
                _bufferSize++;
            }
        }
        currentByteIndex++;
    }
}

uint8_t ModbusMasterTCP::validateRequest()
{
    if ((_buffer[1] & 0x80) != 0)
    {
        return _buffer[2];
    }
    if (!(_telegrammSlave->isSupportFunction(_buffer[1])))
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
    _telegrammServer = _currentServer;
    _telegrammSlave = _currentSlave;
    _telegrammTable = _currentSlaveTable;
    _telegrammStartAddres = _currentSlaveStartAddress;
    _telegrammNumbeRegs = _currentSlaveTable->readRegsSize(_telegrammStartAddres);
    _currentSlaveLastAddress = _telegrammStartAddres + _telegrammNumbeRegs - 1;

    if (_currentSlaveTable->tableType() == FLPROG_COIL)
    {
        _telegrammFunction = 1;
    }
    if (_currentSlaveTable->tableType() == FLPROG_DISCRETE_INPUT)
    {
        _telegrammFunction = 2;
    }
    if (_currentSlaveTable->tableType() == FLPROG_INPUT_REGISTR)
    {
        _telegrammFunction = 4;
    }
    if (_currentSlaveTable->tableType() == FLPROG_HOLDING_REGISTR)
    {
        _telegrammFunction = 3;
    }
    return true;
}

bool ModbusMasterTCP::nextStep()
{
    if (!hasReadyServer())
    {
        return false;
    }
    if (_currentServer == 0)
    {
        _currentServer = firstReadyServer();
        _currentSlave = _currentServer->firstReadySlave();
        _currentSlaveTable = _currentSlave->firstTabe();
        _currentSlaveStartAddress = _currentSlaveTable->getMinAdress();
        return true;
    }
    return nextRegistor();
}

bool ModbusMasterTCP::nextRegistor()
{
    _currentSlaveStartAddress = _currentSlaveTable->findNextStartAddres(_currentSlaveLastAddress);
    if (_currentSlaveStartAddress == -1)
    {
        return nextTable();
    }
    return true;
}

bool ModbusMasterTCP::nextTable()
{
    _currentSlaveTable = _currentSlave->nextTable(_currentSlaveTable);
    if (_currentSlaveTable == 0)
    {
        return nextSlave();
    }

    _currentSlaveStartAddress = _currentSlaveTable->getMinAdress();
    return true;
}

bool ModbusMasterTCP::nextSlave()

{
    if (_currentSlave != 0)
    {
        _currentSlave->lastReqest(millis());
        _currentSlave->setInit();
    }
    _currentSlave = _currentServer->nextSlave(_currentSlave);
    if (_currentSlave == 0)
    {
        return nextServer();
    }
    _currentSlaveTable = _currentSlave->firstTabe();
    _currentSlaveStartAddress = _currentSlaveTable->getMinAdress();
    return true;
}

bool ModbusMasterTCP::nextServer()
{
    stop();
    _currentServer = nextReadyServer(_currentServer);

    if (_currentServer == 0)
    {
        return false;
    }
    _currentSlave = _currentServer->firstReadySlave();
    _currentSlaveTable = _currentSlave->firstTabe();
    _currentSlaveStartAddress = _currentSlaveTable->getMinAdress();
    return true;
}

ModbusTCPSlaveServer *ModbusMasterTCP::firstReadyServer()
{
    for (int i = 0; i < _serversSize; i++)
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
    for (int i = 0; i < _serversSize; i++)
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
    for (int i = 0; i < _serversSize; i++)
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
    _telegrammAnswerId++;
    _mbapBuffer[0] = highByte(_telegrammAnswerId);
    _mbapBuffer[1] = lowByte(_telegrammAnswerId);
    _mbapBuffer[2] = 0;
    _mbapBuffer[3] = 0;
    _buffer[0] = _telegrammSlave->slaveAddres();
    _buffer[1] = _telegrammFunction;
    create_PDU(_telegrammTable, _telegrammStartAddres, _telegrammNumbeRegs);
    _startSendTime = millis();
    _status = FLPROG_MODBUS_WAITING_ANSWER;
}

bool ModbusMasterTCP::createWriteTelegramm(ModbusTCPSlaveServer *writeServer)
{
    _telegrammServer = writeServer;
    _telegrammSlave = _telegrammServer->firstWriteSlave();
    _telegrammTable = _telegrammSlave->firstWriteTable();
    _telegrammStartAddres = _telegrammTable->firstWriteAddress();
    _telegrammNumbeRegs = _telegrammTable->writeRegsSize(_telegrammStartAddres);
    if (_telegrammTable->tableType() == FLPROG_HOLDING_REGISTR)
    {
        if (_telegrammNumbeRegs == 1)
        {
            _telegrammFunction = 6;
        }
        else
        {
            _telegrammFunction = 16;
        }
    }
    else
    {
        if (_telegrammNumbeRegs == 1)
        {
            _telegrammFunction = 5;
        }
        else
        {
            _telegrammFunction = 15;
        }
    }
    for (uint8_t i = 0; i < _telegrammNumbeRegs; i++)
    {
        _telegrammTable->resetWriteFlag(_telegrammStartAddres + i);
    }
    return true;
}

ModbusTCPSlaveServer *ModbusMasterTCP::firstWriteServer()
{
    for (int i = 0; i < _serversSize; i++)
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
    _mbapBuffer[4] = highByte(_bufferSize);
    _mbapBuffer[5] = lowByte(_bufferSize);
    for (int i = 0; i < 6; i++)
    {
        resultBuffer[resultBufferSize] = _mbapBuffer[i];
        resultBufferSize++;
    }
    for (int i = 0; i < _bufferSize; i++)
    {
        resultBuffer[resultBufferSize] = _buffer[i];
        resultBufferSize++;
    }
    connect(_telegrammServer);
    write(_telegrammServer, resultBuffer, resultBufferSize);
    _bufferSize = 0;
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
    _isInit = true;
}

//---------------------ModbusMasterRTUoverTCP-------------------------------------

void ModbusMasterRTUoverTCP::checkAnswer()
{
    if (flprog::isTimer(_startSendTime, (_telegrammSlave->getTimeOutTime())))
    {
        _telegrammSlave->setLastError(244);
        _status = FLPROG_MODBUS_READY;
        return;
    }
    getRxBuffer();
    if (_bufferSize == 0)
    {
        return;
    }
    uint8_t exception = validateRequest();
    if (exception > 0)
    {
        _telegrammSlave->setLastError(exception);
        _status = FLPROG_MODBUS_READY;
        return;
    }
    _telegrammSlave->setLastError(0);
    writeMaserData(_telegrammTable, _telegrammStartAddres, _telegrammNumbeRegs);
}

uint8_t ModbusMasterRTUoverTCP::validateRequest()
{
    int16_t msgCRC = ((_buffer[_bufferSize - 2] << 8) | _buffer[_bufferSize - 1]);
    if (flprogModus::modbusCalcCRC(_bufferSize - 2, _buffer) != msgCRC)
    {
        return 255;
    }
    if ((_buffer[1] & 0x80) != 0)
    {
        return _buffer[2];
    }
    if (!(_telegrammSlave->isSupportFunction(_buffer[1])))
    {
        return 1;
    }
    return 0;
}

void ModbusMasterRTUoverTCP::getRxBuffer()
{

    _bufferSize = 0;
    while (available())
    {
        if (_bufferSize < 64)
        {
            _buffer[_bufferSize] = read();
            _bufferSize++;
        }
        else
        {
            read();
        }
    }
}

void ModbusMasterRTUoverTCP::sendTxBuffer()
{

    int crc = flprogModus::modbusCalcCRC(_bufferSize, _buffer);
    _buffer[_bufferSize] = crc >> 8;
    _bufferSize++;
    _buffer[_bufferSize] = crc & 0x00ff;
    _bufferSize++;
    connect(_telegrammServer);
    write(_telegrammServer, _buffer, _bufferSize);
    _bufferSize = 0;
}