//Подключаем необходимую библиотеку
#include "flprogModbusTCP.h"

FLProgOnBoardWifi WifiInterface;

/*  Создаем объект непосредстредственно Модбас мастера на необходимом интерфейсе
  Второй параметр - количество серверов
*/
ModbusMasterTCP Master1(&WifiInterface, 1);
//так же можно создать мастера RTU OVER TCP
//ModbusMasterRTUoverTCP Master1(&WifiInterface, 1);

//Вспомогательные переменные для демонстрации
int tempInt;
bool tempBool;
int value, oldValue;
unsigned long startTime;

void setup()
{

 WifiInterface.clientOn();
  WifiInterface.mac(0x78, 0xAC, 0xC0, 0x2C, 0x3E, 0x28);
  WifiInterface.localIP(IPAddress(192, 168, 1, 177));
  WifiInterface.resetDhcp();
  WifiInterface.setClientSsidd("yana");
  WifiInterface.setClientPassword("12345678");


  //Задаём порт для сервера
  Master1.setServerPort(0, 502);
  //Устанавливаем IP адрес сервера
  Master1.setServerIpAdress(0, IPAddress(192, 168, 1, 108));

  //Задаём количество слейвов на сервере
  Master1.setServerSlavesSize(0, 2);

  //Задаём адреса для слейвов
  Master1.setSlaveAddress(0, 0, 1);
  Master1.setSlaveAddress(0, 1, 2);

  //задаём таблицы для  слейва с адресом 1 на сервере 0
  Master1.configDataTable(0, 1, FLPROG_HOLDING_REGISTR,  8);
  Master1.configDataTable(0, 1, FLPROG_INPUT_REGISTR,  8);
  Master1.configDataTable(0, 1, FLPROG_COIL, 2);
  Master1.configDataTable(0, 1, FLPROG_DISCRETE_INPUT, 2);

  //так же для второго слейва
  Master1.configDataTable(0, 2, FLPROG_HOLDING_REGISTR,  8);
  Master1.configDataTable(0, 2, FLPROG_INPUT_REGISTR,  8);
  Master1.configDataTable(0, 2, FLPROG_COIL, 2);
  Master1.configDataTable(0, 2, FLPROG_DISCRETE_INPUT, 2);

  /*
    Устанавливаем период опроса в милисекундах
    Вызов не обязятелен, значение по умолчанию - 1000 миллисекунд
    значение можно изменять в режиме выполнения программы
  */
  Master1.setPollingPeriod(0, 1, 1000);
  Master1.setPollingPeriod(0, 2, 1000);

  /*
    Устанавливаем период таймаута для слейва
    Вызов не обязятелен, значение по умолчанию - 1000 миллисекунд
    значение можно изменять в режиме выполнения программы
  */
  Master1.setTimeOutTime(0, 1, 1000);
  Master1.setTimeOutTime(0, 2, 1000);

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
  Master1.setLongOrder(0, 1, FLPROG_ABCD_ORDER);
  Master1.setFloatOrder(0, 1, FLPROG_ABCD_ORDER);
  Master1.setUnsignedlongOrder(0, 1, FLPROG_ABCD_ORDER);
  Master1.setIntOrder(0, 1, FLPROG_AB_ORDER);

  Master1.setLongOrder(0, 2, FLPROG_ABCD_ORDER);
  Master1.setFloatOrder(0, 2, FLPROG_ABCD_ORDER);
  Master1.setUnsignedlongOrder(0, 2, FLPROG_ABCD_ORDER);
  Master1.setIntOrder(0, 2, FLPROG_AB_ORDER);
}

void loop()
{
  // Цикл работы интерфейса
  WifiInterface.pool();
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
       slave - адрес нужного слейва
  */
  value = Master1.readInteger(0, 1, FLPROG_INPUT_REGISTR, 0);

  Master1.saveBool(0, 1, ! tempBool, FLPROG_COIL, 0);

  Master1.saveBool(0, 2, tempBool, FLPROG_COIL, 1);

  Master1.saveInteger(0, 1, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveInteger(0, 1, tempInt, FLPROG_HOLDING_REGISTR, 2);
  Master1.saveInteger(0, 2, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveInteger(0, 2, tempInt, FLPROG_HOLDING_REGISTR, 2);
  Master1.saveInteger(0, 1, value, FLPROG_HOLDING_REGISTR, 0);
  Master1.saveInteger(0, 2, value, FLPROG_HOLDING_REGISTR, 0);
}