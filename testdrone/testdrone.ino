
//#include <Wire.h>
//#include <SPI.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BMP3XX.h>
//#include <RH_ASK.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

int GPSBaud = 9600;
/*#include <SPI.h>/*
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
//int ppm[chanel_number];
/*
bool begin = true;
bool armed = true;
int armPin = 8;*/
TinyGPSPlus gps; // create gps object
SoftwareSerial gpsSerial(6,4);
/*

long droneLat, droneLong, droneAlt, objectLat, droneHeading,objectLong, objectAlt, distX, distY, distZ, truedist;
int offset; //so drone doesn't crash into object


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




*/
void setup() {/*
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

*/

Serial.begin(9600);
Serial.println("ready");
gpsSerial.begin(GPSBaud);

}
void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      Serial.println(getLat());

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}


String getLat(){
 if (gps.location.isValid())
  {
    return String(gps.location.lng(),6);
  }
}
String getLong(){
 if (gps.location.isValid())
  {
    return String(gps.location.lat(),6);
  }
}
