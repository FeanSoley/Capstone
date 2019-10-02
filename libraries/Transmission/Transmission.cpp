#include "Arduino.h"
#include "Transmission.h"
#include "Packet.h"
#define POLY 0x1021


void Transmission::addPacket(int code, int data)
{
    _transmission[_currentNumPackets].setCode(code);
    _transmission[_currentNumPackets].setData(data);
    _currentNumPackets++;
}

uint16_t Transmission::getCRCValue(){
    return crcValue;
}


uint8_t Transmission::convertArrayToByte(bool * data){
    uint8_t returnValue = 0;
    for(int i = 0; i < 8; i++){
        returnValue = returnValue + (2^(i))*data[i];
    }
    return returnValue;
}


void Transmission::getCRC(){
    uint8_t crcByte0 = 65;
    uint8_t crcByte1 = 0;
    uint8_t currentByteArray[4];
    int currentByteInData;
    uint8_t totalDataInByteArray[40];    
    uint8_t addressPacketByteArray[4];
    
    uint16_t totalSize = _currentNumPackets*4 + 6;
    
    addressPacket.packetToByteArray(addressPacketByteArray);
    Serial.print("1\n");
    for(int i = 0; i < _currentNumPackets; i++){
        currentByteInData = 6 + i*4;
        _transmission[i].packetToByteArray(currentByteArray);
        for(int j = 0; j < 4; j++){
            totalDataInByteArray[currentByteInData+j] = currentByteArray[j];
        }
    }
    Serial.print("2\n");
    crcValue = gen_crc16(totalDataInByteArray, totalSize);
    Serial.print("3\n");
    crcPacket.setPosition(1);
    crcPacket.setCode(1);
    crcPacket.setData(crcValue);
    
}  

void Transmission::setAddress(int address){
    _transmissionAddress = address;
}

// Source https://people.cs.umu.se/isak/snippets/crc-16.c have not confirmed it works
uint16_t Transmission::gen_crc16(uint8_t * data_p, uint16_t length)
{
      uint8_t i;
      unsigned int data;
      unsigned int crc = 0xffff;

      if (length == 0)
            return (~crc);

      do
      {
            for (i=0, data=(unsigned int)0xff & *data_p++;
                 i < 8; 
                 i++, data >>= 1)
            {
                  if ((crc & 0x0001) ^ (data & 0x0001))
                        crc = (crc >> 1) ^ POLY;
                  else  crc >>= 1;
            }
      } while (--length);

      crc = ~crc;
      data = crc;
      crc = (crc << 8) | (data >> 8 & 0xff);

      return (crc);
}


void Transmission::sendPackets(float bitRate, int pin)
{  
    addressPacket.setPosition(0);
	addressPacket.setCode(0);
	addressPacket.setData(_transmissionAddress);
	
    addressPacket.encodePacket();
    addressPacket.sendPacket(bitRate, pin);
    crcPacket.encodePacket();;
    crcPacket.sendPacket(bitRate, pin);
    
    for(int i = 0; i < _currentNumPackets; i++){
        if(i == _currentNumPackets-1){    
            _transmission[i].setPosition(2);
        }
        else {
            _transmission[i].setPosition(1);
        }
        _transmission[i].encodePacket();
        _transmission[i].sendPacket(bitRate, pin);
    }
}

