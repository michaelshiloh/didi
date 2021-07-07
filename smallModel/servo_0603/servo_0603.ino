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

//========================================

void setup() {

  Serial.begin(9600);

  // initialize PWM drivers
  pwm1.begin();
  pwm1.setPWMFreq(FREQUENCY);
  pwm2.begin();
  pwm2.setPWMFreq(FREQUENCY);

  for (int i = 0; i < 10; i++) {
    pwm1.setPWM(i, 0, 102); // starting position
    pwm2.setPWM(i, 0, 102);
  }

  Serial.println("setup finished");
  delay(5000);

}

//========================================

void loop() {

  // All at the same time
  for (int angleIndex = 0 ; angleIndex < 6; angleIndex++) {
    servomoveall(angleIndex);
    delay(stopservoangle);
  }

  // One at a time
  for (int mo = 0; mo < 20; mo++) {
    oneup(0, mo);
    delay(delaystep);
  }
  delay(stopservoangle);
  for (int mo = 0; mo < 20; mo++) {
    oneup(1, 19 - mo);
    delay(delaystep);
  }
  delay(stopservoangle);
  for (int mo = 0; mo < 20; mo++) {
    oneup(2, mo);
    delay(delaystep);
  }
  delay(stopservoangle);
  for (int mo = 0; mo < 20; mo++) {
    oneup(3, 19 - mo);
    delay(delaystep);
  }
  delay(stopservoangle);
  for (int mo = 0; mo < 20; mo++) {
    oneup(4, mo);
    delay(delaystep);
  }
  delay(stopservoangle);
  for (int mo = 0; mo < 20; mo++) {
    oneup(5, 19 - mo);
    delay(delaystep);
  }

  // Two at a time
  delay(stopservoangle);
  for (int st = 1; st < 6; st++) {
    twoservo(st);
    delay(stopservoangle);
  }
}

// Move all servo motors to the angle _after_ the one indicated by angleIndex
// I think this assumes we are already at angleIndex.
void servomoveall(int angleIndex) {
  Serial.println("servomoveall");

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

void twoservo(int angleIndex) {
  Serial.print("angleIndex: ");
  Serial.println(angleIndex);

  if (servoangle[angleIndex] < servoangle[angleIndex + 1]) { // בודק כיוון פןלס
    if (angleIndex % 2 == 1) {
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
    } else if (angleIndex % 2 == 0) {
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
    if (angleIndex % 2 == 1) {
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
    else if (angleIndex % 2 == 0) {
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
