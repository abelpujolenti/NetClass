#pragma once

#include "UdpAddress.h"
#include "UdpPacket.h"

class UdpSendPacketDelegate
{
public:
	virtual void SendAccumulating(UdpAddress address, UdpPacket packet) = 0;
	virtual void SendImmediately(UdpAddress address, UdpPacket packet) = 0;
};

