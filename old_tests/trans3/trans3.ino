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

int count = 0;
int inByte[4] = {0,0,0,0};
void loop(void){

//delay(1000);

int iterator=0;
if (Serial.available()) {
    int buffs = Serial.read()-'0';
    inByte[count]=buffs;
    count += 1;

  }

if (count == 4){
  int i;
  int sendByte = 0;
  for ( i = 0; i <4 ; i=i+1) {
    Serial.println(i);
    Serial.println((int)ceil(inByte[i]*(pow(10,3-i))));
    sendByte = sendByte + (int)ceil(inByte[i]*(pow(10,3-i)));
    Serial.println(sendByte);
    inByte[i] = 0;
  } 
  Serial.println(sendByte);
  
  msg[0]=sendByte;
  radio.write(msg, 2); 
  radio.startListening();  
  //Serial.println("");
  count=0;
  msg[0]=0;
  sendByte=0;
}

if (radio.available()){
  Serial.print("in listening");
  bool done = false;
  while (!done){
  done = radio.read(gsm, 2);
  }
  Serial.println(gsm[0]);
  delay(10);
}else{
  //Serial.println("No radio available");

  }
/*
times = millis();
if (preTime+2000<times){
  if (msg[0]<9000){
    msg[0] += 500;
  } else{
    msg[0] = 0000;
  }
  
  Serial.println(msg[0]);
  radio.startListening();
  preTime=millis();
} else{
 
}


if (radio.available()){
  Serial.print("in listening");
  bool done = false;
  while (!done){
  done = radio.read(gsm, 2);
  }
  Serial.println(gsm[0]);
  delay(10);
}else{
  //Serial.println("No radio available");

  }
*/
}
