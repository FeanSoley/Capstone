
#include <Packet.h>
#include <Transmission.h>

#define BIT_RATE 6200.0
#define PORT_NUM 5
#define TIME_BETWEEN 500.0

void setup() { 
  Serial.begin(9600);
  pinMode(PORT_NUM, OUTPUT); 
  digitalWrite(PORT_NUM, HIGH);
  delay(100);

  // On a packet level, transmission object handles this for each packet
  /*Packet dataPacket;
  dataPacket.setPosition(0);
  dataPacket.setCode(3);
  dataPacket.setData(55);
  dataPacket.encodePacket();
  dataPacket.sendPacket(3, 8);*/
 

}

void loop() {
    //digitalWrite(PORT_NUM, LOW); 
    // put your main code here, to run repeatedly:
    Transmission transmission;
    // Set address
    transmission.setAddress(1350);
    // addPacket(CODE, DATA)
    transmission.addPacket(SPEED_CODE, 30);
    // Gets CRC value from packets
    transmission.getCRC();
    // Builds address packet and crc packet and sends all 5
    //sendPacket(BIT RATE, PIN)
    transmission.sendPackets(BIT_RATE, PORT_NUM);
    //analogWrite(PORT_NUM, 128);
    //digitalWrite(PORT_NUM, HIGH);
   //delay(1000); 
    //delayMicroseconds(1/BIT_RATE*1000000*3);

    int iters = (TIME_BETWEEN)/(1/BIT_RATE*1000*5.7);
    for(int i = 0; i < iters; i++){
      digitalWrite(PORT_NUM, HIGH); 
      delayMicroseconds(1/BIT_RATE*1000000*3.05);
      digitalWrite(PORT_NUM, LOW); 
      delayMicroseconds(1/BIT_RATE*1000000*2.95);
    }  
    digitalWrite(PORT_NUM, HIGH); 
   
}
