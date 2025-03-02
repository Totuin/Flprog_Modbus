#include "flprogModbusData.h"

//------------------- ModbusTable--------------------------------------

ModbusTable::ModbusTable(uint8_t table, uint16_t dataSize, uint16_t startAddres)
{
    if (_table > 4)
    {
        return;
    }
    _table = table;
    _startAddres = startAddres;
    init(dataSize);
}

ModbusTable::ModbusTable(uint8_t table, uint16_t dataSize, int *adresses)
{
    if (_table > 4)
    {
        return;
    }
    _table = table;
    init(dataSize);
    _adresses = adresses;
}

void ModbusTable::init(int16_t dataSize, int *adresses)
{
    if (_adresses != 0)
    {
        _adresses = 0;
    }
    _adresses = adresses;
    _startAddres = -1;
    init(dataSize);
}

void ModbusTable::init(int16_t dataSize, uint16_t startAddres)
{
    if (_adresses != 0)
    {
        _adresses = 0;
    }
    _startAddres = startAddres;
    init(dataSize);
}

void ModbusTable::init(int16_t dataSize)
{
    _tableSize = dataSize;
    if ((_table == FLPROG_INPUT_REGISTR) || (_table == FLPROG_HOLDING_REGISTR))
    {
        _worldData = new int16_t[_tableSize];
    }
    else
    {
        _boolData = new bool[_tableSize];
    }
    _sendRegisters = new bool[_tableSize];
    for (int16_t i = 0; i < _tableSize; i++)
    {
        _sendRegisters[i] = false;
    }
}

void ModbusTable::setData(int16_t adress, bool value)
{
    if ((_table == FLPROG_HOLDING_REGISTR) || (_table == FLPROG_INPUT_REGISTR))
    {
        return;
    }
    int16_t index = indexForAddres(adress);
    if (index == -1)
    {
        return;
    }
    if (value == _boolData[index])
    {
        return;
    }
    if (_sendRegisters != 0)
    {
        _sendRegisters[index] = true;
    }
    _boolData[index] = value;
    if (_newDataCallback != 0)
    {
        uint16_t temp = 0;
        if (value)
        {
            temp = 1;
        }
        _newDataCallback(_table, adress, temp);
    }
}

void ModbusTable::setData(int16_t adress, int16_t value)
{
    if ((_table == FLPROG_COIL) || (_table == FLPROG_DISCRETE_INPUT))
    {
        return;
    }
    int16_t index = indexForAddres(adress);
    if (index == -1)
    {
        return;
    }
    if (value == _worldData[index])
    {
        return;
    }
    if (_sendRegisters != 0)
    {
        _sendRegisters[index] = true;
    }
    _worldData[index] = value;
    if (_newDataCallback != 0)
    {
        _newDataCallback(_table, adress, value);
    }
}

void ModbusTable::writeRegister(int16_t adress, bool value)
{
    if ((_table == FLPROG_HOLDING_REGISTR) || (_table == FLPROG_INPUT_REGISTR))
    {
        return;
    }
    int16_t index = indexForAddres(adress);
    if (index != -1)
    {
        if (_boolData[index] == value)
        {
            return;
        }
        _boolData[index] = value;
        if (_newDataCallback != 0)
        {
            uint16_t temp = 0;
            if (value)
            {
                temp = 1;
            }
            _newDataCallback(_table, adress, temp);
        }
    }
}

void ModbusTable::writeRegister(int16_t adress, int16_t value)
{
    if ((_table == FLPROG_COIL) || (_table == FLPROG_DISCRETE_INPUT))
    {
        return;
    }
    int16_t index = indexForAddres(adress);
    if (index != -1)
    {
        if (_worldData[index] == value)
        {
            return;
        }
        _worldData[index] = value;
        if (_newDataCallback != 0)
        {
            _newDataCallback(_table, adress, value);
        }
    }
}

int16_t ModbusTable::readWorldRegister(int16_t adress)
{
    if ((_table == FLPROG_COIL) || (_table == FLPROG_DISCRETE_INPUT))
    {
        return 0;
    }
    int16_t index = indexForAddres(adress);
    if (index == -1)
    {
        return 0;
    }
    return _worldData[index];
}

bool ModbusTable::readBoolRegister(int16_t adress)
{
    if ((_table == FLPROG_HOLDING_REGISTR) || (_table == FLPROG_INPUT_REGISTR))
    {
        return false;
    }
    int16_t index = indexForAddres(adress);
    if (index == -1)
    {
        return false;
    }

    return _boolData[index];
}

int16_t ModbusTable::indexForAddres(int16_t addr)
{
    if ((_adresses == 0) && (!(_startAddres < 0)))
    {
        int16_t ind = addr - _startAddres;
        if (ind < _tableSize)
        {
            return ind;
        }
        else
        {
            return -1;
        }
    }
    for (int16_t i = 0; i < _tableSize; i++)
    {
        if (adressForIndex(i) == addr)
        {
            return i;
        }
    }
    return -1;
}

int16_t ModbusTable::adressForIndex(int16_t index)
{

    if (!(index < _tableSize))
    {
        return -1;
    }
    if (_adresses == 0)
    {
        if (_startAddres < 0)
        {
            return -1;
        }
        return _startAddres + index;
    }
    return _adresses[index];
}

int16_t ModbusTable::firstWriteAddress()
{
    if (_sendRegisters == 0)
    {
        return -1;
    }
    for (int16_t i = 0; i < _tableSize; i++)
    {
        if (_sendRegisters[i])
        {
            return adressForIndex(i);
        }
    }
    return -1;
}

int16_t ModbusTable::writeRegsSize(int16_t startAddres)
{
    return recursiveWriteRegsSize(startAddres, 1);
}

int16_t ModbusTable::recursiveWriteRegsSize(int16_t startAddres, int16_t result)
{
    if (result > 24)
    {
        return result;
    }
    int16_t newAddress = startAddres + 1;
    int16_t newResult = result + 1;
    if (!hasAdress(newAddress))
    {
        return result;
    }
    if (!_sendRegisters[indexForAddres(newAddress)])
    {
        return result;
    }

    return recursiveWriteRegsSize(newAddress, newResult);
}

int16_t ModbusTable::readRegsSize(int16_t startAddres)
{
    return recursiveReadRegsSize(startAddres, 1);
}

int16_t ModbusTable::recursiveReadRegsSize(int16_t startAddres, int16_t result)
{
    if (result > 24)
    {
        return result;
    }
    int16_t newAddress = startAddres + 1;
    int16_t newResult = result + 1;
    if (hasAdress(newAddress))
    {
        return recursiveReadRegsSize(newAddress, newResult);
    }
    return result;
}

int16_t ModbusTable::findNextStartAddres(int16_t result)
{
    int16_t nextAddres = result + 1;
    if (nextAddres > getMaxAdress())
    {
        return -1;
    }

    for (int16_t i = 0; i < _tableSize; i++)
    {
        if (adressForIndex(i) == nextAddres)
        {
            return adressForIndex(i);
        }
    }
    return findNextStartAddres(nextAddres);
}

int16_t ModbusTable::getMinAdress()
{
    if (_minAdress == -1)
    {
        _minAdress = 1000;
    }
    else
    {
        return _minAdress;
    }
    for (int16_t i = 0; i < _tableSize; i++)
    {
        if (adressForIndex(i) < _minAdress)
        {
            _minAdress = adressForIndex(i);
        }
    }
    return _minAdress;
}

int16_t ModbusTable::getMaxAdress()
{
    if (_maxAdress != -1)
    {
        return _maxAdress;
    }
    for (int16_t i = 0; i < _tableSize; i++)
    {
        if (adressForIndex(i) > _maxAdress)
        {
            _maxAdress = adressForIndex(i);
        }
    }
    return _maxAdress;
}

void ModbusTable::resetWriteFlag(int16_t addres)
{
    _sendRegisters[indexForAddres(addres)] = false;
}

bool ModbusTable::hasAdress(int16_t address)
{
    return ((indexForAddres(address)) != -1);
}

// ******************ModbusMainData******************************
void ModbusMainData::setDataTable(ModbusTable *table)
{
    if (table->tableType() == FLPROG_HOLDING_REGISTR)
    {
        _tableHoldingRegistr = table;
        return;
    }
    if (table->tableType() == FLPROG_INPUT_REGISTR)
    {
        _tableInputRegistr = table;
        return;
    }
    if (table->tableType() == FLPROG_COIL)
    {
        _tableCoil = table;
        return;
    }
    if (table->tableType() == FLPROG_DISCRETE_INPUT)
    {
        _tableDiscreteInput = table;
        return;
    }
}

void ModbusMainData::configDataTable(uint8_t _table, uint16_t dataSize, int16_t startAddres)
{
    ModbusTable *table = tableForType(_table);
    if (table == 0)
    {
        setDataTable(new ModbusTable(_table, dataSize, startAddres));
        return;
    }
    table->init(dataSize, startAddres);
}

void ModbusMainData::setDataTable(uint8_t _table, uint16_t dataSize, int *_adresses)
{
    ModbusTable *table = tableForType(_table);
    if (table == 0)
    {
        setDataTable(new ModbusTable(_table, dataSize, _adresses));
        return;
    }
    table->init(dataSize, _adresses);
}

bool ModbusMainData::hasTable(uint8_t table)
{
    switch (table)
    {
    case FLPROG_COIL:
        return _tableCoil != 0;
        break;
    case FLPROG_DISCRETE_INPUT:
        return _tableDiscreteInput != 0;
        break;
    case FLPROG_INPUT_REGISTR:
        return _tableInputRegistr != 0;
        break;
    case FLPROG_HOLDING_REGISTR:
        return _tableHoldingRegistr != 0;
        break;
    default:
        return false;
        break;
    }
    return false;
}

void ModbusMainData::setIntOrder(uint8_t order)
{
    _intOrder = order;
}

void ModbusMainData::setLongOrder(uint8_t order)
{
    _longOrder = order;
}

void ModbusMainData::setFloatOrder(uint8_t order)
{
    _floatOrder = order;
}

void ModbusMainData::setUnsignedlongOrder(uint8_t order)
{
    _unsignedlongOrder = order;
}

ModbusTable *ModbusMainData::tableForType(uint8_t table)
{
    if (table == FLPROG_HOLDING_REGISTR)
    {
        return _tableHoldingRegistr;
    }
    if (table == FLPROG_INPUT_REGISTR)
    {
        return _tableInputRegistr;
    }
    if (table == FLPROG_COIL)
    {
        return _tableCoil;
    }
    if (table == FLPROG_DISCRETE_INPUT)
    {
        return _tableDiscreteInput;
    }
    return 0;
}

int16_t ModbusMainData::indexForAddres(int16_t addr, uint8_t table)
{
    return tableForType(table)->indexForAddres(addr);
}

bool ModbusMainData::checkModbusAddres(int16_t addr, uint8_t table)
{
    if (!hasTable(table))
    {
        return false;
    }
    return (!((indexForAddres(addr, table)) == -1));
}

bool ModbusMainData::checkModbusRange(int16_t startAddr, int16_t addrNumber, uint8_t table)
{

    for (int16_t i = 0; i < addrNumber; i++)
    {
        if (!(checkModbusAddres((startAddr + i), table)))
        {
            return false;
        }
    }
    return true;
}

ModbusTable *ModbusMainData::tableForStartArddres(uint8_t table, int16_t startAddres, bool isTwoWord)
{
    if (!hasTable(table))
    {
        return 0;
    }
    ModbusTable *tableData = tableForType(table);
    if (tableData == 0)
    {
        return 0;
    }
    if (tableData->hasAdress(startAddres))
    {
        if (isTwoWord)
        {
            if (tableData->hasAdress(startAddres + 1))
            {
                return tableData;
            }
        }
        else
        {
            return tableData;
        }
    }
    return 0;
}

void ModbusMainData::saveLong(int32_t value, uint8_t table, int16_t startAddres)
{
    unsigned char sourse[4];
    memcpy(sourse, &value, 4);
    saveForByteWithOrder(sourse, table, startAddres, _longOrder);
}

void ModbusMainData::saveUnsignedLong(uint32_t value, uint8_t table, int16_t startAddres)
{
    unsigned char sourse[4];
    memcpy(sourse, &value, 4);
    saveForByteWithOrder(sourse, table, startAddres, _unsignedlongOrder);
}

void ModbusMainData::saveFloat(float value, uint8_t table, int16_t startAddres)
{
    unsigned char sourse[4];
    memcpy(sourse, &value, 4);
    saveForByteWithOrder(sourse, table, startAddres, _floatOrder);
}

void ModbusMainData::saveInteger(int16_t value, uint8_t table, int16_t startAddres)
{
    if (!canSaveTable(table))
    {
        return;
    }
    ModbusTable *tableData = tableForStartArddres(table, startAddres, false);
    if (tableData == 0)
    {
        return;
    }
    int16_t w1;
    int16_t index = tableData->indexForAddres(startAddres);
    if (_intOrder == 1)
    {
        w1 = value;
        tableData->setData(startAddres, w1);
        return;
    }
    w1 = (int(word(lowByte(value), highByte(value))));
    tableData->setData(index, w1);
}

void ModbusMainData::saveByte(uint8_t value, uint8_t table, int16_t startAddres)
{
    if (!canSaveTable(table))
    {
        return;
    }
    ModbusTable *tableData = tableForStartArddres(table, startAddres, false);
    if (tableData == 0)
    {
        return;
    }
    int16_t w1 = value;
    tableData->setData(startAddres, w1);
}

void ModbusMainData::saveBool(bool value, uint8_t table, int16_t startAddres)
{
    if (!canSaveTable(table))
    {
        return;
    }
    ModbusTable *tableData = tableForStartArddres(table, startAddres, false);
    if (tableData == 0)
    {
        return;
    }
    tableData->setData(startAddres, value);
}

void ModbusMainData::saveForByteWithOrder(unsigned char *sourse, uint8_t table, int16_t startAddres, uint8_t order)
{
    if (!canSaveTable(table))
    {
        return;
    }
    ModbusTable *tableData = tableForStartArddres(table, startAddres, true);
    if (tableData == 0)
    {
        return;
    }
    int16_t temp;
    if (order == 1) //(ABCD)
    {
        temp = int(word(sourse[3], sourse[2]));
        tableData->setData(startAddres, temp);
        temp = int(word(sourse[1], sourse[0]));
        tableData->setData((startAddres + 1), temp);
    }
    if (order == 2) //(CDAB)
    {
        temp = int(word(sourse[1], sourse[0]));
        tableData->setData(startAddres, temp);
        temp = int(word(sourse[3], sourse[2]));
        tableData->setData((startAddres + 1), temp);
    }
    if (order == 3) //(BADC)
    {
        temp = int(word(sourse[2], sourse[3]));
        tableData->setData(startAddres, temp);
        temp = int(word(sourse[0], sourse[1]));
        tableData->setData((startAddres + 1), temp);
    }
    if (order == 4) //(DCBA)
    {
        temp = int(word(sourse[0], sourse[1]));
        tableData->setData(startAddres, temp);
        temp = int(word(sourse[2], sourse[3]));
        tableData->setData((startAddres + 1), temp);
    }
}

uint8_t ModbusMainData::readByte(uint8_t table, int16_t startAddres)
{
    ModbusTable *tableData = tableForStartArddres(table, startAddres, false);
    if (tableData == 0)
    {
        return 0;
    }
    return tableData->readWorldRegister(startAddres);
}

int16_t ModbusMainData::readInteger(uint8_t table, int16_t startAddres)
{
    ModbusTable *tableData = tableForStartArddres(table, startAddres, false);
    if (tableData == 0)
    {
        return 0;
    }
    int16_t w1 = tableData->readWorldRegister(startAddres);
    if (_intOrder == FLPROG_AB_ORDER)
    {
        return w1;
    }

    return (int(word(lowByte(w1), highByte(w1))));
}

float ModbusMainData::readFloat(uint8_t table, int16_t startAddres)
{
    float result;
    unsigned char sourse[4];
    readForByteWithOrder(sourse, table, startAddres, _floatOrder);
    memcpy(&result, sourse, 4);
    return result;
}

int32_t ModbusMainData::readLong(uint8_t table, int16_t startAddres)
{
    int32_t result;
    unsigned char sourse[4];
    readForByteWithOrder(sourse, table, startAddres, _longOrder);
    memcpy(&result, sourse, 4);
    return result;
}

uint32_t ModbusMainData::readUnsignedLong(uint8_t table, int16_t startAddres)
{
    uint32_t result;
    unsigned char sourse[4];
    readForByteWithOrder(sourse, table, startAddres, _unsignedlongOrder);
    memcpy(&result, sourse, 4);
    return result;
}

void ModbusMainData::readForByteWithOrder(unsigned char *sourse, uint8_t table, int16_t startAddres, uint8_t order)
{
    ModbusTable *tableData = tableForStartArddres(table, startAddres, true);
    if (tableData == 0)
    {
        return;
    }
    int16_t temp = tableData->readWorldRegister(startAddres);
    int16_t temp1 = tableData->readWorldRegister(startAddres + 1);
    if (order == 1) //(ABCD)
    {
        sourse[0] = lowByte(temp1);
        sourse[1] = highByte(temp1);
        sourse[2] = lowByte(temp);
        sourse[3] = highByte(temp);
    }
    if (order == 2) //(CDAB)
    {
        sourse[0] = lowByte(temp);
        sourse[1] = highByte(temp);
        sourse[2] = lowByte(temp1);
        sourse[3] = highByte(temp1);
    }
    if (order == 3) //(BADC)
    {
        sourse[0] = highByte(temp1);
        sourse[1] = lowByte(temp1);
        sourse[2] = highByte(temp);
        sourse[3] = lowByte(temp);
    }
    if (order == 4) // (DCBA)
    {
        sourse[0] = highByte(temp);
        sourse[1] = lowByte(temp);
        sourse[2] = highByte(temp1);
        sourse[3] = lowByte(temp1);
    }
}

bool ModbusMainData::readBool(uint8_t table, int16_t startAddres)
{
    ModbusTable *tableData = tableForStartArddres(table, startAddres, false);
    if (tableData == 0)
    {
        return 0;
    }
    return tableData->readBoolRegister(startAddres);
}

bool ModbusMainData::isSupportFunction(uint8_t function)
{
    if (function == 1)
    {
        return _tableCoil != 0;
    }
    if (function == 2)
    {
        return _tableDiscreteInput != 0;
    }
    if (function == 3)
    {
        return _tableHoldingRegistr != 0;
    }
    if (function == 4)
    {
        return _tableInputRegistr != 0;
    }
    if (function == 5)
    {
        return _tableCoil != 0;
    }
    if (function == 6)
    {
        return _tableHoldingRegistr != 0;
    }
    if (function == 15)
    {
        return _tableCoil != 0;
    }
    if (function == 16)
    {
        return _tableHoldingRegistr != 0;
    }
    return false;
}

bool ModbusMainData::canSaveTable(uint8_t table)
{
    return (hasTable(table));
}

void ModbusMainData::setCallBack(FLProgModbusNewDataCallback func)
{
    if (_tableCoil != 0)
    {
        _tableCoil->setCallBack(func);
    }
    if (_tableDiscreteInput != 0)
    {
        _tableDiscreteInput->setCallBack(func);
    }
    if (_tableInputRegistr != 0)
    {
        _tableInputRegistr->setCallBack(func);
    }
    if (_tableHoldingRegistr != 0)
    {
        _tableHoldingRegistr->setCallBack(func);
    }
}

// ModbusSlaveInMaster******************************8
uint8_t ModbusSlaveInMaster::getLastError()
{
    return _lastError;
}

void ModbusSlaveInMaster::setPollingPeriod(uint32_t period)
{
    _pollingPeriod = period;
}

void ModbusSlaveInMaster::setTimeOutTime(uint32_t time)
{
    _timeOutTime = time;
}

uint32_t ModbusSlaveInMaster::getTimeOutTime()
{
    return _timeOutTime;
}

void ModbusSlaveInMaster::setLastError(uint8_t error)
{
    _lastError = error;
}

bool ModbusSlaveInMaster::canSaveTable(uint8_t table)
{
    if (!ModbusMainData::canSaveTable(table))
    {
        return false;
    }
    return ((table == FLPROG_HOLDING_REGISTR) || (table == FLPROG_COIL));
}

bool ModbusSlaveInMaster::hasWriteRegisters()
{

    ModbusTable *table1 = tableForType(FLPROG_HOLDING_REGISTR);
    if (table1 != 0)
    {

        if (table1->firstWriteAddress() != -1)
        {
            return true;
        }
    }
    table1 = tableForType(FLPROG_COIL);

    if (table1 != 0)
    {

        if (table1->firstWriteAddress() != -1)
        {
            return true;
        }
    }
    return false;
}

ModbusTable *ModbusSlaveInMaster::firstWriteTable()
{

    ModbusTable *table1 = tableForType(FLPROG_HOLDING_REGISTR);
    if (table1 != 0)
    {

        if (table1->firstWriteAddress() != -1)
        {
            return table1;
        }
    }
    table1 = tableForType(FLPROG_COIL);

    if (table1 != 0)
    {

        if (table1->firstWriteAddress() != -1)
        {
            return table1;
        }
    }
    return 0;
}

ModbusTable *ModbusSlaveInMaster::firstTabe()
{
    if (hasTable(FLPROG_COIL))
    {

        return tableForType(FLPROG_COIL);
    }
    if (hasTable(FLPROG_DISCRETE_INPUT))
    {

        return tableForType(FLPROG_DISCRETE_INPUT);
    }
    if (hasTable(FLPROG_INPUT_REGISTR))
    {

        return tableForType(FLPROG_INPUT_REGISTR);
    }
    if (hasTable(FLPROG_HOLDING_REGISTR))
    {

        return tableForType(FLPROG_HOLDING_REGISTR);
    }
    return 0;
}

ModbusTable *ModbusSlaveInMaster::lastTable()
{
    if (hasTable(FLPROG_HOLDING_REGISTR))
    {

        return tableForType(FLPROG_HOLDING_REGISTR);
    }
    if (hasTable(FLPROG_INPUT_REGISTR))
    {

        return tableForType(FLPROG_INPUT_REGISTR);
    }

    if (hasTable(FLPROG_DISCRETE_INPUT))
    {

        return tableForType(FLPROG_DISCRETE_INPUT);
    }

    if (hasTable(FLPROG_COIL))
    {

        return tableForType(FLPROG_COIL);
    }

    return 0;
}

ModbusTable *ModbusSlaveInMaster::nextTable(ModbusTable *table)
{
    if (table == 0)
    {
        return firstTabe();
    }
    if (table == lastTable())
    {
        return 0;
    }
    int16_t code = nextTable(table->tableType());
    if (code == -1)
    {
        return 0;
    }
    return tableForType(code);
}

int16_t ModbusSlaveInMaster::nextTable(int16_t table)
{
    int16_t result = -1;
    if (table == FLPROG_COIL)
    {
        result = FLPROG_DISCRETE_INPUT;
    }
    if (table == FLPROG_DISCRETE_INPUT)
    {
        result = FLPROG_INPUT_REGISTR;
    }
    if (table == FLPROG_INPUT_REGISTR)
    {
        result = FLPROG_HOLDING_REGISTR;
    }
    if (table == FLPROG_HOLDING_REGISTR)
    {
        return -1;
    }
    if (!hasTable(result))
    {
        return nextTable(result);
    }
    return result;
}

bool ModbusSlaveInMaster::isReady()
{
    if (!_isActive)
    {
        return false;
    }
    if (!_initFlag)
    {
        return true;
    }
    if (flprog::isTimer(_lastReqestTime, _pollingPeriod))
    {
        return true;
    }
    return false;
}

void ModbusSlaveInMaster::setSlaveAddress(uint8_t adr)
{
    _slaveAddres = adr;
}
