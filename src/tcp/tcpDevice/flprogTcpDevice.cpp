#include "flprogTcpDevice.h"

//------------FLProgTcpDevice--------
FLProgTcpDevice::FLProgTcpDevice(FlprogAbstractEthernet *sourse)
{
    type = FLPROG_ETHERNET_INTERFACE;
    ethernetInterface = sourse;
}

FLProgTcpDevice::FLProgTcpDevice(FLProgAbstracttWiFiInterface *sourse)
{
#ifdef FLPROG_WIFI_TCP_DEVICE
    type = FLPROG_WIFI_INTERFACE;
    wifiInterface = sourse;
#else
    (void)sourse;
#endif
}

void FLProgTcpDevice::setPort(int port)
{
    if (tcpPort == port)
        return;
    tcpPort = port;
    restart();
}

void FLProgTcpDevice::beClient()
{
    if (!mode)
        return;
    mode = FLPROG_TSP_CLIENT_MODE;
    restart();
}

void FLProgTcpDevice::beServer()
{
    if (mode)
        return;
    mode = FLPROG_TCP_SERVER_MODE;
    restart();
}

void FLProgTcpDevice::restart()
{
    stop();
    if (mode && (hasServer()))
    {
        begin();
    }
}

void FLProgTcpDevice::stop()
{
    tcpClient()->stop();
}

uint8_t FLProgTcpDevice::read()
{
    return tcpClient()->read();
}

int FLProgTcpDevice::read(uint8_t *buf, size_t size)
{
    return tcpClient()->read(buf, size);
}

bool FLProgTcpDevice::connected()
{
    return tcpClient()->connected();
}

int FLProgTcpDevice::connect(IPAddress newIp, int newPort)
{
    if (mode)
    {
        if (!tcpClient()->connected())
        {
            setAvalibleClientFromServer();
            return 1;
        }
        return 1;
    }
    if (tcpClient()->connected())
    {
        if (clientRemoteIp() == newIp)
        {
            if (clientRemotePort() == newPort)
            {
                return 1;
            }
        }
    }
    stop();
    return tcpClient()->connect(newIp, newPort);
}

int FLProgTcpDevice::connect(const char *host, uint16_t newPort)
{
    return tcpClient()->connect(host, newPort);
}

void FLProgTcpDevice::begin()
{
    if (mode)
    {
        if (type == FLPROG_ETHERNET_INTERFACE)
        {
            ethernetServer = new FlprogEthernetServer(ethernetInterface, tcpPort);
            ethernetServer->begin();
        }
#ifdef FLPROG_WIFI_TCP_DEVICE
        if (type == FLPROG_WIFI_INTERFACE)
        {
            wifiServer = new WiFiServer(tcpPort);
            wifiServer->begin();
        }
#endif
    }
}

void FLProgTcpDevice::setAvalibleClientFromServer()
{
    if (type == FLPROG_ETHERNET_INTERFACE)
    {
        if (ethernetServer == 0)
            return;
        ethernetClient = ethernetServer->accept();
    }
#ifdef FLPROG_WIFI_TCP_DEVICE
    if (type == FLPROG_WIFI_INTERFACE)
    {
        if (wifiServer == 0)
            return;
        wifiClient = wifiServer->accept();
    }
#endif
}

Client *FLProgTcpDevice::tcpClient()
{
    if (type == FLPROG_ETHERNET_INTERFACE)
    {
        ethernetClient.init(ethernetInterface);
        return &ethernetClient;
    }
#ifdef FLPROG_WIFI_TCP_DEVICE
    if (type == FLPROG_WIFI_INTERFACE)
    {
        return &wifiClient
    }
#endif
    return 0;
}

bool FLProgTcpDevice::hasClient()
{
    if (type == FLPROG_ETHERNET_INTERFACE)
    {
        return ethernetClient;
    }
#ifdef FLPROG_WIFI_TCP_DEVICE
    if (type == FLPROG_WIFI_INTERFACE)
    {
        return wifiClient;
    }
#endif
    return false;
}

bool FLProgTcpDevice::hasServer()
{
    if (type == FLPROG_ETHERNET_INTERFACE)
    {
        return ethernetServer != 0;
    }
#ifdef FLPROG_WIFI_TCP_DEVICE
    if (type == FLPROG_WIFI_INTERFACE)
    {
        return wifiServer != 0;
    }
#endif
    return false;
}

IPAddress FLProgTcpDevice::clientRemoteIp()
{
    if (type == FLPROG_ETHERNET_INTERFACE)
    {
        return ethernetClient.remoteIP();
    }
#ifdef FLPROG_WIFI_TCP_DEVICE
    if (type == FLPROG_WIFI_INTERFACE)
    {
        return wifiClient.remoteIP();
    }
#endif
    return IPAddress(0, 0, 0, 0);
}

int FLProgTcpDevice::clientRemotePort()
{
    if (type == FLPROG_ETHERNET_INTERFACE)
    {
        return ethernetClient.remotePort();
    }
#ifdef FLPROG_WIFI_TCP_DEVICE
    if (type == FLPROG_WIFI_INTERFACE)
    {
        return wifiClient.remotePort();
    }
#endif
    return 0;
}
