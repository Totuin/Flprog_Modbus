#include "flprogModbusData.h"

//------------------- ModbusTable--------------------------------------

ModbusTable::ModbusTable(uint8_t _table, uint16_t dataSize, uint16_t _startAdr)
{
    if (_table > 4)
    {
        return;
    }
    table = _table;
    startAdr = _startAdr;
    init(dataSize);
}

ModbusTable::ModbusTable(uint8_t _table, uint16_t dataSize, int *_adresses)
{
    if (_table > 4)
    {
        return;
    }
    table = _table;
    init(dataSize);
    adresses = _adresses;
}

void ModbusTable::init(int16_t dataSize, int *_adresses)
{
    if (adresses != 0)
    {
        adresses = 0;
    }
    adresses = _adresses;
    startAdr = -1;
    init(dataSize);
}

void ModbusTable::init(int16_t dataSize, uint16_t _startAdr)
{
    if (adresses != 0)
    {
        adresses = 0;
    }
    startAdr = _startAdr;
    init(dataSize);
}

void ModbusTable::init(int16_t dataSize)
{
    tableSize = dataSize;
    if ((table == FLPROG_INPUT_REGISTR) || (table == FLPROG_HOLDING_REGISTR))
    {
        worldData = new int16_t[tableSize];
    }
    else
    {
        boolData = new bool[tableSize];
    }
    sendRegisters = new bool[tableSize];
    for (int16_t i = 0; i < tableSize; i++)
    {
        sendRegisters[i] = false;
    }
}

void ModbusTable::setData(int16_t adress, bool value)
{
    if ((table == FLPROG_HOLDING_REGISTR) || (table == FLPROG_INPUT_REGISTR))
    {
        return;
    }
    int16_t index = indexForAddres(adress);
    if (index == -1)
    {
        return;
    }
    if (value == boolData[index])
    {
        return;
    }
    if (sendRegisters != 0)
    {
        sendRegisters[index] = true;
    }
    boolData[index] = value;
}

void ModbusTable::setData(int16_t adress, int16_t value)
{
    if ((table == FLPROG_COIL) || (table == FLPROG_DISCRETE_INPUT))
    {
        return;
    }
    int16_t index = indexForAddres(adress);
    if (index == -1)
    {
        return;
    }

    if (value == worldData[index])
    {
        return;
    }
    if (sendRegisters != 0)
    {
        sendRegisters[index] = true;
    }
    worldData[index] = value;
}

void ModbusTable::writeRegister(int16_t adress, bool value)
{
    if ((table == FLPROG_HOLDING_REGISTR) || (table == FLPROG_INPUT_REGISTR))
    {
        return;
    }
    int16_t index = indexForAddres(adress);
    if (index != -1)
    {
        boolData[index] = value;
    }
}

void ModbusTable::writeRegister(int16_t adress, int16_t value)
{
    if ((table == FLPROG_COIL) || (table == FLPROG_DISCRETE_INPUT))
    {
        return;
    }
    int16_t index = indexForAddres(adress);
    if (index != -1)
    {
        worldData[index] = value;
    }
}

int16_t ModbusTable::readWorldRegister(int16_t adress)
{
    if ((table == FLPROG_COIL) || (table == FLPROG_DISCRETE_INPUT))
    {
        return 0;
    }
    int16_t index = indexForAddres(adress);
    if (index == -1)
    {
        return 0;
    }
    return worldData[index];
}

bool ModbusTable::readBoolRegister(int16_t adress)
{
    if ((table == FLPROG_HOLDING_REGISTR) || (table == FLPROG_INPUT_REGISTR))
    {
        return false;
    }
    int16_t index = indexForAddres(adress);
    if (index == -1)
    {
        return false;
    }

    return boolData[index];
}

int16_t ModbusTable::indexForAddres(int16_t addr)
{
    if ((adresses == 0) && (!(startAdr < 0)))
    {
        int16_t ind = addr - startAdr;
        if (ind < tableSize)
        {
            return ind;
        }
        else
        {
            return -1;
        }
    }
    for (int16_t i = 0; i < tableSize; i++)
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

    if (!(index < tableSize))
    {
        return -1;
    }
    if (adresses == 0)
    {
        if (startAdr < 0)
        {
            return -1;
        }
        return startAdr + index;
    }
    return adresses[index];
}

int16_t ModbusTable::firstWriteAddress()
{
    if (sendRegisters == 0)
    {
        return -1;
    }
    for (int16_t i = 0; i < tableSize; i++)
    {
        if (sendRegisters[i])
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
    if (!sendRegisters[indexForAddres(newAddress)])
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

    for (int16_t i = 0; i < tableSize; i++)
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
    if (minAdress == -1)
    {
        minAdress = 1000;
    }
    else
    {
        return minAdress;
    }
    for (int16_t i = 0; i < tableSize; i++)
    {
        if (adressForIndex(i) < minAdress)
        {
            minAdress = adressForIndex(i);
        }
    }
    return minAdress;
}

int16_t ModbusTable::getMaxAdress()
{
    if (maxAdress != -1)
    {
        return maxAdress;
    }
    for (int16_t i = 0; i < tableSize; i++)
    {
        if (adressForIndex(i) > maxAdress)
        {
            maxAdress = adressForIndex(i);
        }
    }
    return maxAdress;
}

void ModbusTable::resetWriteFlag(int16_t addres)
{
    sendRegisters[indexForAddres(addres)] = false;
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
        tableHoldingRegistr = table;
        return;
    }
    if (table->tableType() == FLPROG_INPUT_REGISTR)
    {
        tableInputRegistr = table;
        return;
    }
    if (table->tableType() == FLPROG_COIL)
    {
        tableCoil = table;
        return;
    }
    if (table->tableType() == FLPROG_DISCRETE_INPUT)
    {
        tableDiscreteInput = table;
        return;
    }
}

void ModbusMainData::configDataTable(uint8_t _table, uint16_t dataSize, int16_t _startAdr)
{
    ModbusTable *table = tableForType(_table);
    if (table == 0)
    {
        setDataTable(new ModbusTable(_table, dataSize, _startAdr));
        return;
    }
    table->init(dataSize, _startAdr);
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
        return tableCoil != 0;
        break;
    case FLPROG_DISCRETE_INPUT:
        return tableDiscreteInput != 0;
        break;
    case FLPROG_INPUT_REGISTR:
        return tableInputRegistr != 0;
        break;
    case FLPROG_HOLDING_REGISTR:
        return tableHoldingRegistr != 0;
        break;
    default:
        return false;
        break;
    }
    return false;
}

void ModbusMainData::setIntOrder(uint8_t order)
{
    intOrder = order;
}

void ModbusMainData::setLongOrder(uint8_t order)
{
    longOrder = order;
}

void ModbusMainData::setFloatOrder(uint8_t order)
{
    floatOrder = order;
}

void ModbusMainData::setUnsignedlongOrder(uint8_t order)
{
    unsignedlongOrder = order;
}

ModbusTable *ModbusMainData::tableForType(uint8_t table)
{
    if (table == FLPROG_HOLDING_REGISTR)
    {
        return tableHoldingRegistr;
    }
    if (table == FLPROG_INPUT_REGISTR)
    {
        return tableInputRegistr;
    }
    if (table == FLPROG_COIL)
    {
        return tableCoil;
    }
    if (table == FLPROG_DISCRETE_INPUT)
    {
        return tableDiscreteInput;
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
    saveForByteWithOrder(sourse, table, startAddres, longOrder);
}

void ModbusMainData::saveUnsignedLong(uint32_t value, uint8_t table, int16_t startAddres)
{
    unsigned char sourse[4];
    memcpy(sourse, &value, 4);
    saveForByteWithOrder(sourse, table, startAddres, unsignedlongOrder);
}

void ModbusMainData::saveFloat(float value, uint8_t table, int16_t startAddres)
{
    unsigned char sourse[4];
    memcpy(sourse, &value, 4);
    saveForByteWithOrder(sourse, table, startAddres, floatOrder);
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
    if (intOrder == 1)
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
    unsigned char target[2];
    int16_t temp;
    uint8_t orderArray[4];
    modbusOrderAray(orderArray, order);
    target[0] = sourse[orderArray[0]];
    target[1] = sourse[orderArray[1]];
    memcpy(&temp, target, 2);
    tableData->setData((startAddres + 1), temp);
    target[0] = sourse[orderArray[2]];
    target[1] = sourse[orderArray[3]];
    memcpy(&temp, target, 2);
    tableData->setData((startAddres), temp);
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
    if (intOrder == FLPROG_AB_ORDER)
    {
        return w1;
    }

    return (int(word(lowByte(w1), highByte(w1))));
}

float ModbusMainData::readFloat(uint8_t table, int16_t startAddres)
{
    float result;
    unsigned char sourse[4];
    readForByteWithOrder(sourse, table, startAddres, floatOrder);
    memcpy(&result, sourse, 4);
    return result;
}

int32_t ModbusMainData::readLong(uint8_t table, int16_t startAddres)
{
    int32_t result;
    unsigned char sourse[4];
    readForByteWithOrder(sourse, table, startAddres, longOrder);
    memcpy(&result, sourse, 4);
    return result;
}

uint32_t ModbusMainData::readUnsignedLong(uint8_t table, int16_t startAddres)
{
    uint32_t result;
    unsigned char sourse[4];
    readForByteWithOrder(sourse, table, startAddres, unsignedlongOrder);
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
    unsigned char target[2];
    uint8_t orderArray[4] = {0, 1, 2, 3};
    modbusOrderAray(orderArray, order);
    int16_t temp = tableData->readWorldRegister(startAddres);
    memcpy(target, &temp, 2);
    sourse[2] = target[orderArray[0]];
    sourse[3] = target[orderArray[1]];
    temp = tableData->readWorldRegister(startAddres + 1);
    memcpy(target, &temp, 2);
    sourse[0] = target[orderArray[0]];
    sourse[1] = target[orderArray[1]];
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
        return tableCoil != 0;
    }
    if (function == 2)
    {
        return tableDiscreteInput != 0;
    }
    if (function == 3)
    {
        return tableHoldingRegistr != 0;
    }
    if (function == 4)
    {
        return tableInputRegistr != 0;
    }
    if (function == 5)
    {
        return tableCoil != 0;
    }
    if (function == 6)
    {
        return tableHoldingRegistr != 0;
    }
    if (function == 15)
    {
        return tableCoil != 0;
    }
    if (function == 16)
    {
        return tableHoldingRegistr != 0;
    }
    return false;
}

void ModbusMainData::modbusOrderAray(uint8_t *orderArray, uint8_t order)
{
    if (order == 1)
    {
        orderArray[0] = 0;
        orderArray[1] = 1;
        orderArray[2] = 2;
        orderArray[3] = 3;
    }
    if (order == 2)
    {
        orderArray[0] = 2;
        orderArray[1] = 3;
        orderArray[2] = 0;
        orderArray[3] = 1;
    }
    if (order == 3)
    {
        orderArray[0] = 1;
        orderArray[1] = 0;
        orderArray[2] = 3;
        orderArray[3] = 2;
    }
    if (order == 4)
    {
        orderArray[0] = 3;
        orderArray[1] = 2;
        orderArray[2] = 1;
        orderArray[3] = 0;
    }
}

bool ModbusMainData::canSaveTable(uint8_t table)
{
    return (hasTable(table));
}

void ModbusMainData::sortTwoWordRegistors(uint8_t *orderArray, uint8_t order, uint8_t *data, uint16_t w1, uint16_t w2)
{
    modbusOrderAray(orderArray, order);
    for (uint8_t i = 0; i < 8; i++)
        bitWrite(data[orderArray[0]], i, bitRead(w2, i));
    for (uint8_t i = 0; i < 8; i++)
        bitWrite(data[orderArray[1]], i, bitRead(w2, (i + 8)));
    for (uint8_t i = 0; i < 8; i++)
        bitWrite(data[orderArray[2]], i, bitRead(w1, i));
    for (uint8_t i = 0; i < 8; i++)
        bitWrite(data[orderArray[3]], i, bitRead(w1, (i + 8)));
}

// ModbusSlaveInMaster******************************8
uint8_t ModbusSlaveInMaster::getLastError()
{
    return lastError;
}

void ModbusSlaveInMaster::status(bool status)
{
    isActive = status;
}

void ModbusSlaveInMaster::setPollingPeriod(uint32_t period)
{
    pollingPeriod = period;
}

void ModbusSlaveInMaster::setTimeOutTime(uint32_t time)
{
    timeOutTime = time;
}

uint32_t ModbusSlaveInMaster::getTimeOutTime()
{
    return timeOutTime;
}

void ModbusSlaveInMaster::setLastError(uint8_t error)
{
    lastError = error;
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
    if (!isActive)
    {
        return false;
    }
    if (!initFlag)
    {
        return true;
    }
    if (flprog::isTimer(lastReqestTime, pollingPeriod))
    {
        return true;
    }
    return false;
}

void ModbusSlaveInMaster::setSlaveAddress(uint8_t adr)
{
    slaveAdr = adr;
}
