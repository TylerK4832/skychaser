String recvString = "10.00,20.00,50.00";
float r, p, y;



void setup() {
 Serial.begin(9600);
  //Serial.println(r);
  //recvString.toCharArray(s, recvString.length() + 1);
  //int result = sscanf(recvString.c_str(), "%f,%f,%f,", &r, &p ,&y);
  int i1 = recvString.indexOf(',');
  int i2 = recvString.indexOf(',', i1+1);
  Serial.println(i1);
  Serial.println(i2);

  String firstValue = recvString.substring(0, i1);
  String secondValue = recvString.substring(i1 + 1, i2);
  String thirdValue = recvString.substring(i2 + 1);

  Serial.println(firstValue);
  Serial.println(secondValue);
  Serial.println(thirdValue);
}

void loop() {
     char  message[] = "agg,bgg,cgg";
char * ptr = message;
char myString [] = "qwertyuiop,asdfghjk";
int b = 0;
int e = 0;
for(e=e+1; e <12 -1; e++){  // find the comma
//   Serial.print(message[e]);
  if (message[e] == ','){
//    Serial.print(message);
    for(b=b+1; b < e -1; b++){
    Serial.print(message[b]);
  }
  Serial.println();
  Serial.println(e);
  }


delay(250);
}
delay(5000);
}
