#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"
#include "flprogModbusUtilites.h"
#include "flprogModbusData.h"


class Modbus
{
public:
    virtual void setLastError(byte error){};
    virtual void begin(){};

protected:
    byte workStatus = MODBUS_READY;
    byte bufferSize = 0;
    byte buffer[64];
    unsigned long startSendTime;
    virtual void sendTxBuffer(){};
    void process_modbus_FC1(ModbusMainData *data, byte table);
    void process_modbus_FC3(ModbusMainData *data, byte table);
    void process_modbus_FC5(ModbusMainData *data);
    void process_modbus_FC6(ModbusMainData *data);
    void process_modbus_FC15(ModbusMainData *data);
    void process_modbus_FC16(ModbusMainData *data);
    void get_FC1(ModbusTable *table, int startAddress, int numberRegs);
    void get_FC3(ModbusTable *table, int startAddress, int numberRegs);
    void create_Read(int numberRegs);
    void create_Write_FC5(ModbusTable *table, int startAddress);
    void create_Write_FC6(ModbusTable *table, int startAddress);
    void create_Write_FC15(ModbusTable *table, int startAddress, int numberRegs);
    void create_Write_FC16(ModbusTable *table, int startAddress, int numberRegs);
    void create_PDU(ModbusTable *table, int startAddress, int numberRegs);
    void writeMaserData(ModbusTable *table, int startAddress, int numberRegs);
    byte validateSlaveReqest(ModbusMainData *data);
    void buildException(byte exception);
    virtual byte validateRequest(){};
    void executeSlaveReqest(ModbusMainData *data);
    
};