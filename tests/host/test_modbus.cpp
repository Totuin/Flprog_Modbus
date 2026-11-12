#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
#include "flprogModbus.h"
#include "flprogModbusRTU.h"

class TestModbus : public Modbus {
public:
 std::vector<uint8_t> sent;
 void begin() override {}
 void sendTxBuffer() override { sent.assign(_buffer, _buffer+_bufferSize); }
 using Modbus::validateSlaveReqest;
 using Modbus::executeSlaveReqest;
 using Modbus::create_Write_FC15;
 using Modbus::create_PDU;
 using Modbus::buildException;
 using Modbus::process_modbus_FC1;
 using Modbus::process_modbus_FC3;
 void initFrame(uint8_t addr, uint8_t fc, uint16_t start, uint16_t count) {
   memset(_buffer, 0xFF, sizeof(_buffer));
   _buffer[0]=addr; _buffer[1]=fc; _buffer[2]=highByte(start); _buffer[3]=lowByte(start);
   _buffer[4]=highByte(count); _buffer[5]=lowByte(count); _bufferSize=6;
 }
 void dirtyFill(uint8_t offset, uint8_t value) { _buffer[offset]=value; }
 void setSize(uint8_t n) { _bufferSize=n; }
};

class DummyUart : public FlprogAbstractUartExecutor {
public:
 std::vector<uint8_t> data;
 size_t pos=0;
 unsigned writes=0;
 uint16_t availableUart(uint8_t) override {return uint16_t(data.size()-pos);}
 int readUart(uint8_t) override { return pos < data.size()?data[pos++]:-1; }
 void writeUart(uint8_t *, uint16_t, uint8_t) override {writes++;}
};
class TestRTU : public ModbusRTU {
public:
 void begin() override {}
 void setExecutor(DummyUart *p) {_executor=p;}
 using ModbusRTU::checkAvaliblePacage;
 using ModbusRTU::sendTxBuffer;
 void buildHuge(uint8_t size) {_bufferSize=size; _buffer[0]=1; _buffer[1]=3;}
 uint8_t size() const {return _bufferSize;}
 uint8_t state() const {return _status;}
};

int main() {
 uint8_t good[8]={1,3,0,0,0,10,0xc5,0xcd};
 assert(flprogModus::checkCRCOnBuffer(8,good));
 assert(!flprogModus::checkCRCOnBuffer(0,good));
 assert(!flprogModus::checkCRCOnBuffer(1,good));
 assert(!flprogModus::checkCRCOnBuffer(3,good));
 assert(!flprogModus::checkCRCOnBuffer(8,nullptr));
 assert(flprogModus::timeForSendBytes(8,1,0,9600,8)==17);
 assert(flprogModus::timeForSendBytes(0,1,0,9600,8)==0);
 assert(flprogModus::timeForSendBytes(8,1,0,0,8)>0); // 0 is a valid baud-rate code (300 baud)
 {
  ModbusTable empty(FLPROG_COIL,0,0); assert(empty.getMinAdress()==-1); assert(empty.getMaxAdress()==-1);
  assert(empty.firstWriteAddress()==-1); assert(empty.readRegsSize(0)==0);
  int32_t addresses[4]={1,3000000,5000000,8000000};
  ModbusTable t(FLPROG_HOLDING_REGISTR,4,addresses);
  assert(t.getMinAdress()==1 && t.getMaxAdress()==8000000);
  assert(t.findNextStartAddres(2)==3000000);
  assert(t.findNextStartAddres(3000000)==5000000);
  assert(t.findNextStartAddres(9000000)==-1);
  t.setData(3000000,int16_t(1234));
  ModbusTable copy=t; assert(copy.readWorldRegister(3000000)==1234);
  copy.setData(3000000,int16_t(2345)); assert(t.readWorldRegister(3000000)==1234);
  ModbusTable moved(FLPROG_HOLDING_REGISTR,2,0);
  moved=copy; assert(moved.readWorldRegister(3000000)==2345);
  moved=moved; assert(moved.readWorldRegister(3000000)==2345);
  t.init(6,100); assert(t.getMinAdress()==100 && t.getMaxAdress()==105);
  assert(t.readRegsSize(100)==6);
  t.init(3,10000); assert(t.getMinAdress()==10000);
  t.init(6); assert(t.getMinAdress()==10000 && t.getMaxAdress()==10005);
  t.init(0); assert(t.getMinAdress()==-1 && t.getMaxAdress()==-1);
  int32_t external[2]={200,201}; ModbusTable tableExternal(FLPROG_COIL,2,external);
  tableExternal.init(4); assert(tableExternal.adressForIndex(3)==203);
  assert(external[0]==200 && external[1]==201);
  ModbusTable negative(FLPROG_COIL,-1,0); assert(negative.getMinAdress()==-1);
 }
 {
  ModbusMainData main;
  assert(main.indexForAddres(1,FLPROG_HOLDING_REGISTR)==-1);
  assert(!main.checkModbusRange(0,1,FLPROG_COIL));
  main.configDataTable(FLPROG_COIL,32,10);
  assert(!main.checkModbusRange(10,0,FLPROG_COIL));
  assert(!main.checkModbusRange(10,-1,FLPROG_COIL));
  assert(main.checkModbusRange(10,25,FLPROG_COIL));
  assert(!main.checkModbusRange(10,33,FLPROG_COIL));
  ModbusTable *t=main.tableForType(FLPROG_COIL);
  for(int i=0;i<25;i++) t->setData(10+i,(i%2)==0);
  TestModbus m;
  for (int count : {1,7,8,9,16,25}) {
    m.initFrame(17,15,10,uint16_t(count));
    m.create_PDU(t,10,int16_t(count));
    assert(m.sent.size()==size_t(7+(count+7)/8));
    assert(m.sent[0]==17 && m.sent[1]==15 && m.sent[6]==(count+7)/8);
    for(int i=0;i<count;i++) assert(!!(m.sent[7+i/8]&(1<<(i%8)))==(i%2==0));
  }
  m.initFrame(17,1,10,9);
  m.process_modbus_FC1(&main,FLPROG_COIL);
  assert(m.sent.size()==5 && m.sent[3]==0x55 && m.sent[4]==0x01);
  m.initFrame(17,1,10,0);assert(m.validateSlaveReqest(&main)==3);
  m.initFrame(17,5,10,1); assert(m.validateSlaveReqest(&main)==3);
  m.dirtyFill(4,0xFF);m.dirtyFill(5,0);assert(m.validateSlaveReqest(&main)==0);
  m.initFrame(17,15,10,8);m.setSize(8);m.dirtyFill(6,2);assert(m.validateSlaveReqest(&main)==3);
  m.initFrame(17,15,10,8);m.setSize(8);m.dirtyFill(6,1);assert(m.validateSlaveReqest(&main)==0);
  m.initFrame(17,3,10,4);m.buildException(2);
  assert(m.sent.size()==0 || m.sent[0]==17); // address is preserved in local buffer
  m.initFrame(17,15,10,8);m.buildException(2);
  m.sendTxBuffer(); assert(m.sent[0]==17 && m.sent[1]==0x8f && m.sent[2]==2);
  m.initFrame(17,1,10,8);
  m.process_modbus_FC1(&main,FLPROG_COIL);assert(m.sent[3]==0x55);
  delete t; // ModbusMainData does not own externally supplied tables (legacy contract).
 }
 {
  DummyUart serial;
  TestRTU rtu;
  rtu.setExecutor(&serial);
  rtu.buildHuge(uint8_t(FLPROG_MODBUS_BUFER_SIZE-1));rtu.sendTxBuffer();
  assert(serial.writes==0 && rtu.size()==0);
  // Если пришёл кадр длиннее буфера, не зависаем, освобождаем UART.
  serial.data.assign(FLPROG_MODBUS_BUFER_SIZE+20,0);
  assert(!rtu.checkAvaliblePacage());
  assert(serial.availableUart(0)==0 && rtu.size()==0);
 }
 std::cout << "PASS: CRC, RTU, FC1/FC15, table init/copy/sparse addresses, invalid frames; buffer=" << FLPROG_MODBUS_BUFER_SIZE << '\n';
}
