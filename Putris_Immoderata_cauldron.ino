/*  Putris Immoderata - Dual Key Switch 
 *  Idea Fab Labs - Chico
 *  Cauldron
 */
 
#include <WS2812FX.h>

#define LED_COUNT 95  // Cauldron
#define LED_PIN 6     // NeoPixel Pin
#define SW1_PIN 8     // 'Key Inserted' Button
#define SW2_PIN 9     // Key Switch One (putris)
#define SW3_PIN 10    // Key Switch Two (vivus)

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

int sw1State      = HIGH; // current state of sw1
int sw1lastState  = HIGH; // previous state of sw1
int sw2State      = HIGH; // current state of sw2
int sw2lastState  = HIGH; // previous state of sw2
int sw3State      = HIGH; // current state of sw3
int sw3lastState  = HIGH; // previous state of sw3

unsigned long tmrBounce = 0;
const unsigned long BOUNCE_INTERVAL = 10;

bool Running = false;
unsigned long tmrTimeout = 0;
const unsigned long TIMEOUT_INTERVAL = 10000;

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start ...");
  
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(SW3_PIN, INPUT_PULLUP);  
  
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSegment(0, 0, 0, FX_MODE_FADE, RED, 5000, NO_OPTIONS); // segment 0 is leds 0 - 0
  ws2812fx.setIdleSegment(1, 0, 0, FX_MODE_STATIC, WHITE, 5000, NO_OPTIONS); // segment 0 is leds 0 - 0
  ws2812fx.setSegment(2, 1, 14, FX_MODE_HYPER_SPARKLE , COLORS(RED, ORANGE, YELLOW), 5000, NO_OPTIONS);
// Putris
  ws2812fx.setSegment(3, 15, 54, FX_MODE_STATIC, BLACK, 5000, REVERSE); 
  ws2812fx.setIdleSegment(4, 15, 54, FX_MODE_STATIC, COLORS(RED, ORANGE, YELLOW), 1200, NO_OPTIONS);
// Vivus
  ws2812fx.setSegment(5, 55, 94, FX_MODE_STATIC, BLACK, 5000, REVERSE); 
  ws2812fx.setIdleSegment(6, 55, 94, FX_MODE_STATIC, COLORS(DIM(GREEN), BLUE), 1000, NO_OPTIONS);
  ws2812fx.start();
}

void loop() {
  ws2812fx.service();
  process_switches();
}

void process_switches() {
  unsigned long currentMillis = millis();
  if(currentMillis - tmrBounce >= BOUNCE_INTERVAL) {
    sw1State = digitalRead(SW1_PIN);
    sw2State = digitalRead(SW2_PIN);
    sw3State = digitalRead(SW3_PIN);
  
    // compare the switchs State to its previous state
    if(sw1State != sw1lastState) {
      if(sw1State == LOW) {
        // if the current state is LOW then the button went from off to on:
        Serial.println("sw1 on");
        ws2812fx.swapActiveSegment(0, 1);
      } else {
        // if the current state is HIGH then the button went from on to off:
        Serial.println("sw1 off");
        ws2812fx.swapActiveSegment(1, 0);
      }
      // save the current state as the last state, for next time through the loop
      sw1lastState = sw1State;    
    }

    if(sw2State != sw2lastState) { // Putris
      if(sw2State == LOW) {
        // if the current state is LOW then the switch went from off to on:
        Serial.println("sw2 on");
        if(Running) {
          if(ws2812fx.isActiveSegment(6)) {
            ws2812fx.swapActiveSegment(6, 5);
            ws2812fx.swapActiveSegment(3, 4);            
          }
        } else {
          ws2812fx.swapActiveSegment(3, 4);
        }
      } else {
        // if the current state is HIGH then the switch went from on to off:
        Serial.println("sw2 off");
        tmrTimeout = currentMillis;
        Running = true;
      }
      // save the current state as the last state, for next time through the loop
      sw2lastState = sw2State;    
    }

    if(sw3State != sw3lastState) { // Vivus
      if(sw3State == LOW) {
        // if the current state is LOW then the switch went from off to on:
        Serial.println("sw3 on");
        if(Running) {
          if(ws2812fx.isActiveSegment(4)) {
            ws2812fx.swapActiveSegment(4, 3);
            ws2812fx.swapActiveSegment(5, 6);            
          }
        } else {
          ws2812fx.swapActiveSegment(5, 6);
        }
      } else {
        // if the current state is HIGH then the switch went from on to off:
        Serial.println("sw3 off");
        tmrTimeout = currentMillis;
        Running = true;
      }
      // save the current state as the last state, for next time through the loop
      sw3lastState = sw3State;    
    }
    tmrBounce = currentMillis;
  }
  if(Running) {
    if(currentMillis - tmrTimeout >= TIMEOUT_INTERVAL) {
      if(ws2812fx.isActiveSegment(4)) {   // if seg[4] is active, switch to seg[3]
        ws2812fx.swapActiveSegment(4, 3);
      } else {                            // else, switch seg[5]
        ws2812fx.swapActiveSegment(6, 5);
      }
      Running = false;
    }
  }
}                                                            
