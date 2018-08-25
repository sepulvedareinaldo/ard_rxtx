/*
 open to anyone

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * first shot at home sensor network as a standard library to load to all Arduino-Sensors that varies only by initialization
 *
 * Class declaration for home_sen_net
 *
 */
/*
_Ihave definition mapping:
0 - Emitter node (connected to a Sever)
1 - Red led Bookshelf
2 - Green led Bookshelf
3 - Blue led Bookshelf
4 - Dim-able light TV

*/
#include "Arduino.h"
#include "HomeSenNet.h"
#include "Arduino.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdarg.h> //for variable length arg functions
#include <stdio.h> //for variable length arg functions


/****************************************************************************/

HSN::HSN()
{

	//_Ihave = _Ihave; //state who this Arduino will control - check _Ihave map table
	_FeedBackTimer = millis(); // initialize feedback timer for use in FeedBack loop (time out for resend request)
	
	//PERIPHERAL detail
	
	
}

/****************************************************************************/

void HSN::I_Have(int Ihave , ...){
	
	
	va_list args; //creates list of arguments
    va_start( args, Ihave ); //starts iterator of arguments
	
	for ( int x = 0; x < Ihave; x++ ) // iterate over all arguments and initialize peripherals matrix
    {
        int peri = va_arg ( args, int ); 
		Peripherals[peri] = 1; // Peripherals linked to this network
		if (peri==0){
			Serial.print("I am: "); Serial.print(sensor_map[peri]); //let host know who I am/have
			Serial.print(" (");Serial.print(peri);Serial.println( ")");
		}else{
			Serial.print("I have: "); Serial.print(sensor_map[peri]); //let host know who I am/have
			Serial.print(" (");Serial.print(peri);Serial.println( ")");
		}
    }
    va_end ( args );  
	
	
	//Show Peripherals matrix
	int peri_size = sizeof(Peripherals) / sizeof(int);
	Serial.print("Peripherals: [");Serial.print(Peripherals[0]);
	for ( int x = 1; x < peri_size; x++ ) {
		Serial.print(",");Serial.print(Peripherals[x]);
	}
	Serial.println("]");
	//end - show peripherals matrix
	
	_Ihave = Ihave; //state how many peripherals on this micro-controller
	
}

/****************************************************************************/

void HSN::ReadSerial()
{
	if (Serial.available()) { // read messages form host server - messages shall be no longer that 5 digits (largest 2 byte 
		_SerialBuffer = Serial.read()-'0';
		_inByte[_tempCount]=_SerialBuffer;
		_tempCount += 1;
	}
}

/****************************************************************************/

bool HSN::MessageAvailable(RF24 RADIO)
{
	//RF24 radio(9,10); //???
	if (_tempCount == _messageSize) {//messageSize = 5 - size of message coming form host
		int i;
		unsigned int sendByte = 0;
		for ( i = 0; i <_messageSize ; i=i+1) {
			sendByte = sendByte + (int)ceil(_inByte[i]*(pow(10,_messageSize-1-i)));//encode message so we sent last 3 digits as value and identifyier in front
			_inByte[i] = 0;
		} 
		New_Message[0] = sendByte; // store message as a int array to be able to send
		_tempCount = 0; // reset serialBuffer count (so we only read 5 int to send to sensors)
		////
		Serial.print("sending message: "); Serial.println(New_Message[0]);
		RADIO.write(New_Message, 2); //send message to sensors - 2 bytes 
		RADIO.startListening();  //set nrf24 back to listening - after right is needs to be set back to listining
		////
		unsigned int x = (New_Message[0] / 1000U) % 10;//update feedback matrix to listen for feedback
		FeedBackTimer[x]=millis();
		//Serial.print("feedback matrix index "); Serial.print(x); Serial.print("  timer started at ");Serial.println(FeedBackTimer[x]);
		
		////
		return true; // message is available to send
	}
	return false; // no message in que
}



bool HSN::FeedBackChecker(RF24 RADIO)
{
	
	if (RADIO.available()){ // receiving message - need to check if it's feedback
		bool done = false;
		unsigned int feedback[1];
		while (!done){
			done = RADIO.read(feedback, 2);
		}
		Serial.print("received feedback: ");
		Serial.println(feedback[0]);
		///
		if (feedback[0]==New_Message[0]){
			unsigned int x = (feedback[0] / 1000U) % 10;//update feedback matrix to listen for feedback
			FeedBackTimer[x]=0;
			FeedBackCounter[x]=0;
		}
		//Serial.print("feedback matrix index "); Serial.print(x); Serial.println("  is false (0)");
		///
		return true;
	}
	int elementCount = sizeof(FeedBackTimer) / sizeof(long);	
	for (int i = 0; i < elementCount; i++){
		//Serial.print("in loop ");Serial.println(i);
		if ((FeedBackTimer[i]!=0) && ((millis() - FeedBackTimer[i]) > 200)){
			//Serial.print("elapsed time "); Serial.println(((millis() - FeedBackTimer[i])));
			FeedBackTimer[i]=millis(); //reset timer since meesage has not arrived
			//Serial.print("elapsed time "); Serial.println(((millis() - FeedBackTimer[i])));
			FeedBackCounter[i]+=1; //store lost attempted
			Serial.print(FeedBackCounter[i]);Serial.print(" resending message ");Serial.println(New_Message[0]);
			RADIO.write(New_Message, 2); //re-write message
			RADIO.startListening(); //re-start listening
		}
		if (FeedBackCounter[i]>4){ // how many times should we try to right message
			Serial.print("Failed to write to device: ");Serial.print(i);Serial.print(" message "); Serial.println(New_Message[0]);
			FeedBackCounter[i] = 0; //reset counter off lost message
			FeedBackTimer[i] = 0; //reset counter off lost message
		}
	}
	
	
	return false;
		
}

/****************************************************************************/

