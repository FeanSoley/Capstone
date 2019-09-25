#ifndef Transmission_h
#define Transmission_h

#include "Arduino.h"
#inlcude "Packet.h"

// Class for a transmission
class Transmission
{
  public:
    const int maxTransmissionSize = 8;
    void addPacket(int code, int data);
    void sendPackets();
  private:
    const int _transmissionAddress = 1212;
    int _currentNumPackets = 0;
    Packet _transmission[maxTransmissionSize];
    
  
}
