# NES Classic To Keyboard Converter
NES Classic Controller to HID Keyboard converter software written for the Arduino Uno board. It REQUIRES the ATmega16u2 to be flashed to behave like a keyboard using FLIP first. 

## Prerequisites
+ Arduino v1.8+
+ This library: https://github.com/dmadison/NintendoExtensionCtrl
+ Atmel Flip (you can find it easily on the web)

## Installation
First, open the Arduino software with the sketch. Then, upload it to your Arduino/Genuino board. After you've done that, it's time to turn the arduino to a HID keyboard. To do that, you need to short the 2 pins on the ICSP header next to the pins closest to the USB connector, after which you will undo the short. This will put the device into DFU mode. After that, you will open Flip, press CTRL+U and click "Open", then click on the arrow pointing towards book icon. Select "Arduino-keyboard-0.3.hex", supplied with this project, then click on the icon with the arrow pointing towards a chip. It should say "Programming done", after which you unplug and reconnect the device to your USB port.

## Functionality
The NES Classic to keyboard converter takes the NES Classic Controller's button inputs and turns them into key strokes which can be mapped to your favorite NES emulator of choice. Most of them do allow keyboard play, after all.
