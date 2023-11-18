// Подключаем необходимую библиотеку
#include "flprogModbusTCP.h"

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

ModbusKaScadaCloud Slave1(&WiznetInterface);

// Вспомогательные переменные для демонстрации
int tempInt;
unsigned long tempUnsigned;

void setup()
{

  WiznetInterface.mac(0x78, 0xAC, 0xC0, 0x0D, 0x5B, 0x86);

  // Инициализация вспомогательных переменных
  tempUnsigned = millis() - 1000;

  // Описываем таблицы слейва
  Slave1.configDataTable(FLPROG_HOLDING_REGISTR, 10);
  Slave1.configDataTable(FLPROG_INPUT_REGISTR, 10);
  Slave1.configDataTable(FLPROG_COIL, 10, 5);
  Slave1.configDataTable(FLPROG_DISCRETE_INPUT, 10);
  /*
    Задаём имя устройства KaScadaCloud.
    Вызов обязателен и так же может быть изменён в любой момент исполнения программы
  */
  Slave1.setKaScadaCloudDevceId("DKK6T9TPE4");
}

void loop()
{
  // Цикл работы интерфейса
  WiznetInterface.pool();
  // Цикл работы слейва
  Slave1.pool();

  // Демонстрационная логика - раз в секунду изменяем значение переменной.

  if (flprog::isTimer(tempUnsigned, 1000))
  {
    tempUnsigned = millis();
    tempInt++;
  }

  /*
    Записываем значение в переменной  tempInt в Holder Registr с адресом 0.

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

  */
  Slave1.saveInteger(tempInt, FLPROG_HOLDING_REGISTR, 0);

  /*
    Дополнительные функции
    byte getLastError(); - получение кода последней ошибки слейва
  */
}