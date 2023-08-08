#include "flprogModbus.h"

//------------------Modbus------------------------------------

void Modbus::process_modbus_FC1(ModbusMainData *data, uint8_t table)
{
    ModbusTable *dataTable = data->tableForType(table);
    uint8_t bytesNo, bitsNo;
    int16_t currentCoil, coil;
    bool value;
    int16_t startCoil = word(buffer[2], buffer[3]);
    int16_t coilNo = word(buffer[4], buffer[5]);
    bytesNo = uint8_t(coilNo / 8);
    if (coilNo % 8 != 0)
        bytesNo++;
    buffer[2] = bytesNo;
    bufferSize = 3;
    bitsNo = 0;
    for (currentCoil = 0; currentCoil < coilNo; currentCoil++)
    {
        coil = startCoil + currentCoil;
        value = dataTable->readBoolRegister(coil);
        bitWrite(buffer[bufferSize], bitsNo, value);
        bitsNo++;
        if (bitsNo > 7)
        {
            bitsNo = 0;
            bufferSize++;
        }
    }
    if (coilNo % 8 != 0)
        bufferSize++;
    sendTxBuffer();
}

void Modbus::process_modbus_FC3(ModbusMainData *data, uint8_t table)
{
    ModbusTable *dataTable = data->tableForType(table);
    int16_t startAddr = word(buffer[2], buffer[3]);
    int16_t byteRegsno = word(buffer[4], buffer[5]);
    int16_t i;
    int16_t value;
    buffer[2] = byteRegsno * 2;
    bufferSize = 3;
    for (i = startAddr; i < startAddr + byteRegsno; i++)
    {
        value = dataTable->readWorldRegister(i);
        buffer[bufferSize] = highByte(value);
        bufferSize++;
        buffer[bufferSize] = lowByte(value);
        bufferSize++;
    }
    sendTxBuffer();
}

void Modbus::process_modbus_FC5(ModbusMainData *data)
{
    int16_t addres = word(buffer[2], buffer[3]);
    ModbusTable *table = data->tableForType(FLPROG_COIL);
    bool value = (buffer[4] == 0xff);
    table->writeRegister(addres, value);
    bufferSize = 6;
    sendTxBuffer();
}

void Modbus::process_modbus_FC6(ModbusMainData *data)
{
    int16_t addres = word(buffer[2], buffer[3]);
    ModbusTable *table = data->tableForType(FLPROG_HOLDING_REGISTR);
    int16_t value = word(buffer[4], buffer[5]);
    table->writeRegister(addres, value);
    bufferSize = 6;
    sendTxBuffer();
}

void Modbus::process_modbus_FC15(ModbusMainData *data)
{
    uint8_t frameByte, bitsNo;
    uint8_t currentCoil;
    int16_t addres;
    int16_t startCoil = word(buffer[2], buffer[3]);
    int16_t coilNo = word(buffer[4], buffer[5]);
    bitsNo = 0;
    frameByte = 7;
    bool value;
    ModbusTable *table = data->tableForType(FLPROG_COIL);
    for (currentCoil = 0; currentCoil < coilNo; currentCoil++)
    {
        addres = startCoil + currentCoil;
        value = bitRead(buffer[frameByte], bitsNo);
        table->writeRegister(addres, value);
        bitsNo++;
        if (bitsNo > 7)
        {
            bitsNo = 0;
            frameByte++;
        }
    }
    bufferSize = 6;
    sendTxBuffer();
}

void Modbus::process_modbus_FC16(ModbusMainData *data)
{
    int16_t value, addres;
    ModbusTable *table = data->tableForType(FLPROG_HOLDING_REGISTR);
    int16_t startAddr = buffer[2] << 8 | buffer[3];
    int16_t byteRegsno = buffer[4] << 8 | buffer[5];
    int16_t i;
    buffer[4] = 0;
    buffer[5] = byteRegsno;
    bufferSize = 6;
    for (i = 0; i < byteRegsno; i++)
    {
        addres = startAddr + i;
        value = word(buffer[7 + i * 2], buffer[8 + i * 2]);
        table->writeRegister(addres, value);
    }
    sendTxBuffer();
}

uint8_t Modbus::tabeTypeForFunction(uint8_t function)
{
    if ((function == 1) || (function == 5) || (function == 15))
    {
        return FLPROG_COIL;
    }
    if (function == 2)
    {
        return FLPROG_DISCRETE_INPUT;
    }
    if ((function == 3) || (function == 6) || (function == 16))
    {
        return FLPROG_HOLDING_REGISTR;
    }
    if (function == 4)
    {
        return FLPROG_INPUT_REGISTR;
    }
    return 100;
}

uint8_t Modbus::validateSlaveReqest(ModbusMainData *data)
{
    if (data == 0)
    {
        return 1;
    }
    if (!(data->isSupportFunction(buffer[1])))
    {
        return 1;
    }
    uint16_t startAdr = word(buffer[2], buffer[3]);
    uint16_t count;
    uint8_t function = buffer[1];
    uint8_t workTableType = tabeTypeForFunction(function);
    if (workTableType == 100)
    {
        return 2;
    }
    if ((function == 5) || (function == 6))
    {
        if (!(data->checkModbusAddres(startAdr, workTableType)))
        {
            return 2;
        }
    }
    else
    {
        count = word(buffer[4], buffer[5]);
        if (count > 25)
        {
            return 2;
        }
        if (!(data->checkModbusRange(startAdr, count, workTableType)))
        {
            return 2;
        }
    }
    return 0;
}

void Modbus::executeSlaveReqest(ModbusMainData *data, uint8_t address)
{
    if ((buffer[0] != address) && (buffer[0] != 0))
        return;
    uint8_t exception = validateRequest();
    if (exception > 0)
    {
        setLastError(exception);
        if (exception != 255)
        {
            buildException(exception);
            sendTxBuffer();
        }
        return;
    }
    setLastError(0);
    if (buffer[1] == 1)
    {
        process_modbus_FC1(data, FLPROG_COIL);
        return;
    }
    if (buffer[1] == 2)
    {
        process_modbus_FC1(data, FLPROG_DISCRETE_INPUT);
        return;
    }
    if (buffer[1] == 5)
    {
        process_modbus_FC5(data);
        return;
    }

    if (buffer[1] == 15)
    {
        process_modbus_FC15(data);
        return;
    }

    if (buffer[1] == 3)
    {
        process_modbus_FC3(data, FLPROG_HOLDING_REGISTR);
        return;
    }
    if (buffer[1] == 4)
    {
        process_modbus_FC3(data, FLPROG_INPUT_REGISTR);
        return;
    }
    if (buffer[1] == 6)
    {
        process_modbus_FC6(data);
        return;
    }
    if (buffer[1] == 16)
    {
        process_modbus_FC16(data);
        return;
    }
}

void Modbus::buildException(uint8_t exception)
{
    uint8_t func = buffer[1];
    buffer[0] = 1;
    buffer[1] = func + 0x80;
    buffer[2] = exception;
    bufferSize = 3;
}

void Modbus::get_FC1(ModbusTable *table, int16_t startAddress, int16_t numberRegs)
{
    uint8_t currentByte = 3;
    uint8_t currentBit = 0;
    bool value;
    int16_t currentIndex = startAddress;
    for (uint8_t i = 0; i < numberRegs; i++)
    {
        value = bitRead(buffer[currentByte], currentBit);
        table->writeRegister(currentIndex, value);
        currentIndex++;
        currentBit++;
        if (currentBit > 7)
        {
            currentBit = 0;
            currentByte++;
        }
    }
}

void Modbus::get_FC3(ModbusTable *table, int16_t startAddress, int16_t numberRegs)
{
    int16_t currentIndex = startAddress;
    uint8_t currentByte = 3;
    int16_t value;
    for (int16_t i = 0; i < numberRegs; i++)
    {
        value = word(buffer[currentByte], buffer[currentByte + 1]);
        table->writeRegister((currentIndex + i), value);
        currentByte += 2;
    }
}

void Modbus::create_Read(int16_t numberRegs)
{
    buffer[4] = highByte(numberRegs);
    buffer[5] = lowByte(numberRegs);
    bufferSize = bufferSize + 2;
}

void Modbus::create_Write_FC5(ModbusTable *table, int16_t startAddress)
{
    if (table->readBoolRegister(startAddress))
    {
        buffer[4] = 0xFF;
    }
    else
    {
        buffer[4] = 0;
    }
    buffer[5] = 0;
    bufferSize = bufferSize + 2;
}

void Modbus::create_Write_FC6(ModbusTable *table, int16_t startAddress)
{
    int16_t value = table->readWorldRegister(startAddress);
    buffer[4] = highByte(value);
    buffer[5] = lowByte(value);
    bufferSize = bufferSize + 2;
}

void Modbus::create_Write_FC15(ModbusTable *table, int16_t startAddress, int16_t numberRegs)
{
    bool value;
    buffer[4] = highByte(numberRegs);
    buffer[5] = lowByte(numberRegs);
    bufferSize = bufferSize + 3;
    uint8_t byteCounter = 0;
    uint8_t bitCounter = 0;
    uint8_t temp = 0;
    for (int16_t i = 0; i < numberRegs; i++)
    {
        if (bitCounter == 7)
        {
            buffer[7 + byteCounter] = temp;
            bufferSize++;
            byteCounter++;
            bitCounter = 0;
            temp = 0;
        }
        value = table->readBoolRegister(startAddress + i);
        bitWrite(temp, bitCounter, value);
        bitCounter++;
    }
    if (bitCounter < 7)
    {
        buffer[7 + byteCounter] = temp;
        bufferSize++;
    }
    buffer[6] = byteCounter + 1;
}

void Modbus::create_Write_FC16(ModbusTable *table, int16_t startAddress, int16_t numberRegs)
{
    int16_t value;
    buffer[4] = highByte(numberRegs);
    buffer[5] = lowByte(numberRegs);
    buffer[6] = numberRegs * 2;
    bufferSize = bufferSize + 3;
    uint8_t byteCounter = 0;
    for (int16_t i = 0; i < numberRegs; i++)
    {
        value = table->readWorldRegister(startAddress + i);
        buffer[7 + byteCounter] = highByte(value);
        buffer[8 + byteCounter] = lowByte(value);
        bufferSize = bufferSize + 2;
        byteCounter = byteCounter + 2;
    }
}

void Modbus::create_PDU(ModbusTable *table, int16_t startAddress, int16_t numberRegs)
{
    buffer[2] = highByte(startAddress);
    buffer[3] = lowByte(startAddress);
    bufferSize = 4;
    if (buffer[1] < 5)
    {
        create_Read(numberRegs);
    }
    if (buffer[1] == 5)
    {
        create_Write_FC5(table, startAddress);
    }
    if (buffer[1] == 6)
    {
        create_Write_FC6(table, startAddress);
    }
    if (buffer[1] == 15)
    {
        create_Write_FC15(table, startAddress, numberRegs);
    }
    if (buffer[1] == 16)
    {
        create_Write_FC16(table, startAddress, numberRegs);
    }
    sendTxBuffer();
}

void Modbus::writeMaserData(ModbusTable *table, int16_t startAddress, int16_t numberRegs)
{
    if ((buffer[1] == 1) || (buffer[1] == 2))
    {
        get_FC1(table, startAddress, numberRegs);
        workStatus = FLPROG_MODBUS_READY;
        return;
    }
    if ((buffer[1] == 3) || (buffer[1] == 4))
    {
        get_FC3(table, startAddress, numberRegs);
        workStatus = FLPROG_MODBUS_READY;
        return;
    }
    workStatus = FLPROG_MODBUS_READY;
}
