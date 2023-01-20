#include <Servo.h>

const int SERVO_MIN = 75;
const int SERVO_MAX = 135;

Servo myservo;

int val;
int average;
bool recalibrated;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), calibrate, FALLING);
  average = 0;
  recalibrated = true;
}

//Driver Code
void loop() {
  make_line();
  detect_lies();
}

//Main function
void detect_lies() {
  while (true) {
    if (recalibrated) { make_line(); };
    update_val();
    Serial.println(val);
    myservo.write(val);
    delay(50);
  }
}

//Draw line after interrupt
void make_line() {
  myservo.write(SERVO_MIN);
  delay(400);
  myservo.write(SERVO_MAX);
  delay(400);
  recalibrated = false;
}

//calculate valid servo value
void update_val() {
  val = map(analogRead(A0) - average, -10, 10, SERVO_MIN, SERVO_MAX);
  if (val > SERVO_MAX) { val = SERVO_MAX; }
  if (val < SERVO_MIN) { val = SERVO_MIN; }
}

//Interrupt handler
void calibrate() {
  long sum = 0;
  for (int i = 0; i < 10000; i++) {
    sum += analogRead(A0);
  }
  average = floor(sum / 10000.0);
  recalibrated = true;
}