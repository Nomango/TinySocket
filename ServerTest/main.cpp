#include <iostream>
#include <tinysocket.h>

int main()
{
	using namespace tinysocket;

	Server server;
	// Start to listen TCP
	if (server.Listen(Protocol::TCP, Address::LocalHost(1234)))
	{
		std::cout << "Server is running on " << server.GetAddress().GetIPv4Address() << ":"
			<< server.GetAddress().GetPort() << std::endl;

		while (true)
		{
			// Accept a connection
			Connection conn = server.Accept();
			// Read a message from the client
			std::string received = conn.Receive();
			std::cout << "Received message: " << received << std::endl;
			// Echo
			conn.Send(received);
		}
	}
	// Stop the server
	server.Close();
	return 0;
}
