
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;


#include <RH_ASK.h>
RH_ASK driver;

//long   lat,lon; // create variable for latitude and longitude object
float lat = 28.5458,lon = 77.1703; // create variable for latitude and longitude object 
SoftwareSerial gpsSerial(3,4);//rx,tx
TinyGPS gps; // create gps object

const int led_pin = 9;
const int transmit_pin = 8;
const int receive_pin = 2;
const int transmit_en_pin = 3;

void setup() {
  
  Serial.begin(115200);
  
  // Initialise the IO and ISR
  if (!driver.init()){
         Serial.println("init failed");

  }

  while (!Serial){
  
  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode
  //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  }
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void loop() {
  
  if(gpsSerial.available()){ // check for gps data
    if(gps.encode(gpsSerial.read()))// encode gps data
    { 
    gps.f_get_position(&lat,&lon); // get latitude and longitude
    // display position

    //Serial.print("Position: ");
    //Serial.print("Latitude:");
    //Serial.print(lat,6);
    //Serial.print(";");
    //Serial.print("Longitude:");
    //Serial.println(lon,6); 

    //Serial.print(lat);
    //Serial.print(" ");
    
   }
  
  






  
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  
  Serial.print("Approx. Altitude = ");
  double altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print(altitude);
  Serial.println(" m");
  String latitude = String(lat,6);
  String longitude = String(lon,6);
  Serial.println(latitude+";"+longitude);
  String strAlt = String(altitude) + "," +  latitude + "," + longitude;
  int msgLen = strAlt.length();
  char msg[msgLen];
  strAlt.toCharArray(msg, msgLen);

  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  delay(1000);

  digitalWrite(led_pin, LOW);

  Serial.println();
  
}
