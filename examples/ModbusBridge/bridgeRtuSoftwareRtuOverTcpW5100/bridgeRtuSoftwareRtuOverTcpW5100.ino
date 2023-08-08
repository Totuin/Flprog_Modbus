#include "flprogModbusBridge.h"

byte ethernet_mac[] = {0x78, 0xAC, 0xC0, 0x7E, 0xC1, 0x01};
IPAddress ethernet_ip(192, 168, 1, 125);
byte ethernet_dns[] = {192, 168, 1, 1};
byte ethernet_gateway[] = {192, 168, 1, 1};
byte ethernet_subnet[] = {255, 255, 255, 0};

ModbusRtuOverTcpBridge _Bridge;

FLProgSoftwareUart RtuDevice(2, 3);

FLProgW5100TcpDevice TcpDevice;

void setup()
{

  Ethernet.begin(ethernet_mac, ethernet_ip, ethernet_dns, ethernet_gateway, ethernet_subnet);
  delay(1000);

  _Bridge.setRTUDevice(&RtuDevice);
  _Bridge.setTCPDevice(&TcpDevice);
  _Bridge.byClient();
  _Bridge.setTCPRemoteIp(192, 168, 1, 108);
  _Bridge.begin();
}

void loop()
{
  _Bridge.pool();

  _Bridge.setTCPPort(502);
}