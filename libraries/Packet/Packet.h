#ifndef Packet_h
#define Packet_h

#include "Arduino.h"
#define positionSize 2
#define codeSize 6
#define dataSize 24
#define packetSize 32


// Class for a transmission
class Packet
{
  public:
    void setPosition(int position);
    void setCode(int code);
    void setData(int data);
    void getPosition(bool * position); 
    void getCode(bool * code); 
    void getData(bool * data); 
    void getPacket(bool * packet);
    void encodePacket();
    void sendPacket(float bitRate, int pin);
    void packetToByteArray(uint8_t * data);
    uint8_t convertArrayToByte(bool * data);

	void receivePacket(float bitRate, int pin);
	void decodePacket();

	bool _rxPosition[positionSize];
	bool _rxCode[codeSize];
	bool _rxData[dataSize];
	bool _rxEncodedPacket[packetSize * 2];
    
  private:
    bool _position[positionSize];
    bool _code[codeSize];
    bool _data[dataSize];
    bool _encodedPacket[packetSize*2];
  
};

#endif


