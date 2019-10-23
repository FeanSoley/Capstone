#ifndef Transmission_h
#define Transmission_h

#include "Arduino.h"
#include "Packet.h"

#define maxTransmissionSize 4


// Class for a transmission
class Transmission
{
  public:
    int bitArrayToInt(bool * array, int length);
    void addPacket(int code, int data);
    void addReceivedPacket(int pos, int code, int data);
    void sendPackets(float bitRate, int pin);
    uint16_t getCRCValue();
    void getCRC();
    void setAddress(int address);
    uint16_t gen_crc16(uint8_t * data_p, uint16_t length);
    uint8_t convertArrayToByte(bool * data);
    bool checkForMessage(float bitRate, int pin);
    bool receiveTransmission(float bitRate, int pin);
    void decodeTransmission();
    void deconstructTransmission();
    void printTransmission();
    void printDecoded();
    void cleanTransmission();
    void printEncoded();
    int getTotalReceivedBits();
    
  private:
    Packet addressPacket;
    Packet crcPacket;
    uint16_t crcValue;
    int _transmissionAddress = 1212;
    int _currentNumPackets = 0;
    int _maxTransmissionSize = maxTransmissionSize;
    Packet _transmission[maxTransmissionSize]; 
    bool receiveBuffer[2*32*(maxTransmissionSize+2)];
    bool decodedTransmission[32*(maxTransmissionSize+2)];
    int totalReceivedBits;
    int totalDecodedBits;
};
#endif