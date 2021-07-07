


// ----------LIBRARIES--------------


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// --------CONSTANTS (won't change)---------------
#define MIN_PULSE_WIDTH 500
#define MAX_PULSE_WIDTH 2400
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
uint8_t servonum = 0;






const long interval = 15;
int servoDegrees = 1;
int servoangle[] = {102, 232, 361, 491, 361, 232, 102};
int stopservo = 1000;
int delaystep = 50;


//========================================

void setup() {

  Serial.begin(115200);
  pwm1.begin();
  pwm1.setPWMFreq(FREQUENCY);
  pwm2.begin();
  pwm2.setPWMFreq(FREQUENCY);





  for (int i = 0; i < 16; i++) {
    pwm1.setPWM(i, 0, 102);

  }
  Serial.println("start position");
  delay(1000);



}

//========================================

void loop() {

  /*


    for (int s=0 ; s<6;s++){
     servomoveall(s);
     delay(1000);
    }


    ///////////////////
    for (int mo=0;mo<20;mo++){
      oneup(0, mo);
     delay(delaystep);
     }
     delay(stopservo);
    for (int mo=0;mo<20;mo++){
      oneup(1, 19-mo);
     delay(delaystep);
     }
    delay(stopservo);
    for (int mo=0;mo<20;mo++){
      oneup(2, mo);
     delay(delaystep);
     }
     delay(stopservo);
    for (int mo=0;mo<20;mo++){
      oneup(3, 19-mo);
     delay(delaystep);
     }
     delay(stopservo);
    for (int mo=0;mo<20;mo++){
      oneup(4, mo);
     delay(delaystep);
     }
    delay(stopservo);
    for (int mo=0;mo<20;mo++){
      oneup(5, 19-mo);
     delay(delaystep);
     }
  */
  ///////////////////////////
  delay(stopservo);
  for (int mo = 0; mo < 10; mo++) {
    twoservo(0, mo);
    delay(delaystep);
  }
  delay(stopservo);
  for (int mo = 10; mo >= 0; mo--) {
    twoservo(1, mo);
    delay(delaystep);
  }
  delay(stopservo);
  for (int mo = 0; mo < 10; mo++) {
    twoservo(2, mo);
    delay(delaystep);
  }
  delay(stopservo);
  for (int mo = 10; mo >= 0; mo--) {
    twoservo(3, mo);
    delay(delaystep);
  }
  delay(stopservo);
  for (int mo = 0; mo < 10; mo++) {
    twoservo(4, mo);
    delay(delaystep);
  }
  delay(stopservo);
  for (int mo = 10; mo >= 0; mo--) {
    twoservo(5, mo);
    delay(delaystep);
  }

}






void servomoveall(int mystep) {
  Serial.println(mystep);
  if (servoangle[mystep] < servoangle[mystep + 1]) {

    for ( int p = servoangle[mystep]; p <= servoangle[mystep + 1]; p++) {
      Serial.print ("start:");
      Serial.print(servoangle[mystep]);
      Serial.print (" stop:");
      Serial.print(servoangle[mystep + 1]);
      Serial.print (" pulse:");
      Serial.println(p);


      for (int m = 0; m < 20; m++) {

        if (m >= 0 && m <= 9) {
          pwm1.setPWM(m, 0, p);
        }
        else if (m > 9) {
          pwm2.setPWM(m - 10, 0, p);
        }
      }

      delay(interval);
    }
  }

  else if (servoangle[mystep] > servoangle[mystep + 1]) {
    for ( int p = servoangle[mystep]; p >= servoangle[mystep + 1]; p--) {
      Serial.print ("start:");
      Serial.print(servoangle[mystep]);
      Serial.print (" stop:");
      Serial.print(servoangle[mystep + 1]);
      Serial.print (" pulse:");
      Serial.println(p);

      for (int m = 0; m < 20; m++) {

        if (m >= 0 && m <= 9) {
          pwm1.setPWM(m, 0, p);
        }
        else if (m > 9) {
          pwm2.setPWM(m - 10, 0, p);
        }
      }

      delay(interval);
    }

  }

}

void oneup(int mystep, int m) {
  Serial.println(mystep);
  if (servoangle[mystep] < servoangle[mystep + 1]) {

    for ( int p = servoangle[mystep]; p <= servoangle[mystep + 1]; p++) {
      Serial.print ("start:");
      Serial.print(servoangle[mystep]);
      Serial.print (" stop:");
      Serial.print(servoangle[mystep + 1]);
      Serial.print (" pulse:");
      Serial.println(p);




      if (m >= 0 && m <= 9) {
        pwm1.setPWM(m, 0, p);
      }
      else if (m > 9) {
        pwm2.setPWM(m - 10, 0, p);
      }
      delay(interval / 2);
    }


  }


  else if (servoangle[mystep] > servoangle[mystep + 1]) {
    for ( int p = servoangle[mystep]; p >= servoangle[mystep + 1]; p--) {
      Serial.print ("start:");
      Serial.print(servoangle[mystep]);
      Serial.print (" stop:");
      Serial.print(servoangle[mystep + 1]);
      Serial.print (" pulse:");
      Serial.println(p);



      if (m >= 0 && m <= 9) {
        pwm1.setPWM(m, 0, p);
      }
      else if (m > 9) {
        pwm2.setPWM(m - 10, 0, p);
      }


      delay(interval / 2);
    }

  }
}

void twoservo(int mystep, int m) {
  Serial.println(mystep);
  if (servoangle[mystep] < servoangle[mystep + 1]) {

    for ( int p = servoangle[mystep]; p <= servoangle[mystep + 1]; p++) {
      Serial.print ("start:");
      Serial.print(servoangle[mystep]);
      Serial.print (" stop:");
      Serial.print(servoangle[mystep + 1]);
      Serial.print (" pulse:");
      Serial.println(p);





      pwm1.setPWM(m, 0, p);
      pwm2.setPWM(9 - m, 0, p);

      delay(interval / 2);
    }


  }


  else if (servoangle[mystep] > servoangle[mystep + 1]) {
    for ( int p = servoangle[mystep]; p >= servoangle[mystep + 1]; p--) {
      Serial.print ("start:");
      Serial.print(servoangle[mystep]);
      Serial.print (" stop:");
      Serial.print(servoangle[mystep + 1]);
      Serial.print (" pulse:");
      Serial.println(p);



      pwm1.setPWM(m, 0, p);
      pwm2.setPWM(9 - m, 0, p);


      delay(interval / 2);
    }

  }
}
