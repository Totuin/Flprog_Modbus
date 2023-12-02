// Подключаем необходимую библиотеку
#include "flprogModbusTCP.h"

/*
  -------------------------------------------------------------------------------------------------
        Создание интерфейса для работы с чипом W5100(W5200,W5500)
        Шина SPI и пин CS берутся из  RT_HW_Base.device.spi.busETH и RT_HW_Base.device.spi.csETH
  -------------------------------------------------------------------------------------------------
*/
FLProgWiznetInterface WiznetInterface;

/*
  -------------------------------------------------------------------------------------------------
        Второй вариант cоздания интерфейса для работы с чипом W5100(W5200,W5500).
        С непосредственной привязкой  пину.
        Пин CS - 10
        Шина SPI берётся из RT_HW_Base.device.spi.busETH
  -------------------------------------------------------------------------------------------------
*/
// FLProgWiznetInterface WiznetInterface(10);

/*
  -------------------------------------------------------------------------------------------------
      Третий вариант cоздания интерфейса для работы с чипом W5100(W5200,W5500).
      С непосредственной привязкой  пину и шине.
      Пин CS - 10
      Шина SPI - 0
  -------------------------------------------------------------------------------------------------
*/
// FLProgWiznetInterface WiznetInterface(10, 0);

// Создаем объект непосредстредственно Модбас слейва на необходимом интерфейсе
ModbusSlaveTCP Slave1(&WiznetInterface);

// Так же создается слейв RTU OVER TCP
//ModbusSlaveRTUoverTCP Slave1(&WiznetInterface);

int _DiscreteInputAddreses[] = {8, 9, 10, 11, 12, 14, 15, 16, 17, 18};

// Вспомогательные переменные для демонстрации

int counter = -10;
bool blinkVar = 0;
uint32_t startTimer;
int lastError;


uint32_t blinkStartTime = 0;
void setup()
{
  WiznetInterface.mac(0x78, 0xAC, 0xC0, 0x0D, 0x5B, 0x86);
  WiznetInterface.localIP(IPAddress(192, 168, 199, 177));
  WiznetInterface.resetDhcp();

  // Описываем таблицы слейва
  Slave1.configDataTable(FLPROG_HOLDING_REGISTR, 10);
  Slave1.configDataTable(FLPROG_INPUT_REGISTR, 10);
  Slave1.configDataTable(FLPROG_COIL, 10, 5);
  Slave1.setDataTable(FLPROG_DISCRETE_INPUT, 10, _DiscreteInputAddreses);

  /*
    Задаем последовательность байтов для хранения различных типов данных
    Возможные знаыения:

    Для Integer:
    FLPROG_AB_ORDER
    FLPROG_BA_ORDER

    Для остальных типов:
    FLPROG_ABCD_ORDER
    FLPROG_CDAB_ORDER
    FLPROG_BADC_ORDER
    FLPROG_DCBA_ORDER

    Вызов данных функций не обязателен.
    По умолчанию заданны  значения  FLPROG_AB_ORDER  и FLPROG_ABCD_ORDER
    Вызов данных функций возможен в любое время если необходимо изменить эти значения в режиме выполнения программы
  */

  startTimer = millis();
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop()
{
  // Цикл работы интерфейса
  WiznetInterface.pool();
  // Цикл работы слейва
  Slave1.pool();
  blinkLed();

  /*
    Играемся с регистрами

    Доступные функции:
       void saveLong(long value, byte table, int startAddres);
       void saveUnsignedLong(unsigned long value, byte table, int startAddres);
       void saveFloat(float value, byte table, int startAddres);
       void saveInteger(int value, byte table, int startAddres);
       void saveByte(byte value, byte table, int startAddres);
       void saveBool(bool value, byte table, int startAddres);
       byte readByte(byte table, int startAddres);
       int readInteger(byte table, int startAddres);
       float readFloat(byte table, int startAddres);
       long readLong(byte table, int startAddres);
       unsigned long readUnsignedLong(byte table, int startAddres);
       bool readBool(byte table, int startAddres);


    tempByte = Slave1.readByte(FLPROG_HOLDING_REGISTR, 1);
    Slave1.saveByte(tempByte, FLPROG_INPUT_REGISTR, 1);

    tempInt = Slave1.readInteger(FLPROG_HOLDING_REGISTR, 0);
    tempFloat = Slave1.readFloat(FLPROG_HOLDING_REGISTR, 2);
    tempLong = Slave1.readLong(FLPROG_HOLDING_REGISTR, 6);
    tempUnsigned = Slave1.readUnsignedLong(FLPROG_HOLDING_REGISTR, 4);
    tempBool1 = Slave1.readBool(FLPROG_COIL, 0);
    tempBool2 = Slave1.readBool(FLPROG_COIL, 1);

    Slave1.saveLong(tempLong, FLPROG_INPUT_REGISTR, 6);
    Slave1.saveUnsignedLong(tempUnsigned, FLPROG_INPUT_REGISTR, 4);
    Slave1.saveFloat(tempFloat, FLPROG_INPUT_REGISTR, 2);
    Slave1.saveInteger(tempInt, FLPROG_INPUT_REGISTR, 0);
    Slave1.saveBool(tempBool1, FLPROG_DISCRETE_INPUT, 0);
    Slave1.saveBool(tempBool2, FLPROG_DISCRETE_INPUT, 1);
  */

  Slave1.saveInteger((Slave1.readInteger(FLPROG_HOLDING_REGISTR, 0)), FLPROG_INPUT_REGISTR, 9);
  Slave1.saveLong((Slave1.readLong(FLPROG_HOLDING_REGISTR, 6)), FLPROG_INPUT_REGISTR, 5);
  Slave1.saveInteger((Slave1.readFloat(FLPROG_HOLDING_REGISTR, 4) * 100), FLPROG_INPUT_REGISTR, 0);
  Slave1.saveUnsignedLong((Slave1.readUnsignedLong(FLPROG_HOLDING_REGISTR, 8)), FLPROG_INPUT_REGISTR, 7);
  Slave1.saveBool(blinkVar, FLPROG_COIL, 5);
  Slave1.saveBool(blinkVar, FLPROG_DISCRETE_INPUT, 8);
  Slave1.saveBool((Slave1.readBool(FLPROG_COIL, 6)), FLPROG_DISCRETE_INPUT, 9);

  if (flprog::isTimer(startTimer, 1000))
  {
    counter++;
    if (counter > 10)
    {
      counter = -10;
    }
    blinkVar = !blinkVar;
    Slave1.saveInteger(counter, FLPROG_INPUT_REGISTR, 6);
    Slave1.saveInteger(counter, FLPROG_HOLDING_REGISTR, 1);
    Slave1.saveFloat((counter / 10.0), FLPROG_HOLDING_REGISTR, 2);
    Slave1.saveFloat((counter / 100.0), FLPROG_INPUT_REGISTR, 4);
    startTimer = millis();
  }
  // получение последней ошибки слейва
  lastError = Slave1.getLastError();
}


void blinkLed()
{
  if (flprog::isTimer(blinkStartTime, 50))
  {
    blinkStartTime = millis();
    digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
  }
}