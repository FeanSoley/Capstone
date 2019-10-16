#include <Packet.h>
#include <receiver.h>

void setup() {

Serial.begin(9600);
bool data[32];

Receiver receiver;
Packet rxPacket;

//receivePacket digitalReads diode pin
rxPacket.receivePacket(3, 13);
//decodePacket decodes received signal. Reverse of Sean's encoding, so it might be incorrect.
rxPacket.decodePacket();
//rxGet____ takes position, code, and data and assigns to variable
rxPacket.rxGetPosition();
rxPacket.rxGetCode();
rxPacket.rxGetData();

receiver.displayData();

//Proceed to algo stuff, processing, printing
}

//determine when trx begins and ends to stop reading input and begin to decode?
void loop() {

}
