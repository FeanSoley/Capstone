#include <Packet.h>
#include <Transmission.h>

#define BIT_RATE 4800.0
#define PORT_NUM 5

void setup() {
  // put your setup code here, to run once:
  pinMode(PORT_NUM, INPUT);
  bool isMessage;
  bool failed;
  bool crcCheck;
  int pinreading, bitCount;
  Transmission transmission;
  Serial.begin(9600);
  pinMode(PORT_NUM, INPUT); 
  Serial.print("Start");
  while(1==1){   
    pinreading = digitalRead(PORT_NUM);
    if(pinreading != 1){
      delayMicroseconds(1/BIT_RATE*1000000/8);
      failed = transmission.receiveTransmission(BIT_RATE, PORT_NUM);
      bitCount = transmission.getTotalReceivedBits();
      if(failed==0){
        Serial.print("passed\n");
        //transmission.printEncoded();
        transmission.decodeTransmission();
        //transmission.printDecoded();
        transmission.deconstructTransmission();
        crcCheck = transmission.checkCRC();
        if(crcCheck){
          transmission.printTransmission();
        } 
        else {
          Serial.print("CRC Error");
        }
        transmission.cleanTransmission();
        crcCheck = transmission.checkCRC();
        
        Serial.println();
        delay(5);
      }
      else if(failed == 1 && bitCount < 150){
        //transmission.printEncoded();
        transmission.cleanTransmission();
        
      }
      else{
        //Serial.print("Failed\n");
        //Serial.print(bitCount);
        //Serial.println();
        //transmission.printEncoded();
        transmission.cleanTransmission();   
      }
    }  
  }
}



void loop() {

}
  
