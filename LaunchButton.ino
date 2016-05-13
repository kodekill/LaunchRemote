//Have the go go power rangers sound effect play, when the device is turned on. 
#include <Wire.h>
#include <SPI.h>
#include "RF24.h"
#include <Adafruit_NeoPixel.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#define PIN 9
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

const int RedButton = 2;
const int GreenButton = 5;
const int RED_LED = 3;
const int GREEN_LED = 6;
const int NeoPixel = 9;
const int Piezo = 4;

int RedButtonState = 0;
int GreenButtonState = 0;
int Transmit = 1;

int countDown = 5; // Determines how long countdown will be
int sound = 400; //Pitch for the piezo
int countDelay = 500; //Time for the countdown

bool radioNumber = 0;
RF24 radio(9,10);
byte addresses[][6] = {"1Node","2Node"};
Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RedButton, INPUT);
  pinMode(GreenButton, INPUT);
  pixel.begin();
  pixel.show(); // Initialize all pixels to 'off'
  matrix.begin(0x70);
  
  radio.begin();
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]); 
  delay(500);
}

void loop() {
  RedButtonState = digitalRead(RedButton);
  GreenButtonState = digitalRead(GreenButton);
  
  matrix.print(0000, DEC);
  matrix.writeDisplay();
  
  if (RedButtonState == HIGH){
    countdown(); 
  } 

  if (GreenButtonState == HIGH){
    checkCommunication(); 
  } 
  
  else{
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
  }
}

void countdown(){
  for (int i = countDown; i >= 1; i--)
  {
    digitalWrite(RED_LED, HIGH);
    tone(Piezo, sound);
    matrix.print(i, DEC);
    matrix.writeDisplay();
    pixel.setPixelColor(0, pixel.Color(0,25,0)); // Moderately bright green color.
    pixel.show();
    delay(countDelay);
    
    digitalWrite(RED_LED, LOW);
    noTone(Piezo);
    pixel.setPixelColor(0, pixel.Color(0,0,0)); // Moderately bright green color.
    pixel.show();
    delay(countDelay);
  }
  
  matrix.print(0, DEC);
  matrix.writeDisplay();
  
  radio.write(&Transmit, sizeof(Transmit));
  delay(500);
}


//Change this after I test button/LED
void checkCommunication(){
    for (int i = countDown; i >= 1; i--)
  {
    digitalWrite(GREEN_LED, HIGH);
    tone(Piezo, sound);
    matrix.print(i, DEC);
    matrix.writeDisplay();
    delay(countDelay);
    digitalWrite(GREEN_LED, LOW);
    noTone(Piezo);
    delay(countDelay);
  }
  
  matrix.print(0, DEC);
  matrix.writeDisplay();
  
  radio.write(&Transmit, sizeof(Transmit));
  delay(500);
}


