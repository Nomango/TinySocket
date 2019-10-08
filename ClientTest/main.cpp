#include <iostream>
#include "tinysocket.h"

int main()
{
	using namespace tinysocket;

	Client client;
	// 连接服务端
	if (client.Connect(Protocol::TCP, Address::LocalHost(1234)))
	{
		std::cout << client.GetAddress().GetIPv4Address() << ":" << client.GetAddress().GetPort() << std::endl;

		// 发送数据
		if (client.Send("Hello world") != 0)
			std::cout << "Send message successfully" << std::endl;

		// 接收数据
		std::string received = client.Receive();
		std::cout << "Received message: " << received << std::endl;
	}
	// 关闭客户端
	client.Close();

	// 按任意键结束
	std::cout << "Press Enter to quit!" << std::endl;
	std::cin.ignore();
	return 0;
}
