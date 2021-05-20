
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <RH_ASK.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define BMP_SCK 8  //13
#define BMP_MISO 7  //12
#define BMP_MOSI 5  //11
#define BMP_CS 3    //10
#define SEALEVELPRESSURE_HPA (1023.6) //1013.25

Adafruit_BMP3XX bmp;
int joyX = A0; // analog pin used to connect the X - axis of Joystick
int joyY = A1; // analog pin used to connect the Y - axis of Joystick
int X,Y;

RH_ASK driver(2000, 6,8);

//long   lat,lon; // create variable for latitude and longitude object
String lat = "11.111",lon = "11.111"; // create variable for latitude and longitude object 
SoftwareSerial ss(3,4);//rx,tx  
TinyGPSPlus gps; // create gps object
String speed;
String courseDeg;

void setup() {
  
  Serial.begin(115200);

  Serial.println("initiating");
  // Initialise the IO and ISR
  if (!driver.init()){
         Serial.println("init failed");

 

  }
 pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
//    
// if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
//    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
//    while (1);
//  }
//  
//  // Set up oversampling and filter initialization
//  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
//  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
//  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
//  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
//  Serial.println("initiation complete");
}

void loop() {
  
    lat = String(analogRead(joyX));
    
    lon = String(analogRead(joyY)); 
   double altitude =0;

  String strAlt = String(altitude) + "," +  lat + "," + lon;
  int msgLen = strAlt.length();
  char msg[msgLen];
  strAlt.toCharArray(msg, msgLen);

  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
//  delay(500);


  Serial.println(strAlt);
delay(250);
}
