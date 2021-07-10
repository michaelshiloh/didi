/* Controller for Didi Vardi's small model of rotating pictures
 * 
 * Based on servo_0603 by Shy Vardi
 * 
 * Change log:
 * 
 * 27 Jun 2021 - ms - between each simple sequence, rotate all the pictures at once 
 *                    so it looks more interesting. 
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
const int servoangle[] = {102, 250, 403, 535, 403, 250, 102};

const int stopservoangle = 1500; // stop at picture
const int delaystep = 750; // stop after stepp
const long interval = 350; // servo speed  Microseconds

int angleIndex = 0;

//========================================

void setup() {

  Serial.begin(9600);

  // initialize PWM drivers
  pwm1.begin();
  pwm1.setPWMFreq(FREQUENCY);
  pwm2.begin();
  pwm2.setPWMFreq(FREQUENCY);

  // Set all motors to position 0
  for (int i = 0; i < 10; i++) {
    pwm1.setPWM(i, 0, 102); // starting position
    pwm2.setPWM(i, 0, 102);
  }

  Serial.println("setup finished");
  delay(5000);

}

//========================================

/*

   initialize angle to zero (done in reset)
   After each step, increment angleIndex
   When angleIndex reaches 6, reset to 0
   
   Sequence:

  1 all at once
  2 right to left
  3 all at once
  4 outside to center
  5 all at once
  6 left to right
  7 all at once
  8 center to outside
  repeat
*/

void loop() {

  Serial.print(" index ");
  Serial.print(angleIndex);
  Serial.print(" angle ");
  Serial.println(servoangle[angleIndex]);

  allAtOnce(angleIndex);
  angleIndex++;
  angleIndex %= 6;
  delay(stopservoangle);

  Serial.println("right to left");
  rightToLeft(angleIndex);
  angleIndex++;
  angleIndex %= 6;
  delay(stopservoangle);

  allAtOnce(angleIndex);
  angleIndex++;
  angleIndex %= 6;
  delay(stopservoangle);

  twoAtOnce(angleIndex, false);
  angleIndex++;
  angleIndex %= 6;
  delay(stopservoangle);

  allAtOnce(angleIndex);
  angleIndex++;
  angleIndex %= 6;
  delay(stopservoangle);

  leftToRight(angleIndex);
  angleIndex++;
  angleIndex %= 6;
  delay(stopservoangle);

  allAtOnce(angleIndex);
  angleIndex++;
  angleIndex %= 6;
  delay(stopservoangle);

  twoAtOnce(angleIndex, true);
  angleIndex++;
  angleIndex %= 6;
  delay(stopservoangle);
}

void rightToLeft(int angleIndex) {

  // from high to low
  for (int mo = 0; mo < 20; mo++) {
    Serial.print("motor ");
    Serial.print(mo);
    Serial.print(" index ");
    Serial.println(angleIndex);
    oneup(angleIndex, 19 - mo);
    delay(delaystep);
  }
  delay(stopservoangle);
}
void leftToRight(int angleIndex) {
  // from low to how
  for (int mo = 0; mo < 20; mo++) {
    Serial.print("motor ");
    Serial.print(mo);
    Serial.print(" index ");
    Serial.println(angleIndex);
    oneup(angleIndex, mo);
    delay(delaystep);
  }
  delay(stopservoangle);
}

// Move all servo motors to the angle _after_ the one indicated by angleIndex
// I think this assumes we are already at angleIndex.
void allAtOnce(int angleIndex) {

  // If we are at an angle less than the target
  if (servoangle[angleIndex] < servoangle[angleIndex + 1]) {

    // increment the angle until we reach target
    for ( int p = servoangle[angleIndex]; p <= servoangle[angleIndex + 1]; p++) {

      // loop over all 20 motors
      for (int m = 0; m < 20; m++) {

        // motors 0-9 are on pwm1 controller
        if (m >= 0 && m <= 9) {
          pwm1.setPWM(m, 0, p);
        }

        // motors 9-19 are on pwm2 controller
        else if (m > 9) {
          pwm2.setPWM(m - 10, 0, p);
        }
      }
      delayMicroseconds(interval);
    }
  }

  // If the desired angle is less than where we are now, then decrement
  else if (servoangle[angleIndex] > servoangle[angleIndex + 1]) {
    for ( int p = servoangle[angleIndex]; p >= servoangle[angleIndex + 1]; p--) {
      for (int m = 0; m < 20; m++) {
        if (m >= 0 && m <= 9) {
          pwm1.setPWM(m, 0, p);
        }
        else if (m > 9) {
          pwm2.setPWM(m - 10, 0, p);
        }
      }
      delayMicroseconds(interval);
    }
  }
}

void oneup(int angleIndex, int m) {
  Serial.println("oneup");

  // Move to the angle indicated by servoangle[angleIndex + 1]
  // by incrementing the angle
  if (servoangle[angleIndex] < servoangle[angleIndex + 1]) {
    for ( int p = servoangle[angleIndex]; p <= servoangle[angleIndex + 1]; p++) {
      if (m >= 0 && m <= 9) {
        pwm1.setPWM(m, 0, p);
      }
      else if (m > 9) {
        pwm2.setPWM(m - 10, 0, p);
      }
      delayMicroseconds(interval / 2);
    }
  }

  else if (servoangle[angleIndex] > servoangle[angleIndex + 1]) {
    for ( int p = servoangle[angleIndex]; p >= servoangle[angleIndex + 1]; p--) {
      if (m >= 0 && m <= 9) {
        pwm1.setPWM(m, 0, p);
      }
      else if (m > 9) {
        pwm2.setPWM(m - 10, 0, p);
      }
      delayMicroseconds(interval / 2);
    }
  }
}


/* Move to servos at once, from the angle indicated by angleIndex to the next angle.
    If direction = true, move from outside in
    If direction = false, move from inside out.
*/
void twoAtOnce(int angleIndex, boolean direction) {
  Serial.print("angleIndex: ");
  Serial.println(angleIndex);

  if (servoangle[angleIndex] < servoangle[angleIndex + 1]) { // בודק כיוון פןלס
    if (direction) {
      for (int m = 0; m < 10; m++) {
        for ( int p = servoangle[angleIndex]; p <= servoangle[angleIndex + 1]; p++) {
          pwm1.setPWM(m, 0, p);
          pwm2.setPWM(9 - m, 0, p);
          delayMicroseconds(interval / 2);
        }
        Serial.print("motor: ");
        Serial.print(m);
        Serial.print(":");
        Serial.println(9 - m);
        delay (delaystep);
      }
    } else  {
      for ( int m = 9; m >= 0; m--) {

        for ( int p = servoangle[angleIndex]; p <= servoangle[angleIndex + 1]; p++) {
          pwm1.setPWM(m, 0, p);
          pwm2.setPWM(9 - m, 0, p);
          delayMicroseconds(interval / 2);
        }
        Serial.print("motor: ");
        Serial.print(m);
        Serial.print(":");
        Serial.println(9 - m);
        delay (delaystep);
      }
    }
  } else if (servoangle[angleIndex] > servoangle[angleIndex + 1]) {
    if (direction) {
      for (int m = 0; m < 10; m++) {

        for ( int p = servoangle[angleIndex]; p >= servoangle[angleIndex + 1]; p--) {
          pwm1.setPWM(m, 0, p);
          pwm2.setPWM(9 - m, 0, p);
          delayMicroseconds(interval / 2);
        }
        Serial.print("motor: ");
        Serial.print(m);
        Serial.print(":");
        Serial.println(9 - m);
        delay (delaystep);
      }
    }
    else  {
      for ( int m = 9; m >= 0; m--) {

        for ( int p = servoangle[angleIndex]; p >= servoangle[angleIndex + 1]; p--) {
          pwm1.setPWM(m, 0, p);
          pwm2.setPWM(9 - m, 0, p);
          delayMicroseconds(interval / 2);
        }
        Serial.print("motor: ");
        Serial.print(m);
        Serial.print(":");
        Serial.println(9 - m);
        delay (delaystep);
      }
    }
  }
}
