
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <RH_ASK.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;


RH_ASK driver(2000, 8,7);

//long   lat,lon; // create variable for latitude and longitude object
float lat = 11.111,lon = 11.111; // create variable for latitude and longitude object 
SoftwareSerial gpsSerial(4,3);//rx,tx
TinyGPS gps; // create gps object


void setup() {
  
  Serial.begin(115200);
  Serial.println("initiating");
  // Initialise the IO and ISR
  if (!driver.init()){
         Serial.println("init failed");

  }

  while (!Serial);
  
 if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  Serial.println("initiation complete");
}

void loop() {
  
if(gpsSerial.available()){ // check for gps data
    if(gps.encode(gpsSerial.read()))// encode gps data
    { 
    gps.f_get_position(&lat,&lon); // get latitude and longitude
    // display position

    Serial.print("Position: ");
    Serial.print("Latitude:");
    Serial.print(lat,3);
    Serial.print(";");
    Serial.print("Longitude:");
    Serial.println(lon,3); 

    Serial.print(lat);
    Serial.print(" ");
    
   }else{
    Serial.println("can't gps encode");
     String strAlt = "error - gps encode" ;
  int msgLen = strAlt.length();
  char msg[msgLen];
  strAlt.toCharArray(msg, msgLen);

  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  delay(500);
  return;
   }
  }else{
     String strAlt = "Gps not available";
  int msgLen = strAlt.length();
  char msg[msgLen];
  strAlt.toCharArray(msg, msgLen);

  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  delay(500);
  Serial.println("Gps not available");
  return;
  }
  






  if (! bmp.performReading()) {
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
  String latitude = String(lat,3);
  String longitude = String(lon,3);
  Serial.println(latitude+";"+longitude);
  String strAlt = String(altitude) + "," +  latitude + "," + longitude;
  int msgLen = strAlt.length();
  char msg[msgLen];
  strAlt.toCharArray(msg, msgLen);

  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  delay(500);


  Serial.println();
  delay(500);
}
