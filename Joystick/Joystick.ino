// Controlling movements of two servos using a Joystick
// Joystick's x- and y- axes are connected to A0 and A1 analog pins of Arduino.
// Servos are connectd to PWM Pins 9 and 10.
// By Explore Labs

#include <Servo.h>

Servo tilt, pan; // create servo object to control a servo 

int joyX = A0; // analog pin used to connect the X - axis of Joystick
int joyY = A1; // analog pin used to connect the Y - axis of Joystick
int X,Y; // variables to read the values from the analog pins 

void setup()
{ 

  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  Serial.begin(9600);
} 

void loop()
{ 
 X = analogRead(joyX);    // reads the value of the Joystick's X - axis (value between 0 and 1023) 
 Y = analogRead(joyY);    // reads the value of the Joystick's Y - axis (value between 0 and 1023) 
//  x = map(analogRead(joyX), 0, 1023, 900, 2100); // scale it to use with the servo b/w 900 usec to 2100 usec
//  y = map(analogRead(joyY), 0, 1023, 900, 2100);
  Serial.print("X = ");
  Serial.println(X);
  Serial.print("Y = ");
  Serial.println(Y);
  Serial.println();
  delay(500); // waits for the servos to get there 
}
