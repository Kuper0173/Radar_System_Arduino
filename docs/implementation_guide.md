# Implementation Guide

The project was built in stages so students can test each part before adding the next one.

## Stage 1: Servo movement

The first version controls the servo angle. Originally the angle was sent through the Arduino IDE Serial Monitor. Later, the joystick replaced the command line input.

The final version uses the joystick as an incremental controller:

- Moving the joystick changes the current servo angle.
- Releasing the joystick keeps the servo at the last angle.
- Pressing the joystick button returns the servo to 90 degrees.

## Stage 2: Ultrasonic distance

The HC-SR04 sensor measures distance using `Trig` and `Echo`.

The code sends a short pulse through `Trig`, waits for the `Echo` response, and converts the duration into centimeters.

To reduce ghost readings, the code:

- ignores invalid readings below 2 cm or above 200 cm
- stores the last valid readings
- uses the median value instead of every raw reading
- keeps the last valid distance briefly if no echo appears

## Stage 3: OLED display

The OLED shows:

- filtered distance
- current servo angle
- a blinking `DETECTED!` alert when the object is closer than 5 cm

The display uses I2C:

- `SDA` on Arduino Uno is `A4`
- `SCL` on Arduino Uno is `A5`

## Stage 4: Classroom resource

The HTML file in `resources/` is a student-facing connection challenge.

It uses a simple scroll design:

- one component per card
- tabs for connections, how it works, and tips
- a theory lab section for concepts like control, sensing, embedded systems, signals, and interfaces

