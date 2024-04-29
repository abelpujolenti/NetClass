#include "UdpSocket.h"

class ConnectionHandshake : public ICodable
{
private:

	UdpAddress _address;
	bool _isHello;


public:

	ConnectionHandshake() = default;
	ConnectionHandshake(UdpAddress address, bool isHello) : _address(address), _isHello(isHello)
	{};

	UdpAddress GetAddress() 
	{
		return _address;
	};

	bool IsHello() 
	{
		return _isHello;
	};

	void Code(sf::Packet& packet) override
	{
		packet << _address << _isHello;
	}

	void Decode(sf::Packet& packet) override
	{
		packet >> _address >> _isHello;
	}
};

UdpSocket::UdpSocket(UdpAddress::PortNumber port, OnConnectionEnter onConnectionEnter) : _onConnectionEnter(onConnectionEnter)
{
	sf::IpAddress ipAddress{ sf::IpAddress::getLocalAddress() };
	_address = UdpAddress(ipAddress.toString(), port);

	if (bind(port) != sf::Socket::Done)
	{
		return;
	}

	_loopThread = std::make_unique<std::thread>(&UdpSocket::ReceiveLoop, this);
	_loopThread->detach();
}

UdpSocket::~UdpSocket()
{

}

void UdpSocket::ConnectTo(UdpAddress address)
{
	ConnectionHandshake helloHandshake{ ConnectionHandshake(GetAddress(), true) };
	SendImmediately(address, UdpPacket::NormalPacket(0, helloHandshake));
}

UdpAddress UdpSocket::GetAddress()
{
	return _address;
}

void UdpSocket::ReceiveLoop()
{
	_isRunning = true;

	bool isRunning{ true };

	do
	{
		UdpPacket packet;
		sf::IpAddress ip;
		UdpAddress::PortNumber port;

		sf::Socket::Status status{ receive(packet, ip, port) };

		UdpAddress address{ UdpAddress(ip.toString(), port)};

		_receiveFunctions[status](packet, address);

		_isRunningMutex.lock();
		isRunning = _isRunning;
		_isRunningMutex.unlock();

	} while (isRunning);
}

void UdpSocket::ManageReceivePacketDone(UdpPacket packet, UdpAddress address)
{
	std::string addressKey{ address.ToString() };

	std::map<std::string, std::shared_ptr<UdpConnection>>::iterator it{ _connectionsMap.find(addressKey) };

	if (it != _connectionsMap.end())
	{
		it->second->ManageReceivedPacket(packet);
		return;
	}

	sf::Uint8 intType;
	UdpPacket::PacketKey key;
	ConnectionHandshake helloHandshake;

	packet >> intType >> key >> helloHandshake;

	if (helloHandshake.GetAddress().ToString() != addressKey)
	{
		return;
	}

	std::shared_ptr<UdpConnection> newConnection{ std::make_shared<UdpConnection>(this, address) };
	_connectionsMap[addressKey] = newConnection;
	_onConnectionEnter(newConnection);

	if (!helloHandshake.IsHello())
	{
		return;
	}

	ConnectionHandshake welcomeHandshake{ ConnectionHandshake(GetAddress(), false) };
	SendImmediately(address, UdpPacket::NormalPacket(0, welcomeHandshake));
	//TODO Convert to Critical
}

void UdpSocket::SendAccumulating(UdpAddress address, UdpPacket packet)
{
	//TODO
}

void UdpSocket::SendImmediately(UdpAddress address, UdpPacket packet)
{
	send(packet, address.ip, address.port);
}