project(libmodbus_tcp_demo)



add_executable(${PROJECT_NAME} TCP_server/main.cpp)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libmodbus.so)