#include "flprogModbusMasterRTUESP32Bluetooth.h"

#ifdef ESP32

ModbusMasterRTUESP32Bluetooth::ModbusMasterRTUESP32Bluetooth(ModbusSlaveInMaster table[], int size)
{
   slavesSize = size;
    slaves = table;   
}

void ModbusMasterRTUESP32Bluetooth::begin()
{
    port.begin(deviceName);
    if (isMaster)
    {
        port.connect("test23");
    }

}

void ModbusMasterRTUESP32Bluetooth::beBluetoothMaster()
{
    if (isMaster)
    {
        return;
    }
    isMaster = true;
    restartPort();
}

void ModbusMasterRTUESP32Bluetooth::beBluetoothSlave()
{
    if (!isMaster)
    {
        return;
    }
    isMaster = false;
    restartPort();
}

void ModbusMasterRTUESP32Bluetooth::setPartnerName(String name)
{
    if (partnerName.equals(name))
    {
        return;
    }
    partnerName = name;
    restartPort();
}

void ModbusMasterRTUESP32Bluetooth::setDeviceName(String name)
{
    if (deviceName.equals(name))
    {
        return;
    }
    deviceName = name;
    restartPort();
}

void ModbusMasterRTUESP32Bluetooth::restartPort()
{
    port.end();
    port.begin(deviceName);
    if (isMaster)
    {
        port.connect("test23");
    }
}

byte ModbusMasterRTUESP32Bluetooth::portAvailable()
{
    return port.available();
}

byte ModbusMasterRTUESP32Bluetooth::portRead()
{
    return port.read();
}

byte ModbusMasterRTUESP32Bluetooth::portWrite(byte *buffer, byte size)
{
    return port.write(buffer, size);
}

bool ModbusMasterRTUESP32Bluetooth::hasPort()
{
    return true;
}

#endif