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
#define NUM_MODES 2
#define DEBOUNCE_TIME 1000
#define NUM_LEDS 3
int MODE = 0;
long last_mode_change = 0;
long animation_step = 0;
int leds[] = {9,10,11};
// the setup function runs once when you press reset or power the board
void setup() {
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
      if(animation_step % 6 > 3){
        digitalWrite(leds[animation_step%3],HIGH);
      }else{
        digitalWrite(leds[animation_step%3],LOW); 
      }
      animation_step++;
      delay(100);
    break;
  }
}

void change_mode(){
  if(millis() - last_mode_change > DEBOUNCE_TIME){
    MODE = (MODE + 1) % NUM_MODES;
  }
}



