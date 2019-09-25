#ifndef Packet_h
#define Packet_h

#include "Arduino.h"
#include <vector.h>


// Class for a transmission
class Packet
{
  public:
    const int positionSize = 2;
    const int codeSize = 6;
    const int dataSize = 24;
    const int packetSize = 32;
    void setPosition(int position);
    void setCode(int code);
    void setData(int data);
    void getPosition(bool * position); 
    void getCode(bool * code); 
    void getData(bool * data); 
    void getPacket(bool * packet);
    void encodePacket();
    void sendPacket(int rate);
    
  private:
    bool _position[positionSize];
    bool _code[codeSize];
    bool _data[dataSize];
    bool _encodedPacket[packetSize*2];
     
  
}

#endif


myArray = newArray