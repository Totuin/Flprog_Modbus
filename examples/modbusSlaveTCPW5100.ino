#include "flprogModbusSlaveTCPW5100.h"

byte ethernet_mac[] = {0x78, 0xAC, 0xC0, 0x77, 0xE3, 0x05};
IPAddress ethernet_ip(192, 168, 0, 5);
byte ethernet_dns[] = {192, 168, 0, 1};
byte ethernet_gateway[] = {192, 168, 0, 1};
byte ethernet_subnet[] = {255, 255, 255, 0};

int _modbusSlaveDataTable_4[8];
int _modbusSlaveAddresTable_4[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable Holder(HOLDING_REGISTR, _modbusSlaveDataTable_4, _modbusSlaveAddresTable_4, 8);

int _modbusSlaveDataTable_3[8];
int _modbusSlaveAddresTable_3[8] = {0, 1, 2, 3, 4, 5, 6, 7};
ModbusWorldTable InputRegistr(INPUT_REGISTR, _modbusSlaveDataTable_3, _modbusSlaveAddresTable_3, 8);

bool _modbusSlaveDataTable_0[2];
int _modbusSlaveAddresTable_0[2] = {0, 1};
ModbusBoolTable Coil(COIL, _modbusSlaveDataTable_0, _modbusSlaveAddresTable_0, 2);

bool _modbusSlaveDataTable_1[2];
int _modbusSlaveAddresTable_1[2] = {0, 1};
ModbusBoolTable DiscreteInput(DISCRETE_INPUT, _modbusSlaveDataTable_1, _modbusSlaveAddresTable_1, 2);

ModbusMainData TestData;

EthernetServer tempServer(502);

ModbusSlaveTCPW5100 Slave1(&tempServer);

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
    Serial.begin(9600);
    TestData.setDataTable(&Holder);
    TestData.setDataTable(&InputRegistr);
    TestData.setDataTable(&Coil);
    TestData.setDataTable(&DiscreteInput);

    TestData.setLongOrder(DCBA_ORDER);
    TestData.setFloatOrder(BADC_ORDER);
    TestData.setUnsignedlongOrder(CDAB_ORDER);
    TestData.setIntOrder(AB_ORDER);

    Slave1.setData(&TestData);

    Ethernet.begin(ethernet_mac, ethernet_ip, ethernet_dns, ethernet_gateway, ethernet_subnet);
    delay(1000);
    Slave1.begin();
}

void loop()
{
    Slave1.pool();

    tempByte = Slave1.readByte(HOLDING_REGISTR, 1);
    Slave1.saveByte(tempByte, INPUT_REGISTR, 1);

    tempInt = Slave1.readInteger(HOLDING_REGISTR, 0);
    tempFloat = Slave1.readFloat(HOLDING_REGISTR, 2);
    tempLong = Slave1.readLong(HOLDING_REGISTR, 6);
    tempUnsigned = Slave1.readUnsignedLong(HOLDING_REGISTR, 4);
    tempBool1 = Slave1.readBool(COIL, 0);
    tempBool2 = Slave1.readBool(COIL, 1);

    Slave1.saveLong(tempLong, INPUT_REGISTR, 6);
    Slave1.saveUnsignedLong(tempUnsigned, INPUT_REGISTR, 4);
    Slave1.saveFloat(tempFloat, INPUT_REGISTR, 2);
    Slave1.saveInteger(tempInt, INPUT_REGISTR, 0);

    Slave1.saveBool(tempBool1, DISCRETE_INPUT, 0);
    Slave1.saveBool(tempBool2, DISCRETE_INPUT, 1);

    lastError = Slave1.getLastError();
}