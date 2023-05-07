//  ***************************************************************
//  NES-Classic-As-Keyboard.ino - Creation date: 25/03/2020
//  -------------------------------------------------------------
//  Copyright (C) 2020 iProgramInCpp - Licensed under LGPL-3.0
//
//  ***************************************************************
//  Programmer(s):  iProgramInCpp (iprogramincpp@gmail.com)
//  ***************************************************************

// Note: This can be expanded for any kind of controller supported by the NintendoExtensionCtrl library.

#include <NintendoExtensionCtrl.h>

NESMiniController nes;

// note: The reason the buffer is 10 bytes instead of 8, is because we can theoretically press all 8 buttons at once.
// This would lead to a buffer overflow if the size of the buffer was 8. We do not use the last 2 bytes, they're
// merely a dud.
uint8_t kbdBuffer[10] = { 0 };

uint8_t kbdKeyCodes[8] = { 26, 22, 4, 7, 21, 23, 28, 24 }; // WSADRTYU (you can change this)

boolean led0State = 0;
int keyIndex = 0;

void PushKeyToSerial(uint8_t key)
{
	kbdBuffer[2 + keyIndex++] = kbdKeyCodes[key];
}

void PushKeysAndReset()
{
	// the last 2 keys pressed will get cut off, since USB only supports up to 6 keys pressed at once.
	Serial.write(kbdBuffer, 8);
	memset(kbdBuffer, 0, 10);
	keyIndex = 0;
}

void setup()
{
	Serial.begin(9600);
	nes.begin();
	
	// this flashes the LED on the board on and off until a controller is found
	while (!nes.connect())
	{
		digitalWrite(13, !led0State);
		led0State ^= 1;
		delay(1000);
	}
	
	// Uh oh, looks like your controller isn't genuine?
	if (nes.isThirdParty())
	{
		nes.setRequestSize(8);  // Requires 8 or more bytes for third party controllers
	}
}

void loop()
{
	// Get new data from the controller
	boolean success = nes.update();
	
	if (success == true)
	{
		// We've got data!
		
		// Turn the LED off if needed.
		if (led0State)
		{
			led0State = 0;
			digitalWrite(13, 0);
		}
		
		nes.fixThirdPartyData();  // If third party controller, fix the data
		
		
		if (nes.dpadUp())       PushKeyToSerial(0);
		if (nes.dpadDown())     PushKeyToSerial(1);
		if (nes.dpadLeft())     PushKeyToSerial(2);
		if (nes.dpadRight())    PushKeyToSerial(3);
		if (nes.buttonA())      PushKeyToSerial(4);
		if (nes.buttonB())      PushKeyToSerial(5);
		if (nes.buttonStart())  PushKeyToSerial(6);
		if (nes.buttonSelect()) PushKeyToSerial(7);
		//nes.printDebug();  // Print all of the values
		
		// submit.
		PushKeysAndReset();
	}
	else
	{
		// Data is bad. Controller may have been disconnected.
		nes.reconnect();
		digitalWrite(13, led0State);
		led0State ^= 1;
		delay(1000);
	}
	
	// Update roughly 100 times per second. Reduce this if you need to lower input lag.
	delay(10);
}
