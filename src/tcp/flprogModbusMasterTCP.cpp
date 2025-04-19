#include "flprogModbusMasterTCP.h"

ModbusMasterTCP::ModbusMasterTCP(FLProgAbstractTcpInterface *sourse, uint8_t size)
{
    _serversSize = size;
    _servs = new ModbusTCPSlaveServer[_serversSize];
    _interface = sourse;
    _tcpClient.setSourse(sourse);
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

uint8_t ModbusMasterTCP::serverMode(uint8_t serverIndex)
{
    if (hasServer(serverIndex))
    {
        return server(serverIndex)->mode();
    }
    // return 255; ander
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
        if ((server(serverIndex)->getPort()) != port)
        {
            _tcpClient.stop();
            server(serverIndex)->setPort(port);
        }
    }
}

int16_t ModbusMasterTCP::serverPort(uint8_t serverIndex)
{
    if (hasServer(serverIndex))
    {
        return server(serverIndex)->port();
    }
    return 0;
}

void ModbusMasterTCP::setServerMode(uint8_t serverIndex, uint8_t mode)
{

    if (hasServer(serverIndex))
    {
        server(serverIndex)->setMode(mode);
    }
}

void ModbusMasterTCP::setServerIpAdress(uint8_t serverIndex, IPAddress ip)
{
    if (hasServer(serverIndex))
    {
        if ((server(serverIndex)->getIp()) != ip)
        {
            _tcpClient.stop();
            server(serverIndex)->setIpAdress(ip);
        }
    }
}

IPAddress ModbusMasterTCP::serverIpAdress(uint8_t serverIndex)
{
    if (hasServer(serverIndex))
    {
        return server(serverIndex)->ipAdress();
    }
    return FLPROG_INADDR_NONE;
}

void ModbusMasterTCP::setServerHost(uint8_t serverIndex, String host)
{
    if (hasServer(serverIndex))
    {
        server(serverIndex)->setHost(host);
    }
}

void ModbusMasterTCP::setSlaveAddress(uint8_t serverIndex, uint8_t slaveIndex, uint16_t addr)
{
    if (hasServer(serverIndex))
    {
        server(serverIndex)->setSlaveAddress(slaveIndex, addr);
    }
}

uint8_t ModbusMasterTCP::getSlaveAddress(uint8_t serverIndex, uint8_t slaveIndex)
{
    if (hasServer(serverIndex))
    {
        return server(serverIndex)->getSlaveAddress(slaveIndex);
    }
    return 0;
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

uint32_t ModbusMasterTCP::pollingPeriod(uint8_t serverIndex, uint8_t slaveAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->pollingPeriod(slaveAddres, isIndex);
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

void ModbusMasterTCP::setCallBack(uint8_t serverIndex, uint8_t slaveAdr, FLProgModbusNewDataCallback func, bool isIndex)
{

    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return;
    }
    serv->setCallBack(slaveAdr, func, isIndex);
}

uint8_t ModbusMasterTCP::intOrder(uint8_t serverIndex, uint8_t slaveAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->intOrder(slaveAddres, isIndex);
}

uint8_t ModbusMasterTCP::longOrder(uint8_t serverIndex, uint8_t slaveAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->longOrder(slaveAddres, isIndex);
}

uint8_t ModbusMasterTCP::floatOrder(uint8_t serverIndex, uint8_t slaveAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->floatOrder(slaveAddres, isIndex);
}

uint8_t ModbusMasterTCP::unsignedlongOrder(uint8_t serverIndex, uint8_t slaveAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return 0;
    }
    return serv->unsignedlongOrder(slaveAddres, isIndex);
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
    serv->status(slaveAddres, status, isIndex);
}

bool ModbusMasterTCP::slaveStatus(uint8_t serverIndex, uint8_t slaveAddres, bool isIndex)
{
    ModbusTCPSlaveServer *serv = server(serverIndex);
    if (serv == 0)
    {
        return false;
    }
    return serv->slaveStatus(slaveAddres, isIndex);
}

void ModbusMasterTCP::connect(ModbusTCPSlaveServer *server)
{

    if (_tempCurrentServer != server)
    {
        _tcpClient.stop();
        _tempCurrentServer = server;
    }

    if (_tempCurrentServer == 0)
    {
        _status = FLPROG_MODBUS_READY;
        return;
    }

    if (!_tcpClient.connected())
    {
        uint8_t result;
        if (_tempCurrentServer->serverAsHost())
        {
            result = _tcpClient.connect(_tempCurrentServer->getHost(), _tempCurrentServer->getPort());
        }
        else
        {
            result = _tcpClient.connect(_tempCurrentServer->getIp(), _tempCurrentServer->getPort());
        }
        if (result == FLPROG_WAIT)
        {
            _status = FLPROG_MODBUS_WAITING_CONNECT_CLIENT;
            return;
        }
        if (result == FLPROG_ERROR)
        {
            _telegrammSlave->setLastError(244);
            _tcpClient.stop();
            _status = FLPROG_MODBUS_READY;
            return;
        }
        return;
    }
    if (_telegrammServer->mode() == FLPROG_TCP_MODBUS)
    {
        _tcpClient.write(_mbapBuffer, 6);
    }
    _tcpClient.write(_buffer, _bufferSize);
    _status = FLPROG_MODBUS_WAITING_ANSWER;
    _bufferSize = 0;
}

bool ModbusMasterTCP::hasServer(uint8_t serverIndex)
{
    return serverIndex < _serversSize;
}

void ModbusMasterTCP::pool()
{
    if (_interface == 0)
    {
        return;
    }
    if (_eventsCount < _skippingEvents)
    {
        _eventsCount++;
        return;
    }
    _eventsCount = 0;

    if (!_interface->isReady())
    {
        return;
    }

    if (!_isInit)
    {
        begin();
    }
    if (_status == FLPROG_MODBUS_WAITING_CONNECT_CLIENT)
    {
        connect(_telegrammServer);
        return;
    }
    if (_status == FLPROG_MODBUS_WAITING_ANSWER)
    {
        checkAnswer();
        return;
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
        _tcpClient.stop();
        _telegrammSlave->setLastError(244);
        _status = FLPROG_MODBUS_READY;
        return;
    }
    getRxBuffer();
    if (_bufferSize == 0)
    {
        return;
    }
    if (_telegrammServer->mode() == FLPROG_TCP_MODBUS)
    {
        if (_mbapBuffer[0] != highByte(_telegrammAnswerId))
        {
            return;
        }
        if (_mbapBuffer[1] != lowByte(_telegrammAnswerId))
        {
            return;
        }
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
    _tcpClient.stop();
}

void ModbusMasterTCP::getRxBuffer()
{
    uint8_t currentByte = 0;
    uint8_t currentByteIndex = 0;
    _bufferSize = 0;
    uint8_t mode = _telegrammServer->mode();
    while (_tcpClient.available())
    {
        currentByte = _tcpClient.read();
        if ((mode == FLPROG_TCP_MODBUS) && (currentByteIndex < 6))
        {
            _mbapBuffer[currentByteIndex] = currentByte;
        }
        else
        {
            if (_bufferSize < 200)
            {
                _buffer[_bufferSize] = currentByte;
                _bufferSize++;
            }
            else
            {
                _tcpClient.read();
            }
        }
        currentByteIndex++;
    }
}

uint8_t ModbusMasterTCP::validateRequest()
{
    /*
        if (_telegrammServer->mode() == FLPROG_RTU_OVER_TCP_MODBUS)
        {
            int16_t msgCRC = ((_buffer[_bufferSize - 2] << 8) | _buffer[_bufferSize - 1]);
            if (flprogModus::modbusCalcCRC(_bufferSize - 2, _buffer) != msgCRC)
            {

                return 255;

            }
        }
    */
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
    _buffer[0] = _telegrammSlave->slaveAddress();
    _buffer[1] = _telegrammFunction;
    create_PDU(_telegrammTable, _telegrammStartAddres, _telegrammNumbeRegs);
    _startSendTime = millis();
    //_status = FLPROG_MODBUS_WAITING_ANSWER;
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
    if (_telegrammServer->mode() == FLPROG_TCP_MODBUS)
    {
        _mbapBuffer[4] = highByte(_bufferSize);
        _mbapBuffer[5] = lowByte(_bufferSize);
    }
    else
    {
        int crc = flprogModus::modbusCalcCRC(_bufferSize, _buffer);
        _buffer[_bufferSize] = crc >> 8;
        _bufferSize++;
        _buffer[_bufferSize] = crc & 0x00ff;
        _bufferSize++;
    }
    connect(_telegrammServer);
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
    _tcpClient.stop();
    _isInit = true;
}
