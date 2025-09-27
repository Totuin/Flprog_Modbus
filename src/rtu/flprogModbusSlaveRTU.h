#pragma once
#include "flprogModbusRTU.h"

class ModbusSlaveRTU : public ModbusRTU
{
public:
  ModbusSlaveRTU() {};
  ModbusSlaveRTU(uint8_t number, FlprogAbstractUartExecutor *executor);
  void begin();
  void pool();
  void setSlaveAddress(uint8_t adr) { _slaveAddres = adr; };
  uint8_t slaveAddress() { return _slaveAddres; };
  void setData(ModbusMainData *data) { _data = data; };

  void setDataTable(ModbusTable *table) { mainData()->setDataTable(table); };
  void setDataTable(uint8_t _table, uint16_t dataSize, int *_adresses) { mainData()->setDataTable(_table, dataSize, _adresses); };
  void configDataTable(uint8_t _table, uint16_t dataSize, int16_t _startAdr = 0) { mainData()->configDataTable(_table, dataSize, _startAdr); };

  void saveLong(int32_t val, uint8_t table, int adr) { mainData()->saveLong(val, table, adr); };
  void saveUnsignedLong(uint32_t val, uint8_t table, int adr) { mainData()->saveUnsignedLong(val, table, adr); };
  void saveFloat(float val, uint8_t table, int adr) { mainData()->saveFloat(val, table, adr); };
  void saveInteger(int val, uint8_t table, int adr) { mainData()->saveInteger(val, table, adr); };
  void saveByte(uint8_t val, uint8_t table, int adr) { mainData()->saveByte(val, table, adr); };
  void saveBool(bool val, uint8_t table, int adr) { mainData()->saveBool(val, table, adr); };

  uint8_t readByte(uint8_t table, int adr) { return mainData()->readByte(table, adr); };
  int readInteger(uint8_t table, int adr) { return mainData()->readInteger(table, adr); };
  float readFloat(uint8_t table, int adr) { return mainData()->readFloat(table, adr); };
  int32_t readLong(uint8_t table, int adr) { return mainData()->readLong(table, adr); };
  uint32_t readUnsignedLong(uint8_t table, int adr) { return mainData()->readUnsignedLong(table, adr); };
  bool readBool(uint8_t table, int adr) { return mainData()->readBool(table, adr); };

  void setLongOrder(uint8_t order) { mainData()->setLongOrder(order); };
  void setFloatOrder(uint8_t order) { mainData()->setFloatOrder(order); };
  void setUnsignedlongOrder(uint8_t order) { mainData()->setUnsignedlongOrder(order); };
  void setIntOrder(uint8_t order) { mainData()->setIntOrder(order); };

  uint8_t intOrder() { return mainData()->intOrder(); };
  uint8_t longOrder() { return mainData()->longOrder(); };
  uint8_t floatOrder() { return mainData()->floatOrder(); };
  uint8_t unsignedlongOrder() { return mainData()->unsignedlongOrder(); };

  int16_t getAdress(uint8_t table, int16_t index) { return mainData()->getAdress(table, index); };
  void setAdress(uint8_t table, int16_t index, int16_t addr) { mainData()->setAdress(table, index, addr); };

  ModbusMainData *mainData();
  void setCallBack(FLProgModbusNewDataCallback func);

protected:
  uint8_t _slaveAddres = 1;
  virtual uint8_t validateRequest();
  virtual uint16_t rtuPacadgeSize(uint16_t length, uint8_t bufferArray[]);

private:
  ModbusMainData *_data;
};
