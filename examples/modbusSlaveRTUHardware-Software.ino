#include "flprogModbusMasterRTUHardware.h"

int table_4_1[8];
int tableA_4_1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
bool tableS_4_1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
ModbusWorldTable Holder1(HOLDING_REGISTR, table_4_1, tableA_4_1,tableS_4_1, 8 );


int table_3_1[8];
int tableA_3_1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr1(INPUT_REGISTR, table_3_1, tableA_3_1, 8 );


bool table_0_1[2];
int tableA_0_1[2] = {0, 1};
bool tableS_0_1[2] = {0, 0};
ModbusBoolTable Coil1 (COIL, table_0_1, tableA_0_1,tableS_0_1, 2 );

bool table_1_1[2];
int tableA_1_1[2] = {0, 1};
ModbusBoolTable DiscreteInput1 (DISCRETE_INPUT, table_1_1, tableA_1_1, 2 );

ModbusSlaveInMaster TestData1(1, 1000);

ModbusSlaveInMaster Master1SlavesTable[1] = {TestData1};

ModbusMasterRTUHardware Master1(Master1SlavesTable, 1);

void setup() {

  TestData1.setSlaveAddress(1);
  TestData1.setDataTable(&Holder1);
  TestData1.setDataTable(&InputRegistr1);
  TestData1.setDataTable(&Coil1);
  TestData1.setDataTable(&DiscreteInput1);

  TestData1.setLongOrder(DCBA_ORDER);
  TestData1.setFloatOrder(BADC_ORDER);
  TestData1.setUnsignedlongOrder(CDAB_ORDER);
  TestData1.setIntOrder(AB_ORDER);

  Master1.setPinPeDe(13);
  Master1.begin(&Serial);

}

void loop() {
  Master1.pool();
  
  Master1.setPortSpeed(SPEED_14400);
  Master1.setPortDataBits(8);
  Master1.setPortStopBits(2);
  Master1.setPortParity(2);

}