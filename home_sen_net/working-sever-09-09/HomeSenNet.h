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
Peripherals definition mapping:
0 - Emitter node (connected to a Sever)
1 - Red led Bookshelf
2 - Green led Bookshelf
3 - Blue led Bookshelf
4 - Dim-able light TV

*/
#ifndef __HomeSenNet__
#define __HomeSenNet__

//initial definitions
#include "Arduino.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdarg.h> //for variable length arg functions
#include <stdio.h> //for variable length arg functions

#include <PWM.h>// fix for oscillating intensity (due to pins 3 and 5/6 are on different timers - pin3 should set to frequency 100 or over 4000 hz (higher frequency are audible)

/**
 * Define intilizations? or sensors?
 *
 * 
 */
 
//////////////////include info if necessary 
 
/**
 * Driver for nRF24L01(+) 2.4GHz Wireless Transceiver
 */

class HSN
{
	public:
		/////////// for All
		HSN();
		void I_Have(int Ihave , ...); //rename peripheral
		unsigned int New_Message[1]; //message that is sent over nRF24
		/////////// for Sever
		void ReadSerial();
		bool MessageAvailable(RF24 radio); //checks if message is available and sends message
		bool FeedBackChecker(RF24 RADIO); //checks for feedback and tries to resend message is none. (max 5 times, once every 100ms)
		/////////// for Client
		bool RadioMessageAvailable(RF24 radio);
		void ExecuteCommand(RF24 RADIO);
		bool ProvideFeedback(RF24 radio);
	
	private:
		int _Ihave;
		unsigned long _FeedBackTimer;
		int _SerialBuffer;
		int _messageSize = 5;// needs to be the same as _inByte
		int _inByte[5] = {0,0,0,0,0}; // largest number for 2 Bytes - coded info for ECU (byte1) and value (byte2)
		int _tempCount;
		unsigned long FeedBackTimer[10]={0,0,0,0,0,0,0,0,0,0};
		int FeedBackCounter[10]={0,0,0,0,0,0,0,0,0,0};
		///////////for Client
		int Peripherals[10] = {0,0,0,0,0,0,0,0,0,0};
		int _version;
		
		  
		//Peripherals
		String sensor_map[10] = {
		"Master node", 			//0
		"Red led Bookshelf",	//1
		"Green Led Bookshelf",	//2
		"Blue Led Bookshelf",	//3
		"TV light Dimmer",		//4
		"OPEN", 				//5
		"OPEN", 				//6
		"OPEN", 				//7
		"OPEN", 				//8
		"OPEN" 					//9
		};;
		int sensor_pin_map[10] = {
		0, 	//no pin - Master node - node 0
		6,	//pin 6 - "Red led Bookshelf" - node 1
		5,	//pin 5 - "Green Led Bookshelf"  - node 2
		3,	//pin 3 - "Blue Led Bookshelf" - node 3
		0,	//no pin - "TV light Dimmer"  - node 4
		0, 	//no pin - OPEN  - node 5
		0, 	//no pin - OPEN  - node 6
		0, 	//no pin - OPEN  - node 7
		0, 	//no pin - OPEN  - node 8
		0 	//no pin - OPEN  - node 9
		};;
		int sensor_state[10] = {0,0,0,0,0,0,0,0,0,0};
};

#endif // __HSN__


