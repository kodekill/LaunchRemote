/*
Connect CLK to the I2C clock - on Arduino UNO thats Analog #5, on the Leonardo it's Digital #3, on the Mega it's digital #21
Connect DAT to the I2C data - on Arduino UNO thats Analog #4, on the Leonardo it's Digital #2, on the Mega it's digital #20
Connect GND to common ground
Connect VCC+ to power - 5V is best but 3V also seems to work for 3V microcontrollers.
https://learn.adafruit.com/adafruit-led-backpack/0-dot-56-seven-segment-backpack
*/


#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <SPI.h>
#include "RF24.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

const int Button = 2;
const int LED =  3;
const int Piezo = 4;
int buttonState = 0;
int Transmit = 1;
int sound = 400; //Pitch for the piezo
int count = 500; //Time for the countdown

bool radioNumber = 0;
RF24 radio(9,10);
byte addresses[][6] = {"1Node","2Node"};
Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(Button, INPUT);
  matrix.begin(0x70); // // // // // // // // May need to check my I2C serial address. 
  
  radio.begin();
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]); 
  delay(500);
}

void loop() {
  buttonState = digitalRead(Button);
  
  if (buttonState == HIGH){
    countdown(); 
  } 
  
  else{
    digitalWrite(LED, LOW);
  }
}

void countdown(){
  for (int i = 10; i >= 1; i--)
  {
    digitalWrite(LED, HIGH);
    tone(Piezo, sound);
    matrix.print(i, DEC);
    matrix.writeDisplay();
    delay(count);
    digitalWrite(LED, LOW);
    noTone(Piezo);
    delay(count);
  }
  radio.write(&Transmit, sizeof(Transmit));
}


