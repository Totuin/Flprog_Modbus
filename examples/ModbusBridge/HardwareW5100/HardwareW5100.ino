#include "flprogModbusBridge.h"

//-------------------------------------------------------------------------------------------------
//         Вариант с  шиной (SPI0) и пином(10) по умолчаниюю. Пин потом можно поменять.
//         Но если на этой шине висит ещё какое то устройство лучше применять второй вариант
//-------------------------------------------------------------------------------------------------
FLProgWiznetInterface WiznetInterface; //--Создание интерфейса для работы с чипом W5100(W5200,W5500) (по умолчанию CS pin - 10,  Шина SPI - 0);

//-------------------------------------------------------------------------------------------------
//        Второй вариант с непосредственной привязкой к шине и пину.
//-------------------------------------------------------------------------------------------------
 //FLProgWiznetInterface WiznetInterface(10, 0); //--Создание интерфейса для работы с чипом W5100(W5200,W5500) CS pin - 10, Шина SPI - 0;
 //FLProgWiznetInterface WiznetInterface(10); //--Создание интерфейса для работы с чипом W5100(W5200,W5500) CS pin - 10, (по умолчанию Шина SPI - 0);

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