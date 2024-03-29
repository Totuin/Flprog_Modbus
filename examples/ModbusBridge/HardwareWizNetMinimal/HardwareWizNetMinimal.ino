#include "flprogModbusBridge.h"
/*
  -------------------------------------------------------------------------------------------------
        Создание интерфейса для работы с чипом W5100(W5200,W5500)
        Шина SPI и пин CS берутся из  RT_HW_Base.device.spi.busETH и RT_HW_Base.device.spi.csETH
  -------------------------------------------------------------------------------------------------
*/
FLProgWiznetInterface WiznetInterface;

/*
  -------------------------------------------------------------------------------------------------
        Второй вариант cоздания интерфейса для работы с чипом W5100(W5200,W5500).
        С непосредственной привязкой  пину.
        Пин CS - 10
        Шина SPI берётся из RT_HW_Base.device.spi.busETH
  -------------------------------------------------------------------------------------------------
*/
// FLProgWiznetInterface WiznetInterface(10);

/*
  -------------------------------------------------------------------------------------------------
      Третий вариант cоздания интерфейса для работы с чипом W5100(W5200,W5500).
      С непосредственной привязкой  пину и шине.
      Пин CS - 10
      Шина SPI - 0
  -------------------------------------------------------------------------------------------------
*/
// FLProgWiznetInterface WiznetInterface(10, 0);

/*
  -----------------------------------------------------------------------------------------
     Создаем объект непосредстредственно Модбас моста на необходимом интерфейсе и UART мосту
  -----------------------------------------------------------------------------------------
*/

ModbusBridge Bridge(0, &WiznetInterface);


//=================================================================================================
void setup()
{
  /*
    -----------------------------------------------------------------------------------------
         Настройка интерфейса
    -----------------------------------------------------------------------------------------
  */
  WiznetInterface.mac(0x78, 0xAC, 0xC0, 0x0D, 0x5B, 0x86);
  WiznetInterface.localIP(IPAddress(192, 168, 1, 10));
  WiznetInterface.resetDhcp();

  /*
     -----------------------------------------------------------------------------------------
          Настройка моста
     -----------------------------------------------------------------------------------------
  */
  
  Bridge.setSpeedUart(19200);


  // Bridge.byClient();// Режим по умолчанию
  Bridge.byServer();

  // Bridge.byTcp(); // Режим по умолчанию
  Bridge.byRtuOverTcp();
  // Bridge.byKasCadaCloud(); // Переключение режимов клиент/сервер не действует

  Bridge.setTCPPort(502);
  Bridge.setTCPRemoteIp(IPAddress(192, 168, 1, 1));
  Bridge.setKaScadaCloudIp(94, 250, 249, 225);
  Bridge.setKaScadaCloudPort(25000);
  Bridge.setKaScadaCloudDevceId("DKK6T9TPE4");

}

//=================================================================================================
void loop()
{
  WiznetInterface.pool();
  Bridge.pool();
}
//=================================================================================================

