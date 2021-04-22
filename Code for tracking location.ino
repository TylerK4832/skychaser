
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <RH_ASK.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;


RH_ASK driver(2000, 8,7);

//long   lat,lon; // create variable for latitude and longitude object
String lat = "11.111",lon = "11.111"; // create variable for latitude and longitude object 
SoftwareSerial ss(6,4);//rx,tx  
TinyGPSPlus gps; // create gps object


long droneLat, droneLong, droneAlt, objectLat, droneHeading,objectLong, objectAlt, distX, DistY, DistZ;
int offset; //so drone doesn't crash into object


int yawChannel; //TODO
int changeYawValue = 15;
int pitchChannel; //TODO
int changePitchValue = 20;
int throttleChannel; //TODO
int ChangeThrottleValue 60;
int rollChannel; //TODO
int changeRollValue = changePitchValue;
int defaultPPMvalue = 1496;
const int numChannels = 8;
int ppm[numChannels];

void loop(){
	if (gps.location.isValid()){
		update();
		
		faceDirection(0.0);
		if(distX > 0){ //Move roll
			rollRight();
		}else if(distX < 0){
			rollLeft();
		}else if(distX == 0){
			println("X is in the right position")
			ppm[rollChannel] = defaultPPMvalue;

		}
		
		if(distY > 0){ //move Pitch
			pitchForward();
		}else if(distY < 0){
			pitchBackward();
		}else if(distY == 0){
			println("Y is in the right position")
			ppm[pitchChannel] = defaultPPMvalue;

		}
		
		if(distZ > 0){ //Move Throttle
			throttleUp();
		}else if(distZ < 0){
			throttleDown;
		}else if(distZ == 0){
			println("Z is in the right position")
		}
	}
}

void getLat(String object){
    if(object == "drone"){
	return String(gps.location.lat(),6);
    }else{
        
    }
}
void getLong(String object){
	return String(gps.location.lng(),6);
}
void getAlt(String object){
	return bmp.readAltitude(SEALEVELPRESSURE_HPA);

}
void getBearing(){
	return String(gps.course.deg(),6);
}
long getDist(plane){
	return truedist - offset
} //X,Y,or Z
bool checkDistdirection(){}

void update(){
	droneLat = getLat(drone);
	droneLong = getLong(drone);
    droneAlt = getAlt(drone);
	droneHeading = getHeading();
	objectLat = getLat(object);
	objectLong = getLong(object);
	objectAlt = getAlt(object);
	distX = getDist("X");
	distY = getDist("Y");
	distZ = getDist("Z");
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
void faceDirection(double angle){ //turns drone to desired heading
	while(droneHeading != angle){
		if(droneHeading > angle){
			yawLeft();
		}else if(droneHeading < angle){
			yawRight();
		}
	}
	ppm[yawChannel] = defaultPPMvalue;

}