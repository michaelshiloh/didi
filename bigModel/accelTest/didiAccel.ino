/*
 * stepper motor, accel driver (thanks!), and proximity sensor
 * 
 * white wire from drvir - pin 2
 * yellow sensor - 3
 * whit sensor vin
 * black sensor - gnd
 */


/*
0
1
2 - M1 En
3 - M1 Dir
4 - M1 Step
5 - M2 En
6 - M2 Dir
7 - M2 Step
8 - M3
9 - M3
10 - M3
11 - M4
12 - M4
13
A0 - M4
A1
A2
A3
A4 - I2C
A5 - I2C
*/

#include <AccelStepper.h>

// Define a stepper and the pins it will use
// AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepper(
  1, //  Motor interface type must be set to 1 when using a driver
  2, //  step pin
  8); //  direction pin

int fullTurnStepCount;
const int maxSpeed = 3000;


/*
    State machine
    state = 0 = setup
    state = 1 moving from 0 to 180
    state = 2 pause at 180
    state = 3 moving from 180 back to 0
    state = 4 pause at 0
*/
int state = 0;
unsigned long pauseTime = 3000;
unsigned long startedTimerAt;

void setup()
{
  Serial.begin(9600);



  Serial.println("counting steps to home");
  countStepsToHome();
  Serial.println("seeking home");
  goHome();

  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(maxSpeed);
  stepper.setAcceleration(1500);

  Serial.println("setup finished");
  //while (1);
}

void loop()
{

  // start moving
  switch (state) {
    case 0: // state = 0: setup
      stepper.moveTo(400); // nonblocking; sets target for run()
      Serial.println("current state 0, moving to state 1");
      state = 1;
      break;

    // moving or starting timer
    case 1: // state = 1: moving from 0 to 90
      if (0 == stepper.distanceToGo()) {
        startedTimerAt = millis();
        Serial.println("current state 1, moving to state 2");
        state = 2;
      } else {
        stepper.run();
      }
      break;

    // pausing or start moving
    case 2: // state = 2: pause at 90
      if (millis() - startedTimerAt > pauseTime) {
        stepper.moveTo(800);
        Serial.println("current state 2, moving to state 3");
        state = 3;
      }
      break;

    // moving or starting timer
    case 3: // state = 3: moving from 90 to 180
      if (0 == stepper.distanceToGo()) {
        startedTimerAt = millis();
        Serial.println("current state 3, moving to state 4");
        state = 4;
      } else {
        stepper.run();
      }
      break;

    // pausing at 180 or start moving to 270
    case 4:
      if (millis() - startedTimerAt > pauseTime) {
        stepper.moveTo(1200);
        Serial.println("current state 4, moving to state 5");
        state = 5;
      }
      break;

    // moving to 270 or starting timer
    case 5:
      if (0 == stepper.distanceToGo()) {
        // Destination reached, set timer
        startedTimerAt = millis();
        Serial.println("current state 5, moving to state 6");
        state = 6;
      } else {
        stepper.run();
      }
      break;

    // pausing at 270 or start moving to 0
    case 6:
      if (millis() - startedTimerAt > pauseTime) {
        // timer expired, move to next position
        stepper.moveTo(0);
        Serial.println("current state 6, moving to state 7");
        state = 0;
      } else { // otherwise, pause here
        Serial.println("pausing at 270");
      }
      break;
/*360
    // moving or starting timer
    case 7:
      if (0 == stepper.distanceToGo()) {
        startedTimerAt = millis();
        Serial.println("current state 7, moving to state 8");
        state = 8;
      } else {
        stepper.run();
      }
      break;

    // pausing or start moving
    case 8:
      if (millis() - startedTimerAt > pauseTime) {
        // timer expired, move to next position
        stepper.moveTo(0);
        Serial.println("current state 8, moving to state 0");
        state = 0;
      }
      break;
*/

    default:
      Serial.println("invalid state");
      break;
  }

  stepper.run(); // If we are at our destination, this will do nothing
}
