/*  Putris Immoderata - Dual Key Switch 
 *  Idea Fab Labs - Chico
 */
#include <WS2812FX.h>

#define LED_COUNT 30
#define LED_PIN 6

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

const int  sw1 = 8;  // 'Key Inserted' Button
const int  sw2 = 9;  // Key Switch One
const int  sw3 = 10; // Key Switch Two

int sw1State      = HIGH; // current state of sw1
int sw1lastState  = HIGH; // previous state of sw1
int sw2State      = HIGH; // current state of sw2
int sw2lastState  = HIGH; // previous state of sw2
int sw3State      = HIGH; // current state of sw3
int sw3lastState  = HIGH; // previous state of sw3

//unsigned long currentMillis;
unsigned long tmrCheck = 0;
const unsigned long CHECK_INTERVAL = 10;

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start ...");
  
  ws2812fx.init();
  ws2812fx.setBrightness(50);
  ws2812fx.setSpeed(3000);
  ws2812fx.setColor(0xFF2222);
  //ws2812fx.setMode(FX_MODE_FADE);
  ws2812fx.setMode(53);
  ws2812fx.start();
  
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
}

void loop() {
  ws2812fx.service();
  process_switches();
}

void process_switches() {
  unsigned long currentMillis = millis();
  if(currentMillis - tmrCheck >= CHECK_INTERVAL) {
    sw1State = digitalRead(sw1);
    sw2State = digitalRead(sw2);
    sw3State = digitalRead(sw3);
  
    // compare the switch State to its previous state
    if(sw1State != sw1lastState) {
      if(sw1State == LOW) {
        // if the current state is LOW then the button went from off to on:
        Serial.println("sw1 on");
        ws2812fx.setBrightness(100);
        ws2812fx.setSpeed(3000);
        ws2812fx.setColor(0xFF0000);
        ws2812fx.setMode(15);

      } else {
        // if the current state is HIGH then the button went from on to off:
        Serial.println("sw1 off");
        ws2812fx.setBrightness(50);
        ws2812fx.setSpeed(3000);
        ws2812fx.setColor(0xFF2222);
        ws2812fx.setMode(53);
      }
      // save the current state as the last state, for next time through the loop
      sw1lastState = sw1State;    
    }

    if(sw2State != sw2lastState) {
      if(sw2State == LOW) {
        // if the current state is LOW then the button went from off to on:
        Serial.println("sw2 on");
        ws2812fx.setBrightness(100);
        ws2812fx.setSpeed(3000);
        ws2812fx.setColor(0xFF2222);
        ws2812fx.setMode(51);        
      } else {
        // if the current state is HIGH then the button went from on to off:
        Serial.println("sw2 off");
      }

      // save the current state as the last state, for next time through the loop
      sw2lastState = sw2State;    
    }

    if(sw3State != sw3lastState) {
      if(sw3State == LOW) {
        // if the current state is LOW then the button went from off to on:
        Serial.println("sw3 on");
        ws2812fx.setBrightness(100);
        ws2812fx.setSpeed(3000);
        ws2812fx.setColor(0xFF2222);
        ws2812fx.setMode(11);
      } else {
        // if the current state is HIGH then the button went from on to off:
        Serial.println("sw3 off");
      }
      // save the current state as the last state, for next time through the loop
      sw3lastState = sw3State;    
    }
    tmrCheck = currentMillis;
  }
}
