const int Button = 2;
const int LED =  3;
int buttonState = 0;

int sound = 400; //Pitch for the piezo
int count = 500; //Time for the countdown

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(Button, INPUT);
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
  for (int i = 1; i<=10; i++)
  {
    digitalWrite(LED, HIGH);
    tone(9, sound);
    delay(count);
    digitalWrite(LED, LOW);
    noTone(9);
    delay(count);
  }
}

