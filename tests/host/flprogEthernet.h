#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"
#include <string>
class String {
 std::string value;
public:
 String() {}
 String(const char *s):value(s?s:"") {}
 size_t length() const { return value.size(); }
 bool equals(const String &rhs) const { return value==rhs.value; }
 void toCharArray(char *buf, size_t n) const { if(n) { strncpy(buf,value.c_str(),n-1);buf[n-1]=0; } }
};
class IPAddress {
 uint32_t value=0;
public:
 IPAddress() {}
 IPAddress(uint32_t a):value(a) {}
 IPAddress(uint8_t a,uint8_t b,uint8_t c,uint8_t d):value(uint32_t(a)<<24|uint32_t(b)<<16|uint32_t(c)<<8|d) {}
 bool operator==(const IPAddress &rhs) const {return value==rhs.value;}
 bool operator!=(const IPAddress &rhs) const {return value!=rhs.value;}
};
#define FLPROG_HOST_NAME_LENGTH 128
#define FLPROG_INADDR_NONE IPAddress()
class FLProgAbstractTcpInterface { public: virtual ~FLProgAbstractTcpInterface() {} virtual bool isReady() {return true;} };
class FLProgEthernetClient {
public:
 void setSourse(FLProgAbstractTcpInterface*) {}
 bool connected() {return true;}
 uint8_t connect(const char *,int16_t) {return 1;}
 uint8_t connect(IPAddress,int16_t) {return 1;}
 void stop() {}
 int available() {return 0;}
 int read() {return -1;}
 size_t write(const uint8_t *,size_t n) {return n;}
 size_t print(String) {return 0;}
};
class FLProgEthernetServer {
public:
 void setSourse(FLProgAbstractTcpInterface*) {}
 bool connected() {return true;}
 void stop() {}
 void stopConnection() {}
 void setPort(int16_t) {}
 void begin() {}
 int available() {return 0;}
 int read() {return -1;}
 size_t write(const uint8_t *,size_t n) {return n;}
};
namespace flprog {
inline IPAddress numberToIp(uint32_t num) {return IPAddress(num);}
inline uint32_t timeBack(uint32_t n) {return uint32_t(millis()-n);}
}
