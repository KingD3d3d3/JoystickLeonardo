# Arduino Leonardo as USB Game Controller

This code runs on an Arduino Leonardo allowing it to act as a USB Game Controller

First make a backup copy of the following two files located in the 
“%PROGRAMFILES%\Arduino\hardware\arduino\avr\cores\arduino” folder.
* HID.cpp
* USBAPI.h

Replace them with the ones above to add a generic Game Controller to the Arduino Leonardo.

Connections as follows:

Arduino Pin	 |	Function
------------ |  ---------
4						 |	Button 1
7						 |	Button 2
8						 |	Button 3
9						 |	Button 4
11					 |	Button 6
A0					 |	Joystick Vertical
A1					 |	Joystick Horizontal
10					 |	Joystick Button
GND					 |	Joystick GND
5V					 |	Joystick VCC
