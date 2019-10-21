
#include <Packet.h>
#include <Transmission.h>

#define BIT_RATE 4800.0
#define PORT_NUM 8

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
    transmission.setAddress(1221);
    // addPacket(CODE, DATA)
    transmission.addPacket(SPEED_CODE, 10);
    // Gets CRC value from packets
    transmission.getCRC();
    // Builds address packet and crc packet and sends all 5
    //sendPacket(BIT RATE, PIN)
    transmission.sendPackets(BIT_RATE, PORT_NUM);
    
    digitalWrite(PORT_NUM, HIGH); 
    
    delay(2000);
  
}
