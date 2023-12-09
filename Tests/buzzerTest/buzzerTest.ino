const int buzzerPin = 2;
int buzzerTone = 1000;
void setup() {
}
void loop() {
  tone(buzzerPin, buzzerTone, 500);
  delay(100);
  noTone(buzzerPin);
  delay(100);
  buzzerTone += 50;
}
