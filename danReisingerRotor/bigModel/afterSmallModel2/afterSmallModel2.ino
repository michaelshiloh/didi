/*
  Controller for Didi Vardi's big model of rotating pictures

  Based on twoMotors and didiSmallModel_michael_2 by Michael Shiloh
  which in turn is based on servo_0603 by Shy Vardi

  Uses accelStepper (http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html)
  Requires non-blocking functinos so that multiple motors can be moved

  Goal: Move 4 motors, each starting a little after the other (similar to
  rightToLeft(angleIndex, OVERLAP_PAUSE_mSEC))


   Change log:

   12 Sep 2021 - ms - initial entry


   Driver
   white - step
   green - dir
   red - enable

   Sensor
   yellow sensor - 3
   whit sensor vin
   black sensor - gnd
*/


/* Proposed pin assignment:

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
  13 - M4
  A0 - S1
  A1 - S2
  A2 - S3
  A3 - S4
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

AccelStepper m3(
  1, //  Motor interface type must be set to 1 when using a driver
  10, //  step pin
  9); //  direction pin

AccelStepper m4(
  1, //  Motor interface type must be set to 1 when using a driver
  13, //  step pin
  12); //  direction pin

// Is this still useful?
// What if each motor has a different count?
int fullTurnStepCount;

const int maxSpeed = 3000;

// Timing stuff
const int PICTURE_PAUSE_mSEC = 1500; // Millisecond pause at picture
const long PULSE_MOD_PAUSE_uSEC = 350; // Microseconds to pause as pulse is being modified
const int OVERLAP_PAUSE_mSEC = 200; // Millisecond overlap pause
const long MAXRUNTIME_MINUTES = 3; // Run for only 3 minutes
unsigned long runningStartedAt = 0; // When did we start running?

/*
  Setup:
   1. Initialize things
   2. Zero each rotator (i.e. seek sensor)
   3. Set the current step position to zero
*/
void setup()
{
  Serial.begin(9600);

  Serial.println("counting steps to home");
  countStepsToHome();
  Serial.println("seeking home");
  goHome();

  m1.setCurrentPosition(0);
  m2.setCurrentPosition(0);

  Serial.println("Setting running speeds");
  m1.setMaxSpeed(maxSpeed);
  m1.setAcceleration(1500);
  m2.setMaxSpeed(maxSpeed);
  m2.setAcceleration(1500);

  Serial.println("setup finished");
  //while (1);

}

/*
   Loop:
   1. rightToLeft(90 degrees, OVERLAP_PAUSE_mSEC) ( non-blocking)
   2. wait for everyone to arrive
   3. Set the current step position to zero
   */
void loop() {

  Serial.print("right to left, destination = 400");
  Serial.println();
  rightToLeft(OVERLAP_PAUSE_mSEC);
  // wait for all to arrive
  while (m1.distanceToGo() && m2.distanceToGo()) {
    m1.run();
    m2.run();
  }
  delay(PICTURE_PAUSE_mSEC);
  m1.setCurrentPosition(0);
  m2.setCurrentPosition(0);

  // blockIfMAXRUNTIME_MINUTESExpired();
}

// Right to left, each one starts a little after the previous
// OVERLAP_PAUSE_mSEC is the time between when one motor
// starts and when the next starts
void rightToLeft(int OVERLAP_PAUSE_mSEC) {

  m1.moveTo(400);
  unsigned long lastMotorStartedAt = millis();
  while ( (millis() - lastMotorStartedAt) < OVERLAP_PAUSE_mSEC ) {
    m1.run();
  }
  // time to start the next motor
  m2.moveTo(400);
  lastMotorStartedAt = millis();
  while ( (millis() - lastMotorStartedAt) < OVERLAP_PAUSE_mSEC ) {
    m1.run();
    m2.run();
  }
}
