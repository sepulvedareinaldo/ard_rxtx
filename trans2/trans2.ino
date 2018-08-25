#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
int gsm[1];
RF24 radio(9,10);
const uint64_t pipe_write = 0xE8E8F0F0E1LL;
const uint64_t pipe_read = 0xE8E8F0F0E1AA;
int SW1 = 7;

unsigned long times;
unsigned long preTime;

void setup(void){
Serial.begin(9600);
radio.begin();
radio.openWritingPipe(pipe_write);
radio.openReadingPipe(1,pipe_read);
radio.startListening();
preTime=millis();
msg[0] = 100;
}

void loop(void){

//delay(1000);


times = millis();
if (preTime+2000<times){
  if (msg[0]<200){
    msg[0] += 1;
  } else{
    msg[0] = 100;
  }
  radio.write(msg, 1);
  Serial.println(msg[0]);
  radio.startListening();
  preTime=millis();
} else{
 
}


if (radio.available()){
  Serial.print("in listening");
  bool done = false;
  while (!done){
  done = radio.read(gsm, 1);
  }
  Serial.println(gsm[0]);
  delay(10);
}else{
  //Serial.println("No radio available");

  }

}
