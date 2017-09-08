
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
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
#define NUM_MODES 6
#define DEBOUNCE_TIME 1000
#define NUM_LEDS 4
#define NUM_HEADINGS 30
int MODE = 0;
long last_mode_change = 0;
long animation_step = 0;
int delay_length = 100;
int leds[] = {12,11,10,9};
/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

float headings[NUM_HEADINGS];
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  for(int i = 0; i < NUM_LEDS; i++){
    pinMode(leds[i], OUTPUT);
  }
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), change_mode, FALLING);
  mag.begin();
}

// the loop function runs over and over again forever
void loop() {
  switch(MODE){
    case 0:
      off();
    break;
    case 1:
      blink();
    break;
    case 2:
      marquee_step();
    break;
    case 3:
      back_and_forth();
    break;
    case 4:
      binary_step();
    break;
    case 5:
      compass_step();
    break;
  }
  animation_step++;
  delay(delay_length);
}

void off(){
  for(int i = 0; i < NUM_LEDS; i++){
    digitalWrite(leds[i], LOW);
  }
}

void blink(){
  for(int i = 0; i < NUM_LEDS; i++){
    if(animation_step % 2){
      digitalWrite(leds[i], HIGH);
    }else{
      digitalWrite(leds[i], LOW);
    }
  }
}

void marquee_step(){
  if(animation_step % (NUM_LEDS*2) < NUM_LEDS){
    digitalWrite(leds[animation_step%NUM_LEDS],HIGH);
  }else{
    digitalWrite(leds[animation_step%NUM_LEDS],LOW);
  }
}

void binary_step(){
  int val = animation_step%(1<<NUM_LEDS);
  for(int i =0; i < NUM_LEDS; i++){
    if(val&(1<<i)){
      digitalWrite(leds[i],HIGH);
    }else{
      digitalWrite(leds[i],LOW);
    }
  }
}

void compass_step(){
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;

  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;

  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI;
  headings[animation_step%NUM_HEADINGS] = headingDegrees;
  float led_hdg_step = 90/NUM_LEDS;
  float avg = get_avg_mag();//led_hdg_step*(animation_step%NUM_LEDS)//
  for(int i = 0; i < NUM_LEDS; i++){
    float led_hdg = 45-led_hdg_step/2 - i*led_hdg_step + avg;
    Serial.println(led_hdg);
    if(abs(headingDegrees-led_hdg)*2 < led_hdg_step*1.5){
      digitalWrite(leds[i], HIGH);
    }else{
      digitalWrite(leds[i], LOW);
    }
  }
}

float get_avg_mag(){
  double tot = 0;
  for(int i = 0; i < NUM_HEADINGS; i++){
    tot += headings[i];
  }
  return tot/NUM_HEADINGS;
}

void back_and_forth(){
  if(animation_step % (NUM_LEDS*4) < NUM_LEDS*2){
    if(animation_step % (NUM_LEDS*2) < NUM_LEDS){
      digitalWrite(leds[animation_step%NUM_LEDS],HIGH);
    }else{
      digitalWrite(leds[animation_step%NUM_LEDS],LOW);
    }
  }
  else{
    if(animation_step % (NUM_LEDS*2) < NUM_LEDS){
      digitalWrite(leds[3-(animation_step%NUM_LEDS)],HIGH);
    }else{
      digitalWrite(leds[3-(animation_step%NUM_LEDS)],LOW);
    }
  }
}

void change_mode(){
  if(millis() - last_mode_change > DEBOUNCE_TIME){
    MODE = (MODE + 1) % NUM_MODES;
  }
}
