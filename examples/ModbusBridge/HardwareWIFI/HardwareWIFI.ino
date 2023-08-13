#include "flprogModbusBridge.h"

FLProgOnBoardWifi WifiInterface;

//ModbusKasCadaCloudTcpBridge Bridge(0, &WifiInterface);
ModbusTcpBridge Bridge(0, &WifiInterface);
 //ModbusRtuOverTcpBridge Bridge(0, &WifiInterface);

void setup()
{
  WifiInterface.clientOn();
  WifiInterface.mac(0x78, 0xAC, 0xC0, 0x2C, 0x3E, 0x28);
  WifiInterface.localIP(IPAddress(192, 168, 199, 177));
  WifiInterface.resetDhcp();
  WifiInterface.setClientSsidd("totuin-router");
  WifiInterface.setClientPassword("12345678");


  // Bridge.setRTUDevice(&RtuDevice);
  // Bridge.setTCPDevice(&TcpDevice);
  Bridge.byClient();
  //Bridge.byServer();
  Bridge.setTCPPort(502);
  Bridge.setTCPRemoteIp(IPAddress(192, 168, 199, 95));
  //Bridge.setKaScadaCloudIp(94, 250, 249, 225);
  //Bridge.setKaScadaCloudPort(25000);
  //Bridge.setKaScadaCloudDevceId("DKK6T9TPE4");
}
void loop()
{
  WifiInterface.pool();
  Bridge.pool();
}