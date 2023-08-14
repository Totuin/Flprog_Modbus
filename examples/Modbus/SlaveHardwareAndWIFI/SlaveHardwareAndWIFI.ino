#include "flprogModbusRTU.h"
#include "flprogModbusTCP.h"

FLProgOnBoardWifi WifiInterface;

// Создаем объект непосредстредственно Модбас слейва TCP на необходимом интерфейсе
ModbusSlaveTCP SlaveTCP(&WifiInterface);

// Создаем объект непосредстредственно Модбас слейва на нулевом уарте
ModbusSlaveRTU SlaveRTU(0);

// Набор регистров с непоследовательными адресами
int _DiscreteInputAddreses[] = {8, 9, 10, 11, 12, 14, 15, 16, 17, 18};

// Вспомогательные переменные для демонстрации
int counter = -10;
bool blinkVar = 0;
uint32_t startTimer;
int lastError;

void setup()
{
    WifiInterface.clientOn();
    WifiInterface.mac(0x78, 0xAC, 0xC0, 0x2C, 0x3E, 0x28);
    WifiInterface.localIP(IPAddress(192, 168, 199, 177));
    WifiInterface.resetDhcp();
    WifiInterface.setClientSsidd("totuin-router");
    WifiInterface.setClientPassword("12345678");

    // Конфигурируем набор регистров в слейве TCP
    SlaveTCP.configDataTable(FLPROG_HOLDING_REGISTR, 10);
    SlaveTCP.configDataTable(FLPROG_INPUT_REGISTR, 10);
    SlaveTCP.configDataTable(FLPROG_COIL, 10, 5);
    SlaveTCP.setDataTable(FLPROG_DISCRETE_INPUT, 10, _DiscreteInputAddreses);

    // передаём ссылку на набор регистров из слейва TCP в слейв RTU
    SlaveRTU.setData(SlaveTCP.mainData());
}

void loop()
{
    // Цикл работы интерфейса
    WifiInterface.pool();

    // Цикл работы слейва RTU
    SlaveRTU.pool();

    // Цикл работы слейва TCP
    SlaveTCP.pool();

    // Играемся с регистрами
    SlaveTCP.saveInteger((SlaveRTU.readInteger(FLPROG_HOLDING_REGISTR, 0)), FLPROG_INPUT_REGISTR, 9);
    SlaveTCP.saveLong((SlaveTCP.readLong(FLPROG_HOLDING_REGISTR, 6)), FLPROG_INPUT_REGISTR, 5);
    SlaveRTU.saveInteger((SlaveRTU.readFloat(FLPROG_HOLDING_REGISTR, 4) * 100), FLPROG_INPUT_REGISTR, 0);
    SlaveRTU.saveUnsignedLong((SlaveTCP.readUnsignedLong(FLPROG_HOLDING_REGISTR, 8)), FLPROG_INPUT_REGISTR, 7);
    SlaveTCP.saveBool(blinkVar, FLPROG_COIL, 5);
    SlaveTCP.saveBool(blinkVar, FLPROG_DISCRETE_INPUT, 8);
    SlaveRTU.saveBool((SlaveRTU.readBool(FLPROG_COIL, 6)), FLPROG_DISCRETE_INPUT, 9);

    if (flprog::isTimer(startTimer, 1000))
    {
        counter++;
        if (counter > 10)
        {
            counter = -10;
        }
        blinkVar = !blinkVar;
        SlaveTCP.saveInteger(counter, FLPROG_INPUT_REGISTR, 6);
        SlaveTCP.saveInteger(counter, FLPROG_HOLDING_REGISTR, 1);
        SlaveRTU.saveFloat((counter / 10.0), FLPROG_HOLDING_REGISTR, 2);
        SlaveRTU.saveFloat((counter / 100.0), FLPROG_INPUT_REGISTR, 4);
        startTimer = millis();
    }
    // получение последней ошибки слейва
    lastError = SlaveRTU.getLastError();
}