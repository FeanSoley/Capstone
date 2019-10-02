#include "Arduino.h"
#include "Packet.h"



void Packet::setPosition(int position)
{
  int currentValue;
  for(int i = 0; i < positionSize; i++){
    currentValue = bitRead(position,i);
    _position[positionSize-i-1] = currentValue;
  }
}

void Packet::setCode(int code)
{
  int currentValue;
  for(int i = 0; i < codeSize; i++){
    currentValue = bitRead(code,i);
    _code[codeSize-i-1] = currentValue;
  }
}

uint8_t Packet::convertArrayToByte(bool * data){
    uint8_t returnValue = 0;
    for(int i = 0; i < 8; i++){
        returnValue = returnValue + (2^(i))*data[i];
    }
    return returnValue;
}

void Packet::packetToByteArray(uint8_t * data){
    bool bit07[8], bit815[8], bit1623[8], bit2431[8];
    
    for(int i = 0; i < 8; i++){
        if(i < 2){
            bit07[i] = _position[i];
        }
        else {
            bit07[i] = _code[i];
        }
    }
    
    for(int i = 0; i < 8; i++){
        bit815[i] = _data[i];
    }
    
    for(int i = 0; i < 8; i++){
        bit1623[i] = _data[i+8];
    }
    
    for(int i = 0; i < 8; i++){
        bit2431[i] = _data[i+16];
    }
    
    data[0] = convertArrayToByte(bit07);
    data[1] = convertArrayToByte(bit815);
    data[2] = convertArrayToByte(bit1623);
    data[3] = convertArrayToByte(bit2431);    
}

void Packet::setData(int data)
{
  int currentValue;
  for(int i = 0; i < dataSize; i++){
    currentValue = bitRead(data,i);
    _data[dataSize-i-1] = currentValue;
  }
}


void Packet::getPosition(bool * position)
{
    for(int i = 0; i < positionSize; i++){
        position[i] = _position[i];
    }
}


void Packet::getCode(bool * code)
{
    for(int i = 0; i < codeSize; i++){
        code[i] = _code[i];
    }
}

void Packet::getData(bool * data)
{
    for(int i = 0; i < dataSize; i++){
        data[i] = _data[i];
    }
}

void Packet::getPacket(bool * packet){
    int positionStart, codeStart, dataStart, packetEnd, i, packetIter;
    positionStart = 0;
    codeStart = positionStart + positionSize;
    dataStart = codeStart + codeSize;
    packetEnd = dataStart + dataSize;
        
    for(int i = positionStart; i < codeStart; i++){
        packet[i] = _position[i];
    }
    
    for(int i = 0; i < codeSize; i++){
        packetIter = codeStart;
        packet[packetIter] = _code[i];
        packetIter++;
    }
    
    for(int i = 0; i < dataSize; i++){
        packetIter = dataStart;
        packet[packetIter] = _data[i];
        packetIter++;
    }
}

void Packet::encodePacket()
{
    int positionStart, codeStart, dataStart, packetEnd, i, packetIter;
    positionStart = 0;
    codeStart = positionStart + positionSize;
    dataStart = codeStart + codeSize;
    packetEnd = dataStart + dataSize;
    
    
    
    for(int i = positionStart; i < codeStart; i++){
        packetIter = 2*i;
        if(_position[i] == 1){
            _encodedPacket[packetIter] = 1;
            _encodedPacket[packetIter+1] = 0;
        }
        else{
            _encodedPacket[packetIter] = 0;
            _encodedPacket[packetIter+1] = 1;
        }
        packetIter++;
    }
    
    for(int i = 0; i < codeSize; i++){
        packetIter = 2*i + codeStart*2;
        if(_code[i] == 1){
            _encodedPacket[packetIter] = 1;
            _encodedPacket[packetIter+1] = 0;
        }
        else{
            _encodedPacket[packetIter] = 0;
            _encodedPacket[packetIter+1] = 1;
        }
        packetIter++;
    }
    
    for(int i = 0; i < dataSize; i++){
        packetIter = 2*i + dataStart*2;
        if(_data[i] == 1){
            _encodedPacket[packetIter] = 1;
            _encodedPacket[packetIter+1] = 0;
        }
        else{
            _encodedPacket[packetIter] = 0;
            _encodedPacket[packetIter+1] = 1;
        }
        packetIter++;
    }
}

void Packet::sendPacket(float bitRate, int pin)
{
    float calcDelay = 1/bitRate*1000;
    for(int i = 0; i < packetSize*2; i++)
    {
        if(_encodedPacket[i] == 1){
            Serial.print("High\n");
            digitalWrite(pin, HIGH); // sets the digital pin 13 on
            delay(calcDelay);  
        }
        else{
            Serial.print("Low\n");
            digitalWrite(pin, LOW); // sets the digital pin 13 on
            delay(calcDelay);  
        }
    }
}










