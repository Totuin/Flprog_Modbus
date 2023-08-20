#include "flprogModbusBridge.h"

FLProgSPI spiBus(0);
FLProgWiznetInterface WiznetInterface(&spiBus, 10);

// ModbusKasCadaCloudTcpBridge Bridge(0, &WiznetInterface);
ModbusTcpBridge Bridge(0, &WiznetInterface);
// ModbusRtuOverTcpBridge Bridge(0, &WiznetInterface);

void setup()
{
  WiznetInterface.mac(0x78, 0xAC, 0xC0, 0x0D, 0x5B, 0x86);
  WiznetInterface.localIP(IPAddress(192, 168, 199, 177));
  WiznetInterface.resetDhcp();

  // Bridge.setRTUDevice(&RtuDevice);
  // Bridge.setTCPDevice(&TcpDevice);
  Bridge.byClient();
  // Bridge.byServer();
  Bridge.setTCPPort(502);
  Bridge.setTCPRemoteIp(IPAddress(192, 168, 199, 95));
  // Bridge.setKaScadaCloudIp(94, 250, 249, 225);
  // Bridge.setKaScadaCloudPort(25000);
  // Bridge.setKaScadaCloudDevceId("DKK6T9TPE4");
}
void loop()
{
  WiznetInterface.pool();
  Bridge.pool();
}