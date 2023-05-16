# Frequency-Generator-8051
Square wave frequency generator circuit using Atmel 8051 microcontroller

## Overview
The following project takes an input frequency value from the keypad P1 and is scanned via the
software to determine the pressed key from 0 to 9. The segments of the quad common anode
display module are connected to P2and the control of the four common anodes is done using
P0.0to P0.3via 2N3906 transistors. The refreshing of the display is done using Timer 0 interrupt.
After pressing a key, it is displayed and shifted until 4 digits are entered. After entering the
frequency value to be generated(in Hz), a push button connected to INT0 is pressed to generate an
external interrupt which loads timer 1 with the proper values and enables square wave generation
on pin P3.7. If a new frequency value is to be entered; the switch connected to INT0 is pressed
again.

## Proteus Simulation
https://github.com/seifbassiounyy/Frequency-Generator-8051/assets/104737465/8fd116f1-bd87-4987-b03d-abb11bf508c6

