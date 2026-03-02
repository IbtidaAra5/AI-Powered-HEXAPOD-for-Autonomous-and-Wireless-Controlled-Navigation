#include <BluetoothSerial.h>
#include <Servo.h>

BluetoothSerial SerialBT;

// 12 Servos (6 legs x 2)
Servo leg1_hip, leg1_knee;
Servo leg2_hip, leg2_knee;
Servo leg3_hip, leg3_knee;
Servo leg4_hip, leg4_knee;
Servo leg5_hip, leg5_knee;
Servo leg6_hip, leg6_knee;

// Ultrasonic
#define trigPin 5
#define echoPin 4
long duration;
int distance;

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void stopMovement() {
  leg1_hip.write(90); leg1_knee.write(90);
  leg2_hip.write(90); leg2_knee.write(90);
  leg3_hip.write(90); leg3_knee.write(90);
  leg4_hip.write(90); leg4_knee.write(90);
  leg5_hip.write(90); leg5_knee.write(90);
  leg6_hip.write(90); leg6_knee.write(90);
}

void moveForward() {
  leg1_knee.write(60); leg4_knee.write(60); leg5_knee.write(60);
  delay(150);
  leg1_hip.write(70); leg4_hip.write(70); leg5_hip.write(70);
  delay(150);
  leg1_knee.write(90); leg4_knee.write(90); leg5_knee.write(90);
  delay(150);

  leg2_knee.write(60); leg3_knee.write(60); leg6_knee.write(60);
  delay(150);
  leg2_hip.write(110); leg3_hip.write(110); leg6_hip.write(110);
  delay(150);
  leg2_knee.write(90); leg3_knee.write(90); leg6_knee.write(90);
  delay(150);
  
  leg1_hip.write(90); leg4_hip.write(90); leg5_hip.write(90);
  leg2_hip.write(90); leg3_hip.write(90); leg6_hip.write(90);
}

void moveBackward() {
  leg1_knee.write(60); leg4_knee.write(60); leg5_knee.write(60);
  delay(150);
  leg1_hip.write(110); leg4_hip.write(110); leg5_hip.write(110);
  delay(150);
  leg1_knee.write(90); leg4_knee.write(90); leg5_knee.write(90);
  delay(150);

  leg2_knee.write(60); leg3_knee.write(60); leg6_knee.write(60);
  delay(150);
  leg2_hip.write(70); leg3_hip.write(70); leg6_hip.write(70);
  delay(150);
  leg2_knee.write(90); leg3_knee.write(90); leg6_knee.write(90);
  delay(150);

  leg1_hip.write(90); leg2_hip.write(90); leg3_hip.write(90);
  leg4_hip.write(90); leg5_hip.write(90); leg6_hip.write(90);
}

void turnLeft() {
  leg1_hip.write(110); leg2_hip.write(110); leg3_hip.write(110);
  leg4_hip.write(70); leg5_hip.write(70); leg6_hip.write(70);
  delay(300);
  stopMovement();
}

void turnRight() {
  leg1_hip.write(70); leg2_hip.write(70); leg3_hip.write(70);
  leg4_hip.write(110); leg5_hip.write(110); leg6_hip.write(110);
  delay(300);
  stopMovement();
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("HexapodESP32");

  leg1_hip.attach(13); leg1_knee.attach(12);
  leg2_hip.attach(14); leg2_knee.attach(27);
  leg3_hip.attach(26); leg3_knee.attach(25);
  leg4_hip.attach(33); leg4_knee.attach(32);
  leg5_hip.attach(22); leg5_knee.attach(21);
  leg6_hip.attach(19); leg6_knee.attach(18);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  stopMovement();
}

void loop() {
  int d = getDistance();
  if (d < 15) {
    stopMovement();
    return;
  }

  if (SerialBT.available()) {
    char c = SerialBT.read();
    if (c == 'F') moveForward();
    else if (c == 'B') moveBackward();
    else if (c == 'L') turnLeft();
    else if (c == 'R') turnRight();
    else if (c == 'S') stopMovement();
  }
}