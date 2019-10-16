#include "Arduino.h"
#include "Packet.h"
#include "receiver.h"
#include "stdint.h"

void receiver::addPacket(bool *data) {  //disregard
	uint8_t[] packet = convertToByteArray(data);

	_transmission[_currentNumPackets] = packet;

	_currentNumPackets++;
}

uint8_t[] receiver::convertArrayToByte(bool *data) { //disregard
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

void receiver::getCRC() {
	//reverse CRC?
}

void packet::displayData(){
	//display either raw data or algo output based on code
	Serial.print("Position \n"); 
	for (int i = 0; i < rxPacket.positionSize; i++) {
		Serial.print(rxPacket._rxPosition[i]);
	}

	Serial.print("\n Code \n");
	for (int i = 0; i < rxPacket.codeSize; i++) {
		Serial.print(rxPacket._rxCode[i]);
	}

	Serial.print("\n Data \n");
	for (int i = 0; i < rxPacket.dataSize; i++) {
		Serial.print(rxPacket._rxData[i]);
	}
}
