/*
*  Project     Nintendo Extension Controller Library
*  @author     David Madison
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    LGPLv3 - Copyright (c) 2018 David Madison
*
*  This file is part of the Nintendo Extension Controller Library.
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  Example:      NES_DebugPrint
*  Description:  Connect to an NES Mini controller and continuously print
*                its control data, nicely formatted for debugging, over
*                the serial bus.
*/

#include <NintendoExtensionCtrl.h>

NESMiniController nes;
uint8_t kbdBuffer[10] = {0,};

uint8_t kbdPressed[8] = {0,};
uint8_t kbdLastPressed[8] = {0,};

uint8_t kbdKeyCodes[8] = {26,22,4,7,21,23,28,24}; // WSADRTYU (perhaps)

boolean led0State = 0;
int hack = 0;

void PushKeyToSerial(uint8_t key)
{
  kbdBuffer[2 + hack++] = kbdKeyCodes[key];
}
void PushKeysAndReset()
{
  Serial.write(kbdBuffer, 8); // the last 2 keys pressed will get cut off.
  memset(kbdBuffer, 0, 10);
  hack = 0;
}

void setup() {
	Serial.begin(9600);
	nes.begin();
	while (!nes.connect()) {
		//Serial.println("Classic Controller not detected!");
    digitalWrite(13, !led0State);
    led0State ^= 1;
		delay(1000);
	}

	if (nes.isThirdParty()) {  // Uh oh, looks like your controller isn't genuine?
		nes.setRequestSize(8);  // Requires 8 or more bytes for third party controllers
	}
}

void loop() {
	boolean success = nes.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
    /*
    state1 = nes.dpadUp() << 8 | nes.dpadDown() << 7 | nes.dpadLeft() << 6 | nes.dpadRight() << 5;
    state2 = nes.buttonA() << 8 | nes.buttonB() << 7 | nes.buttonStart() << 6 | nes.buttonSelect() << 5;
    */
    // support 6-key rollover
    if (led0State)
    {
      led0State = 0;
      digitalWrite(13, 0);
    }
    nes.fixThirdPartyData();  // If third party controller, fix the data!
    if (nes.dpadUp())       PushKeyToSerial(0);
    if (nes.dpadDown())     PushKeyToSerial(1);
    if (nes.dpadLeft())     PushKeyToSerial(2);
    if (nes.dpadRight())    PushKeyToSerial(3);
    if (nes.buttonA())      PushKeyToSerial(4);
    if (nes.buttonB())      PushKeyToSerial(5);
    if (nes.buttonStart())  PushKeyToSerial(6);
    if (nes.buttonSelect()) PushKeyToSerial(7);
		//nes.printDebug();  // Print all of the values!

    // submit.
    PushKeysAndReset();
    
    memcpy(kbdLastPressed, kbdPressed, 8);
	}
	else {  // Data is bad :(
		//Serial.println("Controller Disconnected!");
    nes.reconnect();
    digitalWrite(13, led0State);
    led0State ^= 1;
    delay(1000);
	}
  delay(10);
}
