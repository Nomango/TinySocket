#include <iostream>
#include "tinysocket.h"

int main()
{
	using namespace tinysocket;

	Server server;
	// 设置监听端口
	if (server.Listen(Protocol::TCP, Address::LocalHost(1234)))
	{
		std::cout << "Server is running on " << server.GetAddress().GetIPv4Address() << ":" << server.GetAddress().GetPort() << std::endl;

		while (true)
		{
			// 获取连接请求
			Connection conn = server.Accept();
			// 读取客户端发来的数据
			std::string received = conn.Receive();
			std::cout << "Received message: " << received << std::endl;
			// 将相同的数据发回客户端
			conn.Send(received);
		}
	}
	// 关闭服务端
	server.Close();
	return 0;
}
