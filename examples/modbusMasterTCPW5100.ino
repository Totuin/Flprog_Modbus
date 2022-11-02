//Подключаем необходимую библиотеку
#include "flprogModbusMasterTCPW5100.h"
// Задаем данные для интернет соеденения
byte ethernet_mac[] = {0x78, 0xAC, 0xC0, 0x77, 0xE3, 0x05};
IPAddress ethernet_ip(192, 168, 0, 5);
byte ethernet_dns[] = {192, 168, 0, 1};
byte ethernet_gateway[] = {192, 168, 0, 1};
byte ethernet_subnet[] = {255, 255, 255, 0};

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
Обязателен для таблиц Holder Registr и Coil. Для других таблиц (INPUT_REGISTR и  DISCRETE_INPUT) создавать эти массивы не надо
*/
bool tableS_4_1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
/*
Создаем объект для управления регистрами Modbus Holder Registr и сразу передаём этому объекту созданные ранее массивы.
Последний параметр - количество регистров
*/
ModbusWorldTable Holder1(HOLDING_REGISTR, table_4_1, tableA_4_1, tableS_4_1, 8);

//Повторяем действия для второго сдейва
int table_4_2[8];
int tableA_4_2[8] = {0, 1, 2, 3, 4, 5, 6, 7};
bool tableS_4_2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
ModbusWorldTable Holder2(HOLDING_REGISTR, table_4_2, tableA_4_2, tableS_4_2, 8);

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
ModbusBoolTable Coil1(COIL, table_0_1, tableA_0_1, tableS_0_1, 2);
bool table_0_2[2];
int tableA_0_2[2] = {0, 1};
bool tableS_0_2[2] = {0, 0};
ModbusBoolTable Coil2(COIL, table_0_2, tableA_0_2, tableS_0_2, 2);

bool table_1_1[2];
int tableA_1_1[2] = {0, 1};
ModbusBoolTable DiscreteInput1(DISCRETE_INPUT, table_1_1, tableA_1_1, 2);
bool table_1_2[2];
int tableA_1_2[2] = {0, 1};
ModbusBoolTable DiscreteInput2(DISCRETE_INPUT, table_1_2, tableA_1_2, 2);

//Создаем массив слейвов с которыми работает мастер
ModbusSlaveInMaster Master1SlavesTable[2];

//Создаем массив серверов с которыми с которыми работает мастер
ModbusTCPW5100SlaveServer Master1ServersTable[1];

/*
Создаем объект непосредстредственно Модбас мастера
одновременно привязывае к нему таблицу серверов
Последний параметр - количество серверов
*/
ModbusMasterTCPW5100 Master1(Master1ServersTable, 1);

//Вспомогательные переменные для демонстрации
int tempInt;
bool tempBool;
int value, oldValue;
unsigned long startTime;

void setup()
{
  // Привязываем к каждому серверу таблицу его слейвов, последний параметр - количество слейвов на сервере
  Master1ServersTable[0].setSlaves(Master1SlavesTable, 2);

  /*
  Настраиваем сервер:
  Эти параметры можно изменять в процессе выполнения программы

  Устанаовливаем номер порта
  */
  Master1ServersTable[0].setPort(502);

  //Устанавливаем адрес сервера
  Master1ServersTable[0].setIpAdress(192, 168, 0, 100);

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
  Master1SlavesTable[0].setLongOrder(DCBA_ORDER);
  Master1SlavesTable[0].setFloatOrder(BADC_ORDER);
  Master1SlavesTable[0].setUnsignedlongOrder(CDAB_ORDER);
  Master1SlavesTable[0].setIntOrder(AB_ORDER);

  // Таким же образом настраиваем второй слейв
  Master1SlavesTable[1].setSlaveAddress(2);
  Master1SlavesTable[1].setPollingPeriod(1000);
  Master1SlavesTable[1].setTimeOutTime(1000);
  Master1SlavesTable[1].setDataTable(&Holder2);
  Master1SlavesTable[1].setDataTable(&InputRegistr2);
  Master1SlavesTable[1].setDataTable(&Coil2);
  Master1SlavesTable[1].setDataTable(&DiscreteInput2);

  // Стартуем модуль W5100
  Ethernet.begin(ethernet_mac, ethernet_ip, ethernet_dns, ethernet_gateway, ethernet_subnet);

  // Задержка на старт модуля
  delay(1000);

  //Инициализируем мастер
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
     void saveLong(int server, int slave, long value, byte table, int startAddres);
     void saveUnsignedLong(int server, int slave, unsigned long value, byte table, int startAddres);
     void saveFloat(int server, int slave, float value, byte table, int startAddres);
     void saveInteger(int server, int slave, int value, byte table, int startAddres);
     void saveByte(int server, int slave, byte value, byte table, int startAddres);
     void saveBool(int server, int slave, bool value, byte table, int startAddres);
     byte readByte(int server, int slave, byte table, int startAddres);
     int readInteger(int server, int slave, byte table, int startAddres);
     float readFloat(int server, int slave, byte table, int startAddres);
     long readLong(int server, int slave, byte table, int startAddres);
     unsigned long readUnsignedLong(int server, int slave, byte table, int startAddres);
     bool readBool(int server, int slave, byte table, int startAddres);

       Где
       server - индекс нужного сервера в таблице серверов
       slave - индекс нужного слейва в таблице слейвов (НЕ АДРЕС СЛЕЙВА - ОН МОЖЕТ МЕНЯТСЯ!)
   */
  value = Master1.readInteger(0, 0, INPUT_REGISTR, 0);

  Master1.saveBool(0, 0, tempBool, COIL, 0);
  Master1.saveBool(0, 0, tempBool, COIL, 1);
  Master1.saveBool(0, 1, tempBool, COIL, 1);
  Master1.saveBool(0, 1, tempBool, COIL, 0);
  Master1.saveInteger(0, 0, tempInt, HOLDING_REGISTR, 1);
  Master1.saveInteger(0, 0, tempInt, HOLDING_REGISTR, 2);
  Master1.saveInteger(0, 1, tempInt, HOLDING_REGISTR, 1);
  Master1.saveInteger(0, 1, tempInt, HOLDING_REGISTR, 2);
  Master1.saveInteger(0, 0, value, HOLDING_REGISTR, 0);
  Master1.saveInteger(0, 1, value, HOLDING_REGISTR, 0);
}