/*
Modified on Nov 28, 2020
Modified by MehranMaleki from Arduino Examples
Home
*/

// Mux control pins
int s0 = D0;
int s1 = D1;
int s2 = D2;
int s3 = D3;

// Mux in "SIG" pin
int SIG_pin = A0;

void setupMulti()
{
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
}

// void loop(){

//   //Loop through and read all 16 values
//   for(int i = 0; i < 16; i ++){
//     Serial.print("Value at channel ");
//     Serial.print(i);
//     Serial.print("is : ");
//     Serial.println(readMux(i));
//     delay(1000);
//   }

// }

float readMux(int channel)
{
  // int controlPin[] = {s0, s1, s2, s3};

  // int muxChannel[16][4]={
  //   {0,0,0,0}, //channel 0
  //   {1,0,0,0}, //channel 1
  //   {0,1,0,0}, //channel 2
  //   {1,1,0,0}, //channel 3
  //   {0,0,1,0}, //channel 4
  //   {1,0,1,0}, //channel 5
  //   {0,1,1,0}, //channel 6
  //   {1,1,1,0}, //channel 7
  //   {0,0,0,1}, //channel 8
  //   {1,0,0,1}, //channel 9
  //   {0,1,0,1}, //channel 10
  //   {1,1,0,1}, //channel 11
  //   {0,0,1,1}, //channel 12
  //   {1,0,1,1}, //channel 13
  //   {0,1,1,1}, //channel 14
  //   {1,1,1,1}  //channel 15
  // };

  // loop through the 4 sig
  //   for(int i = 0; i < 4; i ++){
  //   digitalWrite(controlPin[i], muxChannel[channel][i]);
  //   }
  if (channel == 0)
  {
    digitalWrite(s0, LOW);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    Serial.println("Channel 0");
  }
  else if (channel == 15)
  {
    /* code */
    digitalWrite(s0, HIGH);
    digitalWrite(s1, HIGH);
    digitalWrite(s2, HIGH);
    digitalWrite(s3, HIGH);
    Serial.println("Channel 15");
  }
  else if (channel == 1)
  {
    digitalWrite(s0, LOW);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
    Serial.println("Channel 1");
  }

  // read the value at the SIG pin
  float val = analogRead(SIG_pin);

  // return the value
  //  float voltage = (val * 5.0) / 1024.0;
  return val;
}
