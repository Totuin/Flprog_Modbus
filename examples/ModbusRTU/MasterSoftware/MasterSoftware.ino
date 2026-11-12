// Подключаем необходимую библиотеку
#include "flprogModbusRTU.h"
// Создаем объект непосредстредственно Модбас мастера длядвух слейвов на програмном уарте (RX пин - 10, TX пин -11)(Для Arduino Mega)
ModbusMasterRTU Master1(10, 11, 2);

// Вспомогательные переменные для демонстрации
int tempInt;
bool tempBool;
int value, oldValue;
unsigned long startTime;

void setup()
{
  startTime = millis() + 1000;

  // Параметрируем первый слейв :
  // Устанавливаем слейв адрес (первый параметр - индекс слейва в мастере, второй параметр - задаваемый адрес)
  Master1.setSlaveAddress(0, 1);

  // Далее все работы со слейвом ведутся по адресу слейва (или по индексу если последним необязательным параметром передать true)
  Master1.setPollingPeriod(1, 1000);
  Master1.setTimeOutTime(1, 1000);

  // описываем таблицы слейва
  Master1.configDataTable(1, FLPROG_HOLDING_REGISTR, 8);
  Master1.configDataTable(1, FLPROG_INPUT_REGISTR, 8);
  Master1.configDataTable(1, FLPROG_COIL, 2);
  Master1.configDataTable(1, FLPROG_DISCRETE_INPUT, 2);

  // аналогично для второго слейва
  Master1.setSlaveAddress(1, 2);
  Master1.setPollingPeriod(2, 100);
  Master1.setTimeOutTime(2, 1000);
  Master1.configDataTable(2, FLPROG_HOLDING_REGISTR, 8);
  Master1.configDataTable(2, FLPROG_INPUT_REGISTR, 8);
  Master1.configDataTable(2, FLPROG_COIL, 2);
  Master1.configDataTable(2, FLPROG_DISCRETE_INPUT, 2);
}

void loop()
{

  // Цикл работы мастера
  Master1.pool();

  // Демонстрационная логика - раз в секунду изменяем значение переменной.
  if ((startTime + 1000) < millis())
  {
    startTime = millis();
    tempInt = tempInt + 1;
    tempBool = !tempBool;
  }
  /*
    Играемся с регистрами

    Доступные функции:
      void saveLong(int slave, long value, byte table, int startAddres);
      void saveUnsignedLong(int slave, unsigned long value, byte table, int startAddres);
      void saveFloat(int slave, float value, byte table, int startAddres);
      void saveInteger(int slave, int value, byte table, int startAddres);
      void saveByte(int slave, byte value, byte table, int startAddres);
      void saveBool(int slave, bool value, byte table, int startAddres);
      byte readByte(int slave, byte table, int startAddres);
      int readInteger(int slave, byte table, int startAddres);
      float readFloat(int slave, byte table, int startAddres);
      long readLong(int slave, byte table, int startAddres);
      unsigned long readUnsignedLong(int slave, byte table, int startAddres);
      bool readBool(int slave, byte table, int startAddres);

      Где slave - адрес нужного слейва
  */
  value = Master1.readInteger(1, FLPROG_INPUT_REGISTR, 0);

  Master1.saveBool(1, tempBool, FLPROG_COIL, 0);
  Master1.saveBool(1, !tempBool, FLPROG_COIL, 1);

  Master1.saveBool(2, !tempBool, FLPROG_COIL, 0);
  Master1.saveBool(2, tempBool, FLPROG_COIL, 1);

  Master1.saveInteger(1, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveInteger(1, tempInt, FLPROG_HOLDING_REGISTR, 2);
  Master1.saveInteger(2, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveInteger(2, tempInt, FLPROG_HOLDING_REGISTR, 2);
  Master1.saveInteger(1, value, FLPROG_HOLDING_REGISTR, 0);
  Master1.saveInteger(2, value, FLPROG_HOLDING_REGISTR, 0);
  Master1.saveInteger(1, tempInt, FLPROG_HOLDING_REGISTR, 6);
  Master1.saveInteger(2, tempInt, FLPROG_HOLDING_REGISTR, 1);
}