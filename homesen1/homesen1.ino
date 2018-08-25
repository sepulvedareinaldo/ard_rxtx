
#include <HomeSenNet.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9,10);
const uint64_t pipe_write = 0xE8E8F0F0E1LL;
const uint64_t pipe_read = 0xE8E8F0F0E1AA;

HSN hsn; //input arguments:(how many periphelars, their names) - Define who this Arduino will control - check IAm map table

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe_write);
  radio.openReadingPipe(1,pipe_read);
  radio.startListening();
  hsn.I_Have(1,0); // let the host know who I am
}



void loop() {
  // put your main code here, to run repeatedly:
  hsn.ReadSerial(); // check if info is coming in from host
  hsn.MessageAvailable(radio); // if message is available, then send message over the air
  hsn.FeedBackChecker(radio); // check to see if fieedback is available, if not re-send message (no more than 5 times

  
}
