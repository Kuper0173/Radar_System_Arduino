#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const byte OLED_ADDRESS = 0x3C; // Try 0x3D if the screen stays blank
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool displayAvailable = false;

// Servo
Servo radarServo;
const byte SERVO_PIN = 9;
const int SERVO_MIN = 0;
const int SERVO_MAX = 180;
int currentAngle = 90;

// Ultrasonic sensor HC-SR04
const byte TRIG_PIN = 10;
const byte ECHO_PIN = 11;
float currentDistance = -1;

// Joystick
const byte JOYSTICK_HORIZONTAL_PIN = A1; // URY controls left-right movement in this build
const byte JOYSTICK_SW_PIN = 2;
const int JOYSTICK_CENTER = 500;
const int DEAD_ZONE = 80;

// Distance filter
const byte FILTER_SIZE = 5;
float distanceBuffer[FILTER_SIZE];
byte distanceCount = 0;
byte distanceIndex = 0;
const float MIN_VALID_DISTANCE = 2.0;
const float MAX_VALID_DISTANCE = 200.0;
const unsigned long NO_ECHO_TIMEOUT = 1000;
unsigned long lastValidDistanceTime = 0;

// Timers
unsigned long previousServoUpdate = 0;
unsigned long previousMeasurement = 0;
unsigned long previousDisplayUpdate = 0;
const unsigned long SERVO_INTERVAL = 25;
const unsigned long MEASUREMENT_INTERVAL = 150;
const unsigned long DISPLAY_INTERVAL = 80;

void setup() {
  Serial.begin(115200);

  radarServo.attach(SERVO_PIN);
  radarServo.write(currentAngle);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(JOYSTICK_SW_PIN, INPUT_PULLUP);

  Wire.begin();
  displayAvailable = display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);

  if (displayAvailable) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("Radar System"));
    display.println(F("Starting..."));
    display.display();
    delay(800);
  } else {
    Serial.println(F("OLED display not found. Check wiring or address."));
  }
}

void loop() {
  updateServoFromJoystick();

  if (millis() - previousMeasurement >= MEASUREMENT_INTERVAL) {
    previousMeasurement = millis();
    float rawDistance = measureDistanceCm();
    updateDistanceFilter(rawDistance);
    printSerialData();
  }

  if (millis() - previousDisplayUpdate >= DISPLAY_INTERVAL) {
    previousDisplayUpdate = millis();
    updateOLED();
  }
}

void updateServoFromJoystick() {
  if (millis() - previousServoUpdate < SERVO_INTERVAL) {
    return;
  }
  previousServoUpdate = millis();

  int joystickValue = analogRead(JOYSTICK_HORIZONTAL_PIN);
  int difference = joystickValue - JOYSTICK_CENTER;

  if (digitalRead(JOYSTICK_SW_PIN) == LOW) {
    currentAngle = 90;
    radarServo.write(currentAngle);
    delay(180);
    return;
  }

  if (abs(difference) > DEAD_ZONE) {
    int movementSpeed = map(abs(difference), DEAD_ZONE, 523, 1, 4);
    movementSpeed = constrain(movementSpeed, 1, 4);

    if (difference > 0) {
      currentAngle += movementSpeed;
    } else {
      currentAngle -= movementSpeed;
    }

    currentAngle = constrain(currentAngle, SERVO_MIN, SERVO_MAX);
    radarServo.write(currentAngle);
  }
}

float measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2.0;
}

void updateDistanceFilter(float rawDistance) {
  bool validReading = rawDistance >= MIN_VALID_DISTANCE && rawDistance <= MAX_VALID_DISTANCE;

  if (validReading) {
    distanceBuffer[distanceIndex] = rawDistance;
    distanceIndex = (distanceIndex + 1) % FILTER_SIZE;

    if (distanceCount < FILTER_SIZE) {
      distanceCount++;
    }

    currentDistance = calculateMedian();
    lastValidDistanceTime = millis();
  } else if (millis() - lastValidDistanceTime > NO_ECHO_TIMEOUT) {
    currentDistance = -1;
    distanceCount = 0;
    distanceIndex = 0;
  }
}

float calculateMedian() {
  float sorted[FILTER_SIZE];

  for (byte i = 0; i < distanceCount; i++) {
    sorted[i] = distanceBuffer[i];
  }

  for (byte i = 0; i < distanceCount - 1; i++) {
    for (byte j = i + 1; j < distanceCount; j++) {
      if (sorted[j] < sorted[i]) {
        float temp = sorted[i];
        sorted[i] = sorted[j];
        sorted[j] = temp;
      }
    }
  }

  if (distanceCount == 0) {
    return -1;
  }

  if (distanceCount % 2 == 1) {
    return sorted[distanceCount / 2];
  }

  return (sorted[(distanceCount / 2) - 1] + sorted[distanceCount / 2]) / 2.0;
}

void updateOLED() {
  if (!displayAvailable) {
    return;
  }

  if (currentDistance >= MIN_VALID_DISTANCE && currentDistance < 5.0) {
    showDetectionAlert();
  } else {
    showNormalInformation();
  }
}

void showNormalInformation() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Radar System"));

  display.setCursor(0, 18);
  display.print(F("Distance: "));
  if (currentDistance < 0) {
    display.println(F("No echo"));
  } else {
    display.print(currentDistance, 1);
    display.println(F(" cm"));
  }

  display.setCursor(0, 34);
  display.print(F("Servo angle: "));
  display.print(currentAngle);
  display.println(F(" deg"));

  display.display();
}

void showDetectionAlert() {
  bool blinkOn = (millis() / 400) % 2 == 0;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (blinkOn) {
    display.setTextSize(2);
    display.setCursor(10, 14);
    display.println(F("DETECTED!"));
  }

  display.setTextSize(1);
  display.setCursor(0, 54);
  display.print(F("Distance: "));
  display.print(currentDistance, 1);
  display.println(F(" cm"));

  display.display();
}

void printSerialData() {
  Serial.print(F("Angle: "));
  Serial.print(currentAngle);
  Serial.print(F(" | Distance: "));

  if (currentDistance < 0) {
    Serial.println(F("No echo"));
  } else {
    Serial.print(currentDistance, 1);
    Serial.println(F(" cm"));
  }
}
