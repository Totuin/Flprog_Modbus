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
     Создаем объект непосредстредственно Модбас слейва на необходимом интерфейсе
  -----------------------------------------------------------------------------------------
*/
ModbusSlaveTCP Slave1(&WifiInterface);

/*
  -----------------------------------------------------------------------------------------
     Создаем таблицу адресов (необходимо при неупорядоченном порядке адресов регисторов)
  -----------------------------------------------------------------------------------------
*/

int _DiscreteInputAddreses[] = {8, 9, 10, 11, 12, 14, 15, 16, 17, 18};

/*
  -----------------------------------------------------------------------------------------
          Определение рабочих параметров и функций
  -----------------------------------------------------------------------------------------
*/

int counter = -10;
uint32_t startTimer;
int lastError;
uint32_t blinkStartTime = 0;
bool blinkVar = 0;

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
  WifiInterface.mac(0x78, 0xAC, 0xC0, 0x2C, 0x3E, 0x28);
  WifiInterface.localIP(IPAddress(192, 168, 199, 177));
  WifiInterface.resetDhcp();
  WifiInterface.setClientSsidd("totuin-router");
  WifiInterface.setClientPassword("12345678");

  Slave1.setSlaveAddress(1);
  // Slave1.byTcp(); // Режим по умолчанию
  // Slave1.byRtuOverTcp();

  // Slave1.byKasCadaCloud();
  // Slave1.setKaScadaCloudDevceId("DKK6T9TPE4");

  /*
    -----------------------------------------------------------------------------------------
          Описываем таблицы слейва
    -----------------------------------------------------------------------------------------
  */
  Slave1.configDataTable(FLPROG_HOLDING_REGISTR, 10);
  Slave1.configDataTable(FLPROG_INPUT_REGISTR, 10);
  Slave1.configDataTable(FLPROG_COIL, 10, 5);
  Slave1.setDataTable(FLPROG_DISCRETE_INPUT, 10, _DiscreteInputAddreses);

  /*
    -----------------------------------------------------------------------------------------
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

  Serial.begin(115200);
  while (!Serial)
  {
  }

  flprog::printConsole(" Тест Modbus Slave TCP - On board Wifi ");

  startTimer = millis();
  pinMode(LED_BUILTIN, OUTPUT);
}

//=================================================================================================
void loop()
{
  WifiInterface.pool(); // Цикл работы интерфейса
  Slave1.pool();        // Цикл работы слейва
  printStatusMessages();
  blinkLed();
  workModbus();
}

//=================================================================================================
void workModbus()
{
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
  */

  Slave1.saveInteger((Slave1.readInteger(FLPROG_HOLDING_REGISTR, 0)), FLPROG_INPUT_REGISTR, 9);
  Slave1.saveLong((Slave1.readLong(FLPROG_HOLDING_REGISTR, 6)), FLPROG_INPUT_REGISTR, 5);
  Slave1.saveInteger((Slave1.readFloat(FLPROG_HOLDING_REGISTR, 4) * 100), FLPROG_INPUT_REGISTR, 0);
  Slave1.saveUnsignedLong((Slave1.readUnsignedLong(FLPROG_HOLDING_REGISTR, 8)), FLPROG_INPUT_REGISTR, 7);
  Slave1.saveBool(blinkVar, FLPROG_COIL, 5);
  Slave1.saveBool(blinkVar, FLPROG_DISCRETE_INPUT, 8);
  Slave1.saveBool((Slave1.readBool(FLPROG_COIL, 6)), FLPROG_DISCRETE_INPUT, 9);

  if (flprog::isTimer(startTimer, 1000))
  {
    counter++;
    if (counter > 10)
    {
      counter = -10;
    }
    blinkVar = !blinkVar;
    Slave1.saveInteger(counter, FLPROG_INPUT_REGISTR, 6);
    Slave1.saveInteger(counter, FLPROG_HOLDING_REGISTR, 1);
    Slave1.saveFloat((counter / 10.0), FLPROG_HOLDING_REGISTR, 2);
    Slave1.saveFloat((counter / 100.0), FLPROG_INPUT_REGISTR, 4);
    startTimer = millis();
  }
  // получение последней ошибки слейва
  lastError = Slave1.getLastError();
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