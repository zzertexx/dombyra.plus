# Dombyra.plus
Simple device to teach playing on Dombyra
<img width="1920" height="1080" alt="dombyra_case" src="https://github.com/user-attachments/assets/f210b4bb-d1ed-4d9a-80ed-d45d29586769" />

## Description
Have you ever wanted to learn playing Dombyra at home?
If yes, then you absolutely looked resources to learn, and what you found was some videos that shows you how they play, while you need to everytime find out which notes is being pressed.

This device is solution for your problem. First hardware addon for Dombyra.

Using bunch of SK6812 LEDs on Dombyra's neck, it will show you which notes need to be pressed right now and after.
By ESP32 and INMP441 mic, it will be controlled by local Website to choose notes and continue showing notes.

## Features
* Wi-Fi website to choose kuis
* 18 LEDs per string with two color mod for Up/Down play
* Battery charge system for autonomuos play
* Comfortable case to play
* Volume spike detector to continue play

## Firmware
* Firmware was written on Arduino IDE
* Currently it has 2 kuis(more coming soon)
* Needed libraries: FastLED.h, i2s.h(core), WiFi.h(core), WebServer.h(core)

## Bill of Materials

| Component | Quantity |
|---|---|
| ESP32 DevKit v1 | 1 |
| SK6812 LEDs | 38 |
| INMP441 module | 1 |
| Li-Po Battery + 2 pin header 2mm | 1 |
| USB-C Receptacle only 6 pin | 1 |
| TP4056 module | 1 |
| MT3608 module | 1 |
| M2 screws | 14 |
| M3 screws | 4 |
| 4 pin Connector Sockets - Angled | 1 |
| 4 pin Connector Sockets - Vertical | 1 |
| NTC 10k | 1 |
| Unpolarized capacitors(THT) 10uF & 22uF | 2 each |
| Inductor 4.7uH | 1 |
| Diode | 2 LED and 1 Schottky |
| Resistors | 110k; 15k; 330 x2; 68k; 2.4k; 1k; 0.4; 5k1 x2 |

<img width="1920" height="1080" alt="3D Case2" src="https://github.com/user-attachments/assets/680663b6-9155-4d0b-91b3-0e8ec0ef954c" />
<img width="1920" height="1080" alt="3D Case1" src="https://github.com/user-attachments/assets/afbbc740-3484-4c99-b3f2-54b834279d16" />
<img width="1710" height="864" alt="image" src="https://github.com/user-attachments/assets/df6e060f-1a22-4cbf-a48d-67cecb664b72" />
<img width="1714" height="824" alt="image" src="https://github.com/user-attachments/assets/315ca53c-785b-4111-a09b-3ba192af2854" />

