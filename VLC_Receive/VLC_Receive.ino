#include <Packet.h>
#include <Transmission.h>

#define BIT_RATE 4800.0
#define PORT_NUM 10

void setup() {
  // put your setup code here, to run once:
  pinMode(PORT_NUM, INPUT);
  bool isMessage;
  bool failed;
  int pinreading;
  Transmission transmission;
  Serial.begin(9600);
  pinMode(PORT_NUM, INPUT); 
  Serial.print("Start");
  while(1==1){   
    pinreading = digitalRead(PORT_NUM);
    if(pinreading != 1){
      delayMicroseconds(1/BIT_RATE*1000000/8);
      failed = transmission.receiveTransmission(BIT_RATE, PORT_NUM);
      if(failed==0){
        Serial.print("passed\n");
        //transmission.printEncoded();
        transmission.decodeTransmission();
        //transmission.printDecoded();
        transmission.deconstructTransmission();
        transmission.printTransmission();
        transmission.cleanTransmission();
      }
      else{
        Serial.print("Failed\n");
        //transmission.printEncoded();
        transmission.cleanTransmission();
        
      }
      }
    }
    
  

}
/*
    bool isMessage;
    // put your main code here, to run repeatedly:
    Transmission transmission;
    //Serial.print("Start");
    delay(100);
    while(1==1){
      isMessage = transmission.checkForMessage(BIT_RATE, PORT_NUM);
      Serial.print(isMessage);
      if(isMessage == 1){
        Serial.print("here");
        transmission.receiveTransmission(BIT_RATE, PORT_NUM);
        transmission.decodeTransmission();
        transmission.deconstructTransmission();
        transmission.printTransmission();
      }
   }
   */


void loop() {

}
  
