#pragma once
#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <vector>

#include "Packet.h"

class TcpSocket : public sf::TcpSocket
{
public:

	typedef std::function<void(Packet packet)> OnReceivePacket;
	typedef std::function<void(TcpSocket* socket)> OnSocketDisconnect;

	bool Connect(std::string ip, unsigned short port);
	void Receive();
	bool Send(Packet::PacketKey key);
	bool Send(Packet::PacketKey key, Packet packet);

	void Subscribe(Packet::PacketKey key, OnReceivePacket onReceivePacket);
	void SubscribeAsync(Packet::PacketKey key, OnReceivePacket onReceivePacket);

	void SubscribeOnDisconnect(OnSocketDisconnect onSocketDisconnect);

private:

	std::map<Packet::PacketKey, OnReceivePacket> _subscriptions;
	std::mutex _subscroptionsMutex;

	std::vector<OnSocketDisconnect> _onSocketDisconnectSubscriptions;
	std::mutex _onSocketDisconnectMutex;

	void ProcessPacket(Packet packet);
};