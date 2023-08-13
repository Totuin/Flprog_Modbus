#include "flprogModbusSlaveRTU.h"

void ModbusSlaveRTU::begin()
{
    uardDevice()->begin();
    bufferSize = 0;
    lastRec = 0;
    if (pinPeDe >= 0)
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
    isInit = true;
}

void ModbusSlaveRTU::pool()
{
    if (!isInit)
    {
        begin();
        return;
    }
    if (workStatus == FLPROG_MODBUS_WAITING_SENDING)
    {
        if ((flprog::isTimer(startSendTime, timeOfSend)))
        {
            workStatus = FLPROG_MODBUS_READY;
            offPeDePin();
        }
        else
        {
            return;
        }
    }
    if (!(checkAvaliblePacage()))
    {
        return;
    }
    if (rxBuffer() < 7)
    {
        setLastError(2);
        buildException(2);
        sendTxBuffer();
        return;
    }
    executeSlaveReqest(mainData(), slaveAddres);
}

uint8_t ModbusSlaveRTU::validateRequest()
{

    int msgCRC =
        ((buffer[bufferSize - 2] << 8) | buffer[bufferSize - 1]);
    if (flprogModus::modbusCalcCRC(bufferSize - 2, buffer) != msgCRC)
    {
        return 255;
    }
    return validateSlaveReqest(mainData());
}

ModbusMainData *ModbusSlaveRTU::mainData()
{
    if (data == 0)
    {
        data = new ModbusMainData();
    }
    return data;
}
