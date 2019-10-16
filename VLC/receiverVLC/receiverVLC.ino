#include <Packet.h>
#include <receiver.h>

void setup() {

Serial.begin(9600);
bool data[32];

//Need pin nformation
int diodeVal = 0;
int bitCounter = 0;
Receiver receiver;
Packet rxPacket;

rxPacket.receivePacket(3, 13);
rxPacket.decodePacket();
rxPacket.rxGetPosition();
rxPacket.rxGetCode();
rxPacket.rxGetData();

//Proceed to algo stuff, processing, printing
}

//determine when trx begins and ends to stop reading input and begin to decode?
void loop() {

}
