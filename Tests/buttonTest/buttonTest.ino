const int buttonPin = A0;
byte buttonState = 0;
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}
void loop() {
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
}