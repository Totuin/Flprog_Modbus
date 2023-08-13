#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"
#include "flprogSPI.h"
#include "flprogEthernet.h"
#include "flprogWiFi.h"

#define FLPROG_TCP_SERVER_MODE 1
#define FLPROG_TSP_CLIENT_MODE 0

#define FLPROG_ANON_INTERFACE 0
#define FLPROG_ETHERNET_INTERFACE 1
#define FLPROG_WIFI_INTERFACE 2

class FLProgTcpDevice : public FLProgStream
{
public:
    FLProgTcpDevice(FlprogAbstractEthernet *sourse);
    FLProgTcpDevice(FLProgAbstracttWiFiInterface *sourse);
    void beClient();
    void beServer();
    uint8_t read();
    int read(uint8_t *buf, size_t size);
    void setPort(int port);
    bool connected();
    void connect() { connect(IPAddress(0, 0, 0, 0), tcpPort); };
    void connect(IPAddress newIp) { connect(newIp, tcpPort); };
    int connect(IPAddress newIp, int newPort);
    int connect(const char *host, uint16_t newPort);
    void begin();
    void restart();
    void stop();
    bool hasClient();
    bool hasServer();

protected:
    bool mode = FLPROG_TSP_CLIENT_MODE;
    int tcpPort = 502;
    uint8_t type = FLPROG_ANON_INTERFACE;

    Stream *stream() { return tcpClient(); };
    bool hasStream() { return tcpClient()->connected(); };

    Client *tcpClient();
    void setAvalibleClientFromServer();
    IPAddress clientRemoteIp();
    int clientRemotePort();
    IPAddress workIp;

private:
    FlprogAbstractEthernet *ethernetInterface = 0;
    FlprogEthernetServer *ethernetServer = 0;
    FlprogEthernetClient ethernetClient;
#ifdef FLPROG_WIFI_TCP_DEVICE
    FLProgAbstracttWiFiInterface *wifiInterface = 0;
    WiFiServer *wifiServer = 0;
    WiFiClient wifiClient;
#endif
};
