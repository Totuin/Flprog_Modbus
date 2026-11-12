#include "flprogModbusRTU.h"
#include "flprogModbusTCP.h"
// Создаем объект шины
//-------------------------------------------------------------------------------------------------
//         Вариант с  шиной (SPI0) и пином(10) по умолчаниюю. Пин потом можно поменять.
//         Но если на этой шине висит ещё какое то устройство лучше применять второй вариант
//-------------------------------------------------------------------------------------------------
FLProgWiznetInterface WiznetInterface; //--Создание интерфейса для работы с чипом W5100(W5200,W5500) (по умолчанию CS pin - 10,  Шина SPI - 0);

//-------------------------------------------------------------------------------------------------
//        Второй вариант с непосредственной привязкой к шине и пину.
//-------------------------------------------------------------------------------------------------
 //FLProgWiznetInterface WiznetInterface(10, 0); //--Создание интерфейса для работы с чипом W5100(W5200,W5500) CS pin - 10, Шина SPI - 0;
 //FLProgWiznetInterface WiznetInterface(10); //--Создание интерфейса для работы с чипом W5100(W5200,W5500) CS pin - 10, (по умолчанию Шина SPI - 0);

// Создаём общий для обеих слейвов набор регистров
ModbusMainData Data;

// Создаем объект непосредстредственно Модбас слейва TCP на необходимом интерфейсе
ModbusSlaveTCP SlaveTCP(&WiznetInterface);

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
  WiznetInterface.mac(0x78, 0xAC, 0xC0, 0x0D, 0x5B, 0x86);
  WiznetInterface.localIP(IPAddress(192, 168, 199, 177));
  WiznetInterface.resetDhcp();

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
  WiznetInterface.pool();

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