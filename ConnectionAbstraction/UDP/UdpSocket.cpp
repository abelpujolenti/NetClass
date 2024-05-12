#include "UdpSocket.h"

#include "../../CreateSharedPointer.h"

class ConnectionHandshake : public ICodable
{
private:

	UdpAddress _address;


public:
	
	bool isHello;

	ConnectionHandshake() = default;
	ConnectionHandshake(UdpAddress address, bool isHello) : _address(address), isHello(isHello)
	{};

	UdpAddress GetAddress() 
	{
		return _address;
	};

	void Code(sf::Packet& packet) override
	{
		packet << _address << isHello;
	}

	void Decode(sf::Packet& packet) override
	{
		packet >> _address >> isHello;
	}
};

UdpSocket::UdpSocket(UdpAddress::PortNumber port, OnConnectionEnter onConnectionEnter) : _onConnectionEnter(
	std::move(onConnectionEnter))
{
	const sf::IpAddress ipAddress{ sf::IpAddress::getLocalAddress() };
	_address = UdpAddress(ipAddress.toString(), port);

	if (bind(port) != sf::Socket::Done)
	{
		return;
	}

	_loopThread = std::make_unique<std::thread>(&UdpSocket::ReceiveLoop, this);
	_loopThread->detach();
}

void UdpSocket::ConnectTo(UdpAddress address)
{
	ConnectionHandshake helloHandshake{ ConnectionHandshake(GetAddress(), true) };

	std::shared_ptr<UdpConnection> newConnection = std::make_shared<UdpConnection>(shared_from_this() /*CreateSharedPointer::ReturnSharedFromThis(this)*/, address);
	
	_pendantConnectionsMapMutex.lock();

	_pendantConnectionsMap.emplace(newConnection->GetAddress().ToString(), newConnection); 

	_pendantConnectionsMapMutex.unlock();

	std::weak_ptr<UdpConnection> weakNewConnection {newConnection};

	newConnection->SendCritical(CONNECTION_KEY, helloHandshake, [&, weakNewConnection](UdpPacket packet)
	{
		if (const std::shared_ptr<UdpConnection> sharedNewConnection {weakNewConnection.lock()})
		{
			std::string key {sharedNewConnection->GetAddress().ToString()};

			_connectionsMap.emplace(key, sharedNewConnection);

			_pendantConnectionsMap.erase(key);

			_onConnectionEnter(sharedNewConnection);			
		}		
	});
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

		const UdpAddress address{ UdpAddress(ip.toString(), port)};

		_receiveFunctions[status](packet, address);

		_isRunningMutex.lock();
		isRunning = _isRunning;
		_isRunningMutex.unlock();

	} while (isRunning);
}

void UdpSocket::ManageReceivePacketDone(UdpPacket packet, UdpAddress address)
{
	const std::string addressKey{ address.ToString() };

	const std::map<std::string, std::shared_ptr<UdpConnection>>::iterator it{ _connectionsMap.find(addressKey) };

	if (it != _connectionsMap.end())
	{
		it->second->ManageReceivedPacket(packet);
		return;
	}

	_pendantConnectionsMapMutex.lock();

	const std::map<std::string, std::shared_ptr<UdpConnection>>::iterator itPendant {_pendantConnectionsMap.find(addressKey)};

	if (itPendant != _pendantConnectionsMap.end())
	{
		itPendant->second->ManageReceivedPacket(packet);
		_pendantConnectionsMapMutex.unlock();
		return;
	}

	sf::Uint8 intType;
	sf::Uint8 intKey;
	UdpPacket::CriticalPacketId id;
	ConnectionHandshake helloHandshake;

	packet >> intType >> intKey >> id >> helloHandshake;

	if (helloHandshake.GetAddress().ToString() != addressKey || !helloHandshake.isHello)
	{
		return;
	}

	const std::shared_ptr<UdpConnection> newConnection{ std::make_shared<UdpConnection>( shared_from_this()/*CreateSharedPointer::ReturnSharedFromThis(this)*/, address) };
	_connectionsMap.emplace(addressKey, newConnection);

	newConnection->SubscribeOnCritical(CONNECTION_KEY, [](UdpPacket helloPacket)
	{
		std::shared_ptr<ConnectionHandshake> handshake {std::make_shared<ConnectionHandshake>()};
		helloPacket >> *handshake;
		handshake->isHello = false;

		return handshake;
	});
	
	_onConnectionEnter(newConnection);

	if (!helloHandshake.isHello)
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

/*void* UdpSocket::GetValues() const
{
	constexpr int sizeOfArray {9};
	
	const void* sizes[sizeOfArray];
	
	int size {sizeof(int)};

	sizes[0] = &size;
	sizes[1] = &_isRunning;
	sizes[2] = &_loopThread;
	sizes[3] = &_isRunningMutex;
	sizes[4] = &_onConnectionEnter;
	sizes[5] = &_address;
	sizes[6] = &_connectionsMap;
	sizes[7] = &_pendantConnectionsMapMutex;
	sizes[8] = &_pendantConnectionsMap;	

	for (int i {0}; i < sizeOfArray; ++i)
	{
		size += sizeof(sizes[i]);
	}
	
    const void* data[sizeOfArray];

	for (int i {0}; i < sizeOfArray; ++i)
	{
        data[i] = sizes[i];
	}

	return data;
}

void UdpSocket::SetValues(void* data)
{
	int currentSizeRead = 0;

	_isRunning = *static_cast<bool*>(&data[currentSizeRead]);
	currentSizeRead++;

	_loopThread.reset(static_cast<std::unique_ptr<std::thread>*>(&data[currentSizeRead])->get());

	_isRunningMutex = *static_cast<std::mutex*>(&data[currentSizeRead]);
	currentSizeRead++;

	_onConnectionEnter = *static_cast<OnConnectionEnter*>(&data[currentSizeRead]);
	currentSizeRead++;

	_address = *static_cast<UdpAddress*>(&data[currentSizeRead]);

	_connectionsMap = *static_cast<std::map<std::string, std::shared_ptr<UdpConnection>>*>(&data[currentSizeRead]);
	currentSizeRead++;

	_pendantConnectionsMapMutex = *static_cast<std::mutex*>(&data[currentSizeRead]);
	currentSizeRead++;

	_pendantConnectionsMap = *static_cast<std::map<std::string, std::shared_ptr<UdpConnection>>*>(&data[currentSizeRead]);

	free(data);
}*/