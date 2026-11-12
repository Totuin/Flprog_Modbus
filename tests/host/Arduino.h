#pragma once
#include <cstdint>
#include <cmath>
#include <cstring>
#include <cstddef>
#include <cstdlib>
#define HIGH 1
#define LOW 0
#define OUTPUT 1
inline void pinMode(int, int) {}
inline void digitalWrite(int, int) {}
inline uint32_t millis() {return 1000;}
inline uint16_t word(uint8_t a, uint8_t b) { return (uint16_t(a)<<8) | b; }
#define highByte(x) (uint8_t((uint32_t(x) >> 8) & 0xff))
#define lowByte(x) (uint8_t(uint32_t(x) & 0xff))
#define bitRead(x,bit) (((x)>>(bit))&1U)
#define bitWrite(x,bit,val) ((x) = (uint8_t)(((x) & ~(uint8_t(1U<<(bit)))) | ((val)?(uint8_t(1U<<(bit))):0)))
