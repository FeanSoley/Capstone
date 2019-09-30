
#include <Packet.h>
#include <Transmission.h>

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);
  bool data[24];
  pinMode(8, INPUT); 
  
  Packet dataPacket;
  dataPacket.setPosition(0);
  dataPacket.setCode(3);
  dataPacket.setData(55);
  dataPacket.encodePacket();
  dataPacket.getData(data);

  for(int i = 0; i < 24; i++){
    Serial.print(data[i]);
  }
  Serial.print("\n");
  dataPacket.sendPacket(3, 8);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
