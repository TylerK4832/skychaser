
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <RH_ASK.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>
#define chanel_number 8  //set the number of chanels
#define default_servo_value 1500  //set the default servo value
#define PPM_FrLen 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 5  //set PPM signal output pin on the arduino
/*this array holds the servo values for the ppm signal5555555555555555555555555555555555555555555555555
 change theese values in your code (usually servo values move between 1000 and 2000)*/
int ppm[chanel_number];
bool begin = true;
bool armed = true;
int armPin = 8;
TinyGPSPlus gps; // create gps object

boolean button_state = 0;
int led_pin = 3;
String lat = "11.111",lon = "11.111"; // create variable for latitude and longitude object 
SoftwareSerial ss(6,4);//rx,tx  
RH_ASK radio(2000,6,8);
int droneAlt, objectLat, droneHeading,objectLong, objectAlt;
int offset = 0; //so drone doesn't crash into object
int distX, distY, distZ;
double truedist = 0.0;
static int homeLat,droneLat, homeLong, droneLong, homeAlt;
String directions = "";


int yawChannel = 3;
int changeYawValue = 15;
int pitchChannel = 1;
int changePitchValue = 20;
int throttleChannel = 0;
int ChangeThrottleValue = 60;
int rollChannel = 2;
int changeRollValue = changePitchValue;
int defaultPPMvalue = 1496;
const int numChannels = 8;



#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;


int joyX = A1; // analog pin used to connect the X - axis of Joystick
int joyY = A0; // analog pin used to connect the Y - axis of Joystick
int X,Y;
int joyX2 = A3; // analog pin used to connect the X - axis of Joystick
int joyY2 = A2; // analog pin used to connect the Y - axis of Joystick
int X2,Y2;


void setup() {
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);

  pinMode(joyX2, INPUT);
  pinMode(joyY2, INPUT);

  Serial.begin(115200);

//radio.begin();
//radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
//radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
//radio.startListening();              //This sets the module as receiver
//char text[32] = "";

}

void loop(){
  delay(20);
    update("drone");
    directions = "";
    int errorRange = 20;
//    faceDirection(0.0); //face north
    if(distX < errorRange*-1){ //adjust roll
      rollRight();
      directions+= "Roll Left,";
//      Serial.println("roll right");
    }else if(distX > errorRange){
      directions += "Roll Right,";
      rollLeft();
//      Serial.println("roll left");
    }else{
      directions += "Longitude is in position,";
//      Serial.println("X is in the right position");
      ppm[rollChannel] = defaultPPMvalue;
    }
    
    if(distY > errorRange){ //adjust Pitch
      directions += "Pitch Forward";
      pitchForward();
//      Serial.println("forward");
    }else if(distY < errorRange*-1){
      pitchBackward();
      directions += "Pitch Backwards";
//      Serial.println("backward");
    }else{
      directions += "Latitude is in position,";
//      Serial.println("Y is in the right position");
      ppm[pitchChannel] = defaultPPMvalue;
    }
    
    if(distZ > 0){ //adjust Throttle
      throttleUp();
//      Serial.println("Up");
    }else if(distZ < 0){
      throttleDown;
      }else if(distZ == 0){
//        Serial.println("Down");
//      Serial.println("Z is in the right position");
    }
//    Serial.println("\n");

String out;
out += droneLat;
out += ",";
out += droneLong;
out += ",";
out += objectLat;
out += ",";
out += objectLong;
out += ",";
out += directions;
Serial.println(out);

  }
int getLat(String object){
  if(object == "drone"){
  return analogRead(joyX);
  }else return analogRead(joyX2);
}
int getLong(String object){
  if(object == "drone"){
    return analogRead(joyY);
  }
  else return analogRead(joyY2);
}
int getAlt(String object){
  if(object == "drone"){
    return 15;
  }else return 15;
  if (gps.location.isValid())
  {
    return 111;
  }

}
int getBearing(){
  return 0;
  return gps.course.deg();
}
int getDist(String plane){
  if(plane == "X"){
//    Serial.print("xxx");
//    Serial.print(droneLat);
//    Serial.print("   ");
//    Serial.println(objectLat);
    return int(droneLat - objectLat); //change to object Lat for movement
  }else if(plane == "Y"){
    return int(droneLong - objectLong); //change to object for movement
  }else if(plane == "Z"){
    return int(droneAlt - objectAlt); //change to object alt
  }

} //X,Y,or Z
//bool checkDistdirection(){}

void update(String drone){
  droneLat = getLat("drone");
  droneLong = getLong("drone");
  droneAlt = getAlt("drone");
  droneHeading = getBearing();
  updateObjectInfo();
  distX = getDist("X");
  distY = getDist("Y");
  distZ = getDist("Z");
//  Serial.print("Lat ");
//  Serial.println(distX);
//  Serial.print("Long ");
//  Serial.println(distY);

}
void rollRight(){
  ppm[rollChannel] = defaultPPMvalue + changeRollValue;
}
void rollLeft(){
  ppm[rollChannel] = defaultPPMvalue - changeRollValue;
}
void pitchBackward(){
  ppm[pitchChannel] = defaultPPMvalue + changePitchValue;
}
void pitchForward(){
  ppm[pitchChannel] = defaultPPMvalue - changePitchValue;
}
void throttleDown(){
  ppm[throttleChannel] -= ChangeThrottleValue;
}
void throttleUp(){
  ppm[throttleChannel] += ChangeThrottleValue;
}
void yawLeft(){
  ppm[yawChannel] = defaultPPMvalue + changeYawValue;
}
void yawRight(){  
  ppm[yawChannel] = defaultPPMvalue - changeYawValue;
}
void faceDirection( double angle){ //turns drone to desired heading
  while(droneHeading != angle){
    if(droneHeading > angle){
      yawLeft();
    }else if(droneHeading < angle){
      yawRight();
    }
  }
  ppm[yawChannel] = defaultPPMvalue;

}

void updateObjectInfo(){
  objectLat = getLat("object");
  objectLong = getLong("object");
  objectAlt = getAlt("object");

  }
