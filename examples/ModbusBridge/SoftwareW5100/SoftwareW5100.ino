#include "flprogModbusBridge.h"

FLProgSPI spiBus(0);
FlprogW5100Interface W5100_Interface(&spiBus, 10);

ModbusKasCadaCloudTcpBridge Bridge(62, 63, &W5100_Interface); // Rx пин - A8 (62), TX пин -A9 (63)- Для Arduino Mega
//ModbusTcpBridge Bridge(62, 63, &W5100_Interface);           // Rx пин - A8 (62), TX пин -A9 (63)- Для Arduino Mega
//ModbusRtuOverTcpBridge Bridge(62, 63, &W5100_Interface);    //  Rx пин - A8 (62), TX пин -A9 (63)- Для Arduino Mega
void setup()
{ 
  Serial.begin(9600);
  W5100_Interface.mac(0x78, 0xAC, 0xC0, 0x0D, 0x5B, 0x86);
  W5100_Interface.localIP(IPAddress(192, 168, 199, 177));
  W5100_Interface.resetDhcp();


  // Bridge.setRTUDevice(&RtuDevice);
  // Bridge.setTCPDevice(&TcpDevice);
  //Bridge.byClient();
  //Bridge.byServer();
  //Bridge.setTCPPort(502);
  //Bridge.setTCPRemoteIp(IPAddress(192, 168, 199, 95));
  //Bridge.setKaScadaCloudIp(94, 250, 249, 225);
  //Bridge.setKaScadaCloudPort(25000);
  Bridge.setKaScadaCloudDevceId("DKK6T9TPE4");
}
void loop()
{
  W5100_Interface.pool();
  Bridge.pool();
}