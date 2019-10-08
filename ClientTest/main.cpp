#include <iostream>
#include <tinysocket.h>

int main()
{
	using namespace tinysocket;

	Client client;
	// Connect to the test server
	if (client.Connect(Protocol::TCP, Address::LocalHost(1234)))
	{
		std::cout << "Connect to " << client.GetAddress().GetIPv4Address() << ":" 
			<< client.GetAddress().GetPort() << " successfully" << std::endl;

		// Send a string message
		if (client.Send("Hello world") != 0)
			std::cout << "Send message successfully" << std::endl;

		// Receive a response from local server
		std::string received = client.Receive();
		std::cout << "Received message: " << received << std::endl;
	}
	// close the connection
	client.Close();

	// wait for exit
	std::cout << "Press Enter to quit!" << std::endl;
	std::cin.ignore();
	return 0;
}
