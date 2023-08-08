#include "flprogModbusBridge.h"

FLProgSPI spiBus(0);
FlprogW5100Interface W5100_Interface(&spiBus, 10);

ModbusKasCadaCloudTcpBridge _Bridge(0, &W5100_Interface);
// ModbusTcpBridge _Bridge(0, &W5100_Interface);
// ModbusRtuOverTcpBridge _Bridge(0, &W5100_Interface);

byte ethernet_mac[] = {0x78, 0xAC, 0xC0, 0x7E, 0xC1, 0x01};
IPAddress ethernet_ip(192, 168, 199, 125);
byte ethernet_dns[] = {192, 168, 199, 1};
byte ethernet_gateway[] = {192, 168, 199, 1};
byte ethernet_subnet[] = {255, 255, 255, 0};

void setup()
{

  W5100_Interface.begin(ethernet_mac, ethernet_ip, ethernet_dns, ethernet_gateway, ethernet_subnet);
  delay(1000);

  // _Bridge.setRTUDevice(&RtuDevice);
  // _Bridge.setTCPDevice(&TcpDevice);
  //_Bridge.byClient();
  //_Bridge.byServer();
  //  _Bridge.setTCPPort(502);
  //_Bridge.setTCPRemoteIp(IPAddress(192, 168, 199, 95));
  _Bridge.setKaScadaCloudIp(94, 250, 249, 225);
  _Bridge.setKaScadaCloudPort(25000);
  _Bridge.setKaScadaCloudDevceId("DKK6T9TPE4");
  _Bridge.begin();
}
void loop()
{
  _Bridge.pool();
}