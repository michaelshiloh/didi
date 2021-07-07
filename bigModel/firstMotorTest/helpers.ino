
//const int stepPin = 2;
const int pulseDelay = 1;
const int sensorPin = 3;

void countStepsToHome() {

  // First go not home, don't bother counting
  while (isHome()) {
    takeSteps(1);
    //    Serial.print("Searching for  not home" );
    //    Serial.println();
  }

  // now go home
  goHome();
  Serial.println("we are home");

  // now go not home, while counting
  while (isHome()) {
    takeSteps(1);
    fullTurnStepCount++;
    //    Serial.print("leaving home, count = " );
    //    Serial.print(fullTurnStepCount);
    //    Serial.println();
  }

  // now go back home while counting
  while (!isHome()) {
    takeSteps(1);
    fullTurnStepCount++;
    //    Serial.print("searching for home, count = " );
    //    Serial.print(fullTurnStepCount);
    //    Serial.println();
  }

  // Found home again, so we have the full count
  Serial.print("One full turn is " );
  Serial.print(fullTurnStepCount);
  Serial.println(" steps");

}



void takeSteps(int stepCount) {
  while ( stepCount--) {
    digitalWrite(stepPin, LOW);
    delay(pulseDelay);
    digitalWrite(stepPin, HIGH);
    delay(pulseDelay);
  }
}

boolean isHome() {
  return (LOW == digitalRead(sensorPin)) ;
}

void goHome() {
  while ( !isHome()) {
    digitalWrite(stepPin, LOW);
    delay(pulseDelay);
    digitalWrite(stepPin, HIGH);
    delay(pulseDelay);
  }
  Serial.println("home");
}
