#include "flprogModbusRTU.h"
#include "flprogModbusTCP.h"
// Создаем объект шины
FLProgSPI spiBus(0);
// Создаём интерфейс W5100
FlprogW5100Interface W5100_Interface(&spiBus, 10);

// Создаём общий для обеих слейвов набор регистров
ModbusMainData Data;

// Создаем объект непосредстредственно Модбас слейва TCP на необходимом интерфейсе
ModbusSlaveTCP SlaveTCP(&W5100_Interface);

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
  W5100_Interface.mac(0x78, 0xAC, 0xC0, 0x0D, 0x5B, 0x86);
  W5100_Interface.localIP(IPAddress(192, 168, 199, 177));
  W5100_Interface.resetDhcp();

  // Конфигурируем набор регистров
  Data.configDataTable(FLPROG_HOLDING_REGISTR, 10);
  Data.configDataTable(FLPROG_INPUT_REGISTR, 10);
  Data.configDataTable(FLPROG_COIL, 10, 5);
  Data.setDataTable(FLPROG_DISCRETE_INPUT, 10, _DiscreteInputAddreses);

  // передаём обеим слейвам ссылку на набор регистров
  SlaveTCP.setData(&Data);
  SlaveRTU.setData(&Data);
}

void loop()
{
  // Цикл работы интерфейса
  W5100_Interface.pool();

  // Цикл работы слейва RTU
  SlaveRTU.pool();

  // Цикл работы слейва TCP
  SlaveTCP.pool();

  // Играемся с регистрами
  Data.saveInteger((Data.readInteger(FLPROG_HOLDING_REGISTR, 0)), FLPROG_INPUT_REGISTR, 9);
  SlaveTCP.saveLong((SlaveTCP.readLong(FLPROG_HOLDING_REGISTR, 6)), FLPROG_INPUT_REGISTR, 5);
  SlaveRTU.saveInteger((SlaveRTU.readFloat(FLPROG_HOLDING_REGISTR, 4) * 100), FLPROG_INPUT_REGISTR, 0);
  Data.saveUnsignedLong((SlaveTCP.readUnsignedLong(FLPROG_HOLDING_REGISTR, 8)), FLPROG_INPUT_REGISTR, 7);
  Data.saveBool(blinkVar, FLPROG_COIL, 5);
  SlaveTCP.saveBool(blinkVar, FLPROG_DISCRETE_INPUT, 8);
  SlaveRTU.saveBool((Data.readBool(FLPROG_COIL, 6)), FLPROG_DISCRETE_INPUT, 9);

  if (flprog::isTimer(startTimer, 1000))
  {
    counter++;
    if (counter > 10)
    {
      counter = -10;
    }
    blinkVar = !blinkVar;
    Data.saveInteger(counter, FLPROG_INPUT_REGISTR, 6);
    Data.saveInteger(counter, FLPROG_HOLDING_REGISTR, 1);
    Data.saveFloat((counter / 10.0), FLPROG_HOLDING_REGISTR, 2);
    Data.saveFloat((counter / 100.0), FLPROG_INPUT_REGISTR, 4);
    startTimer = millis();
  }
  // получение последней ошибки слейва
  lastError = SlaveTCP.getLastError();
}