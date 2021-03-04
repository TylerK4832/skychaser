#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
 
RH_ASK radio(2000,8,7);
 
void setup()
{
    Serial.begin(9600);   // Use this for debugging
 
    // Speed of 2000 bits per second
    // Use pin 7 for reception
    // Use pin 8 for transmission
    
    if (!radio.init())
    {
         Serial.println("Radio module failed to initialize");
    }
    Serial.println("Radio module set up");
}
 
void loop()
{
  // Create a 32 byte char buffer
  uint8_t receive_buffer[32];
  uint8_t buflen = sizeof(receive_buffer);
 
  // If data is available, print it
  if (radio.recv(receive_buffer, &buflen))
  {
    Serial.print("Message: ");
    Serial.println((char*)receive_buffer);         
  }
}
