#include <AFMotor.h>
#include <Servo.h>

// ---------------- CONFIG ----------------
#define TRIG_PIN A0
#define ECHO_PIN A1

#define SAFE_DISTANCE 25
#define FAR_DISTANCE 40

Servo myservo;

// Motors
AF_DCMotor m1(1);
AF_DCMotor m2(2);
AF_DCMotor m3(3);
AF_DCMotor m4(4);

// ---------------- DISTANCE ----------------
long readDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(8);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 10000);
  if(duration == 0) return 300;

  return duration * 0.034 / 2;
}

// ---------------- MOTOR ----------------
void setSpeedAll(int spd){
  m1.setSpeed(spd);
  m2.setSpeed(spd);
  m3.setSpeed(spd);
  m4.setSpeed(spd);
}

void forwardFast(){
  setSpeedAll(170);
  m1.run(FORWARD); m2.run(FORWARD);
  m3.run(FORWARD); m4.run(FORWARD);
}

void forwardSlow(){
  setSpeedAll(130);
  m1.run(FORWARD); m2.run(FORWARD);
  m3.run(FORWARD); m4.run(FORWARD);
}

void stopBot(){
  m1.run(RELEASE); m2.run(RELEASE);
  m3.run(RELEASE); m4.run(RELEASE);
}

// 🔥 FAST SHARP REVERSE
void reverseFast(){
  setSpeedAll(220);   // high speed reverse
  m1.run(BACKWARD); m2.run(BACKWARD);
  m3.run(BACKWARD); m4.run(BACKWARD);
  delay(180);         // short but powerful
  stopBot();
}

void turnLeft(){
  setSpeedAll(170);
  m1.run(BACKWARD); m2.run(BACKWARD);
  m3.run(FORWARD);  m4.run(FORWARD);
  delay(360);
  stopBot();
}

void turnRight(){
  setSpeedAll(170);
  m1.run(FORWARD);  m2.run(FORWARD);
  m3.run(BACKWARD); m4.run(BACKWARD);
  delay(360);
  stopBot();
}

// ---------------- SETUP ----------------
void setup(){

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myservo.attach(10);
  myservo.write(90);

  delay(600);
}

// ---------------- LOOP ----------------
void loop(){

  myservo.write(90);
  long front = readDistance();

  if(front > FAR_DISTANCE){
    forwardFast();
  }
  else if(front > SAFE_DISTANCE){
    forwardSlow();
  }
  else{
    // 🚨 INSTANT ACTION
    stopBot();
    reverseFast();   // fast reverse immediately

    long leftDist, rightDist;

    myservo.write(150);
    delay(100);
    leftDist = readDistance();

    myservo.write(30);
    delay(100);
    rightDist = readDistance();

    myservo.write(90);

    if(leftDist > rightDist)
      turnLeft();
    else
      turnRight();
  }
}
