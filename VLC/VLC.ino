
#include <Packet.h>
#include <Transmission.h>

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);
  //bool data[24];
  pinMode(8, INPUT); 

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
  transmission.sendPackets(5.0, 8);
  
  // On a packet level, transmission object handles this for each packet
  Packet dataPacket;
  dataPacket.setPosition(0);
  dataPacket.setCode(3);
  dataPacket.setData(55);
  dataPacket.encodePacket();
  dataPacket.sendPacket(3, 8);
  
  //dataPacket.getData(data);

  //for(int i = 0; i < 24; i++){
  //  Serial.print(data[i]);
  //}
  //Serial.print("\n");

}

void loop() {
  // put your main code here, to run repeatedly:
  
}
