#include <SoftwareSerial.h>
SoftwareSerial GPS = SoftwareSerial(10,2);

void setup(){
  GPS.begin(4800);
  Serial.begin(9600);
}

void loop(){
  Serial.println(GPS.read());
  delay(1000);
}
