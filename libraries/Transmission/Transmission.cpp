#include "Arduino.h"
#include "Transmission.h"
#include "Packet.h"


#define CRC16_CCITT	0x1021

void Transmission::addPacket(int code, int data)
{
    _transmission[_currentNumPackets].setCode(code);
    _transmission[_currentNumPackets].setData(data);
    _currentNumPackets++;
}

void Transmission::sendPackets()
{
    Packet addressPacket, crcPacket;
    
    addressPacket.setPosition(0);
	addressPacket.setCode(0);
	addressPacket.setData(_transmissionAddress);
	
	addressPacket.setPosition(0);
	addressPacket.setCode(0);
	addressPacket.setData(_transmissionAddress);
}

