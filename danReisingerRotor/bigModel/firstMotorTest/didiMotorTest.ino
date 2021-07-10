
const int enPin = 5;
const int dirPin = 8;
const int stepPin = 2;

int fullTurnStepCount = 0;


void setup() {
  pinMode (enPin, OUTPUT);
  pinMode (dirPin, OUTPUT);
  pinMode (stepPin, OUTPUT);

  //digitalWrite(enPin, HIGH);
  Serial.begin(9600);

  goHome();

  countStepsToHome();
}

void loop() {

  Serial.println("First quarter turn");
  takeSteps(fullTurnStepCount / 4);
  delay(2000);

  Serial.println("Second quarter turn");
  takeSteps(fullTurnStepCount / 4);
  delay(2000);

  Serial.println("Third quarter turn");
  takeSteps(fullTurnStepCount / 4);
  delay(2000);

  Serial.println("Fourth quarter turn; no counting, seek home");
}
