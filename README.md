# Radar System with Arduino

Educational Arduino radar project using a micro servo, HC-SR04 ultrasonic sensor, analog joystick, and OLED display.

The goal is to build a small interactive circuit that can move a sensor from side to side, measure distance, and show the result on screen. The repository also includes an HTML classroom resource called **Build the Radar**, designed as a connection challenge for students.

## Project features

- Servo angle controlled with an analog joystick.
- Ultrasonic distance measurement with basic filtering.
- OLED screen showing distance and servo angle.
- Close-object alert: `DETECTED!` when an object is closer than 5 cm.
- Interactive HTML resource for students.
- Clean documentation for wiring and implementation.

## Repository structure

```text
.
├── arduino/
│   └── radar_system/
│       └── radar_system.ino
├── docs/
│   ├── classroom_challenge.md
│   ├── hardware_connections.md
│   └── implementation_guide.md
├── resources/
│   └── radar_connection_challenge.html
└── README.md
```

## Hardware

- Arduino Uno
- Micro servo, such as SG90
- HC-SR04 ultrasonic sensor
- Analog joystick module
- I2C OLED display, usually SSD1306 128x64
- Breadboard
- Jumper wires

## Main connections

| Component | Pin | Arduino |
|---|---:|---:|
| Servo | Signal | D9 |
| Servo | VCC | 5V |
| Servo | GND | GND |
| HC-SR04 | Trig | D10 |
| HC-SR04 | Echo | D11 |
| HC-SR04 | VCC | 5V |
| HC-SR04 | GND | GND |
| Joystick | URX | A0 |
| Joystick | URY | A1 |
| Joystick | SW | D2 |
| Joystick | 5V | 5V |
| Joystick | GND | GND |
| OLED | SDA | A4 |
| OLED | SCL | A5 |
| OLED | VCC | 5V |
| OLED | GND | GND |

For more detail, see [docs/hardware_connections.md](docs/hardware_connections.md).

## Arduino libraries

Install these libraries in the Arduino IDE:

- `Servo` — included with Arduino IDE
- `Wire` — included with Arduino IDE
- `Adafruit SSD1306`
- `Adafruit GFX Library`

## How to run

1. Open [arduino/radar_system/radar_system.ino](arduino/radar_system/radar_system.ino) in Arduino IDE.
2. Install the required libraries.
3. Select your board and port.
4. Upload the sketch.
5. Open the Serial Monitor at `115200 baud` if you want to view angle and distance values.
6. Move the joystick left and right to control the servo.

## Interactive classroom resource

Open this file in a browser:

[resources/radar_connection_challenge.html](resources/radar_connection_challenge.html)

It contains a simple scroll-based activity where students connect the project one component at a time and explore related electronics concepts.

## Notes

If the servo shakes or the Arduino resets, power the servo from an external regulated 5V supply and connect the external supply GND to Arduino GND.

