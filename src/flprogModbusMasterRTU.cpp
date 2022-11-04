#include "flprogModbusMasterRTU.h"

void ModbusMasterRTU::pool()
{

    if (workStatus == MODBUS_WAITING_SENDING)
    {
        if (flprog::isTimer(startSendTime, timeOfSend))
        {
            workStatus = MODBUS_WAITING_ANSWER;
            startSendTime = millis();
            offPeDePin();
        }
        else
        {
            return;
        }
    }

    if (workStatus == MODBUS_WAITING_ANSWER)
    {
        checkAnswer();
    }
    if (workStatus == MODBUS_READY)
    {
        nextQuery();
    }
}

void ModbusMasterRTU::checkAnswer()
{
    if (flprog::isTimer(startSendTime, (telegrammSlave->getTimeOutTime())))
    {
        telegrammSlave->setLastError(255);
        workStatus = MODBUS_READY;
        return;
    }
    if (!(checkAvalibleBytes()))
        return;
    byte state = rxBuffer();
    if (state < 5)
    {
        telegrammSlave->setLastError(255);
        workStatus = MODBUS_READY;
        return;
    }
    byte exception = validateRequest();
    if (exception > 0)
    {
        telegrammSlave->setLastError(exception);
        workStatus = MODBUS_READY;
        return;
    }
    telegrammSlave->setLastError(0);
    writeMaserData(telegrammTable, telegrammStartAddres, telegrammNumbeRegs);
}

byte ModbusMasterRTU::validateRequest()
{
    uint16_t msgCRC = ((buffer[bufferSize - 2] << 8) | buffer[bufferSize - 1]);
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

ModbusSlaveInMaster *ModbusMasterRTU::slaveForAddress(int adress)
{
    for (int i = 0; i < slavesSize; i++)
    {
        if (slaves[i].slaveAddres == adress)
        {
            return &slaves[i];
        }
    }
    return 0;
}

ModbusSlaveInMaster *ModbusMasterRTU::firstWriteSlave()
{
    for (int i = 0; i < slavesSize; i++)
    {
        if ((slaves[i].isActive) && (slaves[i].hasWriteRegisters()))
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
    if (currentSlaveTable->table == COIL)
    {
        telegrammFunction = 1;
    }
    if (telegrammTable->table == DISCRETE_INPUT)
    {
        telegrammFunction = 2;
    }
    if (telegrammTable->table == INPUT_REGISTR)
    {
        telegrammFunction = 4;
    }
    if (telegrammTable->table == HOLDING_REGISTR)
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
    if (telegrammTable->table == HOLDING_REGISTR)
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
    for (byte i = 0; i < telegrammNumbeRegs; i++)
    {
        telegrammTable->resetWriteFlag(telegrammStartAddres + i);
    }

    return true;
}

void ModbusMasterRTU::sendQuery()
{
    buffer[0] = telegrammSlave->slaveAddres;
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

    for (int i = 0; i < slavesSize; i++)
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
        currentSlave->lastReqestTime = millis();
        currentSlave->initFlag = true;
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
    for (int i = 0; i < slavesSize; i++)
    {
        if (slaves[i].isReady())
        {
            return true;
        }
    }
    return false;
}

bool ModbusMasterRTU::hasSlave(int slaveIndex)
{
    if ((slaveIndex > -1) && (slaveIndex < slavesSize))
    {
        return true;
    }
    return false;
}

void ModbusMasterRTU::saveLong(int slave, long value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveLong(value, table, startAddres);
}

void ModbusMasterRTU::saveUnsignedLong(int slave, unsigned long value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveUnsignedLong(value, table, startAddres);
}

void ModbusMasterRTU::saveFloat(int slave, float value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveFloat(value, table, startAddres);
}

void ModbusMasterRTU::saveInteger(int slave, int value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveInteger(value, table, startAddres);
}

void ModbusMasterRTU::saveByte(int slave, byte value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveByte(value, table, startAddres);
}

void ModbusMasterRTU::saveBool(int slave, bool value, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return;
    }
    slaves[slave].saveBool(value, table, startAddres);
}

byte ModbusMasterRTU::readByte(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readByte(table, startAddres);
}

int ModbusMasterRTU::readInteger(int slave, byte table, int startAddres)
{

    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readInteger(table, startAddres);
}

float ModbusMasterRTU::readFloat(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readFloat(table, startAddres);
}

long ModbusMasterRTU::readLong(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readLong(table, startAddres);
}

unsigned long ModbusMasterRTU::readUnsignedLong(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return 0;
    }
    return slaves[slave].readUnsignedLong(table, startAddres);
}

bool ModbusMasterRTU::readBool(int slave, byte table, int startAddres)
{
    if (!hasSlave(slave))
    {
        return false;
    }
    return slaves[slave].readBool(table, startAddres);
}
