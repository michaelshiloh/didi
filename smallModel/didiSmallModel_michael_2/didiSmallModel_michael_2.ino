/* Controller for Didi Vardi's small model of rotating pictures

   Based on servo_0603 by Shy Vardi

   Change log:

   27 Jun 2021 - ms - between each simple sequence, rotate all the pictures at once
                      so it looks more interesting.
   01 Jul 2021 - ms - Wrote a class for the servo motor which handles updating the
                      motor pulse width without using delay()
   05 Jul 2021 - ms - each motor starts shortly after the previous motor so that they overlap
   06 Jul 2021 - ms - implement the full sequence
   06 Jul 2021 - ms - add a digital input to trigger action
   07 Jul 2021 - ms - run for at least 3 minutes when triggered
*/


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PULSE_WIDTH 500
#define MAX_PULSE_WIDTH 2400
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

// multiples of 90 degrees, in pulse widths
const int servoangle[] = {102, 250, 403, 535, 403, 250};

// Timing stuff
const int PICTURE_PAUSE_mSEC = 1500; // Millisecond pause at picture
const long PULSE_MOD_PAUSE_uSEC = 350; // Microseconds to pause as pulse is being modified
const int OVERLAP_PAUSE_mSEC = 200; // Millisecond overlap pause
const long MAXRUNTIME_MINUTES = 3; // Run for only 3 minutes
unsigned long runningStartedAt = 0; // When did we start running?

// Pulse width modification
const int pulseWidthIncrement = 1; // How much to add or subtract each time

int angleIndex;

// Switches
const int SWITCH1PIN = 2;
const int SWITCH2PIN = 4;


/*
   Class definition for the rototor. Each rotator will
   have its own private set of member variables so there is no naming conflict
   and all rotators share the same behaviors behaviors.
*/
class Rotator
{
    int driverNumber;
    int motorNumber;
    int pulseWidthIncrement; // How much to add each time
    int PULSE_MOD_PAUSE_uSEC; // How long to wait at each step
    int currentPulseWidth;

    int targetPulseWidth;
    unsigned long lastPulseWidthUpdate; // last update of pulseWidth

  public:
    // Nothing to do in the constructor
    Rotator() {}

    void init(int _driverNumber,
              int _motorNumber,
              int _pulseWidthIncrement,
              int _PULSE_MOD_PAUSE_uSEC,
              int _currentPulseWidth) {

      // Store all arguments in private variables
      driverNumber =  _driverNumber;
      motorNumber = _motorNumber;
      pulseWidthIncrement = _pulseWidthIncrement;
      PULSE_MOD_PAUSE_uSEC  = _PULSE_MOD_PAUSE_uSEC;
      currentPulseWidth = _currentPulseWidth;

      // Clear remaining variables
      targetPulseWidth = 0;
      lastPulseWidthUpdate = 0;

      // Set each motors to its initial position
      //      if (driverNumber == 1) {
      //        pwm1.setPWM(motorNumber, 0, currentPulseWidth);
      //      }  else  {
      //        pwm2.setPWM(motorNumber - 10, 0, currentPulseWidth);
      //      }
    } // end of Rotator initialization function

    void setTargetPulseWidth(int _targetPulseWidth)
    {
      targetPulseWidth = _targetPulseWidth;
    }

    boolean isAtTarget() {
      return (abs(targetPulseWidth - currentPulseWidth) < pulseWidthIncrement);
    }

    // The all-important update function does all the hard work of
    // checking to see whether it is time to change the pulse width
    // There is no harm in calling this more frequently than necessary
    // It must be called in all blocking functions or loops

    void update()
    {

      // Are we as close as we can get?
      if (abs(targetPulseWidth - currentPulseWidth) < pulseWidthIncrement ) {
        // Serial.println("close enough");
        return;
      }

      unsigned long now = micros();
      unsigned long elapsedTime;

      // Has the micros() counter overflowed? It's an unsigned long,
      // which can go up to 2,147,483,647 *2.

      // 2,147*2 seconds/60seconds minutes =
      // 2,147*2 /60 minutes = 143 minutes

      // Using a second unsigned long would give me
      // 2,147,483,647 *2,147*2 seconds * 1 minute/60seconds * 1 hour/60minutes * 1 day/24 hours * 1 year/365 days =
      // 2,147,483,647 * 2,147*2 /(60 *60 *24 * 365) years = 292405 years!

      // But perhaps that's not necessary. If we've overflowed, we just need
      // to calculate the elapsed time differently. Is this true?

      if (now < lastPulseWidthUpdate) { // overflowed
        Serial.println("OVERFLOW OVERFLOW OVERFLOW OVERFLOW OVERFLOW OVERFLOW OVERFLOW ");
        elapsedTime = (2147483647 - lastPulseWidthUpdate) + now;
      } else {
        elapsedTime = now - lastPulseWidthUpdate;
      }

      if ( elapsedTime > PULSE_MOD_PAUSE_uSEC) // time to update
      {
        lastPulseWidthUpdate = now;
        // Serial.println("time to update ");

        // Are we going up or down?
        if (targetPulseWidth > currentPulseWidth) {
          currentPulseWidth += pulseWidthIncrement;
        } else {
          currentPulseWidth -= pulseWidthIncrement;
        }

        // finally write to the motor
        if (driverNumber == 1) {
          pwm1.setPWM(motorNumber, 0, currentPulseWidth);
        }  else  {
          pwm2.setPWM(motorNumber - 10, 0, currentPulseWidth);
        }
      }
    } // end of Rotator update function
}; // end of Rotator class


Rotator rotators[20];

//========================================

void setup() {

  Serial.begin(9600);

  angleIndex = 0;

  // initialize PWM drivers
  pwm1.begin();
  pwm1.setPWMFreq(FREQUENCY);
  pwm2.begin();
  pwm2.setPWMFreq(FREQUENCY);

  // Initialize the rotator objects
  for (int i = 0; i < 10; i++) {

    // Motors 0-9 are on driver 1
    rotators[i].init(1, //  driverNumber
                     i, //  motorNumber
                     pulseWidthIncrement, // How much to add each time
                     PULSE_MOD_PAUSE_uSEC, // wait after each pulse width change
                     servoangle[angleIndex]); // each object needs to know where it is now

    // Motors 10-19 are on driver 2
    rotators[i + 10].init(2, //  driverNumber
                          i + 10, //  motorNumber
                          pulseWidthIncrement, // How much to add each time
                          PULSE_MOD_PAUSE_uSEC, // wait after each pulse width change
                          servoangle[angleIndex]); //  each object needs to know where it is now
  }
  for (int i = 0; i < 10; i++) {
    pwm1.setPWM(i, 0, servoangle[angleIndex]); // starting position
    pwm2.setPWM(i, 0, servoangle[angleIndex]);
  }

  // Now that we're at index 0, we need to bump it up to next position
  angleIndex++;

  Serial.println("setup finished");
  delay(3000);

}

//========================================
/*
   initialize angle to zero (done in reset)
   After each step, increment angleIndex
   When angleIndex reaches 6, reset to 0

   Sequence:

  all at once overlapping, right to left
  all at once overlapping, outside to center
  all at once overlapping, left to right
  all at once overlapping, center to outside
  repeat

  Stop after 3 minutes, and wait for a trigger (button press)
*/
void loop() {



  Serial.print("right to left, index = ");
  Serial.print(angleIndex);
  Serial.println();
  rightToLeft(angleIndex, OVERLAP_PAUSE_mSEC);
  angleIndex++;
  angleIndex %= 6;
  delay(PICTURE_PAUSE_mSEC);

  blockIfMAXRUNTIME_MINUTESExpired();

  Serial.print("from inside to outside, index = ");
  Serial.print(angleIndex);
  Serial.println();
  insideOut(angleIndex, OVERLAP_PAUSE_mSEC);
  angleIndex++;
  angleIndex %= 6;
  delay(PICTURE_PAUSE_mSEC);

  blockIfMAXRUNTIME_MINUTESExpired();

  Serial.print("left to right, index = ");
  Serial.print(angleIndex);
  Serial.println();
  leftToRight(angleIndex, OVERLAP_PAUSE_mSEC);
  angleIndex++;
  angleIndex %= 6;
  delay(PICTURE_PAUSE_mSEC);

  blockIfMAXRUNTIME_MINUTESExpired();

  Serial.print("from inside to outside, index = ");
  Serial.print(angleIndex);
  Serial.println();
  outsideIn(angleIndex, OVERLAP_PAUSE_mSEC);
  angleIndex++;
  angleIndex %= 6;
  delay(PICTURE_PAUSE_mSEC);

  blockIfMAXRUNTIME_MINUTESExpired();
}


// All at once, but each one starts a little after the previous
// OVERLAP_PAUSE_mSEC is the time between when one motor
// starts and when the next starts
void rightToLeft(int index, int OVERLAP_PAUSE_mSEC) {

  // Start the first motor
  rotators[0].setTargetPulseWidth(servoangle[angleIndex]);
  unsigned long lastMotorStartedAt = millis();

  int i = 1;
  while ( i < 20 ) {
    if ( (millis() - lastMotorStartedAt) > OVERLAP_PAUSE_mSEC ) {
      // time to start the next motor
      //      Serial.print("starting motor ");
      //      Serial.print(i);
      //      Serial.print(" at time ");
      //      Serial.print(millis());
      //      Serial.print(" lastMotorStartedAt ");
      //      Serial.print(lastMotorStartedAt);
      //      Serial.print(" OVERLAP_PAUSE_mSEC ");
      //      Serial.print(OVERLAP_PAUSE_mSEC);
      //      Serial.println();
      rotators[i].setTargetPulseWidth(servoangle[angleIndex]);
      lastMotorStartedAt = millis();
      i++;
    }

    // Update all the motors. Have to do this within the while() loop
    // because the while() loop is blocking
    for (int i = 0; i < 20; i++) {
      rotators[i].update();
    }
  }

  // all motors have been started, now just wait for them all to arrive.
  // This function blocks, but it calls the Rotator.update() function
  // so it's OK to block
  waitForAllMotorsToArrive(); // blocking
}

void leftToRight(int index, int OVERLAP_PAUSE_mSEC) {

  // Start the last motor
  rotators[19].setTargetPulseWidth(servoangle[angleIndex]);
  unsigned long lastMotorStartedAt = millis();

  int i = 18;
  while ( i >= 0 ) {
    if ( (millis() - lastMotorStartedAt) > OVERLAP_PAUSE_mSEC ) {
      rotators[i].setTargetPulseWidth(servoangle[angleIndex]);
      lastMotorStartedAt = millis();
      i--;
    }

    // Update all the motors. Have to do this within the while() loop
    // because the while() loop will block until all motors have started
    for (int i = 0; i < 20; i++) {
      rotators[i].update();
    }
  }

  // all motors have been started, now just wait for them all to arrive.
  // This function blocks, but it calls the Rotator.update() function
  // so it's OK to block
  waitForAllMotorsToArrive(); // blocking
}

void insideOut(int index, int OVERLAP_PAUSE_mSEC) {

  // Start the first two
  rotators[9].setTargetPulseWidth(servoangle[angleIndex]);
  rotators[10].setTargetPulseWidth(servoangle[angleIndex]);
  unsigned long lastMotorStartedAt = millis();

  int i = 8;
  while ( i >= 0 ) {
    if ( (millis() - lastMotorStartedAt) > OVERLAP_PAUSE_mSEC ) {
      //      Serial.print("doing motors ");
      //      Serial.print(i);
      //      Serial.print(" and ");
      //      Serial.print(19 - i);
      //      Serial.println();
      rotators[i].setTargetPulseWidth(servoangle[angleIndex]);
      rotators[19 - i].setTargetPulseWidth(servoangle[angleIndex]);
      lastMotorStartedAt = millis();
      i--;
    }

    // Update all the motors. Have to do this within the while() loop
    // because the while() loop will block until all motors have started
    for (int i = 0; i < 20; i++) {
      rotators[i].update();
    }
  }

  // all motors have been started, now just wait for them all to arrive.
  // This function blocks, but it calls the Rotator.update() function
  // so it's OK to block
  waitForAllMotorsToArrive(); // blocking
}

void outsideIn(int index, int OVERLAP_PAUSE_mSEC) {

  // Start the first two
  rotators[0].setTargetPulseWidth(servoangle[angleIndex]);
  rotators[19].setTargetPulseWidth(servoangle[angleIndex]);
  unsigned long lastMotorStartedAt = millis();

  int i = 1;
  while ( i < 10 ) {
    if ( (millis() - lastMotorStartedAt) > OVERLAP_PAUSE_mSEC ) {
      //      Serial.print("doing motors ");
      //      Serial.print(i);
      //      Serial.print(" and ");
      //      Serial.print(19 - i);
      //      Serial.println();
      rotators[i].setTargetPulseWidth(servoangle[angleIndex]);
      rotators[19 - i].setTargetPulseWidth(servoangle[angleIndex]);
      lastMotorStartedAt = millis();
      i++;
    }

    // Update all the motors. Have to do this within the while() loop
    // because the while() loop will block until all motors have started
    for (int i = 0; i < 20; i++) {
      rotators[i].update();
    }
  }

  // all motors have been started, now just wait for them all to arrive.
  // This function blocks, but it calls the Rotator.update() function
  // so it's OK to block
  waitForAllMotorsToArrive(); // blocking
}

// IMPORTANT note that this is a blocking function
// I.e. it does not return until all motors have arrived. This
// means that this function must call the Rotator.update() function
void waitForAllMotorsToArrive() {
  while (1) {

    // Update all the motors
    for (int i = 0; i < 20; i++) {
      rotators[i].update();
    }

    // keep updating as long as any servos are not at target

    // Assume all servos have reached their target
    boolean allDone = true;

    // Check each servo
    for (int i = 0; i < 20; i++) {
      // If any servo is not at target
      if (rotators[i].isAtTarget() == false) {
        allDone = false;
        break; // No need to check the rest; go back to updating
      }
    }

    // Leave the loop only if all are done
    if (allDone == true) {
      break;
    }
  }
}

/////////////////////////////////////////////////////////////

void simpleTest() {
  Serial.println("moving to 535");
  rotators[1].setTargetPulseWidth(535);
  while (rotators[1].isAtTarget() == false) {
    rotators[1].update();
  }
  delay(3000);

  Serial.println("moving to 250");
  rotators[1].setTargetPulseWidth(250);
  while (rotators[1].isAtTarget() == false) {
    rotators[1].update();
  }
  delay(3000);
}

// All at once, no overlap. This was used to test the class before
// I implemented the overlapping feature
void allAtOnceNoOverlap(int index) {
  Serial.print("setting all servos to angleIndex ");
  Serial.println(index);

  for (int i = 0; i < 20; i++) {
    rotators[i].setTargetPulseWidth(servoangle[angleIndex]);
  }

  waitForAllMotorsToArrive(); // blocking
}

/* if we've been running for longer than the run time,
    enter an endless loop until we are triggered by
    one of the two switches being closed
*/

void blockIfMAXRUNTIME_MINUTESExpired() {

  float minutesRun = (millis() - runningStartedAt) / 60000.0;

  Serial.print("Have been running for this many minutes: ");
  Serial.println(minutesRun);

  if (minutesRun > MAXRUNTIME_MINUTES) {

    Serial.println("going to sleep until someone presses a switch");
    // wait for a switch closure
    while (!digitalRead(SWITCH1PIN) && !digitalRead(SWITCH2PIN)) {
      Serial.println("waiting");
      delay(100);
    }

    // someone pressed a switch! Note the time we started
    Serial.println("Someone woke us up");
    runningStartedAt = millis();
  }
}
