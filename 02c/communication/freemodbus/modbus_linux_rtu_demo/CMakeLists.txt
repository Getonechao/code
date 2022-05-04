project(modbus_linux_rtu_demo)


include_directories(
    modbus/modbus/include
    modbus/modbus/rtu
    modbus/modbus/ascii
    modbus/modbus/tcp
    modbus/demo/LINUX/port
)

aux_source_directory(modbus/modbus  MODBUS_LIST)
aux_source_directory(modbus/modbus/functions MODBUS_FUNCTIONS_LIST)
aux_source_directory(modbus/modbus/rtu  MODBUS_RTU_LIST)
aux_source_directory(modbus/modbus/ascii  MODBUS_ASCII_LIST)
aux_source_directory(modbus/modbus/tcp  MODBUS_TCP_LIST)
aux_source_directory(modbus/demo/LINUX/port  MODBUS_DEMO_LINUX_PORT_LIST)


add_executable(${PROJECT_NAME} 
test/main.cpp
${MODBUS_LIST}
${MODBUS_FUNCTIONS_LIST}
${MODBUS_RTU_LIST}
${MODBUS_TCP_LIST}
${MODBUS_ASCII_LIST}
${MODBUS_DEMO_LINUX_PORT_LIST}
)