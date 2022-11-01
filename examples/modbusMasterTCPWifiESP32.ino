# include "flprogModbusMasterTCPWifiESP32.h"


bool ESPControllerWifiClient_HRD = 0;
bool ESPControllerWifiClient_status = 1;
bool isWifiClientReconectWite = 0;
unsigned long startWifiClientReconectWite ;
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




int table_4_1[8];
int tableA_4_1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
bool tableS_4_1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
ModbusWorldTable Holder1(HOLDING_REGISTR, table_4_1, tableA_4_1, tableS_4_1, 8 );
int table_4_2[8];
int tableA_4_2[8] = {0, 1, 2, 3, 4, 5, 6, 7};
bool tableS_4_2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
ModbusWorldTable Holder2(HOLDING_REGISTR, table_4_2, tableA_4_2, tableS_4_2, 8 );

int table_3_1[8];
int tableA_3_1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr1(INPUT_REGISTR, table_3_1, tableA_3_1, 8 );
int table_3_2[8];
int tableA_3_2[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr2(INPUT_REGISTR, table_3_2, tableA_3_2, 8 );

bool table_0_1[2];
int tableA_0_1[2] = {0, 1};
bool tableS_0_1[2] = {0, 0};
ModbusBoolTable Coil1 (COIL, table_0_1, tableA_0_1, tableS_0_1, 2 );
bool table_0_2[2];
int tableA_0_2[2] = {0, 1};
bool tableS_0_2[2] = {0, 0};
ModbusBoolTable Coil2 (COIL, table_0_2, tableA_0_2, tableS_0_2, 2 );


bool table_1_1[2];
int tableA_1_1[2] = {0, 1};
ModbusBoolTable DiscreteInput1 (DISCRETE_INPUT, table_1_1, tableA_1_1, 2 );
bool table_1_2[2];
int tableA_1_2[2] = {0, 1};
ModbusBoolTable DiscreteInput2 (DISCRETE_INPUT, table_1_2, tableA_1_2, 2 );




ModbusSlaveInMaster Master1SlavesTable[2];

ModbusTCPWifiESP32SlaveServer Master1ServersTable[1];

ModbusMasterTCPWifiESP32 Master1(Master1ServersTable, 1);

int tempInt;
bool tempBool;
int value, oldValue;
unsigned long startTime;



void setup()
{

  WiFi.mode(WIFI_STA);
  if (WiFi.getAutoConnect() != true) WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  _esp32WifiModuleClientReconnect();
  _parseMacAddressString(WiFi.macAddress(), ESPControllerWifiClient_mac);


Master1ServersTable[0].setSlaves(Master1SlavesTable, 2);
  Master1ServersTable[0].setPort(502);
  Master1ServersTable[0].setIpAdress(192, 168, 0, 100);

  Master1SlavesTable[0].setSlaveAddress(1);
  Master1SlavesTable[0].setPollingPeriod(1000);
  Master1SlavesTable[0].setTimeOutTime(1000);
  Master1SlavesTable[0].setDataTable(&Holder1);
  Master1SlavesTable[0].setDataTable(&InputRegistr1);
  Master1SlavesTable[0].setDataTable(&Coil1);
  Master1SlavesTable[0].setDataTable(&DiscreteInput1);

  Master1SlavesTable[1].setSlaveAddress(2);
  Master1SlavesTable[1].setPollingPeriod(1000);
  Master1SlavesTable[1].setTimeOutTime(1000);
  Master1SlavesTable[1].setDataTable(&Holder2);
  Master1SlavesTable[1].setDataTable(&InputRegistr2);
  Master1SlavesTable[1].setDataTable(&Coil2);
  Master1SlavesTable[1].setDataTable(&DiscreteInput2);


  Master1SlavesTable[0].setLongOrder(DCBA_ORDER);
  Master1SlavesTable[0].setFloatOrder(BADC_ORDER);
  Master1SlavesTable[0].setUnsignedlongOrder(CDAB_ORDER);
  Master1SlavesTable[0].setIntOrder(AB_ORDER);


  Master1.begin();


}

void loop()
{

  if (ESPControllerWifiClient_IsNeedReconect)
  {
    _esp32WifiModuleClientReconnect();
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
      ESPControllerWifiClient_HRD = 1;
    }
  }
  else
  {
    ESPControllerWifiClient_HRD = 0;
  }
  if (ESPControllerWifiClient_workStatus && (! ESPControllerWifiClient_status))
  {
    if (! isWifiClientReconectWite)
    {
      isWifiClientReconectWite = 1;
      startWifiClientReconectWite = millis();
    }
    else
    {
      if (_isTimer(startWifiClientReconectWite , 5000))
      {
        WiFi.disconnect();
        _esp32WifiModuleClientReconnect();
        isWifiClientReconectWite = 0;
      }
    }
  }





 Master1.pool();
  if ((startTime + 1000) < millis())
  {
    startTime = millis();
    tempInt = tempInt + 1;
    tempBool = !tempBool;

  }

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
void _esp32WifiModuleClientReconnect()
{
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


