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






 
int servoDegrees=1; 


int servoangle[] = {102,232,361,491,361,232,102}; // angle
int stopservoangle=2500; // stop at picture
int delaystep=750; // stop after stepp
const long interval = 500; // servo speed  Microseconds


//========================================

void setup() {

  Serial.begin(115200);
  pwm1.begin();
  pwm1.setPWMFreq(FREQUENCY);
  pwm2.begin();
  pwm2.setPWMFreq(FREQUENCY);
  
  
 
 
  
   
          pwm1.setPWM(2, 0,538); 
         
    
 
}

//========================================


void loop() {
  

}
