#include "UdpConnection.h"

#include <utility>

UdpConnection::UdpConnection(std::shared_ptr<UdpSendPacketDelegate> delegate, UdpAddress address) : 
	_delegate(std::move(delegate)), _address(std::move(address))
{}

UdpAddress UdpConnection::GetAddress() const
{
	return _address;
}

void UdpConnection::Send(UdpPacket::PacketKey key, ICodable& codable) const
{
	_delegate->SendImmediately(_address, UdpPacket::NormalPacket(key, codable));
}

void UdpConnection::Subscribe(UdpPacket::PacketKey key, OnReceivePacket onReceivePacket)
{
	_subscriptionsMutex.lock();

	_subscriptions.emplace(key, std::move(onReceivePacket));

	_subscriptionsMutex.unlock();
}

void UdpConnection::SubscribeAsync(UdpPacket::PacketKey key, const OnReceivePacket& onReceivePacket)
{
	const std::unique_ptr <std::thread> subscribeThread {std::make_unique<std::thread>(&UdpConnection::Subscribe, this, key, onReceivePacket)};
	subscribeThread->detach();
}

void UdpConnection::SendCritical(UdpPacket::PacketKey key, ICodable& codable,
	OnReceiveCriticalResponsePacket onReceiveCriticalResponsePacket)
{
	UdpPacket::CriticalPacketId outId;

	UdpPacket criticalPacket {UdpPacket::CriticalPacket(key, codable, outId)};

	UdpCriticalPersistent persistent {UdpCriticalPersistent(criticalPacket, onReceiveCriticalResponsePacket)};

	_criticalResponsesSubscriptionsMutex.lock();
	
	_criticalResponsesSubscriptions.emplace(outId, std::move(persistent));

	_criticalResponsesSubscriptionsMutex.unlock();

	_delegate->SendImmediately(_address, criticalPacket);	
}

void UdpConnection::SubscribeOnCritical(UdpPacket::PacketKey key, OnReceiveCriticalPacket onReceiveCriticalPacket)
{
	_criticalSubscriptionsMutex.lock();

	_criticalSubscriptions.emplace(key, std::move(onReceiveCriticalPacket));

	_criticalSubscriptionsMutex.unlock();	
}

void UdpConnection::SubscribeOnCriticalAsync(UdpPacket::PacketKey key, OnReceiveCriticalPacket onReceiveCriticalPacket)
{
	const std::unique_ptr <std::thread> subscribeThread {std::make_unique<std::thread>(&UdpConnection::SubscribeOnCritical, this, key, onReceiveCriticalPacket)};
	subscribeThread->detach();
}

void UdpConnection::ManageReceivedPacket(UdpPacket packet)
{
	sf::Uint8 intType;

	packet >> intType;

	const UdpPacket::PacketType type {static_cast<UdpPacket::PacketType>(intType)};

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

	_subscriptionsMutex.lock();

	const std::map<UdpPacket::PacketKey, OnReceivePacket>::iterator it {_subscriptions.find(key)};
	
	if (it != _subscriptions.end())
	{
		it->second(packet);
	}
	
	_subscriptionsMutex.unlock();
}

void UdpConnection::ManageCritical(UdpPacket packet)
{
	UdpPacket::PacketKey key;
	UdpPacket::CriticalPacketId id;

	packet >> key >> id;	

	_criticalSubscriptionsMutex.lock();

	const std::map<UdpPacket::PacketKey, OnReceiveCriticalPacket>::iterator it {_criticalSubscriptions.find(key)};

	if (it != _criticalSubscriptions.end())
	{
		std::shared_ptr<ICodable> codableResponse {it->second(packet)};

		const UdpPacket criticalResponse {UdpPacket::CriticalResponsePacket(key, id, *codableResponse)};

		_delegate->SendImmediately(GetAddress(), criticalResponse);
	}

	_criticalSubscriptionsMutex.unlock();	
}

void UdpConnection::ManageCriticalResponse(UdpPacket packet)
{
	UdpPacket::PacketKey key;
	UdpPacket::CriticalPacketId id;

	packet >> key >> id;

	_criticalResponsesSubscriptionsMutex.lock();

	const std::map<UdpPacket::CriticalPacketId, UdpCriticalPersistent>::iterator it{_criticalResponsesSubscriptions.find(id)};

	if (it != _criticalResponsesSubscriptions.end())
	{
		it->second.onReceiveCriticalResponsePacket(packet);
		_criticalResponsesSubscriptions.erase(it);
	}

	_criticalResponsesSubscriptionsMutex.unlock();
}

void UdpConnection::ManageAccumulated(UdpPacket packet)
{
	CodablePointerVector<UdpPacket> packets;

	packet >> packets;

	const CodablePointerVector<UdpPacket>::iterator itEnd {packets.end()};

	for (CodablePointerVector<UdpPacket>::iterator it {packets.begin()}; it != itEnd; ++it)
	{
		ManageReceivedPacket(**it);
	}
}