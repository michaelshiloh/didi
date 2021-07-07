/*
   stepper motor, accel driver (thanks!), and proximity sensor

   Driver
   white - step
   green - dir
   red - enable

   Sensor
   yellow sensor - 3
   whit sensor vin
   black sensor - gnd
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
  A1 - S1
  A2 - S2
  A3 - S3
  A4 - I2C
  A5 - I2C
*/

#include <AccelStepper.h>

AccelStepper m1(
  1, //  Motor interface type must be set to 1 when using a driver
  4, //  step pin
  3); //  direction pin

AccelStepper m2(
  1, //  Motor interface type must be set to 1 when using a driver
  7, //  step pin
  6); //  direction pin

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
unsigned long position = 0;

void setup()
{
  Serial.begin(9600);

  Serial.println("counting steps to home");
  countStepsToHome();
  Serial.println("seeking home");
  goHome();

  Serial.println("Setting running speeds");
  m1.setMaxSpeed(maxSpeed);
  m1.setAcceleration(1500);
  m2.setMaxSpeed(maxSpeed);
  m2.setAcceleration(1500);

  Serial.println("setup finished");
  //while (1);

}

void loop()
{
  stateMachine();
  //allTogether();
  m1.run(); // If we are at our destination, this will do nothing
  m2.run(); // If we are at our destination, this will do nothing

}

void allTogether() {
    if (0 == m1.distanceToGo() && 0 == m2.distanceToGo() && (millis() - startedTimerAt > pauseTime)) {

      position += 400;
      m1.moveTo(position); // nonblocking; sets target for run()
      m2.moveTo(position); // nonblocking; sets target for run()
      startedTimerAt = millis();

    }
  }

void stateMachine() {


  // start moving
  switch (state) {
    case 0: // state = 0: setup
      m1.moveTo(400); // nonblocking; sets target for run()
      m2.moveTo(400); // nonblocking; sets target for run()
      Serial.println("destination: 90 degrees");
      state = 1;
      break;

    // moving or starting timer
    case 1: // state = 1: moving from 0 to 90
      if (0 == m1.distanceToGo()) {
        startedTimerAt = millis();
        Serial.println("current state 1, moving to state 2");
        state = 2;
      } else {
        m1.run();
        m2.run();
      }
      break;

    // pausing or start moving
    case 2: // state = 2: pause at 90
      if (millis() - startedTimerAt > pauseTime) {
        m1.moveTo(800);
        m2.moveTo(800);
        Serial.println("destination: 180 degrees");
        state = 3;
      }
      break;

    // moving or starting timer
    case 3: // state = 3: moving from 90 to 180
      if (0 == m1.distanceToGo()) {
        startedTimerAt = millis();
        Serial.println("current state 3, moving to state 4");
        state = 4;
      } else {
        m1.run();
        m2.run();
      }
      break;

    // pausing at 180 or start moving to 270
    case 4:
      if (millis() - startedTimerAt > pauseTime) {
        m1.moveTo(1200);
        m2.moveTo(1200);
        Serial.println("destination: 270 degrees");
        state = 5;
      }
      break;

    // moving to 270 or starting timer
    case 5:
      if (0 == m1.distanceToGo()) {
        // Destination reached, set timer
        startedTimerAt = millis();
        Serial.println("current state 5, moving to state 6");
        state = 6;
      } else {
        m1.run();
        m2.run();
      }
      break;

    // pausing at 270 or start moving to 0
    case 6:
      if (millis() - startedTimerAt > pauseTime) {
        // timer expired, move to next position
        m1.moveTo(0);
        m2.moveTo(0);
        Serial.println("destination: 0 degrees");
        state = 0;
      } else { // otherwise, pause here
        //Serial.println("pausing at 270");
      }
      break;
    /*360
        // moving or starting timer        Serial.println("current state 6, moving to state 7");

        case 7:
          if (0 == m1.distanceToGo()) {
            startedTimerAt = millis();
            Serial.println("current state 7, moving to state 8");
            state = 8;
          } else {
            m1.run();
          }
          break;

        // pausing or start moving
        case 8:
          if (millis() - startedTimerAt > pauseTime) {
            // timer expired, move to next position
            m1.moveTo(0);
            Serial.println("current state 8, moving to state 0");
            state = 0;
          }
          break;
    */

    default:
      Serial.println("invalid state");
      break;
  }

  
}
