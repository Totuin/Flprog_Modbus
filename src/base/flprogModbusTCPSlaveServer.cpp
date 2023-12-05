#include "flprogModbusTCPSlaveServer.h"

void ModbusTCPSlaveServer::setSlaves(ModbusSlaveInMaster *_table, uint8_t size)
{
    slaves = _table;
    slavesSize = size;
}

void ModbusTCPSlaveServer::setMode(uint8_t mode)
{
    if (mode > 1)
    {
        return;
    }
    if (_mode == mode)
    {
        return;
    }
    _mode = mode;
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