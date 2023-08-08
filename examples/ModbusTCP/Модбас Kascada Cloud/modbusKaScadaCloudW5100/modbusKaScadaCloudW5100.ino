// Подключаем необходимую библиотеку
#include "flprogModbusTCP.h"

// Создаем объект шины
FLProgSPI spiBus(0);

// Создаём интерфейс W5100
FlprogW5100Interface W5100_Interface(&spiBus, 10);

//Создаем объект непосредстредственно Модбас слейв на необходимом интерфейсе
ModbusKaScadaCloud Slave1(&W5100_Interface);

// Задаем данные для интернет соеденения
byte ethernet_mac[] = {0x78, 0xAC, 0xC0, 0x77, 0xE3, 0x05};
IPAddress ethernet_ip(192, 168, 199, 177);



//Вспомогательные переменные для демонстрации
int tempInt;
unsigned long tempUnsigned;

void setup()
{

  // Привязываем слейв к TCP устройству
  
  //Инициализация вспомогательных переменных
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

  // Стартуем модуль W5100
  W5100_Interface.begin(ethernet_mac, ethernet_ip);
  
  // Задержка на старт модуля
  delay(1000);

  //Инициализируем слейв
  Slave1.begin();
}

void loop()
{

  //Цикл работы слейва
  Slave1.pool();

  //Демонстрационная логика - раз в секунду изменяем значение переменной.

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