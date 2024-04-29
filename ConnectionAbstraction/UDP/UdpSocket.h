#pragma once

#include "UdpConnection.h"

class UdpSocket : sf::UdpSocket, public UdpSendPacketDelegate
{
public:

	typedef std::function<void(std::shared_ptr<UdpConnection>)> OnConnectionEnter;

public:

	UdpSocket(UdpAddress::PortNumber port, OnConnectionEnter onConnectionEnter);
	~UdpSocket();

	void ConnectTo(UdpAddress address);

	UdpAddress GetAddress();


private:

	bool _isRunning;
	
	std::unique_ptr<std::thread> _loopThread;

	std::mutex _isRunningMutex;

	OnConnectionEnter _onConnectionEnter;

	UdpAddress _address;

	std::map<std::string, std::shared_ptr<UdpConnection>> _connectionsMap;	

	void ReceiveLoop();
	void ManageReceivePacketDone(UdpPacket packet, UdpAddress address);

	std::map<sf::Socket::Status, std::function<void(UdpPacket, UdpAddress)>> _receiveFunctions
	{
		{sf::Socket::Status::Done, [&](UdpPacket packet, UdpAddress address) {ManageReceivePacketDone(packet, address);}},
		{sf::Socket::Status::NotReady, [&](UdpPacket packet, UdpAddress address) {ManageReceivePacketDone(packet, address);}},
		{sf::Socket::Status::Partial, [&](UdpPacket packet, UdpAddress address) {ManageReceivePacketDone(packet, address);}},
		{sf::Socket::Status::Disconnected, [&](UdpPacket packet, UdpAddress address) {ManageReceivePacketDone(packet, address);}},
		{sf::Socket::Status::Error, [&](UdpPacket packet, UdpAddress address) {ManageReceivePacketDone(packet, address);}},
	};

	void SendAccumulating(UdpAddress address, UdpPacket packet) override;
	void SendImmediately(UdpAddress address, UdpPacket packet) override;

};
