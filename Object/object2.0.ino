
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
String speed;
String courseDeg;


  Serial.begin(115200);

  Serial.println("initiating");
  // Initialise the IO and ISR
  if (!driver.init()){
         Serial.println("init failed");



void setup() {
  
  Serial.begin(115200);

  Serial.println("initiating");
  // Initialise the IO and ISR
  if (!driver.init()){
         Serial.println("init failed");

  }

  /*  
 if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  Serial.println("initiation complete");*/
}

void loop() {
  if (gps.location.isValid()){
    lat = String(gps.location.lat(),6);
    
    lon = String(gps.location.lng(),6);
    Serial.print("Position:\n");
    Serial.print("Latitude:");
    Serial.print(lat);
    Serial.print(";");
    Serial.print("Longitude:");
    Serial.println(lon); 
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  if (gps.speed.isUpdated()){
    speed = String(gps.speed.mps(),4);
    Serial.print("Speed: ");
    Serial.println(speed);
  }
  if (gps.course.isUpdated()){
    courseDeg = String(gps.course.deg(),6);
     Serial.print("Course: ");
    Serial.println(courseDeg);
  }
    
    
     

    Serial.print(lat);
    Serial.print(" ");
    

  /*if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(" + bmp.performReading());
    return;
  }
    if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Approx. Altitude = ");
  double altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  double temp = bmp.temperature;
  
  Serial.print(altitude);
  Serial.println(" m");
  Serial.print("temp");
  Serial.println(temp);
  */
 /* Serial.println(lat+";"+lon);
  String strAlt = String(altitude) + "," +  lat + "," + lon;
  int msgLen = strAlt.length();
  char msg[msgLen];
  strAlt.toCharArray(msg, msgLen);

  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();*/
//  delay(500);


  Serial.println();
delay(250);
  
  
  
  button_state = digitalRead(button_pin);
if(button_state == HIGH)
{
const char text[] = "Your Button State is HIGH";
radio.write(&text, sizeof(text));                  //Sending the message to receiver
}
else
{
const char text[] = "Your Button State is LOW";
radio.write(&text, sizeof(text));                  //Sending the message to receiver 
}
radio.write(&button_state, sizeof(button_state));  //Sending the message to receiver 
delay(1000);
}
