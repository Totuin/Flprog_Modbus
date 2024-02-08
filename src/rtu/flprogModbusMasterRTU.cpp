#include "flprogModbusMasterRTU.h"

ModbusMasterRTU::ModbusMasterRTU(uint8_t portNumber, uint8_t size)
{
    slavesSize = size;
    slaves = new ModbusSlaveInMaster[slavesSize];
    setUart(portNumber);
}

void ModbusMasterRTU::begin()
{
    flprog::beginUart(_uartPortNumber);
    if (_pinPeDe >= 0)
    {
        pinMode(_pinPeDe, OUTPUT);
        digitalWrite(_pinPeDe, LOW);
    }
    _isInit = true;
}

ModbusSlaveInMaster *ModbusMasterRTU::slave(uint8_t adr)
{
    for (uint8_t i = 0; i < slavesSize; i++)
    {
        if (slaves[i].slaveAddress() == adr)
        {
            return &slaves[i];
        }
    }
    return 0;
}

ModbusSlaveInMaster *ModbusMasterRTU::slaveOnIndex(uint8_t slaveIndex)
{
    if (hasSlaveOnIndex(slaveIndex))
    {
        return &slaves[slaveIndex];
    }
    return 0;
}

void ModbusMasterRTU::pool()
{
    if (!_isInit)
    {
        begin();
        return;
    }

    if (_status == FLPROG_MODBUS_WAITING_SENDING)
    {
        if (flprog::isTimer(_startSendTime, _timeOfSend))
        {
            _status = FLPROG_MODBUS_WAITING_ANSWER;
            _startSendTime = millis();
            offPeDePin();
        }
        else
        {
            return;
        }
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

void ModbusMasterRTU::checkAnswer()
{
    if (flprog::isTimer(_startSendTime, (_telegrammSlave->getTimeOutTime())))
    {
        _telegrammSlave->setLastError(244);
        _status = FLPROG_MODBUS_READY;
        return;
    }
    if (!(checkAvaliblePacage()))
        return;
    uint8_t state = rxBuffer();
    if (state < 5)
    {
        _telegrammSlave->setLastError(255);
        _status = FLPROG_MODBUS_READY;
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

uint8_t ModbusMasterRTU::validateRequest()
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

void ModbusMasterRTU::nextQuery()
{
    if (!createNewTelegramm())
    {
        return;
    }
    sendQuery();
}

bool ModbusMasterRTU::hasWriteRegisters()
{
    return !(firstWriteSlave() == 0);
}

ModbusSlaveInMaster *ModbusMasterRTU::slaveForAddress(int16_t adress)
{
    for (int16_t i = 0; i < slavesSize; i++)
    {
        if (slaves[i].slaveAddress() == adress)
        {
            return &slaves[i];
        }
    }
    return 0;
}

ModbusSlaveInMaster *ModbusMasterRTU::firstWriteSlave()
{
    for (int16_t i = 0; i < slavesSize; i++)
    {
        if ((slaves[i].status()) && (slaves[i].hasWriteRegisters()))
        {
            return &slaves[i];
        }
    }
    return 0;
}

bool ModbusMasterRTU::createNewTelegramm()
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

bool ModbusMasterRTU::createReadTelegram()
{
    _telegrammSlave = _currentSlave;
    _telegrammTable = _currentSlaveTable;
    _telegrammStartAddres = _currentSlaveStartAddress;
    _telegrammNumbeRegs = _telegrammTable->readRegsSize(_telegrammStartAddres);
    _currentSlaveLastAddress = _telegrammStartAddres + _telegrammNumbeRegs - 1;
    if (_currentSlaveTable->tableType() == FLPROG_COIL)
    {
        _telegrammFunction = 1;
    }
    if (_telegrammTable->tableType() == FLPROG_DISCRETE_INPUT)
    {
        _telegrammFunction = 2;
    }
    if (_telegrammTable->tableType() == FLPROG_INPUT_REGISTR)
    {
        _telegrammFunction = 4;
    }
    if (_telegrammTable->tableType() == FLPROG_HOLDING_REGISTR)
    {
        _telegrammFunction = 3;
    }
    return true;
}

bool ModbusMasterRTU::createWriteTelegramm()
{
    _telegrammSlave = firstWriteSlave();
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

void ModbusMasterRTU::sendQuery()
{
    _buffer[0] = _telegrammSlave->slaveAddress();
    _buffer[1] = _telegrammFunction;
    create_PDU(_telegrammTable, _telegrammStartAddres, _telegrammNumbeRegs);
}

bool ModbusMasterRTU::nextStep()
{
    if (!hasReadySlave())
    {
        return false;
    }
    if (_currentSlave == 0)
    {
        _currentSlave = firstReadySlave();
        _currentSlaveTable = _currentSlave->firstTabe();
        _currentSlaveStartAddress = _currentSlaveTable->getMinAdress();
        return true;
    }
    return nextRegistor();
}

ModbusSlaveInMaster *ModbusMasterRTU::firstReadySlave()
{

    for (int16_t i = 0; i < slavesSize; i++)
    {
        if (slaves[i].isReady())
        {
            return &slaves[i];
        }
    }
    return 0;
}

bool ModbusMasterRTU::nextRegistor()
{
    _currentSlaveStartAddress = _currentSlaveTable->findNextStartAddres(_currentSlaveLastAddress);
    if (_currentSlaveStartAddress == -1)
    {
        return nextTable();
    }
    return true;
}

bool ModbusMasterRTU::nextSlave()
{
    if (_currentSlave != 0)
    {
        _currentSlave->lastReqest(millis());
        _currentSlave->setInit();
    }
    _currentSlave = nextSlave(_currentSlave);
    if (_currentSlave == 0)
    {
        return false;
    }
    _currentSlaveTable = _currentSlave->firstTabe();
    _currentSlaveStartAddress = _currentSlaveTable->getMinAdress();
    return true;
}

ModbusSlaveInMaster *ModbusMasterRTU::nextSlave(ModbusSlaveInMaster *currentTempSlave)
{
    bool isFindSlave = false;
    for (int16_t i = 0; i < slavesSize; i++)
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
            if (currentTempSlave == &slaves[i])
            {
                isFindSlave = true;
            }
        }
    }
    return 0;
}

bool ModbusMasterRTU::nextTable()
{
    _currentSlaveTable = _currentSlave->nextTable(_currentSlaveTable);
    if (_currentSlaveTable == 0)
    {
        return nextSlave();
    }

    _currentSlaveStartAddress = _currentSlaveTable->getMinAdress();
    return true;
}

bool ModbusMasterRTU::hasReadySlave()
{
    for (int16_t i = 0; i < slavesSize; i++)
    {
        if (slaves[i].isReady())
        {
            return true;
        }
    }
    return false;
}

bool ModbusMasterRTU::hasSlaveOnIndex(uint8_t slaveIndex)
{
    return slaveIndex < slavesSize;
}

void ModbusMasterRTU::saveLong(uint8_t slaveAddres, int32_t value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }

    sl->saveLong(value, table, startAddres);
}

void ModbusMasterRTU::saveUnsignedLong(uint8_t slaveAddres, uint32_t value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }

    sl->saveUnsignedLong(value, table, startAddres);
}

void ModbusMasterRTU::saveFloat(uint8_t slaveAddres, float value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }

    sl->saveFloat(value, table, startAddres);
}

void ModbusMasterRTU::saveInteger(uint8_t slaveAddres, int16_t value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }

    sl->saveInteger(value, table, startAddres);
}

void ModbusMasterRTU::saveByte(uint8_t slaveAddres, uint8_t value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }
    sl->saveByte(value, table, startAddres);
}

void ModbusMasterRTU::saveBool(uint8_t slaveAddres, bool val, uint8_t table, int16_t adr, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }
    sl->saveBool(val, table, adr);
}

uint8_t ModbusMasterRTU::readByte(uint8_t slaveAddres, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readByte(table, startAddres);
}

int16_t ModbusMasterRTU::readInteger(uint8_t slaveAddres, uint8_t table, int16_t startAddres, bool isIndex)
{

    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readInteger(table, startAddres);
}

float ModbusMasterRTU::readFloat(uint8_t slaveAddres, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readFloat(table, startAddres);
}

int32_t ModbusMasterRTU::readLong(uint8_t slaveAddres, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readLong(table, startAddres);
}

uint32_t ModbusMasterRTU::readUnsignedLong(uint8_t slaveAddres, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readUnsignedLong(table, startAddres);
}

bool ModbusMasterRTU::readBool(uint8_t slaveAddres, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readBool(table, startAddres);
}

void ModbusMasterRTU::setDataTable(uint8_t slaveAddres, ModbusTable *table)
{
    ModbusSlaveInMaster *sl;
    sl = slave(slaveAddres);
    if (sl == 0)
    {
        return;
    }
    sl->setDataTable(table);
}

void ModbusMasterRTU::setDataTable(uint8_t slaveAddres, uint8_t _table, int16_t dataSize, int *_adresses)
{
    ModbusSlaveInMaster *sl;
    sl = slave(slaveAddres);
    if (sl == 0)
    {
        return;
    }
    sl->setDataTable(_table, dataSize, _adresses);
}

void ModbusMasterRTU::configDataTable(uint8_t slaveAddres, uint8_t _table, int16_t dataSize)
{
    ModbusSlaveInMaster *sl;
    sl = slave(slaveAddres);
    if (sl == 0)
    {
        return;
    }
    sl->configDataTable(_table, dataSize);
}

void ModbusMasterRTU::configDataTable(uint8_t slaveAddres, uint8_t _table, int16_t dataSize, int16_t _startAdr)
{
    ModbusSlaveInMaster *sl;
    sl = slave(slaveAddres);
    if (sl == 0)
    {
        return;
    }
    sl->configDataTable(_table, dataSize, _startAdr);
}

void ModbusMasterRTU::setSlaveAddress(uint8_t slaveIndex, uint8_t address)
{
    if (hasSlaveOnIndex(slaveIndex))
    {
        slaves[slaveIndex].setSlaveAddress(address);
    }
}

uint8_t ModbusMasterRTU::getSlaveAddress(uint8_t slaveIndex)
{
    if (hasSlaveOnIndex(slaveIndex))
    {
        return slaves[slaveIndex].getSlaveAddress();
    }
    return 0;
}

void ModbusMasterRTU::setPollingPeriod(uint8_t slaveAddres, uint32_t period, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setPollingPeriod(period);
}

uint32_t ModbusMasterRTU::pollingPeriod(uint8_t slaveAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->pollingPeriod();
}

uint8_t ModbusMasterRTU::intOrder(uint8_t slaveAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->intOrder();
}

uint8_t ModbusMasterRTU::longOrder(uint8_t slaveAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->longOrder();
}

uint8_t ModbusMasterRTU::floatOrder(uint8_t slaveAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->floatOrder();
}

uint8_t ModbusMasterRTU::unsignedlongOrder(uint8_t slaveAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->unsignedlongOrder();
}

bool ModbusMasterRTU::slaveStatus(uint8_t slaveAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->slaveStatus();
}

void ModbusMasterRTU::setTimeOutTime(uint8_t slaveAddres, uint32_t time, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setTimeOutTime(time);
}

void ModbusMasterRTU::setLongOrder(uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setLongOrder(order);
}

void ModbusMasterRTU::setFloatOrder(uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setFloatOrder(order);
}

void ModbusMasterRTU::setUnsignedlongOrder(uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setUnsignedlongOrder(order);
}

void ModbusMasterRTU::setIntOrder(uint8_t slaveAddres, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setIntOrder(order);
}

uint8_t ModbusMasterRTU::getLastError(uint8_t slaveAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return 50;
    }
    return sl->getLastError();
}

void ModbusMasterRTU::status(uint8_t slaveAddres, bool status, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAddres);
    }
    else
    {
        sl = slave(slaveAddres);
    }
    if (sl == 0)
    {
        return;
    }
    sl->status(status);
}
