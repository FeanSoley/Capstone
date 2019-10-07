#include "Arduino.h"
#include "Packet.h"
#include "receiver.h"
#include "stdint.h"

void receiver::addPacket(bool *data) {
	uint8_t[] packet = convertToByteArray(data);

	_transmission[_currentNumPackets] = packet;

	_currentNumPackets++;
}

uint8_t[] receiver::convertArrayToByte(bool *data) {
	uint8_t byte = 0;
	uint8_t[] returnValue = new uint8_t[4]
		for (int h = 0; h < 4; h++) {
			for (int i = 0; i < 8; i++) {
				byte = byte + (2 ^ (i)) * data[i];
			}
			returnValue[h] = byte;
			byte = 0;
		}
	return returnValue;
}

void receiver::decodePacket(){


}

void receiver::getCRC() {

}

void packet::displayData(){


}
