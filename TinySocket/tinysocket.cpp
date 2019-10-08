#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cassert>
#include "tinysocket.h"

#pragma comment (lib,"ws2_32.lib")

///////////////////////////////////////////////
//
// WSACore
//
///////////////////////////////////////////////

namespace
{
	class WSACore
	{
	public:
		WSACore()
		{
			WSADATA wsData;
			int ret = WSAStartup(MAKEWORD(2, 2), &wsData);

			assert(ret == 0 && "WSAStartup failed");
		}

		~WSACore()
		{
			WSACleanup();
		}

		static WSACore& GetInstance()
		{
			static WSACore core;  // thread-safe
			return core;
		}
	};
}

///////////////////////////////////////////////
//
// Address
//
///////////////////////////////////////////////

tinysocket::Address::Address()
{
	memset(&addr_, 0, sizeof(addr_));
}

tinysocket::Address::Address(int port)
	: Address()
{
	SetIPv4(port);
}

tinysocket::Address::Address(const SOCKADDR& addr)
{
	addr_ = *reinterpret_cast<const SOCKADDR_IN*>(&addr);
}

tinysocket::Address::Address(const SOCKADDR_IN& addr)
{
	addr_ = addr;
}

tinysocket::Address::Address(const std::string& ip, int port)
	: Address()
{
	SetIPv4(ip, port);
}

void tinysocket::Address::SetIPv4(int port)
{
	addr_.sin_family		= PF_INET;	// ipv4
	addr_.sin_addr.s_addr	= ADDR_ANY;
	addr_.sin_port			= htons(u_short(port));
}

void tinysocket::Address::SetIPv4(const std::string& ip, int port)
{
	addr_.sin_family	= PF_INET;	// ipv4
	addr_.sin_port		= htons(u_short(port));

	inet_pton(addr_.sin_family, ip.c_str(), &addr_.sin_addr.s_addr);
}

int tinysocket::Address::GetPort() const
{
	return static_cast<int>(ntohs(addr_.sin_port));
}

std::string tinysocket::Address::GetIPv4Address() const
{
	char buffer[16] = { 0 };
	inet_ntop(addr_.sin_family, &addr_, buffer, sizeof(buffer));
	return buffer;
}

const sockaddr_in& tinysocket::Address::GetRawAddr() const
{
	return addr_;
}

tinysocket::Address tinysocket::Address::LocalHost(int port)
{
	// Get the local host information
	hostent*	localHost	= gethostbyname("");
	char*		localIP		= inet_ntoa(*(struct in_addr*) * localHost->h_addr_list);

	return Address(localIP, port);
}


///////////////////////////////////////////////
//
// Resource
//
///////////////////////////////////////////////

tinysocket::Resource::Resource()
{
	socket_ = INVALID_SOCKET;
}

tinysocket::Resource::~Resource()
{
	Close();
}

bool tinysocket::Resource::Bind(Protocol protocol, const Address& addr)
{
	addr_ = addr;
	socket_ = socket(AF_INET, SOCK_STREAM, (protocol == Protocol::TCP) ? IPPROTO_TCP : IPPROTO_UDP);
	return socket_ != INVALID_SOCKET;
}

tinysocket::Address tinysocket::Resource::GetAddress() const
{
	return addr_;
}

SOCKET tinysocket::Resource::GetSocket() const
{
	return socket_;
}

void tinysocket::Resource::Close()
{
	// Socket handles may take any value in the range 0 to INVALID_SOCKET¨C1
	if (socket_ != INVALID_SOCKET)
	{
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
	}
}


///////////////////////////////////////////////
//
// Connection
//
///////////////////////////////////////////////

tinysocket::Connection::Connection()
{
}

tinysocket::Connection::Connection(const Address& addr, const SOCKET socket)
{
	addr_ = addr;
	socket_ = socket;
}

tinysocket::Connection::Connection(Connection&& rhs) noexcept
{
	std::swap(addr_, rhs.addr_);
	std::swap(socket_, rhs.socket_);
}

tinysocket::Connection::~Connection()
{
}

bool tinysocket::Connection::IsValid() const
{
	return socket_ != INVALID_SOCKET;
}

std::string tinysocket::Connection::Receive() const
{
	static char buffer[2048] = { 0 };
	int bytes = recv(GetSocket(), buffer, sizeof(buffer), NULL);
	return std::string(buffer, bytes);
}

int tinysocket::Connection::Send(const std::string& data) const
{
	return send(GetSocket(), data.c_str(), static_cast<int>(data.length() + sizeof(char)), 0);
}


///////////////////////////////////////////////
//
// Client
//
///////////////////////////////////////////////

tinysocket::Client::Client()
{
	// Make sure WSA initialized
	(void)WSACore::GetInstance();
}

tinysocket::Client::~Client()
{
}

bool tinysocket::Client::Connect(Protocol protocol, const Address& addr)
{
	if (!Bind(protocol, addr))
	{
		return false;
	}

	auto& name = addr.GetRawAddr();
	int ret = connect(socket_, reinterpret_cast<const SOCKADDR*>(&name), sizeof(SOCKADDR));

	return ret != SOCKET_ERROR;
}

std::string tinysocket::Client::Receive() const
{
	static char buffer[2048] = { 0 };
	int bytes = recv(GetSocket(), buffer, sizeof(buffer), NULL);
	return std::string(buffer, bytes);
}

int tinysocket::Client::Send(const std::string& data) const
{
	return send(GetSocket(), data.c_str(), static_cast<int>(data.length() + sizeof(char)), 0);
}


///////////////////////////////////////////////
//
// Server
//
///////////////////////////////////////////////

tinysocket::Server::Server()
{
	// Make sure WSA initialized
	(void)WSACore::GetInstance();
}

tinysocket::Server::~Server()
{
}

bool tinysocket::Server::Listen(Protocol protocol, const Address& addr, int backlog)
{
	if (!Bind(protocol, addr))
	{
		return false;
	}

	auto& name = addr.GetRawAddr();
	if (bind(socket_, reinterpret_cast<const SOCKADDR*>(&name), sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		return false;
	}

	if (listen(GetSocket(), backlog) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

tinysocket::Connection tinysocket::Server::Accept()
{
	SOCKADDR clientAddr;
	int size = sizeof(SOCKADDR);
	SOCKET clientSocket = accept(GetSocket(), &clientAddr, &size);

	if (clientSocket == INVALID_SOCKET)
	{
		return Connection();
	}

	return Connection(Address(clientAddr), clientSocket);
}
