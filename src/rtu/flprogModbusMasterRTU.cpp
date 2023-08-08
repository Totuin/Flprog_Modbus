#include "flprogModbusMasterRTU.h"

ModbusMasterRTU::ModbusMasterRTU(ModbusSlaveInMaster *table, uint8_t size)
{
    slavesSize = size;
    slaves = table;
}

ModbusMasterRTU::ModbusMasterRTU(FLProgUartBasic *uartPort, ModbusSlaveInMaster *table, uint8_t size)
{
    setUart(uartPort);
    slavesSize = size;
    slaves = table;
}

ModbusMasterRTU::ModbusMasterRTU(uint8_t portNumber, ModbusSlaveInMaster *table, uint8_t size)
{
    setUart(new FLProgUart(portNumber));
    slavesSize = size;
    slaves = table;
}

ModbusMasterRTU::ModbusMasterRTU(uint8_t size)
{
    slavesSize = size;
    slaves = new ModbusSlaveInMaster[slavesSize];
}

ModbusMasterRTU::ModbusMasterRTU(FLProgUartBasic *uartPort, uint8_t size)
{
    slavesSize = size;
    slaves = new ModbusSlaveInMaster[slavesSize];
    setUart(uartPort);
}

ModbusMasterRTU::ModbusMasterRTU(uint8_t portNumber, uint8_t size)
{
    slavesSize = size;
    slaves = new ModbusSlaveInMaster[slavesSize];
    setUart(new FLProgUart(portNumber));
}

void ModbusMasterRTU::begin()
{
    uardDevice()->begin();
    if (!(pinPeDe < 0))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
}

ModbusSlaveInMaster *ModbusMasterRTU::slave(uint8_t adr)
{
    for (uint8_t i = 0; i < slavesSize; i++)
    {
        if (slaves[i].slaveAddres() == adr)
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

    if (workStatus == FLPROG_MODBUS_WAITING_SENDING)
    {
        if (flprog::isTimer(startSendTime, timeOfSend))
        {
            workStatus = FLPROG_MODBUS_WAITING_ANSWER;
            startSendTime = millis();
            offPeDePin();
        }
        else
        {
            return;
        }
    }

    if (workStatus == FLPROG_MODBUS_WAITING_ANSWER)
    {
        checkAnswer();
    }
    if (workStatus == FLPROG_MODBUS_READY)
    {
        nextQuery();
    }
}

void ModbusMasterRTU::checkAnswer()
{
    if (flprog::isTimer(startSendTime, (telegrammSlave->getTimeOutTime())))
    {
        telegrammSlave->setLastError(244);
        workStatus = FLPROG_MODBUS_READY;
        return;
    }
    if (!(checkAvaliblePacage()))
        return;
    uint8_t state = rxBuffer();
    if (state < 5)
    {
        telegrammSlave->setLastError(255);
        workStatus = FLPROG_MODBUS_READY;
        return;
    }
    uint8_t exception = validateRequest();
    if (exception > 0)
    {
        telegrammSlave->setLastError(exception);
        workStatus = FLPROG_MODBUS_READY;
        return;
    }
    telegrammSlave->setLastError(0);
    writeMaserData(telegrammTable, telegrammStartAddres, telegrammNumbeRegs);
}

uint8_t ModbusMasterRTU::validateRequest()
{
    int16_t msgCRC = ((buffer[bufferSize - 2] << 8) | buffer[bufferSize - 1]);
    if (flprogModus::modbusCalcCRC(bufferSize - 2, buffer) != msgCRC)
    {
        return 255;
    }
    if ((buffer[1] & 0x80) != 0)
    {
        return buffer[2];
    }
    if (!(telegrammSlave->isSupportFunction(buffer[1])))
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
        if (slaves[i].slaveAddres() == adress)
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
    telegrammSlave = currentSlave;
    telegrammTable = currentSlaveTable;
    telegrammStartAddres = currentSlaveStartAddress;
    telegrammNumbeRegs = telegrammTable->readRegsSize(telegrammStartAddres);
    currentSlaveLastAddress = telegrammStartAddres + telegrammNumbeRegs - 1;
    if (currentSlaveTable->tableType() == FLPROG_COIL)
    {
        telegrammFunction = 1;
    }
    if (telegrammTable->tableType() == FLPROG_DISCRETE_INPUT)
    {
        telegrammFunction = 2;
    }
    if (telegrammTable->tableType() == FLPROG_INPUT_REGISTR)
    {
        telegrammFunction = 4;
    }
    if (telegrammTable->tableType() == FLPROG_HOLDING_REGISTR)
    {
        telegrammFunction = 3;
    }
    return true;
}

bool ModbusMasterRTU::createWriteTelegramm()
{
    telegrammSlave = firstWriteSlave();
    telegrammTable = telegrammSlave->firstWriteTable();
    telegrammStartAddres = telegrammTable->firstWriteAddress();
    telegrammNumbeRegs = telegrammTable->writeRegsSize(telegrammStartAddres);
    if (telegrammTable->tableType() == FLPROG_HOLDING_REGISTR)
    {
        if (telegrammNumbeRegs == 1)
        {
            telegrammFunction = 6;
        }
        else
        {
            telegrammFunction = 16;
        }
    }
    else
    {
        if (telegrammNumbeRegs == 1)
        {
            telegrammFunction = 5;
        }
        else
        {
            telegrammFunction = 15;
        }
    }
    for (uint8_t i = 0; i < telegrammNumbeRegs; i++)
    {
        telegrammTable->resetWriteFlag(telegrammStartAddres + i);
    }

    return true;
}

void ModbusMasterRTU::sendQuery()
{
    buffer[0] = telegrammSlave->slaveAddres();
    buffer[1] = telegrammFunction;
    create_PDU(telegrammTable, telegrammStartAddres, telegrammNumbeRegs);
}

bool ModbusMasterRTU::nextStep()
{
    if (!hasReadySlave())
    {
        return false;
    }
    if (currentSlave == 0)
    {
        currentSlave = firstReadySlave();
        currentSlaveTable = currentSlave->firstTabe();
        currentSlaveStartAddress = currentSlaveTable->getMinAdress();
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
    currentSlaveStartAddress = currentSlaveTable->findNextStartAddres(currentSlaveLastAddress);
    if (currentSlaveStartAddress == -1)
    {
        return nextTable();
    }
    return true;
}

bool ModbusMasterRTU::nextSlave()
{
    if (currentSlave != 0)
    {
        currentSlave->lastReqest(millis());
        currentSlave->setInit();
    }
    currentSlave = nextSlave(currentSlave);
    if (currentSlave == 0)
    {
        return false;
    }
    currentSlaveTable = currentSlave->firstTabe();
    currentSlaveStartAddress = currentSlaveTable->getMinAdress();
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
    currentSlaveTable = currentSlave->nextTable(currentSlaveTable);
    if (currentSlaveTable == 0)
    {
        return nextSlave();
    }

    currentSlaveStartAddress = currentSlaveTable->getMinAdress();
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

void ModbusMasterRTU::saveLong(uint8_t slaveAdr, int32_t value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }

    sl->saveLong(value, table, startAddres);
}

void ModbusMasterRTU::saveUnsignedLong(uint8_t slaveAdr, uint32_t value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }

    sl->saveUnsignedLong(value, table, startAddres);
}

void ModbusMasterRTU::saveFloat(uint8_t slaveAdr, float value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }

    sl->saveFloat(value, table, startAddres);
}

void ModbusMasterRTU::saveInteger(uint8_t slaveAdr, int16_t value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }

    sl->saveInteger(value, table, startAddres);
}

void ModbusMasterRTU::saveByte(uint8_t slaveAdr, uint8_t value, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->saveByte(value, table, startAddres);
}

void ModbusMasterRTU::saveBool(uint8_t slaveAdr, bool val, uint8_t table, int16_t adr, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->saveBool(val, table, adr);
}

uint8_t ModbusMasterRTU::readByte(uint8_t slaveAdr, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readByte(table, startAddres);
}

int16_t ModbusMasterRTU::readInteger(uint8_t slaveAdr, uint8_t table, int16_t startAddres, bool isIndex)
{

    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readInteger(table, startAddres);
}

float ModbusMasterRTU::readFloat(uint8_t slaveAdr, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readFloat(table, startAddres);
}

int32_t ModbusMasterRTU::readLong(uint8_t slaveAdr, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readLong(table, startAddres);
}

uint32_t ModbusMasterRTU::readUnsignedLong(uint8_t slaveAdr, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readUnsignedLong(table, startAddres);
}

bool ModbusMasterRTU::readBool(uint8_t slaveAdr, uint8_t table, int16_t startAddres, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return 0;
    }
    return sl->readBool(table, startAddres);
}

void ModbusMasterRTU::setDataTable(uint8_t slaveAdr, ModbusTable *table, bool isIndex )
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setDataTable(table);
}

void ModbusMasterRTU::configDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, bool isIndex )
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->configDataTable(_table, dataSize);
}

void ModbusMasterRTU::configDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, int16_t _startAdr, bool isIndex )
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->configDataTable(_table, dataSize, _startAdr);
}

void ModbusMasterRTU::setDataTable(uint8_t slaveAdr, uint8_t _table, int16_t dataSize, int *_adresses, bool isIndex )
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setDataTable(_table, dataSize, _adresses);
}

void ModbusMasterRTU::setSlaveAddress(uint8_t slaveIndex, uint8_t address)
{
    if (hasSlaveOnIndex(slaveIndex))
    {
        slaves[slaveIndex].setSlaveAddress(address);
    }
}

void ModbusMasterRTU::setPollingPeriod(uint8_t slaveAdr, uint32_t period, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setPollingPeriod(period);
}

void ModbusMasterRTU::setTimeOutTime(uint8_t slaveAdr, uint32_t time, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setTimeOutTime(time);
}

void ModbusMasterRTU::setLongOrder(uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setLongOrder(order);
}

void ModbusMasterRTU::setFloatOrder(uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setFloatOrder(order);
}

void ModbusMasterRTU::setUnsignedlongOrder(uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setUnsignedlongOrder(order);
}

void ModbusMasterRTU::setIntOrder(uint8_t slaveAdr, uint8_t order, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->setIntOrder(order);
}

uint8_t ModbusMasterRTU::getLastError(uint8_t slaveAdr, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return 50;
    }
    return sl->getLastError();
}

void ModbusMasterRTU::status(uint8_t slaveAdr, bool status, bool isIndex)
{
    ModbusSlaveInMaster *sl;
    if (isIndex)
    {
        sl = slaveOnIndex(slaveAdr);
    }
    else
    {
        sl = slave(slaveAdr);
    }
    if (sl == 0)
    {
        return;
    }
    sl->status(status);
}
