#ifndef Transmission_h
#define Transmission_h

#include "Arduino.h"
#include "Packet.h"

#define maxTransmissionSize 8


// Class for a transmission
class Transmission
{
  public:
    void addPacket(int code, int data);
    void sendPackets(float bitRate, int pin);
    uint16_t getCRCValue();
    void getCRC();
    void setAddress(int address);
    uint16_t gen_crc16(uint8_t * data_p, uint16_t length);
    uint8_t convertArrayToByte(bool * data);
<<<<<<< Updated upstream
=======
    bool checkForMessage(float bitRate, int pin);
    bool receiveTransmission(float bitRate, int pin);
    void decodeTransmission();
    void deconstructTransmission();
    void printTransmission();
    void printDecoded();
    void cleanTransmission();
    void printEncoded();
	void verifyCRC();
    
>>>>>>> Stashed changes
  private:
    Packet addressPacket;
	Packet rxAddressPacket;
    Packet crcPacket;
	Packet rxCRCPacket;
    uint16_t crcValue;
	uint16_t rxCRCValue;
    int _transmissionAddress = 1212;
    int _currentNumPackets = 0;
    int _maxTransmissionSize = maxTransmissionSize;
    Packet _transmission[maxTransmissionSize]; 
<<<<<<< Updated upstream
=======
	Packet _rxTransmission[maxTransmissionSize];
    bool receiveBuffer[2*32*(maxTransmissionSize+2)];
    bool decodedTransmission[32*(maxTransmissionSize+2)];
    int totalReceivedBits;
    int totalDecodedBits;
	uint16_t rxCRCValue;
>>>>>>> Stashed changes
};
#endif