#include "flprogModbusMasterRTUESP32Bluetooth.h"
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

ModbusMasterRTUESP32Bluetooth Master1(Master1SlavesTable, 2);

int tempInt;
bool tempBool;
int value, oldValue;
unsigned long startTime;

void setup() {

  Serial.begin(9600);
  Master1SlavesTable[0].setSlaveAddress(1);
  Master1SlavesTable[0].setPollingPeriod(100);
  Master1SlavesTable[0].setTimeOutTime(100);
  Master1SlavesTable[0].setDataTable(&Holder1);
  Master1SlavesTable[0].setDataTable(&InputRegistr1);
  Master1SlavesTable[0].setDataTable(&Coil1);
  Master1SlavesTable[0].setDataTable(&DiscreteInput1);
  Master1SlavesTable[0].setLongOrder(DCBA_ORDER);
  Master1SlavesTable[0].setFloatOrder(BADC_ORDER);
  Master1SlavesTable[0].setUnsignedlongOrder(CDAB_ORDER);
  Master1SlavesTable[0].setIntOrder(AB_ORDER);

  Master1SlavesTable[1].setSlaveAddress(2);
  Master1SlavesTable[1].setPollingPeriod(100);
  Master1SlavesTable[1].setTimeOutTime(1000);
  Master1SlavesTable[1].setDataTable(&Holder2);
  Master1SlavesTable[1].setDataTable(&InputRegistr2);
  Master1SlavesTable[1].setDataTable(&Coil2);
  Master1SlavesTable[1].setDataTable(&DiscreteInput2);
  Master1SlavesTable[1].setLongOrder(DCBA_ORDER);
  Master1SlavesTable[1].setFloatOrder(BADC_ORDER);
  Master1SlavesTable[1].setUnsignedlongOrder(CDAB_ORDER);
  Master1SlavesTable[1].setIntOrder(AB_ORDER);

  Master1.setDeviceName("ESP32-BL");
  Master1.begin();

  startTime = millis() + 1000;
}

void loop() {

  Master1.pool();


  if ((startTime + 1000) < millis())
  {
    startTime = millis();
    tempInt = tempInt + 1;
    tempBool = !tempBool;
  }

  Master1.saveInteger(0, tempInt, HOLDING_REGISTR, 0);
  Master1.saveInteger(0, tempInt, HOLDING_REGISTR, 1);
  Master1.saveInteger(0, tempInt, HOLDING_REGISTR, 2);

  value = Master1.readInteger(1, INPUT_REGISTR, 1);

  Master1.saveBool(0, tempBool, COIL, 0);

  Master1.saveInteger(1, tempInt, HOLDING_REGISTR, 1);
  Master1.saveBool(1, tempBool, COIL, 1);

  if (value != oldValue)
  {
    oldValue = value;
    Serial.println(value);
  }

}