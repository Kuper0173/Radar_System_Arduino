# Hardware Connections

This document summarizes the wiring for the Arduino radar system.

## Servo

| Servo wire | Arduino |
|---|---|
| Signal, usually orange/yellow | D9 |
| VCC, usually red | 5V |
| GND, usually brown/black | GND |

The servo rotates the ultrasonic sensor from side to side.

## HC-SR04 ultrasonic sensor

| HC-SR04 pin | Arduino |
|---|---|
| VCC | 5V |
| GND | GND |
| Trig | D10 |
| Echo | D11 |

The sensor measures distance by sending an ultrasonic pulse and measuring the echo time.

## Analog joystick

| Joystick pin | Arduino |
|---|---|
| GND | GND |
| 5V | 5V |
| URX | A0 |
| URY | A1 |
| SW | D2 |

In this project, `URY` on `A1` is used to control left-right servo movement.

The joystick button uses `INPUT_PULLUP`, so it normally reads `HIGH` and reads `LOW` when pressed.

## OLED I2C display

| OLED pin | Arduino Uno |
|---|---|
| GND | GND |
| VCC | 5V |
| SDA | A4 |
| SCL | A5 |

Most SSD1306 OLED modules use I2C address `0x3C`. If the display stays blank, try `0x3D`.

## Important power note

All grounds must be connected together. If the servo uses an external 5V power supply, connect:

- external supply GND
- Arduino GND
- sensor GND
- OLED GND
- joystick GND

to the same common ground.

