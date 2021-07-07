/*
   Control servo motors in Didi's small model
   Based on servo_0503 by Shy Vardi

   Modification log
   26 Jun 2021 - Michael Shiloh - divide into functions
*/


// ----------LIBRARIES--------------

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Constants suggested by Adafruit's examples for the servo drivers
#define MIN_PULSE_WIDTH 500
#define MAX_PULSE_WIDTH 2400
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50

// Objects for each servo driver
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

// Our own constants
const long delayBetweenSteps = 15;
const int servoAngles[] = {102, 232, 361, 491, 361, 232, 102};
const int delayBetweenSequences = 1000;
const int delayBetweenMotors = 50;


//========================================

void setup() {

  Serial.begin(9600);
  pwm1.begin();
  pwm1.setPWMFreq(FREQUENCY);
  pwm2.begin();
  pwm2.setPWMFreq(FREQUENCY);

  // Move all 16 servos to position 120
  for (int i = 0; i < 16; i++) {
    pwm1.setPWM(i, 0, 102);

  }
  Serial.println("Arrived at start position");
  delay(1000);
}

//========================================

void loop() {

  delay(delayBetweenSequences);
  Serial.println("sequence 0");
  for (int mo = 0; mo < 10; mo++) {
    twoservo(0, mo);
    delay(delayBetweenMotors);
  }
  delay(delayBetweenSequences);
  Serial.println("sequence 1");
  for (int mo = 10; mo >= 0; mo--) {
    twoservo(1, mo);
    delay(delayBetweenMotors);
  }
  delay(delayBetweenSequences);
  Serial.println("sequence 2");
  for (int mo = 0; mo < 10; mo++) {
    twoservo(2, mo);
    delay(delayBetweenMotors);
  }
  delay(delayBetweenSequences);
  Serial.println("sequence 3");
  for (int mo = 10; mo >= 0; mo--) {
    twoservo(3, mo);
    delay(delayBetweenMotors);
  }
  delay(delayBetweenSequences);
  Serial.println("sequence 4");
  for (int mo = 0; mo < 10; mo++) {
    twoservo(4, mo);
    delay(delayBetweenMotors);
  }
  delay(delayBetweenSequences);
  Serial.println("sequence 5");
  for (int mo = 10; mo >= 0; mo--) {
    twoservo(5, mo);
    delay(delayBetweenMotors);
  }
}

void twoservo(int mystep, int m) {
 // Serial.println(mystep);
  if (servoAngles[mystep] < servoAngles[mystep + 1]) {

    for ( int p = servoAngles[mystep]; p <= servoAngles[mystep + 1]; p++) {
//      Serial.print ("start:");
//      Serial.print(servoAngles[mystep]);
//      Serial.print (" stop:");
//      Serial.print(servoAngles[mystep + 1]);
//      Serial.print (" pulse:");
//      Serial.println(p);

      pwm1.setPWM(m, 0, p);
      pwm2.setPWM(9 - m, 0, p);

      delay(delayBetweenSteps / 2);
    }
  }

  else if (servoAngles[mystep] > servoAngles[mystep + 1]) {
    for ( int p = servoAngles[mystep]; p >= servoAngles[mystep + 1]; p--) {
//      Serial.print ("start:");
//      Serial.print(servoAngles[mystep]);
//      Serial.print (" stop:");
//      Serial.print(servoAngles[mystep + 1]);
//      Serial.print (" pulse:");
//      Serial.println(p);

      pwm1.setPWM(m, 0, p);
      pwm2.setPWM(9 - m, 0, p);

      delay(delayBetweenSteps / 2);
    }

  }
}
