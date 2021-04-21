unsigned long codes[] = {2724039285,//IR Codes
2724004350,
2724036990,
2724049485,
2724022455,
2724042855,
2724055095,
2724037755,
2724036735,
2724020415,
2724053055,
2724012255,
2724044895,
2724028575,
2724061215,
2724008175,
2724040815,
2724045915,
2724024495,
2724029595,
2724034440,
2724053565,
2724029085,
2724048975,
2724041835,
2724009195,
2724062235,
2724004095,
2724046935,
2724014295,
2724054075,
2724013275,
2724035460,
2724012510,
2724067080,
2724030615,
2724062490,
2724021435,
2724025515,
2724058155,
2724017355,
2724064020,
2724005115,
2724033165,
2724057135,
2724004605,
2724048720};
//this programm will put out a PPM signal

//////////////////////CONFIGURATION///////////////////////////////
#define chanel_number 12  //set the number of chanels
#define default_servo_value 1496  //set the default servo value
#define PPM_FrLen 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 3  //set PPM signal output pin on the arduino
////////////////////////////////////////////////////////////////// 
//IR stuff
#include <IRremote.h>

int RECV_PIN=8; 
IRrecv irrecv(RECV_PIN);
decode_results results ;  
int amountChange = 20;
unsigned long result;
/*this array holds the servo values for the ppm signal
 change theese values in your code (usually servo values move between 1000 and 2000)*/
int ppm[chanel_number];

void setup(){  
irrecv.enableIRIn();  // enable IR
Serial.begin(9600);
Serial.println("ready");
Serial.println(codes[1]);
  //initiallize default ppm values
  for(int i=0; i<chanel_number; i++){
    ppm[i]= default_servo_value;
  }
//ppm[1] = 1501;
//ppm[2] = 1602;
//ppm[3] = 1703;
//ppm[4] = 1804;
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

}

void loop(){
     if(irrecv.decode(&results))    //read ir code
   {
    result = results.value;
    Serial.println(result);
    Serial.println();
     left();
     right();
     power();
     up();
     down();
     REV();
     FWD();
     enter();
     enable();
     Clear();
     one();
     two();
     three();
     four();
     five();
     
     

    delay(15);

     irrecv.resume();
      }
}

ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if(state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PPM_PulseLen * 2;
    state = false;
  }
  else {  //end pulse and calculate when to start the next pulse
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


 
 void left(){ //roll left
  if(results.value == codes[0]){
    Serial.println("left");
    ppm[1] = 1500-amountChange;
  }//else ppm[1] = 1500-3;
 }
 
 void up(){ //pitch foward
   if(result == codes[1]){
    Serial.println("up");
    ppm[2] = 1500-amountChange;
  }//else ppm[1] = 1500-3;
 }
 
 void down(){ //pitch backward
   if(result == codes[2]){
    Serial.println("down");
    ppm[2] = 1500+amountChange;
  }//else ppm[1] = 1500-3;
 }
 
 void right(){ //roll right
   if(results.value == codes[3]){
    Serial.println("right");
    ppm[1] = 1500+amountChange;
  }//else ppm[1] = 1500-3;
 }
 
 void power(){ //increase throttle
  if(results.value == codes[4]){
    Serial.print("power ");
  Serial.println(results.value);
  ppm[0] += 50;
 }
 }
 
 void REV(){ //Yaw Left
   if(result == codes[5]){
    Serial.println("Yaw Left");
    ppm[3] = 1500+amountChange;
  }
 }
 
 void FWD(){ //Yaw Right
    if(result == codes[6]){
    Serial.println("Yaw Right");
    ppm[3] = 1500-amountChange;
  }
 }
 void enter(){ //reset yaw,pitch and roll
   if(result == codes[7]){
    for(int i=1; i<chanel_number-1; i++){
    ppm[i]= default_servo_value;
  }
   }
 }
   void enable(){ //Arms the drone
     if(result == codes[46]){
      if (ppm[11] == 1400)
        ppm[11] = 1800;
      else
        ppm[11] = 1400;
  }
   }

   void Clear(){ //sets throttle back to 750
     if(result ==codes[34]){
       ppm[0] = 750;
     
      }
   }

void one(){
 if(result ==codes[8]){
       ppm[0] = 1500;
     
      }
   }
void two(){
 if(result == codes[9]){
       ppm[0] = 1700;
     
      }
   }
void three(){
 if(result == codes[10]){
       ppm[0] = 1800;
     
      }
   }
void four(){
 if(result == codes[11]){
       ppm[0] = 1900;
     
      }
   }
void five(){
   if(result == codes[12]){
       ppm[0] -= 150 ;
     
      }
}
 
 
