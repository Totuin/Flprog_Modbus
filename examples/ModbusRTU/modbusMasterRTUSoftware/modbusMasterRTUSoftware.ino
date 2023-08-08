//Подключаем необходимую библиотеку
#include "flprogModbusRTU.h"

// Задаем область памяти для хранения регистров Modbus Holder Registr первого слейва
int table_4_1[8];

/*
Создаем  таблицу адресов для регистров Modbus Holder Registr.
Для каждого регистра необходимо указать адрес.
Последовательность адресов любая, просто список необходимых адресов, сортировка так же не обязательна
Последовательно идущие адреса регистров даже при отсутствии сортировки будут опрашиваться одним запросом
*/
int tableA_4_1[8] = {0, 1, 2, 3, 4, 5, 6, 7};

/*
Создаем массив хранения флагов необходимости отправки новых данных на слейв
Обязателен для таблиц Holder Registr и Coil. Для других таблиц (INPUT_REGISTR и  FLPROG_DISCRETE_INPUT) создавать эти массивы не надо
*/
bool tableS_4_1[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/*
Создаем объект для управления регистрами Modbus Holder Registr и сразу передаём этому объекту созданные ранее массивы.
Последний параметр - количество регистров
*/
ModbusWorldTable Holder1(FLPROG_HOLDING_REGISTR, table_4_1, tableA_4_1, tableS_4_1, 8);

//Повторяем действия для второго сдейва
int table_4_2[8];
int tableA_4_2[8] = {0, 1, 2, 3, 4, 5, 6, 7};
bool tableS_4_2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
ModbusWorldTable Holder2(FLPROG_HOLDING_REGISTR, table_4_2, tableA_4_2, tableS_4_2, 8);

/*
Повторяем действия для остальных таблиц Modbus  если есть необходимость
Порядок создания таблиц не имеет значения
*/
int table_3_1[8];
int tableA_3_1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr1(INPUT_REGISTR, table_3_1, tableA_3_1, 8);
int table_3_2[8];
int tableA_3_2[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr2(INPUT_REGISTR, table_3_2, tableA_3_2, 8);

bool table_0_1[2];
int tableA_0_1[2] = {0, 1};
bool tableS_0_1[2] = {0, 0};
ModbusBoolTable Coil1(FLPROG_COIL, table_0_1, tableA_0_1, tableS_0_1, 2);
bool table_0_2[2];
int tableA_0_2[2] = {0, 1};
bool tableS_0_2[2] = {0, 0};
ModbusBoolTable Coil2(FLPROG_COIL, table_0_2, tableA_0_2, tableS_0_2, 2);

bool table_1_1[2];
int tableA_1_1[2] = {0, 1};
ModbusBoolTable DiscreteInput1(FLPROG_DISCRETE_INPUT, table_1_1, tableA_1_1, 2);
bool table_1_2[2];
int tableA_1_2[2] = {0, 1};
ModbusBoolTable DiscreteInput2(FLPROG_DISCRETE_INPUT, table_1_2, tableA_1_2, 2);

//Создаем массив слейвов с которыми работает мастер
ModbusSlaveInMaster Master1SlavesTable[2];

FLProgSoftwareUart RtuDevice(2,3);

//Создаем объект непосредстредственно Модбас мастера, последний параметр - количество слейвов
ModbusMasterRTU Master1(Master1SlavesTable, 2);

//Вспомогательные переменные для демонстрации
int tempInt;
bool tempBool;
int value;
unsigned long startTime;

void setup()
{

  Master1.setUart(&RtuDevice);
  //Инициализация вспомогательных переменных
  startTime = millis() + 1000;

  //Параметрируем первый слейв :

  //Устанавливаем слейв адрес
  Master1SlavesTable[0].setSlaveAddress(1);

  /*
Устанавливаем период опроса в милисекундах
Вызов не обязятелен, значение по умолчанию - 1000 миллисекунд
значение можно изменять в режиме выполнения программы
*/
  Master1SlavesTable[0].setPollingPeriod(1000);

  /*
Устанавливаем период таймаута для слейва
Вызов не обязятелен, значение по умолчанию - 1000 миллисекунд
значение можно изменять в режиме выполнения программы
*/
  Master1SlavesTable[0].setTimeOutTime(1000);

  //Привязываем ссылки на объекты таблиц к слейву
  Master1SlavesTable[0].setDataTable(&Holder1);
  Master1SlavesTable[0].setDataTable(&InputRegistr1);
  Master1SlavesTable[0].setDataTable(&Coil1);
  Master1SlavesTable[0].setDataTable(&DiscreteInput1);

  /*
Задаем последовательность байтов для хранения различных типов данных
Возможные знаыения:

Для Integer:
FLPROG_AB_ORDER
FLPROG_BA_ORDER

Для остальных типов:
ABCD_ORDER
FLPROG_CDAB_ORDER
FLPROG_BADC_ORDER
FLPROG_DCBA_ORDER

Вызов данных функций не обязателен.
По умолчанию заданны  значения  FLPROG_AB_ORDER  и ABCD_ORDER
Вызов данных функций возможен в любое время если необходимо изменить эти значения в режиме выполнения программы
*/
  Master1SlavesTable[0].setLongOrder(FLPROG_DCBA_ORDER);
  Master1SlavesTable[0].setFloatOrder(FLPROG_BADC_ORDER);
  Master1SlavesTable[0].setUnsignedlongOrder(FLPROG_CDAB_ORDER);
  Master1SlavesTable[0].setIntOrder(FLPROG_AB_ORDER);

  // Таким же образом настраиваем второй слейв
  Master1SlavesTable[1].setSlaveAddress(2);
  Master1SlavesTable[1].setPollingPeriod(100);
  Master1SlavesTable[1].setTimeOutTime(1000);
  Master1SlavesTable[1].setDataTable(&Holder2);
  Master1SlavesTable[1].setDataTable(&InputRegistr2);
  Master1SlavesTable[1].setDataTable(&Coil2);
  Master1SlavesTable[1].setDataTable(&DiscreteInput2);
  Master1SlavesTable[1].setLongOrder(FLPROG_DCBA_ORDER);
  Master1SlavesTable[1].setFloatOrder(FLPROG_BADC_ORDER);
  Master1SlavesTable[1].setUnsignedlongOrder(FLPROG_CDAB_ORDER);
  Master1SlavesTable[1].setIntOrder(FLPROG_AB_ORDER);

  //Если необходимо - задаём номер пина управления потоком приёма - передачи
  Master1.setPinPeDe(13);

  /*
 Параметрирузируем порт:
 Вызовы не обязательны по умолчанию параметры порта 9600 8N1
 Параметры порта можно менять во время исполнения программы

 Задаём скорость порта допустимые константы:
 FLPROG_SPEED_300
 FLPROG_SPEED_600
 FLPROG_SPEED_1200
 FLPROG_SPEED_2400
 FLPROG_SPEED_4800
 FLPROG_SPEED_9600
 FLPROG_SPEED_14400
 FLPROG_SPEED_19200
 FLPROG_SPEED_28800
 FLPROG_SPEED_38400
 FLPROG_SPEED_57600
 FLPROG_SPEED_115200
  */
  Master1.setPortSpeed(FLPROG_SPEED_14400);

  /*
  Инициализируем мастер
  Параметры:
  RX pin, TX pin
  */
  Master1.begin();
}

void loop()
{
  //Цикл работы мастера
  Master1.pool();

  //Демонстрационная логика - раз в секунду изменяем значение переменной.
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

       Где slave - индекс нужного слейва в таблице слейвов (НЕ АДРЕС СЛЕЙВА - ОН МОЖЕТ МЕНЯТСЯ!)
   */
  Master1.saveInteger(0, tempInt, FLPROG_HOLDING_REGISTR, 0);
  Master1.saveInteger(0, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveInteger(0, tempInt, FLPROG_HOLDING_REGISTR, 2);

  value = Master1.readInteger(1, INPUT_REGISTR, 1);

  Master1.saveBool(0, tempBool, FLPROG_COIL, 0);
  Master1.saveInteger(1, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveBool(1, tempBool, FLPROG_COIL, 1);
}