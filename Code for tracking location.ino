
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <RH_ASK.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
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

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
boolean button_state = 0;
int led_pin = 3;
void setup() {
	pinMode(armPin, INPUT);
  //initiallize default ppm values
  for(int i=0; i<chanel_number; i++){
    ppm[i]= default_servo_value;
  }

  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();

  ppm[0] = 750;
  ppm[4] = 800;


pinMode(6, OUTPUT);
Serial.begin(9600);
radio.begin();
radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
radio.startListening();              //This sets the module as receiver
char text[32] = "";


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


int yawChannel = 3;
int changeYawValue = 15;
int pitchChannel = 1;
int changePitchValue = 20;
int throttleChannel = 0;
int ChangeThrottleValue 60;
int rollChannel = 2;
int changeRollValue = changePitchValue;
int defaultPPMvalue = 1496;
const int numChannels = 8;
int ppm[numChannels];

void loop(){
	if (radio.available()){        		   //Looking for the data.
								            //Saving the incoming data
		radio.read(&text, sizeof(text));    //Reading the data
		radio.read(&button_state, sizeof(button_state));    //Reading the data
		Serial.println(text);
		updateObjectInfo(text);
	}
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
	if(object == "drone"){
	return String(gps.location.lng(),6);
	}else{
        
    }
}
void getAlt(String object){
	if(object == "drone"){
	return bmp.readAltitude(SEALEVELPRESSURE_HPA);
	}else{
        
    }

}
void getBearing(){
	return String(gps.course.deg(),6);
}
long getDist(plane){
	//TODO
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
void updateObjectInfo


ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if(state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PPM_PulseLen * 2;
    state = false;
  }
  else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= chanel_number){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PPM_PulseLen;// 
      OCR1A = (PPM_FrLen - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PPM_PulseLen) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}