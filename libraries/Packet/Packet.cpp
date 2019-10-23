#include "Arduino.h"
#include "Packet.h"


// This function sets the position bits to a given int
void Packet::setPosition(int position)
{
  int currentValue;
  for(int i = 0; i < positionSize; i++){
    currentValue = bitRead(position,i);
    // Odd indexing is due to bitRead iterating backwards
    _position[positionSize-i-1] = currentValue;
  }
}

// This function sets teh code bits to a given int
void Packet::setCode(int code)
{
  int currentValue;
  for(int i = 0; i < codeSize; i++){
    currentValue = bitRead(code,i);
    // Odd indexing is due to bitRead iterating backwards
    _code[codeSize-i-1] = currentValue;
  }
}

// This function sets the data bits to a given int
void Packet::setData(int data)
{
  int currentValue;
  for(int i = 0; i < dataSize; i++){
    currentValue = bitRead(data,i);
    // Odd indexing is due to bitRead iterating backwards
    _data[dataSize-i-1] = currentValue;
  }
}

// This function converts a 8 bit array to a unint8_t
uint8_t Packet::convertArrayToByte(bool * data){
    uint8_t returnValue = 0;
    for(int i = 0; i < 8; i++){
        returnValue = returnValue + (2^(i))*data[i];
    }
    return returnValue;
}

// This function converts the entire packet into an array of 4 unint8_t's 
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

// Returns a copy of the position bit array
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

int Packet::getIntPosition(){
    int posInt;
    posInt = bitArrayToInt(_position, positionSize);
    return posInt;
}


int Packet::getIntCode(){
    int codeInt;
    codeInt = bitArrayToInt(_code, codeSize);
    return codeInt;
}


int Packet::getIntData(){
    int dataInt;
    dataInt = bitArrayToInt(_data, dataSize);
    return dataInt;
}

void Packet::cleanPacket(){
    setPosition(0);
    setCode(0);
    setData(0);
}

int Packet::bitArrayToInt(bool * array, int length)
{
    int ret = 0;
    int tmp;
    for (int i = 0; i < length; i++) {
        tmp = array[i];
        ret |= tmp << (length - i - 1);
    }
    return ret;
}


void Packet::sendPacket(float bitRate, int pin)
{
    float calcDelay = (1/bitRate)*1000000;
    int currentTime, startTime;
    for(int i = 0; i < packetSize*2; i++)
    {
        startTime = micros();
        if(_encodedPacket[i] == 1){
            //Serial.print("1\n");
            digitalWrite(pin, HIGH); // sets the digital pin 13 on  
        }
        else{
            //Serial.print("0\n");
            digitalWrite(pin, LOW); // sets the digital pin 13 on 
        }
        //currentTime = micros();
        //Serial.print(startTime+calcDelay-currentTime);
        //Serial.println();
        delayMicroseconds(calcDelay);  
    }
    digitalWrite(pin, HIGH);
}

void Packet::printPacket(){
    int data, code;
    
    code = getIntCode();
    data = getIntData();
    
    switch(code) {
        case ADDR_CODE:
            Serial.print("Address: "); 
            delay(10);
            Serial.print(data); 
            delay(10);
            Serial.print("\n"); 
            delay(10);
            break;
        case CRC_CODE:
            Serial.print("CRC: "); 
            delay(10);
            Serial.print(data); 
            delay(10);
            Serial.print("\n"); 
            delay(10);
            break;
        case SPEED_CODE:
            Serial.print("Speed: "); 
            delay(10);
            Serial.print(data); 
            delay(10);
            Serial.print("\n"); 
            delay(10);
            break;           
        default:
            Serial.print("Unknown Code: "); 
            delay(10);
            Serial.print(data); 
            delay(10);
            Serial.print("\n"); 
            delay(10);
            break;    
        
    }
    
}





/*
void Packet::receivePacket(float bitRate, int pin) 
{
	float calcDelay = 1 / bitRate * 1000000;
	for (int i = 0; i < packetSize * 2; i++)
	{
		if (digitalRead(pin) == HIGH) 
		{
			Serial.print("HIGH");
			_rxEncodedPacket[i] = 1;
			delay(calcDelay);
		}
		else 
		{
			Serial.print("LOW");
			_rxEncodedPacket[i] = 0;
			delay(calcDelay);
		}
	}
}

void Packet::decodePacket() //THis is just the reverse of what Sean wrote ie. _position==0 and not 1
{
	int positionStart, codeStart, dataStart, packetEnd, i, packetIter;
	positionStart = 0;
	codeStart = positionStart + positionSize;
	dataStart = codeStart + codeSize;
	packetEnd = dataStart + dataSize;

	for (int i = positionStart; i < codeStart; i++) {
		packetIter = 2 * i;
		if (_position[i] == 0) {
			_rxEncodedPacket[packetIter] = 1;
			_rxEncodedPacket[packetIter + 1] = 0;
		}
		else {
			_rxEncodedPacket[packetIter] = 0;
			_rxEncodedPacket[packetIter + 1] = 1;
		}
		packetIter++;
	}

	for (int i = 0; i < codeSize; i++) {
		packetIter = 2 * i + codeStart * 2;
		if (_code[i] == 0) {
			_rxEncodedPacket[packetIter] = 1;
			_rxEncodedPacket[packetIter + 1] = 0;
		}
		else {
			_rxEncodedPacket[packetIter] = 0;
			_rxEncodedPacket[packetIter + 1] = 1;
		}
		packetIter++;
	}

	for (int i = 0; i < dataSize; i++) {
		packetIter = 2 * i + dataStart * 2;
		if (_data[i] == 0) {
			_encodedPacket[packetIter] = 1;
			_encodedPacket[packetIter + 1] = 0;
		}
		else {
			_encodedPacket[packetIter] = 0;
			_encodedPacket[packetIter + 1] = 1;
		}
		packetIter++;
	}
}

 //Gets info from packet. These are not private because they're used in receiver.cpp
void Packet::rxGetPosition()
{
	for (int i = 0; i < positionSize; i++) {
		_rxPosition[i] = _rxEncodedPacket[i];
	}
}


void Packet::rxGetCode()
{
	for (int i = 0 - 1; i < codeSize; i++) {
		_rxCode[i] = _rxEncodedPacket[i + positionSize];
	}
}

void Packet::rxGetData(bool* data)
{
	for (int i = 0; i < dataSize; i++) {
		_rxData[i] = _rxEncodedPacket[i + positionSize + codeSize];
	}
}

*/








