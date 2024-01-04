# Tools 102 Scoreboard

Scoreboard I made for the Tools 102 workshop in 2019. The goal was to bring in some electronics for people to learn with rather than just the usual mechanical stuff done. To that end I wanted to help build an electric countdown system based around an Arduino Nano which would show the time and keep track of score.

# Hardware

For this project I made the main board and a friend did the auxiliary detector board, both were designed to make the most of the back layer since these boards were going to be milled on campus, not etched in a factory. Thus there would be no plated through holes so keeping everything on the back made soldering and connections easier.

The boards were originally designed in EAGLE, but have since been converted to KiCad 7.0.10.

# Software

The code focused on the display function, and was all written in the Arduino IDE 1.8.13.
