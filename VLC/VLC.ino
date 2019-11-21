
#include <Packet.h>
#include <Transmission.h>

#define BIT_RATE 2400.0
#define PORT_NUM 3
#define TIME_BETWEEN 1000.0

char rxData[20];
char rxIndex=0;

void setup() { 
  Serial.begin(9600);
  Serial1.begin(9600); //Hardware serial connection to the obdii uart
  pinMode(PORT_NUM, OUTPUT); 
  digitalWrite(PORT_NUM, HIGH);
  delay(100);

  // for OBD2
  Serial.print("befotre init\n");
  ODB_init();
  Serial.print("after init\n");

 

}

void loop() {
    int intMPH, rpm;
    double mph;
    
    // Code for reading obd2
    Serial1.flush();
    Serial.print("get rpm \n");
    mph=  0;
    rpm = 0;
    mph = getSpeed() * 0.621371;
    rpm = getRPM();
    intMPH = (int) mph;
    Serial.print(intMPH);
    digitalWrite(PORT_NUM, HIGH);
    delayMicroseconds(1/BIT_RATE*1000000*3);
    Transmission transmission;
    // Set address
    transmission.setAddress(1350);
    // addPacket(CODE, DATA)
    transmission.addPacket(SPEED_CODE, intMPH);
    transmission.addPacket(RPM_CODE, rpm);
    // Gets CRC value from packets
    transmission.getCRC();
    // Builds address packet and crc packet and sends all 5
    //sendPacket(BIT RATE, PIN)
    transmission.sendPackets(BIT_RATE, PORT_NUM);
    transmission.cleanTransmission();
    digitalWrite(PORT_NUM, HIGH);
    delayMicroseconds(1/BIT_RATE*1000000*3);
    analogWrite(PORT_NUM, 128);  
    delay(TIME_BETWEEN); 
}


 void ODB_init(void)
 {
   //Wait for a little while before sending the reset command to the OBD-II-UART
   delay(1000);
   //Reset the OBD-II-UART
   Serial1.print("ATZ\r");
   //Wait for a bit before starting to send commands after the reset.
   delay(1000);
   OBD_read();
   Serial1.print("ATE0\r");
   OBD_read();
   Serial1.flush();
 }

 int getRPM(void)
 {
   //Query the OBD-II-UART for the Vehicle rpm
   Serial1.flush();
   Serial1.print("010C\r");
   OBD_read();
 
   return ((strtol(&rxData[6],0,16)*256)+strtol(&rxData[9],0,16))/4;
 }

 int getSpeed(void)
 {
   //Query the OBD-II-UART for km/h
   Serial1.flush();          
   Serial1.print("010D\r");
   OBD_read();
 
   return strtol(&rxData[6],0,16);
 }


 void OBD_read(void)
 { 
   char c;
   do{
     if(Serial1.available() > 0)
     {
       c = Serial1.read();
       if((c!= '>') && (c!='\r') && (c!='\n')) //Keep these out of our buffer
       {
         rxData[rxIndex++] = c; //Add whatever we receive to the buffer
       }
      }    
   }while(c != '>'); //The ELM327 ends its response with this char so when we get it we exit out.
   rxData[rxIndex++] = '\0';//Converts the array into a string
   rxIndex=0; //Set this to 0 so next time we call the read we get a "clean buffer"
 }
