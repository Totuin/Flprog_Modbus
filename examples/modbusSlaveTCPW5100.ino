//Подключаем необходимую библиотеку
#include "flprogModbusSlaveTCPW5100.h"

// Задаем данные для интернет соеденения
byte ethernet_mac[] = {0x78, 0xAC, 0xC0, 0x77, 0xE3, 0x05};
IPAddress ethernet_ip(192, 168, 0, 5);
byte ethernet_dns[] = {192, 168, 0, 1};
byte ethernet_gateway[] = {192, 168, 0, 1};
byte ethernet_subnet[] = {255, 255, 255, 0};

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

// создаем сервер для прослушки порта
EthernetServer tempServer(502);

//Создаем объект непосредстредственно Модбас слейва и передаем ему ссылку на сервер
ModbusSlaveTCPW5100 Slave1(&tempServer);

//Вспомогательные переменные для демонстрации
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

    //Передаем ссылку на  объект управления таблицами слейву
    Slave1.setData(&TestData);
    // Стартуем модуль W5100
    Ethernet.begin(ethernet_mac, ethernet_ip, ethernet_dns, ethernet_gateway, ethernet_subnet);

    // Задержка на старт модуля
    delay(1000);

    //Инициализируем слейв
    Slave1.begin();
}

void loop()
{
    //Цикл работы слейва
    Slave1.pool();

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
    tempByte = Slave1.readByte(HOLDING_REGISTR, 1);
    Slave1.saveByte(tempByte, INPUT_REGISTR, 1);

    tempInt = Slave1.readInteger(HOLDING_REGISTR, 0);
    tempFloat = Slave1.readFloat(HOLDING_REGISTR, 2);
    tempLong = Slave1.readLong(HOLDING_REGISTR, 6);
    tempUnsigned = Slave1.readUnsignedLong(HOLDING_REGISTR, 4);
    tempBool1 = Slave1.readBool(COIL, 0);
    tempBool2 = Slave1.readBool(COIL, 1);

    Slave1.saveLong(tempLong, INPUT_REGISTR, 6);
    Slave1.saveUnsignedLong(tempUnsigned, INPUT_REGISTR, 4);
    Slave1.saveFloat(tempFloat, INPUT_REGISTR, 2);
    Slave1.saveInteger(tempInt, INPUT_REGISTR, 0);
    Slave1.saveBool(tempBool1, DISCRETE_INPUT, 0);
    Slave1.saveBool(tempBool2, DISCRETE_INPUT, 1);

    // получение последней ошибки слейва
    lastError = Slave1.getLastError();
}