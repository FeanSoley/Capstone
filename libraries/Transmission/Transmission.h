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
  private:
    Packet addressPacket;
    Packet crcPacket;
    uint16_t crcValue;
    int _transmissionAddress = 1212;
    int _currentNumPackets = 0;
    int _maxTransmissionSize = maxTransmissionSize;
    Packet _transmission[maxTransmissionSize]; 
};
#endif