#include "flprogModbusRTU.h"
#include "flprogModbusTCP.h"
#include "flprogModbusBridge.h"
#include <type_traits>
static_assert(std::is_base_of<Modbus,ModbusMasterRTU>::value,"RTU Master API");
static_assert(std::is_base_of<Modbus,ModbusSlaveRTU>::value,"RTU Slave API");
static_assert(std::is_base_of<Modbus,ModbusMasterTCP>::value,"TCP Master API");
static_assert(std::is_base_of<Modbus,ModbusSlaveTCP>::value,"TCP Slave API");
int main(){return 0;}
