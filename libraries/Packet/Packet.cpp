#include "Arduino.h"
#include "Packet.h"



void Packet::setPosition(int position)
{
  int currentValue;
  for(int i = 0; i < positionSize; i++){
    currentValue = bitRead(position,i);
    _position[i] = currentValue;
  }
}

void Packet::setCode(int code)
{
  int currentValue;
  for(int i = 0; i < codeSize; i++){
    currentValue = bitRead(code,i);
    _code[i] = currentValue;
  }
}


void Packet::setData(int data)
{
  int currentValue;
  for(int i = 0; i < dataSize; i++){
    currentValue = bitRead(data,i);
    _data[i] = currentValue;
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
    positionStart = 0;
    codeStart = positionStart + positionSize;
    dataStart = codeStart + codeSize;
    packetEnd = dataStart + dataSize;
        
    for(i = positionStart; i < codeStart; i++){
        packet[i] = _position[i]
    }
    
    for(i = codeStart; i < dataStart; i++){
         packet[i] = _code[i]
    }
    
    for(i = dataStart; i < packetEnd; i++){
         packet[i] = _data[i]
    }
}

void Packet::encodePacket()
{
    int positionStart, codeStart, dataStart, packetEnd, i;
    positionStart = 0;
    codeStart = positionStart + positionSize;
    dataStart = codeStart + codeSize;
    packetEnd = dataStart + dataSize;
    
    
    
    for(i = positionStart; i < codeStart; i++){
        if(_position[i]){
            _encodedPacket[i] = 1;
            _encodedPacket[i+1] = 0;
        }
        else{
            _encodedPacket[i] = 0;
            _encodedPacket[i+1] = 1;
        }
    }
    
    for(i = codeStart; i < dataStart; i++){
        if(_code[i]){
            _encodedPacket[i] = 1;
            _encodedPacket[i+1] = 0;
        }
        else{
            _encodedPacket[i] = 0;
            _encodedPacket[i+1] = 1;
        }
    }
    
    for(i = dataStart; i < packetEnd; i++){
        if(_data[i]){
            _encodedPacket[i] = 1;
            _encodedPacket[i+1] = 0;
        }
        else{
            _encodedPacket[i] = 0;
            _encodedPacket[i+1] = 1;
        }
    }
}

void Packet::sendPacket(float bitRate, int pin)
{
    float calcDelay = 1/bitRate;
    for(int i = 0; i < packetSize*2; i++)
    {
        if(_encodedPacket[i]){
            digitalWrite(pin, HIGH); // sets the digital pin 13 on
            delay(calcDelay);  
        }
        else{
            digitalWrite(pin, LOW); // sets the digital pin 13 on
            delay(calcDelay);  
        }
    }
}










