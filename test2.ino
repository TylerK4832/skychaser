// Servo - Version: Latest
#include <Servo.h>











const int echoPin = 8; // attach pin D2 Arduino to pin Echo of HC-SR04
const int trigPin = 9;
const int power = 7;
int pos = 90;
int upDown = 1;
const int incrementValue = 5;
const int threshold = 50;

Servo myservo;

void setup() {
  myservo.attach(11);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(power, OUTPUT);
  myservo.write(0);
  delay(500);
  myservo.write(180);
  delay(1000);
  myservo.write(90);
}



void loop() {
  myservo.write(pos);
  int distance = getDist();
  if(distance > threshold){
    digitalWrite(power, HIGH);
  }else{
    digitalWrite(power, LOW);
    Serial.println("OBSTRUCTION");
  }
  Serial.print(distance);
  Serial.print("cm");
  Serial.println();
  if(upDown == 1){
    if(pos < 180){
    pos+= incrementValue;
    }else{
      upDown = 0;
      pos-= incrementValue;
    }
  }else{
    if(pos > 0){
      pos-= incrementValue;
    }else{
      upDown = 1;
      pos+= incrementValue;
    }

  }

  Serial.print(pos);
  Serial.println(" postion");
  delay(500);
}


int getDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2;
  return distance;
}
