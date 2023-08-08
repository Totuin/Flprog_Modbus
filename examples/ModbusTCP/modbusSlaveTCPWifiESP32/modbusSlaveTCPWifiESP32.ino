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
IPAddress ESPControllerWifiClient_ip(192, 168, 1, 125);
IPAddress ESPControllerWifiClient_dns(192, 168, 1, 1);
IPAddress ESPControllerWifiClient_gateway(192, 168, 1, 1);
IPAddress ESPControllerWifiClient_subnet(255, 255, 255, 0);
uint8_t ESPControllerWifiClient_mac[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

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

// создаем сервер для прослушки порта


//Создаем объект непосредстредственно Модбас слейва и передаем ему ссылку на сервер
ModbusSlaveTCP Slave1;
FLProgWiFiTcpDevice TcpDevice;

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
  Slave1.setTcpDevice(&TcpDevice);

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
  Slave1.setData(&TestData);

  // Стартуем модуль WiFi
  WiFi.mode(WIFI_STA);
  if (WiFi.getAutoConnect() != true)
    WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  _esp32WifiModuleClientReconnect();
  _parseMacAddressString(WiFi.macAddress(), ESPControllerWifiClient_mac);

  //Инициализируем слейв
  Slave1.begin();
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
  tempByte = Slave1.readByte(FLPROG_HOLDING_REGISTR, 1);
  Slave1.saveByte(tempByte, FLPROG_INPUT_REGISTR, 1);

  tempInt = Slave1.readInteger(FLPROG_HOLDING_REGISTR, 0);
  tempFloat = Slave1.readFloat(FLPROG_HOLDING_REGISTR, 2);
  tempLong = Slave1.readLong(FLPROG_HOLDING_REGISTR, 6);
  tempUnsigned = Slave1.readUnsignedLong(FLPROG_HOLDING_REGISTR, 4);
  tempBool1 = Slave1.readBool(FLPROG_COIL, 0);
  tempBool2 = Slave1.readBool(FLPROG_COIL, 1);

  Slave1.saveLong(tempLong, FLPROG_INPUT_REGISTR, 6);
  Slave1.saveUnsignedLong(tempUnsigned, FLPROG_INPUT_REGISTR, 4);
  Slave1.saveFloat(tempFloat, FLPROG_INPUT_REGISTR, 2);
  Slave1.saveInteger(tempInt, FLPROG_INPUT_REGISTR, 0);
  Slave1.saveBool(tempBool1, FLPROG_DISCRETE_INPUT, 0);
  Slave1.saveBool(tempBool2, FLPROG_DISCRETE_INPUT, 1);

  // получение последней ошибки слейва
  lastError = Slave1.getLastError();
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
  for (byte i = 0; i < 8; i++)
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
