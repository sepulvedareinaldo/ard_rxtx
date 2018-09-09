
#include <HomeSenNet.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <PWM.h>

RF24 radio(9,10);
const uint64_t pipe_read = 0xE8E8F0F0E1LL;
const uint64_t pipe_write = 0xE8E8F0F0E1AA;

HSN hsn; //input arguments:(how many periphelars, their names) - Define who this Arduino will control - check IAm map table

void setup() {
  //InitTimersSafe(); //initialize all timers except for 0, to save time keeping functions
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe_write);
  radio.openReadingPipe(1,pipe_read);
  radio.startListening();
  hsn.I_Have(3,1,2,3); // let the host know who I am

//      int32_t frequency = 100; //set frequency (in Hz) for led strip
//      Serial.println("frequency set to 100 ");
//      
//      bool BLUEsuccess = SetPinFrequencySafe(3, frequency);  //sets the frequency for the specified pin
}



void loop() {
  if (hsn.RadioMessageAvailable(radio)){
    hsn.ExecuteCommand(radio); //feedback included in this method
  }

//pwmWrite(3,155);
}
