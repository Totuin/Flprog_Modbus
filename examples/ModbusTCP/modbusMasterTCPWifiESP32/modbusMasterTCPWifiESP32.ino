//Подключаем необходимую библиотеку
#include "flprogModbusTCP.h"

// Задаем данные для интернет соеденения
bool ESPControllerWifiClient_HRD = 0;
bool ESPControllerWifiClient_status = 1;
bool isWifiClientReconectWite = 0;
unsigned long startWifiClientReconectWite;
bool ESPControllerWifiClient_isDHCP = 0;
bool ESPControllerWifiClient_IsNeedReconect = 0;
bool ESPControllerWifiClient_workStatus = 1;
char ESPControllerWifiClient_SSID[40] = "yana";
char ESPControllerWifiClient_password[40] = "12345678";
IPAddress ESPControllerWifiClient_ip(192, 168, 1, 15);
IPAddress ESPControllerWifiClient_dns(192, 168, 1, 1);
IPAddress ESPControllerWifiClient_gateway(192, 168, 1, 1);
IPAddress ESPControllerWifiClient_subnet(255, 255, 255, 0);
uint8_t ESPControllerWifiClient_mac[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

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
Обязателен для таблиц Holder Registr и Coil. Для других таблиц (FLPROG_INPUT_REGISTR и  FLPROG_DISCRETE_INPUT) создавать эти массивы не надо
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
ModbusWorldTable InputRegistr1(FLPROG_INPUT_REGISTR, table_3_1, tableA_3_1, 8);
int table_3_2[8];
int tableA_3_2[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr2(FLPROG_INPUT_REGISTR, table_3_2, tableA_3_2, 8);

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

//Создаем массив серверов с которыми с которыми работает мастер
ModbusTCPSlaveServer Master1ServersTable[1];

/*
Создаем объект непосредстредственно Модбас мастера
одновременно привязывае к нему таблицу серверов
Последний параметр - количество серверов
*/
ModbusMasterTCP Master1(Master1ServersTable, 1);
FLProgWiFiTcpDevice TcpDevice;


//Вспомогательные переменные для демонстрации
int tempInt;
bool tempBool;
int value, oldValue;
unsigned long startTime;

void setup()
{

  Master1.setTcpDevice(&TcpDevice);
  // Привязываем к каждому серверу таблицу его слейвов, последний параметр - количество слейвов на сервере
  Master1ServersTable[0].setSlaves(Master1SlavesTable, 2);

  /*
 Настраиваем сервер:
 Эти параметры можно изменять в процессе выполнения программы

 Устанаовливаем номер порта
 */
  Master1ServersTable[0].setPort(502);

  //Устанавливаем адрес сервера
  Master1ServersTable[0].setIpAdress(192, 168, 1, 108);

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
  FLPROG_ABCD_ORDER
  FLPROG_CDAB_ORDER
  FLPROG_BADC_ORDER
  FLPROG_DCBA_ORDER

  Вызов данных функций не обязателен.
  По умолчанию заданны  значения  FLPROG_AB_ORDER  и FLPROG_ABCD_ORDER
  Вызов данных функций возможен в любое время если необходимо изменить эти значения в режиме выполнения программы
  */
  Master1SlavesTable[0].setLongOrder(FLPROG_DCBA_ORDER);
  Master1SlavesTable[0].setFloatOrder(FLPROG_BADC_ORDER);
  Master1SlavesTable[0].setUnsignedlongOrder(FLPROG_CDAB_ORDER);
  Master1SlavesTable[0].setIntOrder(FLPROG_AB_ORDER);

  // Таким же образом настраиваем второй слейв
  Master1SlavesTable[1].setSlaveAddress(2);
  Master1SlavesTable[1].setPollingPeriod(1000);
  Master1SlavesTable[1].setTimeOutTime(1000);
  Master1SlavesTable[1].setDataTable(&Holder2);
  Master1SlavesTable[1].setDataTable(&InputRegistr2);
  Master1SlavesTable[1].setDataTable(&Coil2);
  Master1SlavesTable[1].setDataTable(&DiscreteInput2);

 // Стартуем WiFi модуль
  WiFi.mode(WIFI_STA);
  if (WiFi.getAutoConnect() != true)
    WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  _esp32WifiModuleClientReconnect();
  _parseMacAddressString(WiFi.macAddress(), ESPControllerWifiClient_mac);


  //Инициализируем мастер
  Master1.begin();
}

void loop()
{
 // Код для работы с WiFi модулем
  if (ESPControllerWifiClient_IsNeedReconect)
  {
    _esp32WifiModuleClientReconnect();
    ESPControllerWifiClient_IsNeedReconect = 0;
  }
  ESPControllerWifiClient_status = WiFi.status() == WL_CONNECTED;
  if (ESPControllerWifiClient_status)
  {
    if (!ESPControllerWifiClient_HRD)
    {
      ESPControllerWifiClient_ip = WiFi.localIP();
      ESPControllerWifiClient_subnet = WiFi.subnetMask();
      ESPControllerWifiClient_gateway = WiFi.gatewayIP();
      ESPControllerWifiClient_dns = WiFi.dnsIP();
      ESPControllerWifiClient_HRD = 1;
    }
  }
  else
  {
    ESPControllerWifiClient_HRD = 0;
  }
  if (ESPControllerWifiClient_workStatus && (!ESPControllerWifiClient_status))
  {
    if (!isWifiClientReconectWite)
    {
      isWifiClientReconectWite = 1;
      startWifiClientReconectWite = millis();
    }
    else
    {
      if (_isTimer(startWifiClientReconectWite, 5000))
      {
        WiFi.disconnect();
        _esp32WifiModuleClientReconnect();
        isWifiClientReconectWite = 0;
      }
    }
  }

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
  value = Master1.readInteger(0, 0, FLPROG_INPUT_REGISTR, 0);

  Master1.saveBool(0, 0, tempBool, FLPROG_COIL, 0);
  Master1.saveBool(0, 0, tempBool, FLPROG_COIL, 1);
  Master1.saveBool(0, 1, tempBool, FLPROG_COIL, 1);
  Master1.saveBool(0, 1, tempBool, FLPROG_COIL, 0);
  Master1.saveInteger(0, 0, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveInteger(0, 0, tempInt, FLPROG_HOLDING_REGISTR, 2);
  Master1.saveInteger(0, 1, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveInteger(0, 1, tempInt, FLPROG_HOLDING_REGISTR, 2);
  Master1.saveInteger(0, 0, value, FLPROG_HOLDING_REGISTR, 0);
  Master1.saveInteger(0, 1, value, FLPROG_HOLDING_REGISTR, 0);
}

// Функции необходимые для работы с модулем WiFi не имеющие отношения к Modbus
bool _isTimer(unsigned long startTime, unsigned long period)
{
  unsigned long currentTime;
  currentTime = millis();
  if (currentTime >= startTime)
  {
    return (currentTime >= (startTime + period));
  }
  else
  {
    return (currentTime >= (4294967295 - startTime + period));
  }
}

int hexStrToInt(String instring)
{
  byte len = instring.length();
  if (len == 0)
    return 0;
  int result = 0;
  for (byte i = 0; i < 8; i++) // только первые 8 цыфар влезуть в uint32
  {
    char ch = instring[i];
    if (ch == 0)
      break;
    result <<= 4;
    if (isdigit(ch))
      result = result | (ch - '0');
    else
      result = result | (ch - 'A' + 10);
  }
  return result;
}
void _esp32WifiModuleClientReconnect()
{
  if (ESPControllerWifiClient_isDHCP)
  {
    WiFi.config(0U, 0U, 0U, 0U, 0U);
  }
  else
  {
    WiFi.config(ESPControllerWifiClient_ip, ESPControllerWifiClient_gateway, ESPControllerWifiClient_subnet, ESPControllerWifiClient_dns, ESPControllerWifiClient_dns);
  }
  WiFi.begin(ESPControllerWifiClient_SSID, ESPControllerWifiClient_password);
}
bool _checkMacAddres(byte array[])
{
  bool result = 0;
  for (byte i = 0; i < 6; i++)
  {
    if (array[i] == 255)
    {
      return 0;
    }
    if (array[i] > 0)
    {
      result = 1;
    }
  }
  return result;
}
void _parseMacAddressString(String value, byte array[])
{
  int index;
  byte buffer[6] = {255, 255, 255, 255, 255, 255};
  byte raz = 0;
  String tempString;
  while ((value.length() > 0) && (raz <= 6))
  {
    index = value.indexOf(":");
    if (index == -1)
    {
      tempString = value;
      value = "";
    }
    else
    {
      tempString = value.substring(0, index);
      value = value.substring(index + 1);
    }
    buffer[raz] = byte(hexStrToInt(tempString));
    raz++;
  }
  if (_checkMacAddres(buffer))
  {
    for (byte i = 0; i < 6; i++)
    {
      array[i] = buffer[i];
    }
  }
}
bool _compareMacAddreses(byte array1[], byte array2[])
{
  for (byte i = 0; i < 6; i++)
  {
    if (array1[i] != array2[i])
    {
      return 0;
    }
  }
  return 1;
}
bool _compareMacAddresWithString(byte array[], String value)
{
  byte buffer[6] = {255, 255, 255, 255, 255, 255};
  _parseMacAddressString(value, buffer);
  return _compareMacAddreses(array, buffer);
}
bool _checkMacAddresString(String value)
{
  byte buffer[6] = {255, 255, 255, 255, 255, 255};
  _parseMacAddressString(value, buffer);
  return _checkMacAddres(buffer);
}
String _macAddresToString(byte array[])
{
  String result = "";
  String temp = "";
  for (byte i = 0; i < 6; i++)
  {
    temp = String(array[i], HEX);
    if (temp.length() < 2)
    {
      temp = String("0") + temp;
    }
    result = result + temp;
    if (i < 5)
    {
      result = result + String(":");
    }
  }
  result.toUpperCase();
  return result;
}
