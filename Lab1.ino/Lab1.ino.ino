/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/
#define SWITCH_PIN 3
#define POT_PIN 2
#define NUM_MODES 5
#define DEBOUNCE_TIME 1000
#define NUM_LEDS 4
int MODE = 0;
long last_mode_change = 0;
long animation_step = 0;
int delay_length = 100;
int leds[] = {12,11,10,9};
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  for(int i = 0; i < NUM_LEDS; i++){
    pinMode(leds[i], OUTPUT);
  }
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), change_mode, FALLING);
}

// the loop function runs over and over again forever
void loop() {
  switch(MODE){
    case 0:
      marquee_step();
    break;
    case 1:
      binary_step();
    break;
    case 2:
    break;
  }
  animation_step++;
  delay(delay_length);
}

void marquee_step(){
  if(animation_step % (NUM_LEDS*2) < NUM_LEDS){
    digitalWrite(leds[animation_step%NUM_LEDS],HIGH);
  }else{  
    digitalWrite(leds[animation_step%NUM_LEDS],LOW); 
  }
}

void binary_step(){
  
}
void change_mode(){
  if(millis() - last_mode_change > DEBOUNCE_TIME){
    MODE = (MODE + 1) % NUM_MODES;
  }
}



