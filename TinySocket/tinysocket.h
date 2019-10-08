#pragma once

#include <string>
#include <WS2tcpip.h>

namespace tinysocket
{
	// Э������
	enum class Protocol
	{
		UDP,
		TCP
	};

	// Address ��ʾһ�� IPv4 ��ַ
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

	// Resource ��ʾһ�����е�ַ�� socket ��Դ
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

	// Connection ��ʾһ�� Socket ����
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

	// Client ��ʾһ�� Socket �ͻ���
	class Client : public Resource
	{
	public:
		Client();
		virtual ~Client();

		bool Connect(Protocol protocol, const Address& addr);
		std::string Receive() const;
		int Send(const std::string& data) const;
	};

	// Server ��ʾһ�� Socket �����
	class Server : public Resource
	{
	public:
		Server();
		virtual ~Server();

		bool Listen(Protocol protocol, const Address& addr, int backlog = SOMAXCONN);
		Connection Accept();
	};
}
