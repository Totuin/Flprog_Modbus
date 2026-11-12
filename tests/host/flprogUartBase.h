#pragma once
#include "Arduino.h"
class FlprogAbstractUartExecutor {
public:
 virtual ~FlprogAbstractUartExecutor() {}
 virtual void setSpeedUart(uint32_t, uint8_t) {}
 virtual void setDataBitUart(uint8_t, uint8_t) {}
 virtual void setStopBitUart(uint8_t, uint8_t) {}
 virtual void setParityUart(uint8_t, uint8_t) {}
 virtual void setPinRxUart(uint8_t, uint8_t) {}
 virtual void setPinTxUart(uint8_t, uint8_t) {}
 virtual uint16_t getSpeedUart(uint8_t) {return 9600;}
 virtual uint16_t availableUart(uint8_t) {return 0;}
 virtual int readUart(uint8_t) {return -1;}
 virtual void writeUart(uint8_t *, uint16_t, uint8_t) {}
 virtual void beginUart(uint8_t) {}
};
