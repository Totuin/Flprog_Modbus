#include "flprogModbusSlaveRTUESP32Bluetooth.h"
#ifdef ESP32

void ModbusSlaveRTUESP32Bluetooth::begin(byte addres)
{
    port.begin(deviceName);
    if (isMaster)
    {
        port.connect(partnerName);
    }

    slaveAddres = addres;
}
void ModbusSlaveRTUESP32Bluetooth::beBluetoothMaster()
{
    if (isMaster)
    {
        return;
    }
    isMaster = true;
    restartPort();
}

void ModbusSlaveRTUESP32Bluetooth::beBluetoothSlave()
{
    if (!isMaster)
    {
        return;
    }
    isMaster = false;
    restartPort();
}

void ModbusSlaveRTUESP32Bluetooth::setPartnerName(String name)
{
    if (partnerName.equals(name))
    {
        return;
    }
    partnerName = name;
    restartPort();
}

void ModbusSlaveRTUESP32Bluetooth::setDeviceName(String name)
{
    if (deviceName.equals(name))
    {
        return;
    }
    deviceName = name;
    restartPort();
}

void ModbusSlaveRTUESP32Bluetooth::restartPort()
{
    port.end();
    port.begin(deviceName);
    if (isMaster)
    {
        port.connect(partnerName);
    }
}

byte ModbusSlaveRTUESP32Bluetooth::portAvailable()
{
    return port.available();
}

byte ModbusSlaveRTUESP32Bluetooth::portRead()
{
    return port.read();
}

byte ModbusSlaveRTUESP32Bluetooth::portWrite(byte *buffer, byte size)
{
    return port.write(buffer, size);
}

bool ModbusSlaveRTUESP32Bluetooth::hasPort()
{
    return true;
}

#endif