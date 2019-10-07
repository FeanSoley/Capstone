#include <Packet.h>
#include <receiver.h>

void setup() {

Serial.begin(9600);
bool data[32];

//Need pin nformation
int diodeVal = 0;
int bitCounter = 0;
Receiver receiver;

Serial.print("Begin loop \n");
}

//determine when trx begins and ends to stop reading input and decode?
void loop() {
  diodeVal = analogRead(diodePin); //digital vs analogRead

  if(diodeVal > 1000){
    data[bitCounter]=1
  }
  else {
    data[bitcounter]=0;
  }

  if(bitCounter == 32){
    receiver.addPacket(data);
    bitCounter = 0;
  }
  else {
    bitCounter++;
  }
}
