#include <flprogModbusSlaveRTUESP32Bluetooth.h>



int _modbusSlaveDataTable_4[8];
int _modbusSlaveAddresTable_4[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable Holder(HOLDING_REGISTR, _modbusSlaveDataTable_4, _modbusSlaveAddresTable_4, 8 );



int _modbusSlaveDataTable_3[8];
int _modbusSlaveAddresTable_3[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr(INPUT_REGISTR, _modbusSlaveDataTable_3, _modbusSlaveAddresTable_3, 8 );


bool _modbusSlaveDataTable_0[2];
int _modbusSlaveAddresTable_0[2] = {0, 1};
ModbusBoolTable Coil (COIL, _modbusSlaveDataTable_0, _modbusSlaveAddresTable_0, 2 );

bool _modbusSlaveDataTable_1[2];
int _modbusSlaveAddresTable_1[2] = {0, 1};
ModbusBoolTable DiscreteInput (DISCRETE_INPUT, _modbusSlaveDataTable_1, _modbusSlaveAddresTable_1, 2 );

ModbusMainData TestData;


ModbusSlaveRTUESP32Bluetooth SlaveRTU1;
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

  TestData.setDataTable(&Holder);
  TestData.setDataTable(&InputRegistr);
  TestData.setDataTable(&Coil);
  TestData.setDataTable(&DiscreteInput);

  TestData.setLongOrder(DCBA_ORDER);
  TestData.setFloatOrder(BADC_ORDER);
  TestData.setUnsignedlongOrder(CDAB_ORDER);
  TestData.setIntOrder(AB_ORDER);

  SlaveRTU1.setData(&TestData);
  SlaveRTU1.setDeviceName("ESP32-BL");
  //SlaveRTU1.setPartnerName("WORK-NOUT");
  //SlaveRTU1.beBluetoothMaster();

  SlaveRTU1.begin(1);
}

void loop()
{
  SlaveRTU1.pool();
  tempByte = SlaveRTU1.readByte(HOLDING_REGISTR, 1);
  tempInt = SlaveRTU1.readInteger(HOLDING_REGISTR, 0);
  tempFloat = SlaveRTU1.readFloat(HOLDING_REGISTR, 2);
  tempLong = SlaveRTU1.readLong(HOLDING_REGISTR, 6);
  tempUnsigned = SlaveRTU1.readUnsignedLong(HOLDING_REGISTR, 4);
  tempBool1 = SlaveRTU1.readBool(COIL, 0);
  tempBool2 = SlaveRTU1.readBool(COIL, 1);

  SlaveRTU1.saveLong(tempLong, INPUT_REGISTR, 6);
  SlaveRTU1.saveUnsignedLong(tempUnsigned, INPUT_REGISTR, 4);
  SlaveRTU1.saveFloat(tempFloat, INPUT_REGISTR, 2);
  SlaveRTU1.saveInteger(tempInt, INPUT_REGISTR, 0);
  SlaveRTU1.saveByte(tempByte, INPUT_REGISTR, 1);
  SlaveRTU1.saveBool(tempBool1, DISCRETE_INPUT, 0);
  SlaveRTU1.saveBool(tempBool2, DISCRETE_INPUT, 1);

  lastError = SlaveRTU1.getLastError();

}