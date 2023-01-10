//Подключаем необходимую библиотеку
#include "flprogModbusSlaveRTU.h"

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
ModbusWorldTable Holder(HOLDING_REGISTR, _modbusSlaveDataTable_4, _modbusSlaveAddresTable_4, 8);

// Повторяем действия для остальных таблиц Modbus  если есть необходимость
int _modbusSlaveDataTable_3[8];
int _modbusSlaveAddresTable_3[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr(INPUT_REGISTR, _modbusSlaveDataTable_3, _modbusSlaveAddresTable_3, 8);

bool _modbusSlaveDataTable_0[2];
int _modbusSlaveAddresTable_0[2] = {0, 1};
ModbusBoolTable Coil(COIL, _modbusSlaveDataTable_0, _modbusSlaveAddresTable_0, 2);

bool _modbusSlaveDataTable_1[2];
int _modbusSlaveAddresTable_1[2] = {0, 1};
ModbusBoolTable DiscreteInput(DISCRETE_INPUT, _modbusSlaveDataTable_1, _modbusSlaveAddresTable_1, 2);

// Создаём объект для управления всеми таблицами Modbus
ModbusMainData TestData;

//Создаем объект непосредстредственно Модбас слейва
ModbusSlaveRTU SlaveRTU1;
FLProgUart uart1(2,3);


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

  SlaveRTU1.setUart(&uart1);
// Передаем слейву ссылку на обект управления таблицами
  SlaveRTU1.setData(&TestData);

  //Привязываем ссылки на объекты таблиц к объекту управления таблицами
  TestData.setDataTable(&Holder);
  TestData.setDataTable(&InputRegistr);
  TestData.setDataTable(&Coil);
  TestData.setDataTable(&DiscreteInput);

/*
  Задаем последовательность байтов для хранения различных типов данных
  Возможные знаыения:

  Для Integer:
  AB_ORDER
  BA_ORDER

  Для остальных типов:
  ABCD_ORDER
  CDAB_ORDER
  BADC_ORDER
  DCBA_ORDER

  Вызов данных функций не обязателен.
  По умолчанию заданны  значения  AB_ORDER  и ABCD_ORDER
  Вызов данных функций возможен в любое время если необходимо изменить эти значения в режиме выполнения программы
  */
  TestData.setLongOrder(DCBA_ORDER);
  TestData.setFloatOrder(BADC_ORDER);
  TestData.setUnsignedlongOrder(CDAB_ORDER);
  TestData.setIntOrder(AB_ORDER);

  //Если необходимо - задаём номер пина управления потоком приёма - передачи
  SlaveRTU1.setPinPeDe(13);

  /*
 Параметрирузируем порт:
 Вызовы не обязательны по умолчанию параметры порта 9600 8N1
 Параметры порта можно менять во время исполнения программы

 Задаём скорость порта допустимые константы:
 SPEED_300
 SPEED_600
 SPEED_1200
 SPEED_2400
 SPEED_4800
 SPEED_9600
 SPEED_14400
 SPEED_19200
 SPEED_28800
 SPEED_38400
 SPEED_57600
 SPEED_115200
  */
  SlaveRTU1.setPortSpeed(SPEED_14400);

   /*
  Инициализируем мастер
  Параметры:
   номер слейва, RX pin, TX pin
  */
  SlaveRTU1.begin(1);
}

void loop()
{
   //Цикл работы слейва
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
  tempByte = SlaveRTU1.readByte(HOLDING_REGISTR, 1);
  tempInt = SlaveRTU1.readInteger(HOLDING_REGISTR, 0);
  tempFloat = SlaveRTU1.readFloat(HOLDING_REGISTR, 2);
  tempLong = SlaveRTU1.readLong(HOLDING_REGISTR, 6);
  tempUnsigned = SlaveRTU1.readUnsignedLong(HOLDING_REGISTR, 4);
  tempBool1 = SlaveRTU1.readBool(COIL, 0);
  tempBool2 = SlaveRTU1.readBool(COIL, 1);

  SlaveRTU1.saveLong(tempLong, INPUT_REGISTR, 6);
  SlaveRTU1.saveUnsignedLong(tempUnsigned, INPUT_REGISTR, 4);
  SlaveRTU1.saveFloat(tempFloat, INPUT_REGISTR, 2);
  SlaveRTU1.saveInteger(tempInt, INPUT_REGISTR, 0);
  SlaveRTU1.saveByte(tempByte, INPUT_REGISTR, 1);
  SlaveRTU1.saveBool(tempBool1, DISCRETE_INPUT, 0);
  SlaveRTU1.saveBool(tempBool2, DISCRETE_INPUT, 1);

 // получение последней ошибки слейва
  lastError = SlaveRTU1.getLastError();
}