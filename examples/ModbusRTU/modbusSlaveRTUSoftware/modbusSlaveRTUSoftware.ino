// Подключаем необходимую библиотеку
#include "flprogModbusRTU.h"

// Задаем область памяти для хранения регистров Modbus Holder Registr
int _modbusSlaveDataTable_4[8];

/*
Создаем  таблицу адресов для регистров Modbus Holder Registr.
Для каждого регистра необходимо указать адрес.
Последовательность адресов любая, просто список необходимых адресов, сортировка так же не обязательна
*/
int _modbusSlaveAddresTable_4[8] = {0, 1, 2, 3, 4, 5, 6, 7};

/*
Создаем объект для управления регистрами Modbus Holder Registr и сразу передаём этому объекту созданные ранее массивы.
Последний параметр - количество регистров
*/
ModbusWorldTable Holder(FLPROG_HOLDING_REGISTR, _modbusSlaveDataTable_4, _modbusSlaveAddresTable_4, 8);

// Повторяем действия для остальных таблиц Modbus  если есть необходимость
int _modbusSlaveDataTable_3[8];
int _modbusSlaveAddresTable_3[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr(FLPROG_INPUT_REGISTR, _modbusSlaveDataTable_3, _modbusSlaveAddresTable_3, 8);

bool _modbusSlaveDataTable_0[2];
int _modbusSlaveAddresTable_0[2] = {0, 1};
ModbusBoolTable Coil(FLPROG_COIL, _modbusSlaveDataTable_0, _modbusSlaveAddresTable_0, 2);

bool _modbusSlaveDataTable_1[2];
int _modbusSlaveAddresTable_1[2] = {0, 1};
ModbusBoolTable DiscreteInput(FLPROG_DISCRETE_INPUT, _modbusSlaveDataTable_1, _modbusSlaveAddresTable_1, 2);

// Создаём объект для управления всеми таблицами Modbus
ModbusMainData TestData;

// Создаем объект непосредстредственно Модбас слейва
ModbusSlaveRTU SlaveRTU1;
FLProgSoftwareUart RtuDevice(2, 3);

byte tempByte;
int tempInt;
float tempFloat;
long tempLong;
unsigned long tempUnsigned;
bool tempBool1;
bool tempBool2;
byte lastError;

void setup()
{

  SlaveRTU1.setUart(&RtuDevice);
  // Передаем слейву ссылку на обект управления таблицами
  SlaveRTU1.setData(&TestData);

  // Привязываем ссылки на объекты таблиц к объекту управления таблицами
  TestData.setDataTable(&Holder);
  TestData.setDataTable(&InputRegistr);
  TestData.setDataTable(&Coil);
  TestData.setDataTable(&DiscreteInput);

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
  TestData.setLongOrder(FLPROG_DCBA_ORDER);
  TestData.setFloatOrder(FLPROG_BADC_ORDER);
  TestData.setUnsignedlongOrder(FLPROG_CDAB_ORDER);
  TestData.setIntOrder(FLPROG_AB_ORDER);

  // Если необходимо - задаём номер пина управления потоком приёма - передачи
  SlaveRTU1.setPinPeDe(13);

  /*
 Параметрирузируем порт:
 Вызовы не обязательны по умолчанию параметры порта 9600 8N1
 Параметры порта можно менять во время исполнения программы

 Задаём скорость порта допустимые константы:
 FLPROG_SPEED_300
 FLPROG_SPEED_600
 FLPROG_SPEED_1200
 FLPROG_PEED_2400
 FLPROG_SPEED_4800
 FLPROG_SPEED_9600
 FLPROG_SPEED_14400
 FLPROG_SPEED_19200
 FLPROG_SPEED_28800
 FLPROG_SPEED_38400
 FLPROG_SPEED_57600
 FLPROG_SPEED_115200
  */
  SlaveRTU1.setPortSpeed(FLPROG_SPEED_14400);

  /*
 Инициализируем мастер
 Параметры:
  номер слейва, RX pin, TX pin
 */
  SlaveRTU1.begin(1);
}

void loop()
{
  // Цикл работы слейва
  SlaveRTU1.pool();

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
*/
  tempByte = SlaveRTU1.readByte(FLPROG_HOLDING_REGISTR, 1);
  tempInt = SlaveRTU1.readInteger(FLPROG_HOLDING_REGISTR, 0);
  tempFloat = SlaveRTU1.readFloat(FLPROG_HOLDING_REGISTR, 2);
  tempLong = SlaveRTU1.readLong(FLPROG_HOLDING_REGISTR, 6);
  tempUnsigned = SlaveRTU1.readUnsignedLong(FLPROG_HOLDING_REGISTR, 4);
  tempBool1 = SlaveRTU1.readBool(FLPROG_COIL, 0);
  tempBool2 = SlaveRTU1.readBool(FLPROG_COIL, 1);

  SlaveRTU1.saveLong(tempLong, FLPROG_INPUT_REGISTR, 6);
  SlaveRTU1.saveUnsignedLong(tempUnsigned, FLPROG_INPUT_REGISTR, 4);
  SlaveRTU1.saveFloat(tempFloat, FLPROG_INPUT_REGISTR, 2);
  SlaveRTU1.saveInteger(tempInt, FLPROG_INPUT_REGISTR, 0);
  SlaveRTU1.saveByte(tempByte, FLPROG_INPUT_REGISTR, 1);
  SlaveRTU1.saveBool(tempBool1, FLPROG_DISCRETE_INPUT, 0);
  SlaveRTU1.saveBool(tempBool2, FLPROG_DISCRETE_INPUT, 1);

  // получение последней ошибки слейва
  lastError = SlaveRTU1.getLastError();
}