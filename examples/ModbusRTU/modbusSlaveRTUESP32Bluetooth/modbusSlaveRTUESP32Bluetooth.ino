//Подключаем необходимую библиотеку
#include "flprogModbusRTU.h"

// Задаем область памяти для хранения регистров Modbus Holder Registr первого слейва
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

/*
Повторяем действия для остальных таблиц Modbus  если есть необходимость
Порядок создания таблиц не имеет значенияa
*/
int _modbusSlaveDataTable_3[8];
int _modbusSlaveAddresTable_3[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr(INPUT_REGISTR, _modbusSlaveDataTable_3, _modbusSlaveAddresTable_3, 8);

bool _modbusSlaveDataTable_0[2];
int _modbusSlaveAddresTable_0[2] = {0, 1};
ModbusBoolTable Coil(FLPROG_COIL, _modbusSlaveDataTable_0, _modbusSlaveAddresTable_0, 2);

bool _modbusSlaveDataTable_1[2];
int _modbusSlaveAddresTable_1[2] = {0, 1};
ModbusBoolTable DiscreteInput(FLPROG_DISCRETE_INPUT, _modbusSlaveDataTable_1, _modbusSlaveAddresTable_1, 2);

// Создаём объект для управления всеми таблицами Modbus
ModbusMainData TestData;

//Создаем объект непосредстредственно Модбас слейва
ModbusSlaveRTU SlaveRTU1;
FLProgUart RtuDevice("ESP32-BL");

//Вспомогательные переменные для демонстрации
byte tempByte;
int tempInt=0;
float tempFloat;
long tempLong;
unsigned long tempUnsigned;
bool tempBool1;
bool tempBool2;
byte lastError;

void setup()
{
  SlaveRTU1.setUart(&RtuDevice);
 //Привязываем ссылки на объекты таблиц к объекту управления таблицами
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

//Передаем ссылку на  объект управления таблицами слейву
  SlaveRTU1.setData(&TestData);

    /*
  Устанавливаем имя для блютуз устройства (обязательно)
  По умолчанию устройство является блютуз слейвом
  Если необходим режим блютуз мастера необходимо вызвать функцию:

   void beBluetoothMaster();

  Но тогда необходимо задать имя устройства к которому данное устройство будет подключаться:

    void setPartnerName(String name);

  Переключить обратно на слейв:

   void beBluetoothSlave();

  Все эти функции можно вызывать в процессе выполнения программы
  */
  RtuDevice.setDeviceName("ESP32-BL");

  //Инициализируем слейв
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
  tempByte = SlaveRTU1.readByte(FLPROG_HOLDING_REGISTR, 1);
  tempInt = SlaveRTU1.readInteger(FLPROG_HOLDING_REGISTR, 0);
  tempFloat = SlaveRTU1.readFloat(FLPROG_HOLDING_REGISTR, 2);
  tempLong = SlaveRTU1.readLong(FLPROG_HOLDING_REGISTR, 6);
  tempUnsigned = SlaveRTU1.readUnsignedLong(FLPROG_HOLDING_REGISTR, 4);
  tempBool1 = SlaveRTU1.readBool(FLPROG_COIL, 0);
  tempBool2 = SlaveRTU1.readBool(FLPROG_COIL, 1);

  SlaveRTU1.saveLong(tempLong, INPUT_REGISTR, 6);
  SlaveRTU1.saveUnsignedLong(tempUnsigned, INPUT_REGISTR, 4);
  SlaveRTU1.saveFloat(tempFloat, INPUT_REGISTR, 2);
  SlaveRTU1.saveInteger(tempInt, INPUT_REGISTR, 0);
  SlaveRTU1.saveByte(tempByte, INPUT_REGISTR, 1);
  SlaveRTU1.saveBool(tempBool1, FLPROG_DISCRETE_INPUT, 0);
  SlaveRTU1.saveBool(tempBool2, FLPROG_DISCRETE_INPUT, 1);

  
// получение последней ошибки слейва
  lastError = SlaveRTU1.getLastError();
}