#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
RF24 radio(9,10);
const uint64_t pipe_read = 0xE8E8F0F0E1LL;
const uint64_t pipe_write = 0xE8E8F0F0E1AA;
int Rled = 6;
int Gled = 5;
int Bled = 3;

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

void setup(void){
Serial.begin(9600);
radio.begin();
radio.openWritingPipe(pipe_write);
radio.openReadingPipe(1,pipe_read);
radio.startListening();
pinMode(Rled, OUTPUT);
pinMode(Gled, OUTPUT);
pinMode(Bled, OUTPUT);
}

void loop(void){
if (radio.available()){
  bool done = false;
  while (!done){
    done = radio.read(msg, 2);
    }
  //Serial.println(msg[0]);
  //msg[0]=200;
  delay(50);
  changeLED(msg[0]);
  radio.write(msg, 2);
  Serial.print("sent ");
  Serial.println(msg[0]);
  radio.startListening();
}else{
  //Serial.println("No radio available");
  
  
  }

}
