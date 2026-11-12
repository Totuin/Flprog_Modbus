// Автономная демонстрация таблиц Modbus и CRC, без подключения RS-485 или Ethernet.
// Требует установленной Flprog_Modbus и Flprog_Utilites.
#include <flprogModbusRTU.h>

ModbusTable holding(FLPROG_HOLDING_REGISTR, 3, 100);

void setup()
{
  Serial.begin(115200);
  holding.setData(100, int16_t(1234));
  holding.setData(101, int16_t(-12));
  Serial.print("Holding[100] = ");
  Serial.println(holding.readWorldRegister(100));
  Serial.print("Next address = ");
  Serial.println(holding.findNextStartAddres(100));

  // В документации Modbus 0xC5 0xCD — CRC запроса 01 03 00 00 00 0A.
  uint8_t request[8] = {1, 3, 0, 0, 0, 10, 0xC5, 0xCD};
  Serial.println(flprogModus::checkCRCOnBuffer(8, request) ? "CRC OK" : "CRC BAD");
}

void loop() {}
