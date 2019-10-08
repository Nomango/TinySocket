#include <iostream>
#include "tinysocket.h"

int main()
{
	using namespace tinysocket;

	Client client;
	// ���ӷ����
	if (client.Connect(Protocol::TCP, Address::LocalHost(1234)))
	{
		std::cout << client.GetAddress().GetIPv4Address() << ":" << client.GetAddress().GetPort() << std::endl;

		// ��������
		if (client.Send("Hello world") != 0)
			std::cout << "Send message successfully" << std::endl;

		// ��������
		std::string received = client.Receive();
		std::cout << "Received message: " << received << std::endl;
	}
	// �رտͻ���
	client.Close();

	// �����������
	std::cout << "Press Enter to quit!" << std::endl;
	std::cin.ignore();
	return 0;
}
