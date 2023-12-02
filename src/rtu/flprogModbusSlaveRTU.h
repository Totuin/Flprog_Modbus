#pragma once
#include "flprogModbusRTU.h"

class ModbusSlaveRTU : public ModbusRTU
{
public:
    ModbusSlaveRTU(){};
    ModbusSlaveRTU(uint8_t number) { _uartPortNumber = number; };
    void begin();
    void pool();
    void setSlaveAddress(uint8_t adr) { _slaveAddres = adr; };
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

    ModbusMainData *mainData();

protected:
    uint8_t _slaveAddres = 1;
    virtual uint8_t validateRequest();

private:
    ModbusMainData *_data;
};
