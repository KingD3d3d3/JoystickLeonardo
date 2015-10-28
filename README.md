# Arduino Leonardo as USB Game Controller

This code runs on an Arduino Leonardo allowing it to act as a USB Game Controller

First make a backup copy of the following two files located in the 
“%PROGRAMFILES%\Arduino\hardware\arduino\avr\cores\arduino” folder.
* HID.cpp
* USBAPI.h

Replace them with the ones above to add a generic Game Controller to the Arduino Leonardo.

The following describes the Joystick library that is included in the updated USBAPI.h and HID.cpp files.

Connections of the Arduino as follows:

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

Compile and upload the JoystickLeonardo.ino file onto the Arduino Leonardo using the Arduino Software (IDE). Tested with version 1.0.6 of the software.

The following steps are for Windows. If you have a different operating system, these steps may differ.

Go to the Windows Control Panel and select “Hardware and Sound”.

Find your Arduino Leonardo in "“Devices and Printers” list.

Right click and select “Game controller settings” to get to the “Game Controllers” dialog. Select Arduino Leonardo and click "Properties".

You can now test your game controller functionality and modify it by adapting your code.
