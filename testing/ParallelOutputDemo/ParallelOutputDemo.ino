#include <FastLED.h>

#define FASTLED_ESP32_I2S
#define NUM_LEDS_PER_STRIP 32
#define NUM_STRIPS 18
#define BRIGHTNESS  128

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
unsigned long startMillis;
unsigned long endMillis;

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("Starting...");

  FastLED.addLeds<WS2812B,15, GRB>(leds, 0*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B,13, GRB>(leds, 1*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B,12, GRB>(leds, 2*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B,14, GRB>(leds, 3*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B,27, GRB>(leds, 4*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B,26, GRB>(leds, 5*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B,25, GRB>(leds, 6*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B,33, GRB>(leds, 7*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B,32, GRB>(leds, 8*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B, 4, GRB>(leds, 9*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B, 0, GRB>(leds,10*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
  FastLED.addLeds<WS2812B, 2, GRB>(leds,11*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 5, GRB>(leds,12*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B,18, GRB>(leds,13*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B,23, GRB>(leds,14*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B,19, GRB>(leds,15*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B,22, GRB>(leds,16*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B,21, GRB>(leds,17*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

  FastLED.setBrightness( BRIGHTNESS );
}

void loop() {
  startMillis = millis();
//  Serial.println("Loop....");
  
  static uint8_t hue = 0;
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
      leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32*i) + hue+j,192,255);
    }
  }

  // Set the first n leds on each strip to show which strip it is
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j <= i; j++) {
      leds[(i*NUM_LEDS_PER_STRIP) + j] = CRGB::Red;
    }
  }

  hue++;

  FastLED.show();
  // FastLED.delay(100);

  // Print FPS over serial
  endMillis = millis();
  unsigned long loopMillis = endMillis - startMillis;
  Serial.print("FPS: ");
  Serial.println(1000 / loopMillis);
}
