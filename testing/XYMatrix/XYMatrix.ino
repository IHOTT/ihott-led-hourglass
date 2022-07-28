#include <FastLED.h>

#define FASTLED_ESP32_I2S
#define NUM_LEDS_PER_STRIP 32
#define NUM_STRIPS 18
#define BRIGHTNESS  255
#define NUM_LEDS 576

const uint8_t kMatrixWidth  = NUM_STRIPS;
const uint8_t kMatrixHeight = NUM_LEDS_PER_STRIP;
const uint8_t kBorderWidth = 2;

CRGB leds[NUM_LEDS];

void setup() 
{
  // FastLED Setup
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

void loop()
{
  // Apply some blurring to whatever's already on the matrix
  // Note that we never actually clear the matrix, we just constantly
  // blur it repeatedly.  Since the blurring is 'lossy', there's
  // an automatic trend toward black -- by design.
  uint8_t blurAmount = beatsin8(2,10,255);
  blur2d( leds, kMatrixWidth, kMatrixHeight, blurAmount);

  // Use two out-of-sync sine waves
  uint8_t  i = beatsin8( 27, kBorderWidth, kMatrixHeight-kBorderWidth);
  uint8_t  j = beatsin8( 41, kBorderWidth, kMatrixWidth-kBorderWidth);
  // Also calculate some reflections
  uint8_t ni = (kMatrixWidth-1)-i;
  uint8_t nj = (kMatrixWidth-1)-j;
  
  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();  
  leds[XY( i, j)] += CHSV( ms / 11, 200, 255);
  leds[XY( j, i)] += CHSV( ms / 13, 200, 255);
  leds[XY(ni,nj)] += CHSV( ms / 17, 200, 255);
  leds[XY(nj,ni)] += CHSV( ms / 29, 200, 255);
  leds[XY( i,nj)] += CHSV( ms / 37, 200, 255);
  leds[XY(ni, j)] += CHSV( ms / 41, 200, 255);
  
  FastLED.show();
}

// Trivial XY function for the SmartMatrix; use a different XY 
// function for different matrix grids.  See XYMatrix example for code.
uint16_t XY( uint8_t x, uint8_t y) { return (y * kMatrixWidth) + x; }
