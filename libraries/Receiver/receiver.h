#ifndef receiver_h
#def receiver-h

#include "Arduino.h"
#Include "Packet.h"

#define maxTransmissionSize 8

class receiver {
public:
	void addPacket(int code, int data);
	uint16_t getCRCValue();
	int calcTransmissionCRC();
	void getAddress(int address);
	uint16_t gen_crc16(uint8_t* data_p, uint16_t length);
	uint8_t convertArrayToByte(bool* data);

	void receivePacket();
	void decodePacket();
	void displayData();

private:
	Packet addressPacket;
	Packet crcPacket;
	uint16_t crcValue;
	int _transmissionAddress = 1212;
	int _currentNumPackets = 0;
	int _maxTransmissionSize = maxTransmissionSize;
	Packet _transmission[maxTransmissionSize][4];
};
#endif
