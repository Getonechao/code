#include<modbus/modbus.h>
#include<modbus/modbus-version.h>
#include<iostream>
#include<unistd.h>

int main()
{
    //modbus_t 对象
    modbus_t* ctx=modbus_new_tcp("192.168.209.1",9000);
    if(ctx==nullptr){std::cerr<<"err:modbus_new_tcp failed!!!"; return 1; }   
    //debug模式
    // int ret1 = modbus_set_debug(ctx, TRUE);
    // if (-1 == ret1)
    // {
    //     std::cerr<<"Error: 设置Debug模式失败";
    //     modbus_free(ctx);
    //     return 1;
    // }
	//3. 连接Server
	int ret2 = modbus_connect(ctx);
	if (-1 == ret2)
	{
		fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return 1;
	}
    // sleep(10);
    modbus_set_response_timeout(ctx,5,200000);
    modbus_set_indication_timeout(ctx,5,500000);
    std::cout<<"slave set:"<< modbus_set_slave(ctx,10)<<std::endl;
    std::cout<<"slave addr:"<< modbus_get_slave(ctx)<<std::endl;
    std::cout<<"socket:"<< modbus_get_socket(ctx)<<std::endl;
    uint32_t sec,usec;
    modbus_get_response_timeout(ctx,&sec,&usec);
    std::cout<<"response time:"<<sec<<"."<<usec<<std::endl;
    modbus_get_indication_timeout(ctx,&sec,&usec);
    std::cout<<"服务端等待客户端请求超时时间:"<<sec<<"."<<usec<<std::endl;
    modbus_get_byte_timeout(ctx, &sec, &usec);
    std::cout<<"字节连续时间:"<<sec<<"."<<usec<<std::endl;

    uint16_t* read_input_registers_buf = (uint16_t*)malloc(1 * sizeof(uint16_t));
    int ret3 = modbus_read_input_registers(ctx, 1000, 1, read_input_registers_buf);

    std::cout<<"1000:"<<read_input_registers_buf[0]<<"\n";
            //  <<"1001:"<<read_input_registers_buf[1]<<"\n"
            //  <<"1002:"<<read_input_registers_buf[2]<<"\n";
    
    free(read_input_registers_buf);
    modbus_close(ctx);
    modbus_free(ctx);

}