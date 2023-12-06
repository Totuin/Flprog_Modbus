#include "flprogModbusUtilites.h"

//------------------namespace flprogModus--------------------

int flprogModus::modbusCalcCRC(byte length, byte bufferArray[])
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

byte flprogModus::t35TimeForSpeed(int portSpeed)
{
    int speed = portSpeed;
    if (speed > 100)
    {
        speed = flprog::codeFromSpeed(portSpeed);
    }
    switch (speed)
    {
    case FLPROG_SPEED_300:
        return 100;
        break;
    case FLPROG_SPEED_600:
        return 75;
        break;
    case FLPROG_SPEED_1200:
        return 40;
        break;
    case FLPROG_SPEED_2400:
        return 25;
        break;
    case FLPROG_SPEED_4800:
        return 15;
        break;
    case FLPROG_SPEED_9600:
        return 10;
        break;
    case FLPROG_SPEED_14400:
        return 5;
        break;
    case FLPROG_SPEED_19200:
        return 5;
        break;
    case FLPROG_SPEED_28800:
        return 5;
        break;
    case FLPROG_SPEED_38400:
        return 5;
        break;
    default:
        return 5;
        break;
    }
}

int flprogModus::timeForSendBytes(byte portDataBits, byte portStopBits, byte portParity, int portSpeed, int dataSize)
{
    int speed = portSpeed;
    uint8_t temp = 1 + portDataBits + portStopBits;
    if (speed < 100)
    {
        speed = flprog::speedFromCode(portSpeed);
    }
    if (portParity > 0)
    {
        temp = temp + 1;
    }
    float temp1 = (temp / portDataBits);

    temp1 = temp1 * speed;
    temp1 = 1000 / temp1;
    return (ceil(temp1 * (dataSize + 8) * 8));
}
