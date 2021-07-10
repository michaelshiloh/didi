
const int pulseDelay = 1;
const int sensorPin = 3;

void countStepsToHome() {

  /*
    Make sure we are at the begining of not home
    goNotHome();
    goHome();

    Serial.println("we are home");

    goNotHome();

    // count the steps to home. get the current position, go home, get the new position, subtract
    int home = stepper.currentPosition();
    goHome();*/
  // for now just this
  fullTurnStepCount = 1600;

  // Found home again, so we have the full count
  Serial.print("One full turn is " );
  Serial.print(fullTurnStepCount);
  Serial.println(" steps");

}

boolean isHome() {
  return (LOW == digitalRead(sensorPin)) ;
}

void goHome() {
  stepper.setMaxSpeed(100);
  stepper.setAcceleration(100);
  stepper.moveTo(1600);
  while ( HIGH == digitalRead(sensorPin)) {
    stepper.run();
  }
  Serial.println("home");
}

void goNotHome() {
  while ( isHome()) {
    stepper.setSpeed(3000);
    stepper.runSpeed();
  }
  Serial.println("home");
}
