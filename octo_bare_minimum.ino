
const int led = 2;
const int button = 12;
const int latch = 16;

bool state = false;


void setup() {
  pinMode(latch, OUTPUT);
  digitalWrite(latch, HIGH);

  pinMode(button, INPUT);
  state = digitalRead(button);

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  delay(500);

  if (state) {
    for (int n = 0; n < 3; n++) {
      digitalWrite(led, LOW);
      delay(300);
      digitalWrite(led, HIGH);
      delay(300);
    }
  }

  digitalWrite(latch, LOW);

}

void loop() {

}
