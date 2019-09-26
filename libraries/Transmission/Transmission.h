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
    void sendPackets();
    unsigned int calcCRC16(unsigned int crcValue, unsigned char newByte);
    int calcTransmissionCRC();
  private:
    const int _transmissionAddress = 1212;
    int _currentNumPackets = 0;
    int _maxTransmissionSize = maxTransmissionSize;
    Packet _transmission[maxTransmissionSize]; 
};
#endif