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
		/////////// for Sever
		void ReadSerial();
		bool MessageAvailable(RF24 radio); //checks if message is available and sends message
		unsigned int New_Message[1]; //message that is sent over nRF24
		bool FeedBackChecker(RF24 RADIO); //checks for feedback and tries to resend message is none. (max 5 times, once every 100ms)
		/////////// for Client
		
	
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
		
		
		
		//Peripherals
		String sensor_map[10] = {
		"Master node", 			//0
		"Red led Bookshelf",	//1
		"Blue Led Bookshelf",	//2
		"Green Led Bookshelf",	//3
		"TV light Dimmer",		//4
		"OPEN", 				//5
		"OPEN", 				//6
		"OPEN", 				//7
		"OPEN", 				//8
		"OPEN" 					//9
		};;
};

#endif // __HSN__


