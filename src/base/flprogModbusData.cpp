#include "flprogModbusData.h"

//------------------- ModbusTable--------------------------------------

// Конструкторы совместимы с существующим API FLProg. Если адреса переданы
// пользователем, таблица использует их без освобождения. Служебные массивы
// значений и флагов всегда принадлежат таблице.
ModbusTable::ModbusTable(uint8_t table, int32_t dataSize, int32_t startAddress)
{
  _table = table;
  init(dataSize, startAddress);
}

ModbusTable::ModbusTable(uint8_t table, int32_t dataSize, int32_t *adresses)
{
  _table = table;
  init(dataSize, adresses);
}

void ModbusTable::clearData()
{
  delete[] _worldData;
  delete[] _boolData;
  delete[] _sendRegisters;
  _worldData = 0;
  _boolData = 0;
  _sendRegisters = 0;
}

void ModbusTable::clearAll()
{
  clearData();
  if (_ownsAddresses)
  {
    delete[] _adresses;
  }
  _adresses = 0;
  _ownsAddresses = false;
  _tableSize = 0;
  _minAdress = -1;
  _maxAdress = -1;
}

ModbusTable::~ModbusTable()
{
  clearAll();
}

ModbusTable::ModbusTable(const ModbusTable &other)
{
  *this = other;
}

ModbusTable &ModbusTable::operator=(const ModbusTable &other)
{
  if (this == &other)
  {
    return *this;
  }
  clearAll();
  _table = other._table;
  _newDataCallback = other._newDataCallback;
  _tableSize = other._tableSize;
  _minAdress = other._minAdress;
  _maxAdress = other._maxAdress;
  if (_tableSize > 0)
  {
    _adresses = new int32_t[_tableSize];
    _ownsAddresses = true;
    for (int32_t i = 0; i < _tableSize; ++i)
    {
      _adresses[i] = other._adresses ? other._adresses[i] : i;
    }
    if (other._worldData)
    {
      _worldData = new int16_t[_tableSize];
      for (int32_t i = 0; i < _tableSize; ++i) _worldData[i] = other._worldData[i];
    }
    if (other._boolData)
    {
      _boolData = new bool[_tableSize];
      for (int32_t i = 0; i < _tableSize; ++i) _boolData[i] = other._boolData[i];
    }
    _sendRegisters = new bool[_tableSize];
    for (int32_t i = 0; i < _tableSize; ++i)
      _sendRegisters[i] = other._sendRegisters ? other._sendRegisters[i] : false;
  }
  return *this;
}

void ModbusTable::init(int32_t dataSize, int32_t *adresses)
{
  clearAll();
  // Нулевой/отрицательный размер даёт корректную пустую таблицу.
  // При nullptr сохраняем старое поведение: последовательные адреса от нуля.
  if (dataSize <= 0)
  {
    return;
  }
  _adresses = adresses;
  _ownsAddresses = false;
  init(dataSize);
}

void ModbusTable::init(int32_t dataSize, int32_t startAddress)
{
  clearAll();
  if (dataSize <= 0)
  {
    return;
  }
  _adresses = new int32_t[dataSize];
  _ownsAddresses = true;
  for (int32_t i = 0; i < dataSize; ++i)
  {
    _adresses[i] = startAddress + i;
  }
  init(dataSize);
}

void ModbusTable::init(int32_t dataSize)
{
  if (dataSize <= 0)
  {
    clearAll();
    return;
  }
  // Если размер увеличился, существующего массива адресов может не хватить.
  // Для внешнего массива создаём независимую копию: внешний не освобождаем.
  if ((dataSize > _tableSize) && (_tableSize > 0) && (_adresses != 0))
  {
    int32_t *newAddresses = new int32_t[dataSize];
    for (int32_t i = 0; i < dataSize; ++i)
      newAddresses[i] = (i < _tableSize) ? _adresses[i] :
          (_adresses[_tableSize - 1] + (i - _tableSize + 1));
    if (_ownsAddresses) delete[] _adresses;
    _adresses = newAddresses;
    _ownsAddresses = true;
  }
  clearData();
  _minAdress = -1;
  _maxAdress = -1;
  _tableSize = dataSize > 0 ? dataSize : 0;
  if (_tableSize == 0)
  {
    return;
  }
  if (_adresses == 0)
  {
    _adresses = new int32_t[_tableSize];
    _ownsAddresses = true;
    for (int32_t i = 0; i < _tableSize; ++i) _adresses[i] = i;
  }
  if ((_table == FLPROG_INPUT_REGISTR) || (_table == FLPROG_HOLDING_REGISTR))
  {
    _worldData = new int16_t[_tableSize]();
  }
  else
  {
    _boolData = new bool[_tableSize]();
  }
  _sendRegisters = new bool[_tableSize]();
}

void ModbusTable::setData(int32_t address, bool value)
{
  if ((_table == FLPROG_HOLDING_REGISTR) || (_table == FLPROG_INPUT_REGISTR))
  {
    return;
  }
  setDataByIndex((indexForAddres(address)), value);
}

void ModbusTable::setData(int32_t address, int16_t value)
{
  if ((_table == FLPROG_COIL) || (_table == FLPROG_DISCRETE_INPUT))
  {
    return;
  }
  setDataByIndex((indexForAddres(address)), value);
}

void ModbusTable::setDataByIndex(int32_t addressIndex, bool value)
{
  if ((_table == FLPROG_HOLDING_REGISTR) || (_table == FLPROG_INPUT_REGISTR))
  {
    return;
  }
  if (addressIndex < 0)
  {
    return;
  }
  if (addressIndex >= _tableSize)
  {
    return;
  }
  if (value == _boolData[addressIndex])
  {
    return;
  }
  if (_sendRegisters != 0)
  {
    _sendRegisters[addressIndex] = true;
  }
  _boolData[addressIndex] = value;
  if (_newDataCallback != 0)
  {
    uint16_t temp = 0;
    if (value)
    {
      temp = 1;
    }
    _newDataCallback(_table, addressIndex, temp);
  }
}

void ModbusTable::setDataByIndex(int32_t addressIndex, int16_t value)
{
  if ((_table == FLPROG_COIL) || (_table == FLPROG_DISCRETE_INPUT))
  {
    return;
  }
  if (addressIndex < 0)
  {
    return;
  }
  if (addressIndex >= _tableSize)
  {
    return;
  }
  if (value == _worldData[addressIndex])
  {
    return;
  }
  if (_sendRegisters != 0)
  {
    _sendRegisters[addressIndex] = true;
  }
  _worldData[addressIndex] = value;
  if (_newDataCallback != 0)
  {
    _newDataCallback(_table, addressIndex, value);
  }
}

void ModbusTable::writeRegister(int32_t address, bool value)
{
  if ((_table == FLPROG_HOLDING_REGISTR) || (_table == FLPROG_INPUT_REGISTR))
  {
    return;
  }
  int32_t index = indexForAddres(address);
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
      _newDataCallback(_table, index, temp);
    }
  }
}

void ModbusTable::writeRegister(int32_t address, int16_t value)
{
  if ((_table == FLPROG_COIL) || (_table == FLPROG_DISCRETE_INPUT))
  {
    return;
  }
  int32_t index = indexForAddres(address);
  if (index != -1)
  {
    if (_worldData[index] == value)
    {
      return;
    }
    _worldData[index] = value;
    if (_newDataCallback != 0)
    {
      _newDataCallback(_table, index, value);
    }
  }
}

int16_t ModbusTable::readWorldRegister(int32_t address)
{
  if ((_table == FLPROG_COIL) || (_table == FLPROG_DISCRETE_INPUT))
  {
    return 0;
  }
  return readWorldRegisterByIndex(indexForAddres(address));
}

bool ModbusTable::readBoolRegister(int32_t address)
{
  if ((_table == FLPROG_HOLDING_REGISTR) || (_table == FLPROG_INPUT_REGISTR))
  {
    return false;
  }
  return readBoolRegisterByIndex(indexForAddres(address));
}

int16_t ModbusTable::readWorldRegisterByIndex(int32_t addressIndex)
{
  if ((_table == FLPROG_COIL) || (_table == FLPROG_DISCRETE_INPUT))
  {
    return 0;
  }
  if (addressIndex >= _tableSize)
  {
    return false;
  }
  if (addressIndex < 0)
  {
    return false;
  }
  return _worldData[addressIndex];
}

bool ModbusTable::readBoolRegisterByIndex(int32_t addressIndex)
{
  if ((_table == FLPROG_HOLDING_REGISTR) || (_table == FLPROG_INPUT_REGISTR))
  {
    return false;
  }
  if (addressIndex >= _tableSize)
  {
    return false;
  }
  if (addressIndex < 0)
  {
    return false;
  }
  return _boolData[addressIndex];
}

int32_t ModbusTable::indexForAddres(int32_t address)
{
  for (int32_t i = 0; i < _tableSize; i++)
  {
    if (adressForIndex(i) == address)
    {
      return i;
    }
  }
  return -1;
}

int32_t ModbusTable::adressForIndex(int32_t index)
{
  if (index < 0)
  {
    return -1;
  }
  if (index >= _tableSize)
  {
    return -1;
  }
  return _adresses[index];
}

void ModbusTable::adressForIndex(int32_t index, int32_t address)
{
  if (index < 0)
  {
    return;
  }
  if (index >= _tableSize)
  {
    return;
  }
  _minAdress = -1;
  _maxAdress = -1;
  _adresses[index] = address;
}

int32_t ModbusTable::firstWriteAddress()
{
  if (_sendRegisters == 0)
  {
    return -1;
  }
  for (int32_t i = 0; i < _tableSize; i++)
  {
    if (_sendRegisters[i])
    {
      return adressForIndex(i);
    }
  }
  return -1;
}

int16_t ModbusTable::writeRegsSize(int32_t startAddress)
{
  return recursiveWriteRegsSize(startAddress, 1);
}

int16_t ModbusTable::recursiveWriteRegsSize(int32_t startAddress, int16_t result)
{
  // Имя оставлено для совместимости, внутри теперь цикл без глубокой рекурсии.
  if (!hasAdress(startAddress)) return 0;
  int16_t count = result;
  while (count < 25)
  {
    const int32_t next = startAddress + count;
    const int32_t index = indexForAddres(next);
    if ((index < 0) || !_sendRegisters[index]) break;
    ++count;
  }
  return count;
}

int16_t ModbusTable::readRegsSize(int32_t startAddress)
{
  return recursiveReadRegsSize(startAddress, 1);
}

int16_t ModbusTable::recursiveReadRegsSize(int32_t startAddress, int16_t result)
{
  if (!hasAdress(startAddress)) return 0;
  int16_t count = result;
  while ((count < 25) && hasAdress(startAddress + count)) ++count;
  return count;
}

int32_t ModbusTable::findNextStartAddres(int32_t address)
{
  // На редких адресах прежняя рекурсия могла потребовать десятки тысяч
  // вызовов и переполнить стек микроконтроллера.
  int32_t next = -1;
  for (int32_t i = 0; i < _tableSize; ++i)
  {
    const int32_t candidate = _adresses[i];
    if ((candidate > address) && ((next == -1) || (candidate < next)))
      next = candidate;
  }
  return next;
}

int32_t ModbusTable::getMinAdress()
{
  if (_tableSize == 0) return -1;
  if (_minAdress >= 0) return _minAdress;
  int32_t result = _adresses[0];
  for (int32_t i = 1; i < _tableSize; ++i)
    if (_adresses[i] < result) result = _adresses[i];
  _minAdress = result;
  return result;
}

int32_t ModbusTable::getMaxAdress()
{
  if (_tableSize == 0) return -1;
  if (_maxAdress >= 0) return _maxAdress;
  int32_t result = _adresses[0];
  for (int32_t i = 1; i < _tableSize; ++i)
    if (_adresses[i] > result) result = _adresses[i];
  _maxAdress = result;
  return result;
}

void ModbusTable::resetWriteFlag(int32_t address)
{
  int32_t index = indexForAddres(address);
  if (index < 0)
  {
    return;
  }
  if (_sendRegisters != 0) _sendRegisters[index] = false;
}

bool ModbusTable::hasAdress(int32_t address)
{
  return ((indexForAddres(address)) >= 0);
}

// ******************ModbusMainData******************************
void ModbusMainData::setDataTable(ModbusTable *table)
{
  if (table == 0) return;
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

void ModbusMainData::configDataTable(uint8_t _table, int32_t dataSize, int32_t startAddress)
{
  ModbusTable *table = tableForType(_table);
  if (table == 0)
  {
    setDataTable(new ModbusTable(_table, dataSize, startAddress));
    return;
  }
  table->init(dataSize, startAddress);
}

void ModbusMainData::setDataTable(uint8_t _table, int32_t dataSize, int32_t *_adresses)
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

int32_t ModbusMainData::indexForAddres(int32_t address, uint8_t table)
{
  ModbusTable *data = tableForType(table);
  return data ? data->indexForAddres(address) : -1;
}

bool ModbusMainData::checkModbusAddres(int32_t address, uint8_t table)
{
  if (!hasTable(table))
  {
    return false;
  }
  return (!((indexForAddres(address, table)) == -1));
}

bool ModbusMainData::checkModbusRange(int32_t startAddr, int32_t addrNumber, uint8_t table)
{
  if ((addrNumber <= 0) || (startAddr < 0) || (startAddr > 65535) ||
      (addrNumber > (65536 - startAddr))) return false;
  for (int32_t i = 0; i < addrNumber; i++)
  {
    if (!(checkModbusAddres((startAddr + i), table)))
    {
      return false;
    }
  }
  return true;
}

ModbusTable *ModbusMainData::tableForStartArddres(uint8_t table, int32_t startAddress, bool isTwoWord)
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
  if (tableData->hasAdress(startAddress))
  {
    if (isTwoWord)
    {
      if (tableData->hasAdress(startAddress + 1))
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

int32_t ModbusMainData::getAdress(uint8_t table, int32_t index)
{
  if (!hasTable(table))
  {
    return -1;
  }
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return -1;
  }
  return tableData->adressForIndex(index);
}

void ModbusMainData::setAdress(uint8_t table, int32_t index, int32_t address)
{
  if (!hasTable(table))
  {
    return;
  }
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return;
  }
  tableData->adressForIndex(index, address);
}

void ModbusMainData::saveLong(int32_t value, uint8_t table, int32_t startAddress)
{
  unsigned char sourse[4];
  memcpy(sourse, &value, 4);
  saveForByteWithOrder(sourse, table, startAddress, _longOrder);
}

void ModbusMainData::saveUnsignedLong(uint32_t value, uint8_t table, int32_t startAddress)
{
  unsigned char sourse[4];
  memcpy(sourse, &value, 4);
  saveForByteWithOrder(sourse, table, startAddress, _unsignedlongOrder);
}

void ModbusMainData::saveFloat(float value, uint8_t table, int32_t startAddress)
{
  unsigned char sourse[4];
  memcpy(sourse, &value, 4);
  saveForByteWithOrder(sourse, table, startAddress, _floatOrder);
}

void ModbusMainData::saveInteger(int16_t value, uint8_t table, int32_t startAddress)
{
  if (!canSaveTable(table))
  {
    return;
  }
  ModbusTable *tableData = tableForStartArddres(table, startAddress, false);
  if (tableData == 0)
  {
    return;
  }
  int16_t w1 = value;
  if (_intOrder != 1)
  {
    w1 = (int16_t(word(lowByte(value), highByte(value))));
  }
  tableData->setData(startAddress, w1);
}

void ModbusMainData::saveByte(uint8_t value, uint8_t table, int32_t startAddress)
{
  if (!canSaveTable(table))
  {
    return;
  }
  ModbusTable *tableData = tableForStartArddres(table, startAddress, false);
  if (tableData == 0)
  {
    return;
  }
  int16_t w1 = value;
  tableData->setData(startAddress, w1);
}

void ModbusMainData::saveBool(bool value, uint8_t table, int32_t startAddress)
{
  if (!canSaveTable(table))
  {
    return;
  }
  ModbusTable *tableData = tableForStartArddres(table, startAddress, false);
  if (tableData == 0)
  {
    return;
  }
  tableData->setData(startAddress, value);
}

void ModbusMainData::saveLongByIndex(int32_t value, uint8_t table, int32_t startAddress)
{
  unsigned char sourse[4];
  memcpy(sourse, &value, 4);
  saveForByteWithOrderByIndex(sourse, table, startAddress, _longOrder);
}

void ModbusMainData::saveUnsignedLongByIndex(uint32_t value, uint8_t table, int32_t startAddressIndex)
{
  unsigned char sourse[4];
  memcpy(sourse, &value, 4);
  saveForByteWithOrderByIndex(sourse, table, startAddressIndex, _unsignedlongOrder);
}

void ModbusMainData::saveFloatByIndex(float value, uint8_t table, int32_t startAddressIndex)
{
  unsigned char sourse[4];
  memcpy(sourse, &value, 4);
  saveForByteWithOrderByIndex(sourse, table, startAddressIndex, _floatOrder);
}

void ModbusMainData::saveIntegerByIndex(int16_t value, uint8_t table, int32_t startAddressIndex)
{
  if (!canSaveTable(table))
  {
    return;
  }
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return;
  }
  int16_t w1 = value;
  if (_intOrder != 1)
  {
    w1 = (int16_t(word(lowByte(value), highByte(value))));
  }
  tableData->setDataByIndex(startAddressIndex, w1);
}

void ModbusMainData::saveByteByIndex(uint8_t value, uint8_t table, int32_t startAddressIndex)
{
  if (!canSaveTable(table))
  {
    return;
  }
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return;
  }
  int16_t w1 = value;
  tableData->setDataByIndex(startAddressIndex, w1);
}

void ModbusMainData::saveBoolByIndex(bool value, uint8_t table, int32_t startAddress)
{
  if (!canSaveTable(table))
  {
    return;
  }
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return;
  }
  tableData->setDataByIndex(startAddress, value);
}

void ModbusMainData::saveForByteWithOrder(unsigned char *sourse, uint8_t table, int32_t startAddress, uint8_t order)
{
  if (!canSaveTable(table))
  {
    return;
  }
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return;
  }
  int16_t temp;
  if (order == 1) //(ABCD)
  {
    temp = int16_t(word(sourse[3], sourse[2]));
    tableData->setData(startAddress, temp);
    temp = int16_t(word(sourse[1], sourse[0]));
    tableData->setData((startAddress + 1), temp);
  }
  if (order == 2) //(CDAB)
  {
    temp = int16_t(word(sourse[1], sourse[0]));
    tableData->setData(startAddress, temp);
    temp = int16_t(word(sourse[3], sourse[2]));
    tableData->setData((startAddress + 1), temp);
  }
  if (order == 3) //(BADC)
  {
    temp = int16_t(word(sourse[2], sourse[3]));
    tableData->setData(startAddress, temp);
    temp = int16_t(word(sourse[0], sourse[1]));
    tableData->setData((startAddress + 1), temp);
  }
  if (order == 4) //(DCBA)
  {
    temp = int16_t(word(sourse[0], sourse[1]));
    tableData->setData(startAddress, temp);
    temp = int16_t(word(sourse[2], sourse[3]));
    tableData->setData((startAddress + 1), temp);
  }
}

void ModbusMainData::saveForByteWithOrderByIndex(unsigned char *sourse, uint8_t table, int32_t startAddressIndex, uint8_t order)
{
  if (!canSaveTable(table))
  {
    return;
  }
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return;
  }
  int16_t temp;
  if (order == 1) //(ABCD)
  {
    temp = int16_t(word(sourse[3], sourse[2]));
    tableData->setDataByIndex(startAddressIndex, temp);
    temp = int16_t(word(sourse[1], sourse[0]));
    tableData->setDataByIndex((startAddressIndex + 1), temp);
  }
  if (order == 2) //(CDAB)
  {
    temp = int16_t(word(sourse[1], sourse[0]));
    tableData->setDataByIndex(startAddressIndex, temp);
    temp = int16_t(word(sourse[3], sourse[2]));
    tableData->setDataByIndex((startAddressIndex + 1), temp);
  }
  if (order == 3) //(BADC)
  {
    temp = int16_t(word(sourse[2], sourse[3]));
    tableData->setDataByIndex(startAddressIndex, temp);
    temp = int16_t(word(sourse[0], sourse[1]));
    tableData->setDataByIndex((startAddressIndex + 1), temp);
  }
  if (order == 4) //(DCBA)
  {
    temp = int16_t(word(sourse[0], sourse[1]));
    tableData->setDataByIndex(startAddressIndex, temp);
    temp = int16_t(word(sourse[2], sourse[3]));
    tableData->setDataByIndex((startAddressIndex + 1), temp);
  }
}

uint8_t ModbusMainData::readByte(uint8_t table, int32_t startAddress)
{
  ModbusTable *tableData = tableForStartArddres(table, startAddress, false);
  if (tableData == 0)
  {
    return 0;
  }
  return tableData->readWorldRegister(startAddress);
}

int16_t ModbusMainData::readInteger(uint8_t table, int32_t startAddress)
{
  ModbusTable *tableData = tableForStartArddres(table, startAddress, false);
  if (tableData == 0)
  {
    return 0;
  }
  int16_t w1 = tableData->readWorldRegister(startAddress);
  if (_intOrder == FLPROG_AB_ORDER)
  {
    return w1;
  }
  return (int16_t(word(lowByte(w1), highByte(w1))));
}

float ModbusMainData::readFloat(uint8_t table, int32_t startAddress)
{
  float result;
  unsigned char sourse[4];
  readForByteWithOrder(sourse, table, startAddress, _floatOrder);
  memcpy(&result, sourse, 4);
  return result;
}

int32_t ModbusMainData::readLong(uint8_t table, int32_t startAddress)
{
  int32_t result;
  unsigned char sourse[4];
  readForByteWithOrder(sourse, table, startAddress, _longOrder);
  memcpy(&result, sourse, 4);
  return result;
}

uint32_t ModbusMainData::readUnsignedLong(uint8_t table, int32_t startAddress)
{
  uint32_t result;
  unsigned char sourse[4];
  readForByteWithOrder(sourse, table, startAddress, _unsignedlongOrder);
  memcpy(&result, sourse, 4);
  return result;
}

void ModbusMainData::readForByteWithOrder(unsigned char *sourse, uint8_t table, int32_t startAddress, uint8_t order)
{
  ModbusTable *tableData = tableForStartArddres(table, startAddress, true);
  if (tableData == 0)
  {
    return;
  }
  int16_t temp = tableData->readWorldRegister(startAddress);
  int16_t temp1 = tableData->readWorldRegister(startAddress + 1);
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

void ModbusMainData::readForByteWithOrderByIndex(unsigned char *sourse, uint8_t table, int32_t startAddress, uint8_t order)
{
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return;
  }
  int16_t temp = tableData->readWorldRegisterByIndex(startAddress);
  int16_t temp1 = tableData->readWorldRegisterByIndex(startAddress + 1);
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

bool ModbusMainData::readBool(uint8_t table, int32_t startAddress)
{
  ModbusTable *tableData = tableForStartArddres(table, startAddress, false);
  if (tableData == 0)
  {
    return 0;
  }
  return tableData->readBoolRegister(startAddress);
}

uint8_t ModbusMainData::readByteByIndex(uint8_t table, int32_t startAddressIndex)
{
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return 0;
  }
  return tableData->readWorldRegisterByIndex(startAddressIndex);
}

int16_t ModbusMainData::readIntegerByIndex(uint8_t table, int32_t startAddressIndex)
{
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return 0;
  }
  int16_t w1 = tableData->readWorldRegisterByIndex(startAddressIndex);
  if (_intOrder == FLPROG_AB_ORDER)
  {
    return w1;
  }
  return (int16_t(word(lowByte(w1), highByte(w1))));
}

float ModbusMainData::readFloatByIndex(uint8_t table, int32_t startAddressIndex)
{
  float result;
  unsigned char sourse[4];
  readForByteWithOrderByIndex(sourse, table, startAddressIndex, _floatOrder);
  memcpy(&result, sourse, 4);
  return result;
}

int32_t ModbusMainData::readLongByIndex(uint8_t table, int32_t startAddressIndex)
{
  int32_t result;
  unsigned char sourse[4];
  readForByteWithOrderByIndex(sourse, table, startAddressIndex, _longOrder);
  memcpy(&result, sourse, 4);
  return result;
}

uint32_t ModbusMainData::readUnsignedLongByIndex(uint8_t table, int32_t startAddressIndex)
{
  uint32_t result;
  unsigned char sourse[4];
  readForByteWithOrderByIndex(sourse, table, startAddressIndex, _unsignedlongOrder);
  memcpy(&result, sourse, 4);
  return result;
}

bool ModbusMainData::readBoolByIndex(uint8_t table, int32_t startAddressIndex)
{
  ModbusTable *tableData = tableForType(table);
  if (tableData == 0)
  {
    return 0;
  }
  return tableData->readBoolRegisterByIndex(startAddressIndex);
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

void ModbusMainData::setWorkPause(uint32_t time)
{
  _startWorkPauseTime = millis();
  _workPause = time;
  _isWorkPause = true;
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
  if (_isWorkPause)
  {
    if (flprog::isTimer(_startWorkPauseTime, _workPause))
    {
      _isWorkPause = false;
    }
    else
    {
      return false;
    }
  }
  if (!_isActive)
  {
    return false;
  }

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
  if (_isWorkPause)
  {
    if (flprog::isTimer(_startWorkPauseTime, _workPause))
    {
      _isWorkPause = false;
    }
    else
    {
      return false;
    }
  }
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
