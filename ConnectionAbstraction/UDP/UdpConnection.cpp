#include "UdpConnection.h"

UdpConnection::UdpConnection(std::shared_ptr<UdpSendPacketDelegate> delegate, UdpAddress address) : 
	_delegate(delegate), _address(address)
{}

UdpAddress UdpConnection::GetAddress()
{
	return _address;
}

void UdpConnection::Send(UdpPacket::PacketKey key, ICodable& codable)
{
	UdpPacket packet {UdpPacket::NormalPacket(key, codable)};
	_delegate->SendAccumulating(_address, packet);
}

void UdpConnection::Subscribe(UdpPacket::PacketKey key, OnReceivePacket onReceivePacket)
{
	_subscriptionsMutex.lock();

	_subscriptions[key] = onReceivePacket;

	_subscriptionsMutex.unlock();
}

void UdpConnection::SubscribeAsync(UdpPacket::PacketKey key, OnReceivePacket onReceivePacket)
{
	std::unique_ptr <std::thread> subscribeThread = std::make_unique<std::thread>(&UdpConnection::Subscribe, this, key, onReceivePacket);
	subscribeThread->detach();
}

void UdpConnection::ManageReceivedPacket(UdpPacket packet)
{
	UdpPacket::PacketType type;
	sf::Uint8 intType;

	packet >> intType;

	type = static_cast<UdpPacket::PacketType>(intType);

	_managePackets[type](packet);
}

void UdpConnection::Unsubscribe(UdpPacket::PacketKey key)
{

}

void UdpConnection::UnsubscribeAsync(UdpPacket::PacketKey key)
{

}

void UdpConnection::ManageNormal(UdpPacket packet)
{
	UdpPacket::PacketKey key;
	packet >> key;

	std::map<UdpPacket::PacketKey, OnReceivePacket>::iterator it;
	
	_subscriptionsMutex.lock();
	
	it = _subscriptions.find(key);
	if (it != _subscriptions.end())
	{
		it->second(packet);
	}
	
	_subscriptionsMutex.unlock();
}

void UdpConnection::ManageCritical(UdpPacket packet)
{

}

void UdpConnection::ManageCriticalResponse(UdpPacket packet)
{

}

void UdpConnection::ManageAccumulated(UdpPacket packet)
{

}