#pragma once

#include <functional>
#include <mutex>
#include <map>
#include <utility>

#include "UdpSendPacketDelegate.h"

class UdpConnection
{
public:

	typedef std::function<void(UdpPacket)> OnReceivePacket;
	typedef std::function<std::shared_ptr<ICodable>(UdpPacket)> OnReceiveCriticalPacket;
	typedef std::function<void(UdpPacket)> OnReceiveCriticalResponsePacket;

private:

	class UdpCriticalPersistent
	{
		public:
			UdpPacket packet;
			OnReceiveCriticalResponsePacket onReceiveCriticalResponsePacket;
			UdpCriticalPersistent() = default;
			UdpCriticalPersistent(UdpPacket packet, OnReceiveCriticalResponsePacket onReceiveCriticalResponsePacket) :
				packet(std::move(packet)), onReceiveCriticalResponsePacket(std::move(onReceiveCriticalResponsePacket))
			{}
	};


public:

	UdpConnection(std::shared_ptr<UdpSendPacketDelegate> delegate, UdpAddress address);

	UdpAddress GetAddress() const;

	void Send(UdpPacket::PacketKey key, ICodable& codable) const;
	void Subscribe(UdpPacket::PacketKey key, OnReceivePacket onReceivePacket);
	void SubscribeAsync(UdpPacket::PacketKey key, const OnReceivePacket& onReceivePacket);

	void SendCritical(UdpPacket::PacketKey key, ICodable& codable, OnReceiveCriticalResponsePacket onReceiveCriticalResponsePacket);
	void SubscribeOnCritical(UdpPacket::PacketKey key, OnReceiveCriticalPacket onReceiveCriticalPacket);
	void SubscribeOnCriticalAsync(UdpPacket::PacketKey key, OnReceiveCriticalPacket onReceiveCriticalPacket);

	void ManageReceivedPacket(UdpPacket packet);

	void Unsubscribe(UdpPacket::PacketKey key);
	void UnsubscribeAsync(UdpPacket::PacketKey key);

private:

	std::shared_ptr<UdpSendPacketDelegate> _delegate;

	UdpAddress _address;
	
	std::map<UdpPacket::PacketKey, OnReceivePacket> _subscriptions;	
	std::mutex _subscriptionsMutex;
	
	std::map<UdpPacket::PacketKey, OnReceiveCriticalPacket> _criticalSubscriptions;	
	std::mutex _criticalSubscriptionsMutex;
	
	std::map<UdpPacket::CriticalPacketId, UdpCriticalPersistent> _criticalResponsesSubscriptions;	
	std::mutex _criticalResponsesSubscriptionsMutex;

	void ManageNormal(UdpPacket packet);
	void ManageCritical(UdpPacket packet);
	void ManageCriticalResponse(UdpPacket packet);
	void ManageAccumulated(UdpPacket packet);

	std::map<UdpPacket::PacketType, std::function<void(UdpPacket)>> _managePackets 
	{
		{UdpPacket::PacketType::NORMAL, [&](UdpPacket packet) {ManageNormal(packet); }},
		{UdpPacket::PacketType::CRITICAL, [&](UdpPacket packet) {ManageCritical(packet); }},
		{UdpPacket::PacketType::CRITICAL_RESPONSE, [&](UdpPacket packet) {ManageCriticalResponse(packet); }},
		{UdpPacket::PacketType::ACCUMULATED, [&](UdpPacket packet) {ManageAccumulated(packet); }}
	};
};