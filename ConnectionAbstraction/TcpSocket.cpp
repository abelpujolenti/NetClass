#include "TcpSocket.h"

bool TcpSocket::Connect(std::string ip, unsigned short port)
{
	return connect(ip, port) == sf::Socket::Done;
}

void TcpSocket::Receive()
{
	Packet packet;
	sf::Socket::Status status = receive(packet);

	switch (status)
	{
	case sf::Socket::Done:
		ProcessPacket(packet);
		break;
	case sf::Socket::Disconnected:
		_onSocketDisconnectMutex.lock();

		for (OnSocketDisconnect onDisconnect : _onSocketDisconnectSubscriptions)
		{
			onDisconnect(this);
		}
		_onSocketDisconnectSubscriptions.clear();
		_onSocketDisconnectMutex.unlock();
		break;
	case sf::Socket::NotReady:
	case sf::Socket::Partial:
	case sf::Socket::Error:
	default:
		//TODO Deberes Gestionar el error
		break;
	}
}

bool TcpSocket::Send(Packet::PacketKey key)
{
	Packet signedPacket;

	signedPacket << key;

	return send(signedPacket) == sf::Socket::Done;
}

bool TcpSocket::Send(Packet::PacketKey key, Packet packet)
{
	Packet signedPacket;

	signedPacket << key;

	signedPacket.append(packet.getData(), packet.getDataSize());

	return send(signedPacket) == sf::Socket::Done;
}

void TcpSocket::Subscribe(Packet::PacketKey key, OnReceivePacket onReceivePacket)
{
	_subscriptionsMutex.lock();
	
	_subscriptions[key] = onReceivePacket;

	_subscriptionsMutex.unlock();
}

void TcpSocket::SubscribeAsync(Packet::PacketKey key, OnReceivePacket onReceivePacket)
{
	std::thread* subscribeThread = new std::thread(&TcpSocket::Subscribe, this, key, onReceivePacket);
	subscribeThread->detach();
}

void TcpSocket::SubscribeOnDisconnect(OnSocketDisconnect onSocketDisconnect)
{
	_onSocketDisconnectMutex.lock();
	
	_onSocketDisconnectSubscriptions.push_back(onSocketDisconnect);
	
	_onSocketDisconnectMutex.unlock();
}

void TcpSocket::ProcessPacket(Packet packet)
{
	Packet::PacketKey key;
	packet >> key;

	std::map<Packet::PacketKey, OnReceivePacket>::iterator it;

	_subscriptionsMutex.lock();

	it = _subscriptions.find(key);
	if (it != _subscriptions.end())
	{
		it->second(packet);
	}

	_subscriptionsMutex.unlock();
}