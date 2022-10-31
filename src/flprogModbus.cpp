#include "flprogModbus.h"

int Modbus::modbusCalcCRC(byte length, byte bufferArray[])
{
    unsigned int temp, temp2, flag;
    temp = 0xFFFF;
    for (unsigned char i = 0; i < length; i++)
    {
        temp = temp ^ bufferArray[i];
        for (unsigned char j = 1; j <= 8; j++)
        {
            flag = temp & 0x0001;
            temp >>= 1;
            if (flag)
                temp ^= 0xA001;
        }
    }
    temp2 = temp >> 8;
    temp = (temp << 8) | temp2;
    temp &= 0xFFFF;
    return temp;
}

byte Modbus::t35TimeForSpeed(byte speed)
{
    switch (speed)
    {
    case SPEED_300:
        return 75;
        break;
    case SPEED_600:
        return 75;
        break;
    case SPEED_1200:
        return 38;
        break;
    case SPEED_2400:
        return 19;
        break;

    case SPEED_4800:
        return 10;
        break;
    case SPEED_9600:
        return 5;
        break;
    case SPEED_14400:
        return 4;
        break;
    case SPEED_19200:
        return 3;
        break;
    case SPEED_28800:
        return 3;
        break;

    case SPEED_38400:
        return 2;
        break;
    default:
        return 1;
        break;
    }
}

int Modbus::timeForSendBytes(byte portDataBits, byte portStopBits, byte portParity, byte portSpeed, int dataSize)
{
    byte temp = 1 + portDataBits + portStopBits;
    if (portParity > 0)
    {
        temp = temp + 1;
    }
    float temp1 = (temp / portDataBits);

    temp1 = temp1 * (flprog::speedFromCode(portSpeed));
    temp1 = 1000 / temp1;
    return ceil(temp1 * dataSize * 8);
}

void Modbus::process_modbus_FC1(ModbusMainData *data, byte table)
{
    ModbusTable *dataTable = data->tableForType(table);
    byte bytesNo, bitsNo;
    int currentCoil, coil;
    bool value;
    int startCoil = word(buffer[2], buffer[3]);
    int coilNo = word(buffer[4], buffer[5]);
    bytesNo = byte(coilNo / 8);
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

void Modbus::process_modbus_FC3(ModbusMainData *data, byte table)
{
    ModbusTable *dataTable = data->tableForType(table);
    int startAddr = word(buffer[2], buffer[3]);
    int byteRegsno = word(buffer[4], buffer[5]);
    int i;
    int value;
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
    byte index;
    int addres = word(buffer[2], buffer[3]);
    ModbusTable *table = data->tableForType(COIL);
    bool value = (buffer[4] == 0xff);
    table->writeRegister(addres, value);
    bufferSize = 6;
    sendTxBuffer();
}

void Modbus::process_modbus_FC6(ModbusMainData *data)
{
    int addres = word(buffer[2], buffer[3]);
    ModbusTable *table = data->tableForType(HOLDING_REGISTR);
    int value = word(buffer[4], buffer[5]);
    table->writeRegister(addres, value);
    bufferSize = 6;
    sendTxBuffer();
}

void Modbus::process_modbus_FC15(ModbusMainData *data)
{
    byte frameByte, bitsNo;
    byte currentCoil;
    int addres;
    byte index;
    int startCoil = word(buffer[2], buffer[3]);
    int coilNo = word(buffer[4], buffer[5]);
    bitsNo = 0;
    frameByte = 7;
    bool value;
    ModbusTable *table = data->tableForType(COIL);
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
    int value, addres;
    ModbusTable *table = data->tableForType(HOLDING_REGISTR);
    int startAddr = buffer[2] << 8 | buffer[3];
    int byteRegsno = buffer[4] << 8 | buffer[5];
    int i;
    int index;
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

byte Modbus::validateSlaveReqest(ModbusMainData *data)
{
    if (!(data->isSupportFunction(buffer[1])))
    {
        return 1;
    }
    int intRegs = 0;
    byte byteRegs;
    switch (buffer[1])
    {
    case 1:
        if (!(data->checkModbusRange((word(buffer[2], buffer[3])), (word(buffer[4], buffer[5])), COIL)))
        {
            return 2;
        }
        break;
    case 2:
        if (!(data->checkModbusRange((word(buffer[2], buffer[3])), (word(buffer[4], buffer[5])), DISCRETE_INPUT)))
        {
            return 2;
        }
        break;
    case 3:
        if (!(data->checkModbusRange((word(buffer[2], buffer[3])), (word(buffer[4], buffer[5])), HOLDING_REGISTR)))
        {
            return 2;
        }
        break;
    case 4:
        if (!(data->checkModbusRange((word(buffer[2], buffer[3])), (word(buffer[4], buffer[5])), INPUT_REGISTR)))
        {
            return 2;
        }
        break;
    case 5:
        if (!(data->checkModbusAddres((word(buffer[2], buffer[3])), COIL)))
        {
            return 2;
        }
        break;
    case 6:
        if (!(data->checkModbusAddres((word(buffer[2], buffer[3])), HOLDING_REGISTR)))
        {
            return 2;
        }
        break;
    case 15:
        if (!(data->checkModbusRange((word(buffer[2], buffer[3])), (word(buffer[4], buffer[5])), COIL)))
        {
            return 2;
        }
        break;
    case 16:
        if (!(data->checkModbusRange((word(buffer[2], buffer[3])), (word(buffer[4], buffer[5])), HOLDING_REGISTR)))
        {
            return 2;
        }
        break;
    }
    return 0;
}

void Modbus::executeSlaveReqest(ModbusMainData *data)
{
    if ((buffer[0] != 1) && (buffer[0] != 0))
        return;
    byte exception = validateRequest();
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
        process_modbus_FC1(data, COIL);
        return;
    }
    if (buffer[1] == 2)
    {
        process_modbus_FC1(data, DISCRETE_INPUT);
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
        process_modbus_FC3(data, HOLDING_REGISTR);
        return;
    }
    if (buffer[1] == 4)
    {
        process_modbus_FC3(data, INPUT_REGISTR);
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

void Modbus::buildException(byte exception)
{
    byte func = buffer[1];
    buffer[0] = 1;
    buffer[1] = func + 0x80;
    buffer[2] = exception;
    bufferSize = 3;
}

void Modbus::get_FC1(ModbusTable *table, int startAddress, int numberRegs)
{
    byte currentByte = 3;
    byte currentBit = 0;
    bool value;
    int currentIndex = startAddress;
    for (byte i = 0; i < numberRegs; i++)
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

void Modbus::get_FC3(ModbusTable *table, int startAddress, int numberRegs)
{
    int currentIndex = startAddress;
    byte currentByte = 3;
    int value;
    for (int i = 0; i < numberRegs; i++)
    {
        value = word(buffer[currentByte], buffer[currentByte + 1]);
        table->writeRegister((currentIndex + i), value);
        currentByte += 2;
    }
}

void Modbus::create_Read(int numberRegs)
{
    buffer[4] = highByte(numberRegs);
    buffer[5] = lowByte(numberRegs);
    bufferSize = bufferSize + 2;
}

void Modbus::create_Write_FC5(ModbusTable *table, int startAddress)
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

void Modbus::create_Write_FC6(ModbusTable *table, int startAddress)
{
    int value = table->readWorldRegister(startAddress);
    buffer[4] = highByte(value);
    buffer[5] = lowByte(value);
    bufferSize = bufferSize + 2;
}

void Modbus::create_Write_FC15(ModbusTable *table, int startAddress, int numberRegs)
{
    bool value;
    buffer[4] = highByte(numberRegs);
    buffer[5] = lowByte(numberRegs);
    bufferSize = bufferSize + 3;
    byte byteCounter = 0;
    byte bitCounter = 0;
    byte temp = 0;
    for (int i = 0; i < numberRegs; i++)
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

void Modbus::create_Write_FC16(ModbusTable *table, int startAddress, int numberRegs)
{
    int value;
    buffer[4] = highByte(numberRegs);
    buffer[5] = lowByte(numberRegs);
    buffer[6] = numberRegs * 2;
    bufferSize = bufferSize + 3;
    byte byteCounter = 0;
    for (int i = 0; i < numberRegs; i++)
    {
        value = table->readWorldRegister(startAddress + i);
        buffer[7 + byteCounter] = highByte(value);
        buffer[8 + byteCounter] = lowByte(value);
        bufferSize = bufferSize + 2;
        byteCounter = byteCounter + 2;
    }
}

void Modbus::create_PDU(ModbusTable *table, int startAddress, int numberRegs)
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

void Modbus::writeMaserData(ModbusTable *table, int startAddress, int numberRegs)
{
    if ((buffer[1] == 1) || (buffer[1] == 2))
    {
        get_FC1(table, startAddress, numberRegs);
        workStatus = MODBUS_READY;
        return;
    }
    if ((buffer[1] == 3) || (buffer[1] == 4))
    {
        get_FC3(table, startAddress, numberRegs);
        workStatus = MODBUS_READY;
        return;
    }
    workStatus = MODBUS_READY;
}