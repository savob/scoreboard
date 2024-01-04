# Firmware

The firmware here is for the main board and ran on an Arduino Nano programmed using the Arduino IDE, version 1.8.13 when I wrote this.

Although this was meant to show the time remaining and ring the buzzer at completion, I only ended up focusing in on the display functionality since I really wanted to get the seven-segment to work nicely without any flickering as it was multiplexed digit by digit. I managed this in the second version by using hardware registers in the ATmega to control the outputs rather than the repeated and time-consuming `digitalWrite()` calls.
