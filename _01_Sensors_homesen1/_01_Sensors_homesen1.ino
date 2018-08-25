
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
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe_write);
  radio.openReadingPipe(1,pipe_read);
  radio.startListening();
  hsn.I_Have(3,1,2,3); // let the host know who I am
}




int Rled = 6;
int Gled = 5;
int Bled = 3;
int msg[1];

void changeLED(int input){
  Serial.println("in changeLED");
  unsigned x = (input / 1000U) % 10;
  int value = input - x*1000 ; 
  if (x == 1){ //1 is red
    analogWrite(Rled,value);
    Serial.print("red: "); Serial.println(value);
  }
  if (x == 2){ //2 is green
    analogWrite(Gled,value);
    Serial.print("green: "); Serial.println(value);
  }
  if (x == 3){ //3 is blue
    analogWrite(Bled,value);
    Serial.print("blue: "); Serial.println(value);
  }
  
}

void loop() {
if (hsn.RadioMessageAvailable(radio)){
  hsn.ExecuteCommand(radio); //feedback included in this method
}

}
