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

void Transmission::addReceivedPacket(int pos, int code, int data)
{
    _transmission[_currentNumPackets].setCode(pos);
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
    uint8_t crcPacketByteArray[4];
    uint16_t totalSize = _currentNumPackets*4 + 6;
    
    // Build address packet, always first so pos = 0, code value always 0, data = addresss
    addressPacket.setPosition(0);
	addressPacket.setCode(0);
	addressPacket.setData(_transmissionAddress);
    
    // Get address byte array
    addressPacket.packetToByteArray(addressPacketByteArray);

    // add address packet to total byte array
    for(int i = 0; i < 4; i++){
        totalDataInByteArray[i] = addressPacketByteArray[i];
    }
    
    // Build temp CRC packet 
    crcPacket.setPosition(1); // position always in middle so = 1
    crcPacket.setCode(1); // code for CRC = 1
    crcPacket.setData(0); // data set to 0 temp
    
    // Get crc byte array
    crcPacket.packetToByteArray(crcPacketByteArray);
    
    // add crc packet to total byte array
    for(int i = 0; i < 2; i++){ // only to 2 because we dont want to include the 16 bits that will holds crc value
        totalDataInByteArray[i+4] = crcPacketByteArray[i]; // + 4 to offset the address packet bytes
    }
    
    
    // Calc CRC on all the tranmission data packets
    for(int i = 0; i < _currentNumPackets; i++){
        currentByteInData = 6 + i*4;
        _transmission[i].packetToByteArray(currentByteArray);
        for(int j = 0; j < 4; j++){
            totalDataInByteArray[currentByteInData+j] = currentByteArray[j];
        }
    }

    crcValue = gen_crc16(totalDataInByteArray, totalSize);

    // Change crc data value to new calced crc
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


bool Transmission::checkForMessage(float bitRate, int pin){
    // Declare variables
    int pinReading, currentTime, startTime;
    float calcDelay = ((1/bitRate)*1000000)/2;
    
    // Get pin reading
    pinReading = digitalRead(pin);
    
    if(pinReading == 0){
        startTime = micros();
        currentTime = startTime;
        // We are going to constantly check for half of the duration of a cycle that the value stays low
        while(currentTime < startTime + calcDelay){
            pinReading = digitalRead(pin);
            if(pinReading == HIGH){
                return 0;
            }
        }
        return 1;
    }
    return 0;
    
}

bool Transmission::receiveTransmission(float bitRate, int pin){
    float calcDelay = (1/bitRate)*1000000;
    int currentState =0, currentBit = 0, newTime, currentTime, remainder, timer, printVal, timeSinceLastChange;
    currentTime = micros();
    while(1==1){
        newTime = micros();
        timeSinceLastChange = newTime - currentTime;
        if(digitalRead(pin) != currentState){
            // Only 1 bit
            if(timeSinceLastChange <= 1.5*calcDelay && timeSinceLastChange > .5*calcDelay){
                receiveBuffer[currentBit] = currentState;
                currentBit++;
            }
            // no bits, mistake don't change state
            else if(timeSinceLastChange <= .5*calcDelay){
                continue;
            }       
            // 2 Bits
            else if(timeSinceLastChange > 1.5*calcDelay && timeSinceLastChange <= calcDelay*2.5 ){
                receiveBuffer[currentBit] = currentState;
                currentBit++;
                receiveBuffer[currentBit] = currentState;
                currentBit++;
            }
            // 3 bits too long
            else if(timeSinceLastChange > calcDelay*2.5){
                // Probably done
                receiveBuffer[currentBit] = currentState;
                currentBit++;
                receiveBuffer[currentBit] = currentState;
                currentBit++;
                receiveBuffer[currentBit] = currentState;
                currentBit++;
                
                if(currentState == 1){
                    remainder = currentBit % 64;
                    if(remainder == 3){
                        totalReceivedBits = currentBit - 3;
                        return false;
                    }
                    else if(remainder == 2){
                        totalReceivedBits = currentBit - 2;
                        return false;
                    } 
                    else{
                        return true;
                    }
                }
                // Something went wrong
                else{
                    return true;
                }
            }
            currentState = digitalRead(pin);
            currentTime = newTime;
        }
        else if(timeSinceLastChange > calcDelay*2.5){
            // Probably done
            receiveBuffer[currentBit] = currentState;
            currentBit++;
            receiveBuffer[currentBit] = currentState;
            currentBit++;
            receiveBuffer[currentBit] = currentState;
            currentBit++;
            
            if(currentState == 1){
                remainder = currentBit % 64;
                if(remainder == 3){
                    totalReceivedBits = currentBit - 3;
                    return false;
                }
                else if(remainder == 2){
                    totalReceivedBits = currentBit - 2;
                    return false;
                } 
                else{
                    return true;
                }
            }
            // Something went wrong
            else{
                return true;
            }
        }
    }
}
    
    
    
/*
bool Transmission::receiveTransmission(float bitRate, int pin){
    float calcDelay = (1/bitRate)*1000000;
    int pinReading, currentBit = 0, currentTime, startTime, remainder;
    totalReceivedBits = 0;
    
    
    while(1==1){
        // Get pin reading
        startTime = micros();
        pinReading = digitalRead(pin);
        receiveBuffer[currentBit] = pinReading;
        //Serial.print(pinReading);
       // Serial.println();
        totalReceivedBits++;
        
       
        if(currentBit >= 2){
            // If transmission sitting high it is done
            if(receiveBuffer[currentBit] == true && receiveBuffer[currentBit-1] == true && receiveBuffer[currentBit-2] == true){
                remainder = totalReceivedBits % 64;
                if(remainder == 3){
                    totalReceivedBits -= 3;
                    return false;
                }
                else if(remainder == 2){
                    totalReceivedBits -= 2;
                    return false;
                }
                else{
                    return true;
                }
            }
            // If sitting low, it broke line of sight
            if(receiveBuffer[currentBit] == false && receiveBuffer[currentBit-1] == false && receiveBuffer[currentBit-2] == false){
                return true;
            }

        }   
        currentBit++;

        currentTime = micros();
        Serial.print(startTime+calcDelay-currentTime);
        Serial.println();

        currentTime = micros(); 
    
        delayMicroseconds(startTime+calcDelay-currentTime); 
        
    }
}
 */
 
 
void Transmission::decodeTransmission(){
    int decodedIter;
    
    for(int i = 0; i < totalReceivedBits; i+=2){
        decodedIter = i/2;
        if(receiveBuffer[i] == 1 && receiveBuffer[i+1] == 0){
            decodedTransmission[decodedIter] = 1;
        }
        else{
            decodedTransmission[decodedIter] = 0;
        }
    }
    totalDecodedBits = totalReceivedBits/2;  
        
}

void Transmission::printDecoded(){
    Serial.print("decoded Message\n");
    
    for(int i = 0; i <  totalDecodedBits; i++){
       Serial.print(decodedTransmission[i]); 
       Serial.println(); 
    }
}

int Transmission::bitArrayToInt(bool * array, int length)
{
    int ret = 0;
    int tmp;
    for (int i = 0; i < length; i++) {
        tmp = array[i];
        ret |= tmp << (length - i - 1);
    }
    return ret;
}


void Transmission::deconstructTransmission(){
    // Start with first packet
    bool posBits[positionSize], codeBits[codeSize], dataBits[dataSize];
    int posInt, codeInt, dataInt, packetOffset = 0, codeOffset = positionSize, dataOffset = positionSize + codeSize;
    int posIter, codeIter, dataIter;
    _currentNumPackets = 0;
    
    // Address Packet
    for(int i = 0; i < positionSize; i++){
        posIter = i +  packetOffset;
        posBits[i] =  decodedTransmission[posIter];
    }
    
    for(int i = 0; i < codeSize; i++){
        codeIter = i + codeOffset + packetOffset;
        codeBits[i] =  decodedTransmission[codeIter];
    }
    
    for(int i = 0; i < dataSize; i++){
        dataIter = i + dataOffset + packetOffset;
        dataBits[i] =  decodedTransmission[dataIter];
    }
    

    posInt = bitArrayToInt(posBits, positionSize);
    codeInt = bitArrayToInt(codeBits, codeSize);
    dataInt = bitArrayToInt(dataBits, dataSize);
    
    
    addressPacket.setPosition(posInt);
    addressPacket.setCode(codeInt);
    addressPacket.setData(dataInt);
    
    packetOffset = packetOffset + packetSize;
    
    
    for(int i = 0; i < positionSize; i++){
        posIter = i +  packetOffset;
        posBits[i] =  decodedTransmission[posIter];
    }
    
    for(int i = 0; i < codeSize; i++){
        codeIter = i + codeOffset + packetOffset;
        codeBits[i] =  decodedTransmission[codeIter];
    }
    
    for(int i = 0; i < dataSize; i++){
        dataIter = i + dataOffset + packetOffset;
        dataBits[i] =  decodedTransmission[dataIter];
    }
    
    posInt = bitArrayToInt(posBits, positionSize);
    codeInt = bitArrayToInt(codeBits, codeSize);
    dataInt = bitArrayToInt(dataBits, dataSize);
    
    crcPacket.setPosition(posInt);
    crcPacket.setCode(codeInt);
    crcPacket.setData(dataInt);
    
    packetOffset = packetOffset + packetSize;
    
    for(int j = 0; j < totalDecodedBits-packetOffset; j++){
        for(int i = 0; i < positionSize; i++){
            posIter = i +  packetOffset;
            posBits[i] =  decodedTransmission[posIter];
        }
        
        for(int i = 0; i < codeSize; i++){
            codeIter = i + codeOffset + packetOffset;
            codeBits[i] =  decodedTransmission[codeIter];
        }
        
        for(int i = 0; i < dataSize; i++){
            dataIter = i + dataOffset + packetOffset;
            dataBits[i] =  decodedTransmission[dataIter];
        }
        
        posInt = bitArrayToInt(posBits, positionSize);
        codeInt = bitArrayToInt(codeBits, codeSize);
        dataInt = bitArrayToInt(dataBits, dataSize);
        
        addReceivedPacket(posInt, codeInt, dataInt);
        
        packetOffset = packetOffset + packetSize;          
    }
}
    
    
void Transmission::printTransmission(){
    addressPacket.printPacket();
    crcPacket.printPacket();
    for(int i = 0; i < _currentNumPackets; i++){
        _transmission[i].printPacket();
    }
}
    
    
void Transmission::cleanTransmission(){
    addressPacket.cleanPacket();
    crcPacket.cleanPacket();
    
    for(int i = 0; i < maxTransmissionSize; i++){
        _transmission[i].cleanPacket();
    }
    for(int i = 0; i < 2*32*(maxTransmissionSize+2); i++){
        receiveBuffer[i] = NULL;
    }
    for(int i = 0; i < 32*(maxTransmissionSize+2); i++){
        decodedTransmission[i] = 0;
    }
    
    totalReceivedBits = 0;
    totalDecodedBits = 0;
}    
    
void Transmission::printEncoded(){
    for(int i = 0; i < totalReceivedBits; i++){
        Serial.print(receiveBuffer[i]);
        Serial.println();
    }
}    
    
    
    
    
    
    
    
    





































