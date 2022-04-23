#include<modbus/modbus.h>
#include<modbus/modbus-version.h>
#include<iostream>
#include<unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

//TCP模式的Slave端程序
int main(void)
{
	int ret = 0;

	int sockfd = -1;
	modbus_t* ctx = NULL;
	modbus_mapping_t* map = NULL;

	uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];

	//1. 设置串口信息
	ctx = modbus_new_tcp("192.168.209.205", 9000);
    modbus_set_slave(ctx,10);
	if (NULL == ctx)
	{
		fprintf(stderr, "Error: %s\n", modbus_strerror(errno));
		return 1;
	}
	else
	{
		printf("设置TCP信息成功\n");
	}


	//2. 设置调试模式
	ret = modbus_set_debug(ctx, TRUE);
	if (-1 == ret)
	{
		printf("modbus_set_debug failed...\n");
		modbus_free(ctx);
		return 1;
	}


	//3. 申请内存 存放寄存器数据
	map = modbus_mapping_new(500, 500, 500, 500);
	if (NULL == map)
	{
		fprintf(stderr, "Error: mapping %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return 1;
	}

	//4. 开始监听端口
	sockfd = modbus_tcp_listen(ctx, 1);
	if (-1 == sockfd)
	{
		printf("modbus_tcp_listen failed...\n");
		modbus_free(ctx);
		return 1;
	}

	//5. 接受客户端连接
	ret = modbus_tcp_accept(ctx, &sockfd);
	if (-1 == ret)
	{
		printf("modbus_tcp_accept failedl: %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return 1;
	}
	std::cout<<"开始的寄存器地址："<<map->start_input_registers<<std::endl;

    map->tab_input_registers[1]=1;
    map->tab_input_registers[2]=2;
    map->tab_input_registers[3]=3;
    	//6. 循环接受客户端请求，并且响应客户端
	while (1)
	{
		memset(query, 0, sizeof(query));

		//获取查询请求报文
		ret = modbus_receive(ctx, query);
		if (ret >= 0)
		{
			//恢复响应报文,ret是query的长度
			modbus_reply(ctx, query, ret, map);
          
		}
		else
		{
			printf("Connection close\n");

			modbus_close(ctx);

			//等待下一个客户端连接
			modbus_tcp_accept(ctx, &sockfd);
		}
	}

	printf("Quit the loop: %s\n", modbus_strerror(errno));


	//6. 释放内存
	modbus_mapping_free(map);
	
	//7. 关闭设备
	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}