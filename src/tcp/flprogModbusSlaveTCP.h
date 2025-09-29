#pragma once
#include "flprogEthernet.h"
#include "../base/flprogModbus.h"

class ModbusSlaveTCP : public Modbus
{
public:
  ModbusSlaveTCP(FLProgAbstractTcpInterface *sourse);

  virtual void pool();

  void setSlaveAddress(uint8_t adr) { _slaveAddres = adr; };
  uint8_t slaveAddress() { return _slaveAddres; };

  void setTcpPort(int16_t _port);
  uint16_t tcpPort() { return _port; };
  void setKaScadaCloudIp(uint8_t newFirst_octet, uint8_t newSecond_octet, uint8_t newThird_octet, uint8_t newFourth_octet);
  void setKaScadaCloudIp(IPAddress ip);
  void setKaScadaCloudIpNmber(uint32_t ip) { setKaScadaCloudIp(flprog::numberToIp(ip)); };
  IPAddress kaScadaCloudIp() { return _cloudIp; };
  void setKaScadaCloudHost(String host);
  void setKaScadaCloudPort(int16_t newPort);
  uint16_t kaScadaCloudPort() { return _cloudPort; };
  void setKaScadaCloudDevceId(String id);
  String kaScadaCloudDevceId() { return _deniceId; };
  void setData(ModbusMainData *data) { _data = data; };
  void setDataTable(ModbusTable *table) { mainData()->setDataTable(table); };
  void setDataTable(uint8_t _table, uint16_t dataSize, int16_t *_adresses) { mainData()->setDataTable(_table, dataSize, _adresses); };
  void configDataTable(uint8_t _table, uint16_t dataSize, uint16_t _startAdr = 0) { mainData()->configDataTable(_table, dataSize, _startAdr); };

  void saveLong(int32_t val, uint8_t table, int16_t adr) { mainData()->saveLong(val, table, adr); };
  void saveUnsignedLong(uint32_t val, uint8_t table, int16_t adr) { mainData()->saveUnsignedLong(val, table, adr); };
  void saveFloat(float val, uint8_t table, int16_t adr) { mainData()->saveFloat(val, table, adr); };
  void saveInteger(uint16_t val, uint8_t table, int16_t adr) { mainData()->saveInteger(val, table, adr); };
  void saveByte(uint8_t val, uint8_t table, int16_t adr) { mainData()->saveByte(val, table, adr); };
  void saveBool(bool val, uint8_t table, int16_t adr) { mainData()->saveBool(val, table, adr); };

  void saveLongByIndex(int32_t val, uint8_t table, int16_t adr) { mainData()->saveLongByIndex(val, table, adr); };
  void saveUnsignedLongByIndex(uint32_t val, uint8_t table, int16_t adr) { mainData()->saveUnsignedLong(val, table, adr); };
  void saveFloatByIndex(float val, uint8_t table, int16_t adr) { mainData()->saveFloatByIndex(val, table, adr); };
  void saveIntegerByIndex(uint16_t val, uint8_t table, int16_t adr) { mainData()->saveIntegerByIndex(val, table, adr); };
  void saveByteByIndex(uint8_t val, uint8_t table, int16_t adr) { mainData()->saveByteByIndex(val, table, adr); };
  void saveBoolByIndex(bool val, uint8_t table, int16_t adr) { mainData()->saveBoolByIndex(val, table, adr); };

  uint8_t readByte(uint8_t table, int16_t adr) { return mainData()->readByte(table, adr); };
  uint16_t readInteger(uint8_t table, int16_t adr) { return mainData()->readInteger(table, adr); };
  float readFloat(uint8_t table, int16_t adr) { return mainData()->readFloat(table, adr); };
  int32_t readLong(uint8_t table, int16_t adr) { return mainData()->readLong(table, adr); };
  uint32_t readUnsignedLong(uint8_t table, int16_t adr) { return mainData()->readUnsignedLong(table, adr); };
  bool readBool(uint8_t table, int16_t adr) { return mainData()->readBool(table, adr); };

  uint8_t readByteByIndex(uint8_t table, int16_t adr) { return mainData()->readByteByIndex(table, adr); };
  uint16_t readIntegerByIndex(uint8_t table, int16_t adr) { return mainData()->readIntegerByIndex(table, adr); };
  float readFloatByIndex(uint8_t table, int16_t adr) { return mainData()->readFloatByIndex(table, adr); };
  int32_t readLongByIndex(uint8_t table, int16_t adr) { return mainData()->readLongByIndex(table, adr); };
  uint32_t readUnsignedLongByIndex(uint8_t table, int16_t adr) { return mainData()->readUnsignedLongByIndex(table, adr); };
  bool readBoolByIndex(uint8_t table, int16_t adr) { return mainData()->readBoolByIndex(table, adr); };

  void setMode(uint8_t mode);
  void byTcp() { setMode(FLPROG_TCP_MODBUS); };
  void byRtuOverTcp() { setMode(FLPROG_RTU_OVER_TCP_MODBUS); };
  void byKasCadaCloud() { setMode(FLPROG_KASCADA_CLOUD_MODBUS); };
  uint8_t mode() { return _mode; };

  int16_t getAdress(uint8_t table, int16_t index) { return mainData()->getAdress(table, index); };
  void setAdress(uint8_t table, int16_t index, int16_t addr) { mainData()->setAdress(table, index, addr); };

  ModbusMainData *mainData();

  void setLongOrder(uint8_t order) { mainData()->setLongOrder(order); };
  void setFloatOrder(uint8_t order) { mainData()->setFloatOrder(order); };
  void setUnsignedlongOrder(uint8_t order) { mainData()->setUnsignedlongOrder(order); };
  void setIntOrder(uint8_t order) { mainData()->setIntOrder(order); };

  uint8_t intOrder() { return mainData()->intOrder(); };
  uint8_t longOrder() { return mainData()->longOrder(); };
  uint8_t floatOrder() { return mainData()->floatOrder(); };
  uint8_t unsignedlongOrder() { return mainData()->unsignedlongOrder(); };

  void setCallBack(FLProgModbusNewDataCallback func);

protected:
  virtual uint8_t validateRequest();
  virtual void begin();
  virtual void getRxBuffer();
  virtual void sendTxBuffer();
  uint8_t rxBuffer();
  void connect();

  uint8_t _mode = FLPROG_TCP_MODBUS;
  uint8_t _slaveAddres = 1;
  FLProgEthernetServer _server;
  int16_t _port = 502;
  ModbusMainData *_data = 0;

  uint8_t _mbapBuffer[6];
  FLProgAbstractTcpInterface *_interface = 0;

  FLProgEthernetClient _tcpClient;
  int16_t _cloudPort = 25000;
  String _deniceId;
  IPAddress _cloudIp = IPAddress(94, 250, 249, 225);
  char _cloudHost[FLPROG_HOST_NAME_LENGTH] = "";
  bool _cloudAdressAsHost = false;
  uint32_t _kaScadaCloudTimeOutStartTime;
};
