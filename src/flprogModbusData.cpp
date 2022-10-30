#include "flprogModbusData.h"

// ModbusTable*********************************
int ModbusTable::indexForAddres(int addr)
{
    for (byte i = 0; i < tableSize; i++)
    {
        if (adresses[i] == addr)
        {
            return i;
        }
    }
    return -1;
}

int ModbusTable::firstWriteAddress()
{
    if (sendRegisters == 0)
    {
        return -1;
    }
    for (int i = 0; i < tableSize; i++)
    {
        if (sendRegisters[i])
        {
            return adresses[i];
        }
    }
    return -1;
}

int ModbusTable::writeRegsSize(int startAddres)
{
    return recursiveWriteRegsSize(startAddres, 1);
}

int ModbusTable::recursiveWriteRegsSize(int startAddres, int result)
{
    if (result > 24)
    {
        return result;
    }
    int newAddress = startAddres + 1;
    int newResult = result + 1;
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

int ModbusTable::readRegsSize(int startAddres)
{
    return recursiveReadRegsSize(startAddres, 1);
}

int ModbusTable::recursiveReadRegsSize(int startAddres, int result)
{
    if (result > 24)
    {
        return result;
    }
    int newAddress = startAddres + 1;
    int newResult = result + 1;
    if (hasAdress(newAddress))
    {
        return recursiveReadRegsSize(newAddress, newResult);
    }
    return result;
}

int ModbusTable::findNextStartAddres(int result)
{
    int nextAddres = result + 1;
    if (nextAddres > getMaxAdress())
    {
        return -1;
    }

    for (int i = 0; i < tableSize; i++)
    {
        if (adresses[i] == nextAddres)
        {
            return adresses[i];
        }
    }
    return findNextStartAddres(nextAddres);
}

int ModbusTable::getMinAdress()
{
    if (minAdress == -1)
    {
        minAdress = 1000;
    }
    else
    {
        return minAdress;
    }
    for (int i = 0; i < tableSize; i++)
    {
        if (adresses[i] < minAdress)
        {
            minAdress = adresses[i];
        }
    }
    return minAdress;
}

int ModbusTable::getMaxAdress()
{
    if (maxAdress != -1)
    {
        return maxAdress;
    }
    for (int i = 0; i < tableSize; i++)
    {
        if (adresses[i] > maxAdress)
        {
            maxAdress = adresses[i];
        }
    }
    return maxAdress;
}

void ModbusTable::resetWriteFlag(int addres)
{
    sendRegisters[indexForAddres(addres)] = false;
}

bool ModbusTable::hasAdress(int address)
{
    return ((indexForAddres(address)) != -1);
}

// ModbusWorldTable*****************************
ModbusWorldTable::ModbusWorldTable(byte table, int data[], int adresses[], int tableSize)
{
    this->table = table;
    this->data = data;
    this->adresses = adresses;
    this->tableSize = tableSize;
}

ModbusWorldTable::ModbusWorldTable(byte table, int data[], int adresses[], bool sendRegisters[], int tableSize)
{
    this->table = table;
    this->data = data;
    this->adresses = adresses;
    this->tableSize = tableSize;
    this->sendRegisters = sendRegisters;
}

void ModbusWorldTable::setData(int address, uint16_t value)
{
    int index = indexForAddres(address);
    if (index == -1)
    {
        return;
    }

    if (value == data[index])
    {
        return;
    }
    if (sendRegisters != 0)
    {
        sendRegisters[index] = true;
    }
    data[index] = value;
}

void ModbusWorldTable::writeRegister(int addres, int value)
{

    int index = indexForAddres(addres);
    if (index != -1)
    {
        data[index] = value;
    }
}

int ModbusWorldTable::readWorldRegister(int addres)
{
    int index = indexForAddres(addres);
    if (index == -1)
    {
        return 0;
    }
    return data[index];
}

// ModbusBooldTable*****************************
ModbusBoolTable::ModbusBoolTable(byte table, bool data[], int adresses[], int tableSize)
{
    this->table = table;
    this->data = data;
    this->adresses = adresses;
    this->tableSize = tableSize;
}

ModbusBoolTable::ModbusBoolTable(byte table, bool data[], int adresses[], bool sendRegisters[], int tableSize)
{
    this->table = table;
    this->data = data;
    this->adresses = adresses;
    this->tableSize = tableSize;
    this->sendRegisters = sendRegisters;
}

void ModbusBoolTable::setData(int adress, bool value)
{
    int index = indexForAddres(adress);
    if (index == -1)
    {
        return;
    }
    if (value == data[index])
    {
        return;
    }
    if (sendRegisters != 0)
    {
        sendRegisters[index] = true;
    }
    data[index] = value;
}

void ModbusBoolTable::writeRegister(int addres, bool value)
{
    int index = indexForAddres(addres);
    if (index != -1)
    {
        data[index] = value;
    }
}

bool ModbusBoolTable::readBoolRegister(int addres)
{
    int index = indexForAddres(addres);
    if (index == -1)
    {
        return false;
    }

    return data[index];
}

// ModbusMainData******************************
void ModbusMainData::setDataTable(ModbusWorldTable *table)
{
    if (table->table == HOLDING_REGISTR)
    {
        table_4 = table;
        supported[2] = true;
        supported[5] = true;
        supported[7] = true;
        existsTables[3] = true;
        return;
    }
    if (table->table == INPUT_REGISTR)
    {
        table_3 = table;
        supported[3] = true;
        existsTables[2] = true;
        return;
    }
}

void ModbusMainData::setDataTable(ModbusBoolTable *table)
{
    if (table->table == COIL)
    {
        table_0 = table;
        supported[0] = true;
        supported[4] = true;
        supported[6] = true;
        existsTables[0] = true;
        return;
    }
    if (table->table == DISCRETE_INPUT)
    {
        table_1 = table;
        supported[1] = true;
        existsTables[1] = true;
        return;
    }
}

bool ModbusMainData::hasTable(byte table)
{
    switch (table)
    {
    case COIL:
        return existsTables[0];
        break;
    case DISCRETE_INPUT:
        return existsTables[1];
        break;
    case INPUT_REGISTR:
        return existsTables[2];
        break;
    case HOLDING_REGISTR:
        return existsTables[3];
        break;
    default:
        return false;
        break;
    }
    return false;
}

void ModbusMainData::setIntOrder(byte order)
{
    intOrder = order;
}

void ModbusMainData::setLongOrder(byte order)
{
    longOrder = order;
}

void ModbusMainData::setFloatOrder(byte order)
{
    floatOrder = order;
}

void ModbusMainData::setUnsignedlongOrder(byte order)
{
    unsignedlongOrder = order;
}

ModbusTable *ModbusMainData::tableForType(byte table)
{
    if (table == HOLDING_REGISTR)
    {
        return table_4;
    }
    if (table == INPUT_REGISTR)
    {
        return table_3;
    }
    if (table == COIL)
    {
        return table_0;
    }
    if (table == DISCRETE_INPUT)
    {
        return table_1;
    }
    return table_1;
}

int ModbusMainData::indexForAddres(int addr, byte table)
{
    return tableForType(table)->indexForAddres(addr);
}

bool ModbusMainData::checkModbusAddres(int addr, byte table)
{
    return (!((indexForAddres(addr, table)) == -1));
}

bool ModbusMainData::checkModbusRange(int startAddr, int addrNumber, byte table)
{

    for (int i = 0; i < addrNumber; i++)
    {
        if (!(checkModbusAddres((startAddr + i), table)))
        {
            return false;
        }
    }
    return true;
}

void ModbusMainData::saveLong(long value, byte table, int startAddres)
{
    if (!canSaveTable(table))
    {
        return;
    }
    ModbusTable *tableData = tableForType(table);
    uint16_t w1;
    uint16_t w2;
    byte orderArray[4];
    modbusOrderAray(orderArray, longOrder);
    byte b;
    b = *(((byte *)&value) + orderArray[0]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w2, i, bitRead(b, i));
    b = *(((byte *)&value) + orderArray[1]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w2, i + 8, bitRead(b, i));
    b = *(((byte *)&value) + orderArray[2]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w1, i, bitRead(b, i));
    b = *(((byte *)&value) + orderArray[3]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w1, i + 8, bitRead(b, i));
    tableData->setData(startAddres, w1);
    tableData->setData((startAddres + 1), w2);
}

void ModbusMainData::saveUnsignedLong(unsigned long value, byte table, int startAddres)
{
    if (!canSaveTable(table))
    {
        return;
    }
    ModbusTable *tableData = tableForType(table);
    if (!(tableData->hasAdress(startAddres)))
    {
        return;
    }
    uint16_t w1;
    uint16_t w2;
    int index = tableData->indexForAddres(startAddres);
    byte orderArray[4];
    modbusOrderAray(orderArray, unsignedlongOrder);
    byte b;
    b = *(((byte *)&value) + orderArray[0]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w2, i, bitRead(b, i));
    b = *(((byte *)&value) + orderArray[1]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w2, i + 8, bitRead(b, i));
    b = *(((byte *)&value) + orderArray[2]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w1, i, bitRead(b, i));
    b = *(((byte *)&value) + orderArray[3]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w1, i + 8, bitRead(b, i));
    tableData->setData(index, w1);
    tableData->setData((index + 1), w2);
}

void ModbusMainData::saveFloat(float value, byte table, int startAddres)
{
    if (!canSaveTable(table))
    {
        return;
    }
    ModbusTable *tableData = tableForType(table);
    uint16_t w1;
    uint16_t w2;
    byte orderArray[4];
    modbusOrderAray(orderArray, floatOrder);
    byte b;
    b = *(((byte *)&value) + orderArray[0]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w2, i, bitRead(b, i));
    b = *(((byte *)&value) + orderArray[1]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w2, i + 8, bitRead(b, i));
    b = *(((byte *)&value) + orderArray[2]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w1, i, bitRead(b, i));
    b = *(((byte *)&value) + orderArray[3]);
    for (byte i = 0; i < 8; i++)
        bitWrite(w1, i + 8, bitRead(b, i));
    tableData->setData(startAddres, w1);
    tableData->setData((startAddres + 1), w2);
}

void ModbusMainData::saveInteger(int value, byte table, int startAddres)
{

    uint16_t w1;

    if (!canSaveTable(table))
    {
        return;
    }
    ModbusTable *tableData = tableForType(table);
    if (!(tableData->hasAdress(startAddres)))
    {
        return;
    }
    int index = tableData->indexForAddres(startAddres);
    if (intOrder == 1)
    {
        w1 = value;
        tableData->setData(startAddres, w1);
        return;
    }
    w1 = (int(word(lowByte(value), highByte(value))));
    tableData->setData(index, w1);
}

void ModbusMainData::saveByte(byte value, byte table, int startAddres)
{
    if (!canSaveTable(table))
    {
        return;
    }

    ModbusTable *tableData = tableForType(table);
    uint16_t w1 = value;
    tableData->setData(startAddres, w1);
}

void ModbusMainData::saveBool(bool value, byte table, int startAddres)
{
    if (!canSaveTable(table))
    {
        return;
    }
    ModbusTable *tableData = tableForType(table);
    tableData->setData(startAddres, value);
}

byte ModbusMainData::readByte(byte table, int startAddres)
{
    if (!hasTable(table))
    {
        return 0;
    }
    ModbusTable *tableData = tableForType(table);
    return tableData->readWorldRegister(startAddres);
}

int ModbusMainData::readInteger(byte table, int startAddres)
{
    ModbusTable *tableData = tableForType(table);
    if (tableData == 0)
    {
        return 0;
    }

    uint16_t w1 = tableData->readWorldRegister(startAddres);

    if (intOrder == AB_ORDER)
    {
        return w1;
    }

    return (int(word(lowByte(w1), highByte(w1))));
}

float ModbusMainData::readFloat(byte table, int startAddres)
{
    if (!hasTable(table))
    {
        return 0;
    }
    ModbusTable *tableData = tableForType(table);
    byte b[4];
    byte orderArray[4];
    uint16_t w1 = tableData->readWorldRegister(startAddres);
    uint16_t w2 = tableData->readWorldRegister(startAddres + 1);
    sortTwoWordRegistors(orderArray, floatOrder, b, w1, w2);
    return float(*((float *)&b));
}

long ModbusMainData::readLong(byte table, int startAddres)
{
    if (!hasTable(table))
    {
        return 0;
    }
    ModbusTable *tableData = tableForType(table);
    byte b[4];
    byte orderArray[4];
    uint16_t w1 = tableData->readWorldRegister(startAddres);
    uint16_t w2 = tableData->readWorldRegister(startAddres + 1);
    sortTwoWordRegistors(orderArray, longOrder, b, w1, w2);
    return long(*((long *)&b));
}

unsigned long ModbusMainData::readUnsignedLong(byte table, int startAddres)
{
    if (!hasTable(table))
    {
        return 0;
    }
    byte b[4];
    byte orderArray[4];
    ModbusTable *tableData = tableForType(table);
    uint16_t w1 = tableData->readWorldRegister(startAddres);
    uint16_t w2 = tableData->readWorldRegister(startAddres + 1);
    sortTwoWordRegistors(orderArray, unsignedlongOrder, b, w1, w2);
    modbusOrderAray(orderArray, unsignedlongOrder);
    return *((unsigned long *)&b);
}

bool ModbusMainData::readBool(byte table, int startAddres)
{
    if (!hasTable(table))
    {
        return false;
    }

    ModbusTable *tableData = tableForType(table);
    return tableData->readBoolRegister(startAddres);
}

bool ModbusMainData::isSupportFunction(byte function)
{
    if (function == 1)
        return supported[0];
    if (function == 2)
        return supported[1];
    if (function == 3)
        return supported[2];
    if (function == 4)
        return supported[3];
    if (function == 5)
        return supported[4];
    if (function == 6)
        return supported[5];
    if (function == 15)
        return supported[6];
    if (function == 16)
        return supported[7];
    return false;
}

void ModbusMainData::modbusOrderAray(byte orderArray[], byte order)
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

bool ModbusMainData::canSaveTable(byte table)
{
    return (hasTable(table));
}

void ModbusMainData::sortTwoWordRegistors(byte orderArray[], byte order, byte data[], uint16_t w1, uint16_t w2)
{
    modbusOrderAray(orderArray, order);
    for (byte i = 0; i < 8; i++)
        bitWrite(data[orderArray[0]], i, bitRead(w2, i));
    for (byte i = 0; i < 8; i++)
        bitWrite(data[orderArray[1]], i, bitRead(w2, (i + 8)));
    for (byte i = 0; i < 8; i++)
        bitWrite(data[orderArray[2]], i, bitRead(w1, i));
    for (byte i = 0; i < 8; i++)
        bitWrite(data[orderArray[3]], i, bitRead(w1, (i + 8)));
}

// ModbusSlaveInMaster******************************8
byte ModbusSlaveInMaster::getLastError()
{
    return lastError;
}

void ModbusSlaveInMaster::status(bool status)
{
    isActive = status;
}

void ModbusSlaveInMaster::setPollingPeriod(unsigned long period)
{
    pollingPeriod = period;
}

void ModbusSlaveInMaster::setTimeOutTime(unsigned long time)
{
    timeOutTime = time;
}

unsigned long ModbusSlaveInMaster::getTimeOutTime()
{
    return timeOutTime;
}

void ModbusSlaveInMaster::setLastError(byte error)
{
    lastError = error;
}

bool ModbusSlaveInMaster::canSaveTable(byte table)
{
    if (!ModbusMainData::canSaveTable(table))
    {
        return false;
    }
    return ((table == HOLDING_REGISTR) || (table == COIL));
}

bool ModbusSlaveInMaster::hasWriteRegisters()
{

    ModbusTable *table1 = tableForType(HOLDING_REGISTR);
    if (table1 != 0)
    {

        if (table1->firstWriteAddress() != -1)
        {
            return true;
        }
    }
    table1 = tableForType(COIL);

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

    ModbusTable *table1 = tableForType(HOLDING_REGISTR);
    if (table1 != 0)
    {

        if (table1->firstWriteAddress() != -1)
        {
            return table1;
        }
    }
    table1 = tableForType(COIL);

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
    if (hasTable(COIL))
    {

        return tableForType(COIL);
    }
    if (hasTable(DISCRETE_INPUT))
    {

        return tableForType(DISCRETE_INPUT);
    }
    if (hasTable(INPUT_REGISTR))
    {

        return tableForType(INPUT_REGISTR);
    }
    if (hasTable(HOLDING_REGISTR))
    {

        return tableForType(HOLDING_REGISTR);
    }
    return 0;
}

ModbusTable *ModbusSlaveInMaster::nextTable(ModbusTable *table)
{
    if (table->table == HOLDING_REGISTR)
    {
        return 0;
    }
    if (table == 0)
    {
        return firstTabe();
    }

    byte code = nextTable(table->table);
    if (code == -1)
    {
        return 0;
    }
    return tableForType(code);
}

int ModbusSlaveInMaster::nextTable(int table)
{
    int result = -1;
    if (table == COIL)
    {
        result = DISCRETE_INPUT;
    }
    if (table == DISCRETE_INPUT)
    {
        result = INPUT_REGISTR;
    }
    if (table == INPUT_REGISTR)
    {
        result = HOLDING_REGISTR;
    }
    if (table == HOLDING_REGISTR)
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

void ModbusSlaveInMaster::setSlaveAddress(byte address)
{
    slaveAddres = address;
}
