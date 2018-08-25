#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
RF24 radio(9,10);
const uint64_t pipe_read = 0xE8E8F0F0E1LL;
const uint64_t pipe_write = 0xE8E8F0F0E1AA;
int LED1 = 3;

void setup(void){
Serial.begin(9600);
radio.begin();
radio.openWritingPipe(pipe_write);
radio.openReadingPipe(1,pipe_read);
radio.startListening();
pinMode(LED1, OUTPUT);
}

void loop(void){
if (radio.available()){
  bool done = false;
  while (!done){
    done = radio.read(msg, 1);
    }
  Serial.println(msg[0]);
  //msg[0]=200;
  delay(1000);
  radio.write(msg, 1);
  Serial.print("sent ");
  Serial.println(msg[0]);
  radio.startListening();
  delay(10);
}else{
  //Serial.println("No radio available");
  
  
  }

}
