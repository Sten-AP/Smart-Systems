#include <LiquidCrystal_I2C.h> // https://github.com/johnrickman/LiquidCrystal_I2C (install as zip)
#include <WiFi.h>              // Standard ESP32 Library
#include <PubSubClient.h>      // https://github.com/knolleary/pubsubclient (intall with library Manager)

// PWM Settings - DO NOT CHANGE
const int PWM_FREQUENCY = 500;
const int PWM_RESOLUTION = 8;
const  int PWM_MAX_DC = 255;

// Motor PWM Pin declarations - DO NOT CHANGE
const int PWM_CHANNEL_COUNT = 4;
const int MOTOR_COUNT = 2;

int MOTOR_PINS[PWM_CHANNEL_COUNT] = {18, 5, 2, 15}; // Motor 1: 18, 5 -- Motor 2: 2, 15
int MOTOR_CHANNELS[PWM_CHANNEL_COUNT] = {0, 1, 2, 3};

// LED PWM Pin declarations - DO NOT CHANGE
const int LED_CHANNEL_COUNT = 4;
const int LED_COUNT = 4;

int LED_PINS[LED_COUNT] = {13, 12, 14, 27};
int LED_CHANNELS[LED_CHANNEL_COUNT] = {4, 5, 6, 7};
int LED_STATE[] = {0, 0, 0, 0};

// Linetracker
const int linetrackerLed[3] = {A4, A7, A6};

// Phototransitor Pin Declaration - DO NOT CHANGE 
const int PL_PIN = 25;
const int TRIG_PIN = 19;
const int ECHO_PIN = 23;

// I2C LCD Initialisation 
int LCD_COLUMNS = 16;
int LCD_ROWS = 2;
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);

enum COMMANDS {
  LEFT_TURN_FORWARD,      // 0
  FORWARD,                // 1
  RIGHT_TURN_FORWARD,     // ... 
  RIGHT,
  RIGHT_TURN_BACKWARDS,
  BACKWARDS,
  LEFT_TURN_BACKWARDS,
  LEFT,
  STOP,
  BARRIER_ONE_UP,
  BARRIER_ONE_DOWN,
  PRIOR_VEHICLE,
  BARRIER_TWO_UP,
  BARRIER_TWO_DOWN,     //13
};

enum MOTOR_DIRECTION {
  MOTOR_BACKWARDS = -1,
  MOTOR_STOP = 0,
  MOTOR_FORWARD = 1
};

int command, lastCommand;

int drivingSpeed = 128; // default 128

void setup() {
  Serial.begin(115200);
  // init of the linetrackerLeds
  for (int i = 0; i < sizeof(linetrackerLed); i++) {
    pinMode(linetrackerLed[i], INPUT);
  }
  // init of the PWM Pins 
  for (int i = 0; i < PWM_CHANNEL_COUNT; i++) {
    ledcSetup(MOTOR_CHANNELS[i], PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(MOTOR_PINS[i], MOTOR_CHANNELS[i]);
  }
  //init of the LED Pins 
  for (int i = 0; i < LED_COUNT; i++) {
    ledcSetup(LED_CHANNELS[i], PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(LED_PINS[i], LED_CHANNELS[i]);
    ledcWrite(LED_CHANNELS[i], 128);
  }
  // init ultrasonic distance sensor
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT); 

  // lcd init 
  lcd.init();
  lcd.backlight();
}

void loop() {
  int ledLinks = analogRead(linetrackerLed[0]);
  int ledMidden = analogRead(linetrackerLed[1]);
  int ledRechts = analogRead(linetrackerLed[2]);

  Serial.println(String(ledLinks) + " - " + String(ledMidden) + " - " + String(ledRechts));

  if (ledLinks == 4095 && ledMidden == 4095 && ledRechts == 4095) command = LEFT;
  else if (ledMidden == 4095) command = FORWARD;
  else if (ledLinks == 4095) command = RIGHT;
  else if (ledRechts == 4095) command = LEFT;
  else if (ledMidden == 4095 && ledRechts == 4095) command = RIGHT;
  else if (ledMidden == 4095 && ledLinks == 4095) command = LEFT;

  if (lastCommand != command) {
    switch (command) {
      case LEFT_TURN_FORWARD:  // Curved turn forward to the left
        driveMotors(MOTOR_FORWARD, drivingSpeed, MOTOR_FORWARD, drivingSpeed/4);
        break;
      case FORWARD: // Drive forward
        driveMotors(MOTOR_FORWARD, drivingSpeed, MOTOR_FORWARD, drivingSpeed);
        break;
      case RIGHT_TURN_FORWARD: // Curved turn forward to the right
        driveMotors(MOTOR_FORWARD, drivingSpeed/4, MOTOR_FORWARD, drivingSpeed);
        delay(2000);
        break;
      case RIGHT: // Turn right around center point chassis
        driveMotors(MOTOR_BACKWARDS, drivingSpeed, MOTOR_FORWARD, drivingSpeed);
        break;
      case RIGHT_TURN_BACKWARDS: // Curved turn backwards to the right
        driveMotors(MOTOR_BACKWARDS, drivingSpeed/2, MOTOR_BACKWARDS, drivingSpeed);
        break;
      case BACKWARDS: // Drive backwards
        driveMotors(MOTOR_BACKWARDS, drivingSpeed, MOTOR_BACKWARDS, drivingSpeed);
        break;
      case LEFT_TURN_BACKWARDS: // Curved turn backwards to the right 
        driveMotors(MOTOR_BACKWARDS, drivingSpeed, MOTOR_BACKWARDS, drivingSpeed/2);
        break;
      case LEFT: // Turn left around center point chassis
        driveMotors(MOTOR_FORWARD, drivingSpeed, MOTOR_BACKWARDS, drivingSpeed);
        break;
      case STOP: // Stop driving
        driveMotors(MOTOR_STOP, 0, MOTOR_STOP, 0);
        break;
      case BARRIER_ONE_UP: //Raise barrier 1 up
        barrierControl(0, true);
        break;
      case BARRIER_ONE_DOWN: //Lower barrier 1 down
        barrierControl(0, false);
        break;
      case PRIOR_VEHICLE: //Start the priority vehicle protocol 
        priorVehicle();
        break;
      case BARRIER_TWO_UP: //Raise barrier 2 up
        barrierControl(1, true);
        break;
      case BARRIER_TWO_DOWN: //Lower barrier 2 down
        barrierControl(1, false);
        break;
      default: //Set the driving speed 
        drivingSpeed = command;
        break;
    }
  }
  lastCommand = command; //Another question on the assignment, why is this statement necessary 
}

void driveMotors(int leftMotorDirection, int leftMotorSpeed, int rightMotorDirection, int rightMotorSpeed) {
  switch (leftMotorDirection) {
    case MOTOR_FORWARD:
      ledcWrite(MOTOR_CHANNELS[1], leftMotorSpeed);
      ledcWrite(MOTOR_CHANNELS[0], 0);
      break;
    case MOTOR_BACKWARDS:
      ledcWrite(MOTOR_CHANNELS[1], 0);
      ledcWrite(MOTOR_CHANNELS[0], leftMotorSpeed);
      break;
    default:
      ledcWrite(MOTOR_CHANNELS[1], 0);
      ledcWrite(MOTOR_CHANNELS[0], 0);
      break;
  }

  switch (rightMotorDirection) {
    case MOTOR_FORWARD:
      ledcWrite(MOTOR_CHANNELS[2], rightMotorSpeed);
      ledcWrite(MOTOR_CHANNELS[3], 0);
      break;
    case MOTOR_BACKWARDS:
      ledcWrite(MOTOR_CHANNELS[2], 0);
      ledcWrite(MOTOR_CHANNELS[3], rightMotorSpeed);
      break;
    default:
      ledcWrite(MOTOR_CHANNELS[2], 0);
      ledcWrite(MOTOR_CHANNELS[3], 0);
      break;
  }
}

void barrierControl(int barrierID, bool setBarrier) {
  digitalWrite(barrierID, setBarrier);
}

void priorVehicle() {

}