#pragma once

#include <string>
#include <WS2tcpip.h>

namespace tinysocket
{
	// 协议类型
	enum class Protocol
	{
		UDP,
		TCP
	};

	// Address 表示一个 IPv4 地址
	class Address
	{
	public:
		Address();
		explicit Address(int port);
		explicit Address(const SOCKADDR& addr);
		explicit Address(const SOCKADDR_IN& addr);
		Address(const std::string& ip, int port);

		void SetIPv4(int port);
		void SetIPv4(const std::string& ip, int port);

		int GetPort() const;
		std::string GetIPv4Address() const;
		const SOCKADDR_IN& GetRawAddr() const;

		static Address LocalHost(int port);

	protected:
		SOCKADDR_IN addr_;
	};

	// Resource 表示一个带有地址的 socket 资源
	class Resource
	{
	public:
		Resource();
		virtual ~Resource();

		Address GetAddress() const;

		SOCKET GetSocket() const;

		void Close();

	protected:
		bool Bind(Protocol protocol, const Address& addr);

	protected:
		Address	addr_;
		SOCKET	socket_;
	};

	// Connection 表示一个 Socket 连接
	class Connection : public Resource
	{
	public:
		Connection();
		Connection(const Address& addr, const SOCKET socket);
		Connection(Connection&& rhs) noexcept;
		virtual ~Connection();

		bool IsValid() const;
		std::string Receive() const;
		int Send(const std::string& data) const;

	protected:
		Connection(const Connection& rhs) = delete;
		Connection& operator= (const Connection& rhs) = delete;
	};

	// Client 表示一个 Socket 客户端
	class Client : public Resource
	{
	public:
		Client();
		virtual ~Client();

		bool Connect(Protocol protocol, const Address& addr);
		std::string Receive() const;
		int Send(const std::string& data) const;
	};

	// Server 表示一个 Socket 服务端
	class Server : public Resource
	{
	public:
		Server();
		virtual ~Server();

		bool Listen(Protocol protocol, const Address& addr, int backlog = SOMAXCONN);
		Connection Accept();
	};
}
