
#include <HomeSenNet.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9,10);
const uint64_t pipe_read = 0xE8E8F0F0E1LL;
const uint64_t pipe_write = 0xE8E8F0F0E1AA;

HSN hsn; //input arguments:(how many periphelars, their names) - Define who this Arduino will control - check IAm map table

void setup() {
  // put your setup code here, to run once:
  //TCCR2B = TCCR2B & B11111000 | 0x01; // sets pin 3 and 11 timer to have PWM at 980 hz - only for led strips
  //TCCR2B = TCCR2B & 0b11111000 | 0x03;
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe_write);
  radio.openReadingPipe(1,pipe_read);
  radio.startListening();
  hsn.I_Have(3,1,2,3); // let the host know who I am
}



void loop() {
  if (hsn.RadioMessageAvailable(radio)){
    hsn.ExecuteCommand(radio); //feedback included in this method
  }
}
