#include <iostream>
#include "tinysocket.h"

int main()
{
	using namespace tinysocket;

	Server server;
	// ���ü����˿�
	if (server.Listen(Protocol::TCP, Address::LocalHost(1234)))
	{
		std::cout << "Server is running on " << server.GetAddress().GetIPv4Address() << ":" << server.GetAddress().GetPort() << std::endl;

		while (true)
		{
			// ��ȡ��������
			Connection conn = server.Accept();
			// ��ȡ�ͻ��˷���������
			std::string received = conn.Receive();
			std::cout << "Received message: " << received << std::endl;
			// ����ͬ�����ݷ��ؿͻ���
			conn.Send(received);
		}
	}
	// �رշ����
	server.Close();
	return 0;
}
