vol_display
===========
Code for a spinning 3D volumetric display running off of an Arduino.

<b>Computer-End (interpreter.cpp)</b><br>
Converts 3D shapes "drawn" in a text file into strings of bytes (chars) to be sent to the Arduino through a serial port.

<b>Arduino-End (vol_display.ino)</b><br>
Reads the values sent by the computer and relays them to the spinning LEDs (connected to the Arduino through daisy-chained shift registers).
