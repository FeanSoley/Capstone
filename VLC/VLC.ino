
#include <Packet.h>
#include <Transmission.h>

#define BIT_RATE 9600.0
#define PORT_NUM 8

void setup() { 
  Serial.begin(9600);
  pinMode(PORT_NUM, INPUT); 
  
  // On a packet level, transmission object handles this for each packet
  /*Packet dataPacket;
  dataPacket.setPosition(0);
  dataPacket.setCode(3);
  dataPacket.setData(55);
  dataPacket.encodePacket();
  dataPacket.sendPacket(3, 8);*/

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Start \n");
  Transmission transmission;
  // Set address
  transmission.setAddress(1221);
  // addPacket(CODE, DATA)
  transmission.addPacket(5, 10);
  transmission.addPacket(5, 20);
  transmission.addPacket(5, 30);
  // Gets CRC value from packets
  transmission.getCRC();
  // Builds address packet and crc packet and sends all 5
  // sendPacket(BIT RATE, PIN)
  transmission.sendPackets(BIT_RATE, PORT_NUM);
}
