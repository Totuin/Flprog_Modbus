// Подключаем необходимую библиотеку
#include "flprogModbusTCP.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

/*
  -------------------------------------------------------------------------------------------------
        Создание интерфейса для работы с Wifi интерфейсом

  -------------------------------------------------------------------------------------------------
*/
FLProgOnBoardWifiInterface WifiInterface;

/*
  -----------------------------------------------------------------------------------------
     Создаем объект непосредстредственно Модбас мастера на необходимом интерфейсе
  -----------------------------------------------------------------------------------------
*/
// ModbusMasterTCP Master1(&WifiInterface, 1);
ModbusMasterRTUoverTCP Master1(&WifiInterface, 1);

/*
  -----------------------------------------------------------------------------------------
          Определение рабочих параметров и функций
  -----------------------------------------------------------------------------------------
*/

int tempInt;
bool tempBool;
int value, oldValue;
unsigned long startTime;
uint32_t blinkStartTime = 0;

uint8_t ethernetStatus = 255;
uint8_t ethernetError = 255;

bool isNeedClientSendConnectMessage = true;
bool isNeedClientSendDisconnectMessage = true;

bool isNeedApSendConnectMessage = true;
bool isNeedApSendDisconnectMessage = true;

//=================================================================================================
void setup()
{
  /*
     -----------------------------------------------------------------------------------------
         Настройка интерфейса
     -----------------------------------------------------------------------------------------
  */
  WifiInterface.clientOn();
  WifiInterface.mac(0x78, 0xAC, 0xC0, 0x2C, 0x30, 0x45);
  WifiInterface.localIP(IPAddress(192, 168, 199, 38));
  WifiInterface.resetDhcp();
  WifiInterface.setClientSsidd("totuin-router");
  WifiInterface.setClientPassword("12345678");

  /*
     -----------------------------------------------------------------------------------------
            Настройка модбас мастера
     -----------------------------------------------------------------------------------------
  */
  Master1.setServerPort(0, 502);                               // Задаём порт для сервера
  Master1.setServerIpAdress(0, IPAddress(192, 168, 199, 199)); // Устанавливаем IP адрес сервера
  Master1.setServerSlavesSize(0, 2);                           // Задаём количество слейвов на сервере
  Master1.setSlaveAddress(0, 0, 1);                            // Задаём адреса для слейвов
  Master1.setSlaveAddress(0, 1, 2);                            // Задаём адреса для слейвов

  // задаём таблицы для  слейва с адресом 1 на сервере 0
  Master1.configDataTable(0, 1, FLPROG_HOLDING_REGISTR, 8);
  Master1.configDataTable(0, 1, FLPROG_INPUT_REGISTR, 8);
  Master1.configDataTable(0, 1, FLPROG_COIL, 2);
  Master1.configDataTable(0, 1, FLPROG_DISCRETE_INPUT, 2);

  // так же для второго слейва
  Master1.configDataTable(0, 2, FLPROG_HOLDING_REGISTR, 8);
  Master1.configDataTable(0, 2, FLPROG_INPUT_REGISTR, 8);
  Master1.configDataTable(0, 2, FLPROG_COIL, 2);
  Master1.configDataTable(0, 2, FLPROG_DISCRETE_INPUT, 2);

  /*
    Устанавливаем период опроса в милисекундах
    Вызов не обязятелен, значение по умолчанию - 1000 миллисекунд
    значение можно изменять в режиме выполнения программы
  */
  Master1.setPollingPeriod(0, 1, 100);
  Master1.setPollingPeriod(0, 2, 100);

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

  Serial.begin(115200);
  while (!Serial)
  {
  }

  flprog::printConsole(" Тест Modbus Slave TCP - WIFI ");

  startTime = millis();
  pinMode(LED_BUILTIN, OUTPUT);
}
//=================================================================================================
void loop()
{
  WifiInterface.pool(); // Цикл работы интерфейса
  Master1.pool();       // Цикл работы мастера
  printStatusMessages();
  blinkLed();
  workModbus();
}

//=================================================================================================
void workModbus()
{

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

  Master1.saveBool(0, 1, !tempBool, FLPROG_COIL, 0);

  Master1.saveBool(0, 2, tempBool, FLPROG_COIL, 1);

  Master1.saveInteger(0, 1, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveInteger(0, 1, tempInt, FLPROG_HOLDING_REGISTR, 2);
  Master1.saveInteger(0, 2, tempInt, FLPROG_HOLDING_REGISTR, 1);
  Master1.saveInteger(0, 2, tempInt, FLPROG_HOLDING_REGISTR, 2);
  Master1.saveInteger(0, 1, value, FLPROG_HOLDING_REGISTR, 0);
  Master1.saveInteger(0, 2, value, FLPROG_HOLDING_REGISTR, 0);
}

void blinkLed()
{
  if (flprog::isTimer(blinkStartTime, 50))
  {
    blinkStartTime = millis();
    digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
  }
}

void printStatusMessages()
{
  if (WifiInterface.getStatus() != ethernetStatus)
  {
    ethernetStatus = WifiInterface.getStatus();
    Serial.println();
    Serial.print("Статус интерфейса - ");
    Serial.println(flprog::flprogStatusCodeName(ethernetStatus));
  }
  if (WifiInterface.getError() != ethernetError)
  {
    ethernetError = WifiInterface.getError();
    if (ethernetError != FLPROG_NOT_ERROR)
    {
      Serial.println();
      Serial.print("Ошибка интерфейса - ");
      Serial.println(flprog::flprogErrorCodeName(ethernetError));
    }
  }
  printClientConnectMessages();
  printClientDisconnectMessages();
  printApConnectMessages();
  printApDisconnectMessages();
}

void printClientConnectMessages()
{
  if (!WifiInterface.clientIsReady())
  {
    return;
  }
  if (!isNeedClientSendConnectMessage)
  {
    return;
  }
  Serial.println("WiFiClient подключён!");
  Serial.print("Ssid - ");
  Serial.println(WifiInterface.clientSsid());
  Serial.print("Ip - ");
  Serial.println(WifiInterface.localIP());
  Serial.print("DNS - ");
  Serial.println(WifiInterface.dns());
  Serial.print("Subnet - ");
  Serial.println(WifiInterface.subnet());
  Serial.print("Gateway - ");
  Serial.println(WifiInterface.gateway());
  Serial.println();
  isNeedClientSendConnectMessage = false;
  isNeedClientSendDisconnectMessage = true;
}

void printApConnectMessages()
{
  if (!WifiInterface.apIsReady())
  {
    return;
  }
  if (!isNeedApSendConnectMessage)
  {
    return;
  }
  Serial.println("WiFi точка включенна!");
  Serial.print("Ssid - ");
  Serial.println(WifiInterface.apSsid());
  Serial.print("Ip - ");
  Serial.println(WifiInterface.apLocalIP());
  Serial.print("DNS - ");
  Serial.println(WifiInterface.apDns());
  Serial.print("Subnet - ");
  Serial.println(WifiInterface.apSubnet());
  Serial.print("Gateway - ");
  Serial.println(WifiInterface.apGateway());
  Serial.println();
  isNeedApSendConnectMessage = false;
  isNeedApSendDisconnectMessage = true;
}

void printClientDisconnectMessages()
{
  if (WifiInterface.clientIsReady())
  {
    return;
  }
  if (isNeedClientSendConnectMessage)
  {
    return;
  }
  if (!isNeedClientSendDisconnectMessage)
  {
    return;
  }
  Serial.println("WiFiClient отключён!");
  isNeedClientSendConnectMessage = true;
  isNeedClientSendDisconnectMessage = false;
}

void printApDisconnectMessages()
{
  if (WifiInterface.apIsReady())
  {
    return;
  }
  if (isNeedApSendConnectMessage)
  {
    return;
  }
  if (!isNeedApSendDisconnectMessage)
  {
    return;
  }
  Serial.println("WiFi точка отключёна!");
  isNeedApSendConnectMessage = true;
  isNeedApSendDisconnectMessage = false;
}
