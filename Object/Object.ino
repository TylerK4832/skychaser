
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;


#include <RH_ASK.h>
RH_ASK driver;

const int led_pin = 9;
const int transmit_pin = 8;
const int receive_pin = 2;
const int transmit_en_pin = 3;

void setup() {
  // Initialise the IO and ISR
   if (!driver.init()){
         Serial.println("init failed");
}
 

  Serial.begin(115200);
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
  
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Approx. Altitude = ");
 double altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
 Serial.print(altitude);
  Serial.println(" m");

  String strAlt = String(altitude);
  int msgLen = strAlt.length();
  char msg[msgLen];
 strAlt.toCharArray(msg, msgLen);

  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
    
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(1000);

  digitalWrite(led_pin, LOW);
  delay(1000);
  Serial.println();

}
