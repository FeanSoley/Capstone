
#include <Packet.h>
#include <Transmission.h>

#define BIT_RATE 2400.0
#define PORT_NUM 3
#define TIME_BETWEEN 1000.0

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
    // get info from obd2
    digitalWrite(PORT_NUM, HIGH);
    delayMicroseconds(1/BIT_RATE*1000000*3);
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
    digitalWrite(PORT_NUM, HIGH);
    delayMicroseconds(1/BIT_RATE*1000000*3);
    analogWrite(PORT_NUM, 128);  
    delay(TIME_BETWEEN); 
}
