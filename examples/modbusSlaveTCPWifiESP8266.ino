#include <ESP8266WiFi.h>
#include "flprogModbusSlaveTCPWifiESP8266.h"
extern "C"
{
#include "user_interface.h"
}

bool ESPControllerWifiClient_HRD = 0;
bool ESPControllerWifiClient_status = 1;
bool ESPControllerWifiClient_isDHCP = 0;
bool ESPControllerWifiClient_IsNeedReconect = 0;
bool ESPControllerWifiClient_workStatus = 1;
char ESPControllerWifiClient_SSID[40] = "Totuin_Work";
char ESPControllerWifiClient_password[40] = "12345678";
IPAddress ESPControllerWifiClient_ip(192, 168, 0, 15);
IPAddress  ESPControllerWifiClient_dns (192, 168, 0, 1);
IPAddress  ESPControllerWifiClient_gateway (192, 168, 0, 1);
IPAddress ESPControllerWifiClient_subnet (255, 255, 255, 0);
uint8_t ESPControllerWifiClient_mac[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};


int _modbusSlaveDataTable_4[8];
int _modbusSlaveAddresTable_4[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable Holder(HOLDING_REGISTR, _modbusSlaveDataTable_4, _modbusSlaveAddresTable_4, 8);

int _modbusSlaveDataTable_3[8];
int _modbusSlaveAddresTable_3[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr(INPUT_REGISTR, _modbusSlaveDataTable_3, _modbusSlaveAddresTable_3, 8);

bool _modbusSlaveDataTable_0[2];
int _modbusSlaveAddresTable_0[2] = {0, 1};
ModbusBoolTable Coil(COIL, _modbusSlaveDataTable_0, _modbusSlaveAddresTable_0, 2);

bool _modbusSlaveDataTable_1[2];
int _modbusSlaveAddresTable_1[2] = {0, 1};
ModbusBoolTable DiscreteInput(DISCRETE_INPUT, _modbusSlaveDataTable_1, _modbusSlaveAddresTable_1, 2);

ModbusMainData TestData;

WiFiServer tempServer(502);

ModbusSlaveTCPWifiESP8266 Slave1(&tempServer);

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

  WiFi.mode(WIFI_STA);
  if (WiFi.getAutoConnect() != true) WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  Serial.begin(9600);
  TestData.setDataTable(&Holder);
  TestData.setDataTable(&InputRegistr);
  TestData.setDataTable(&Coil);
  TestData.setDataTable(&DiscreteInput);

  TestData.setLongOrder(DCBA_ORDER);
  TestData.setFloatOrder(BADC_ORDER);
  TestData.setUnsignedlongOrder(CDAB_ORDER);
  TestData.setIntOrder(AB_ORDER);

  Slave1.setData(&TestData);




  _esp8266WifiModuleClientReconnect();

  Slave1.begin();
}

void loop()
{
  if (ESPControllerWifiClient_IsNeedReconect)
  {
    _esp8266WifiModuleClientReconnect();
    ESPControllerWifiClient_IsNeedReconect = 0;
  }
  ESPControllerWifiClient_status = WiFi.status() == WL_CONNECTED;
  if (ESPControllerWifiClient_status)
  {
    if (! ESPControllerWifiClient_HRD)
    {
      ESPControllerWifiClient_ip =  WiFi.localIP();
      ESPControllerWifiClient_subnet =  WiFi.subnetMask();
      ESPControllerWifiClient_gateway =  WiFi.gatewayIP();
      ESPControllerWifiClient_dns =  WiFi.dnsIP();
      WiFi.macAddress(ESPControllerWifiClient_mac);
      ESPControllerWifiClient_HRD = 1;
    }
  }
  else
  {
    ESPControllerWifiClient_HRD = 0;
  }



  Slave1.pool();

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

  lastError = Slave1.getLastError();


}


void _esp8266WifiModuleClientReconnect()
{
  if (_checkMacAddres(ESPControllerWifiClient_mac))
  {
    wifi_set_macaddr(0, const_cast<uint8*>(ESPControllerWifiClient_mac));
  }
  if (ESPControllerWifiClient_isDHCP)
  {
    WiFi.config(0U, 0U, 0U, 0U, 0U);
  }
  else
  {
    WiFi.config(ESPControllerWifiClient_ip, ESPControllerWifiClient_gateway, ESPControllerWifiClient_subnet, ESPControllerWifiClient_dns , ESPControllerWifiClient_dns);
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
  _parseMacAddressString(value,  buffer);
  return _compareMacAddreses(array, buffer);
}
bool _checkMacAddresString(String value)
{
  byte buffer[6] = {255, 255, 255, 255, 255, 255};
  _parseMacAddressString(value,  buffer);
  return _checkMacAddres(buffer);
}
String _macAddresToString(byte array[])
{
  String result = "";
  String  temp = "";
  for (byte i = 0; i < 6; i++)
  {
    temp = String(array[i], HEX);
    if (temp.length()  < 2)
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


int hexStrToInt(String instring)
{
  byte len = instring.length();
  if  (len == 0) return 0;
  int result = 0;
  for (byte i = 0; i < 8; i++)    // только первые 8 цыфар влезуть в uint32
  {
    char ch = instring[i];
    if (ch == 0) break;
    result <<= 4;
    if (isdigit(ch))
      result = result | (ch - '0');
    else result = result | (ch - 'A' + 10);
  }
  return result;
}