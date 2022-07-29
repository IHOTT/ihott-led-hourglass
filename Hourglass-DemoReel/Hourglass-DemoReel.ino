#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define FASTLED_ESP32_I2S
#define NUM_LEDS_PER_STRIP 32
#define NUM_STRIPS 18
#define BRIGHTNESS  255
#define NUM_LEDS 576

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
unsigned long startMillis;
unsigned long endMillis;
unsigned long FPS;

bool transitioning = false;
unsigned long transitionEndMillis;
#define TRANSITION_LENGTH_MILLIS 1000;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup start.");
  
  // FastLED Setup for 18 parallel outputs on various ESP32 pins
  FastLED.addLeds<WS2812B,15, GRB>(leds, 0*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B,13, GRB>(leds, 1*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B,12, GRB>(leds, 2*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B,14, GRB>(leds, 3*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B,27, GRB>(leds, 4*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B,26, GRB>(leds, 5*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B,25, GRB>(leds, 6*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B,33, GRB>(leds, 7*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B,32, GRB>(leds, 8*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B, 4, GRB>(leds, 9*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B, 0, GRB>(leds,10*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2812B, 2, GRB>(leds,11*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 5, GRB>(leds,12*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B,18, GRB>(leds,13*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B,23, GRB>(leds,14*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B,19, GRB>(leds,15*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B,22, GRB>(leds,16*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B,21, GRB>(leds,17*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness( BRIGHTNESS );

  Serial.println("Setup end.");
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { juggle }; // TODO: switch away from test pattern for prod
//SimplePatternList gPatterns = { IHOTT, fluxCapacitor };
//SimplePatternList gPatterns = { IHOTT, fluxCapacitor, rainbowGlitter, confetti, sinelon, juggle, bpm , prideUp, prideDown };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  startMillis = millis();

  if (millis() > transitionEndMillis) transitioning = false;
  
  if (transitioning) {
    fadeToBlackBy( leds, NUM_LEDS, 40);
    FastLED.delay(25);
  }
  else {
    // Call the current pattern function once, updating the 'leds' array
    gPatterns[gCurrentPatternNumber]();
  }

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
//  FastLED.delay(1000/FRAMES_PER_SECOND); 

// Update FPS
  endMillis = millis();
  FPS = 1000 / (endMillis - startMillis);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 1 ) { Serial.print("FPS: "); Serial.println(FPS); } // print FPS over serial
  EVERY_N_SECONDS( 100 ) { nextPattern(); } // change patterns periodically TODO: increase this for prod
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
//  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
  
  // randomize the next animation?
  gCurrentPatternNumber = random8() % ARRAY_SIZE(gPatterns);
  
  // add a fade for transition
  transitioning = true;
  transitionEndMillis = millis() + TRANSITION_LENGTH_MILLIS;
}

uint16_t scaleRange( uint16_t value, uint16_t lowest, uint16_t highest ) {
  uint16_t rangewidth = highest - lowest;
  uint16_t scaledbeat = scale16( value, rangewidth);
  return lowest + scaledbeat;
}

void rainbowGlitter() {
  addGlitter(120, CHSV(random8(), 255, 255));
  addGlitter(60, CHSV(random8(), random8(), 255));
  
  fadeToBlackBy( leds, NUM_LEDS, 4);
  meteorHoops(false);
}

void IHOTT() {
//  gHue +=3;
  fadeToBlackBy( leds, NUM_LEDS, 20); // TODO: replace with a blend?

  // set a base animation for all leds
  for(int i = 0; i < NUM_STRIPS; i++) {
    uint16_t row = scaleRange(beat16(40+i), 0, (NUM_LEDS_PER_STRIP / 2) - 1);
    
    leds[(i*NUM_LEDS_PER_STRIP) + row] = CHSV(gHue+(row*3),255,255);
    leds[(i*NUM_LEDS_PER_STRIP) + (NUM_LEDS_PER_STRIP - row)] = CHSV(gHue+(NUM_LEDS_PER_STRIP + (row*3)),255,255);
  }
  
  centerWhiteGlitter();
  meteorHoops(false);
}

void centerWhiteGlitter() {
//  uint16_t brightness = beatsin16( (90 + beatsin16(4, 0, 30)), 64, 255 );
  uint16_t brightness = 255;
  for(int i = 0; i < NUM_STRIPS; i++) {
    if( random8() < 4) leds[((i*NUM_LEDS_PER_STRIP)) + 15] = CRGB(brightness,brightness,brightness);
    if( random8() < 4) leds[((i*NUM_LEDS_PER_STRIP)) + 16] = CRGB(brightness,brightness,brightness);
  }
}

void pulsedHoops(uint8_t hue, uint8_t sat)  {
  uint8_t brightness = scaleRange(beatsin16(140), 128, 255);

  for(int i = 0; i < NUM_STRIPS; i++) {
  // top and bottom
    leds[(i*NUM_LEDS_PER_STRIP)] = CHSV(hue, sat, brightness);
    leds[(((NUM_STRIPS - 1) - i)*NUM_LEDS_PER_STRIP) + 31] = CHSV(hue, sat, brightness);
  }
}

void meteorHoops(bool doubleMeteors)  {
  // set the hoops to black
//  for(int i = 0; i < NUM_STRIPS; i++) {
//    leds[(i*NUM_LEDS_PER_STRIP)] = CRGB(0,0,0);
//    leds[(((NUM_STRIPS - 1) - i)*NUM_LEDS_PER_STRIP) + 31] = CRGB(0,0,0);
//  }
  
  uint16_t column = scaleRange(beat16(60), 0, NUM_STRIPS - 1);
  uint16_t column2 = (column + ((NUM_STRIPS - 1) / 2)) % (NUM_STRIPS - 1);
  
  // top and bottom
    leds[(column*NUM_LEDS_PER_STRIP)] = CRGB(255,255,255);
    leds[(((NUM_STRIPS - 1) - column)*NUM_LEDS_PER_STRIP) + 31] = CRGB(255,255,255);
    if (doubleMeteors) {
      leds[(column2*NUM_LEDS_PER_STRIP)] = CRGB(255,255,255);
      leds[(((NUM_STRIPS - 1) - column2)*NUM_LEDS_PER_STRIP) + 31] = CRGB(255,255,255);
    }
}

// some chasing spiraling sand particles
// TODO: stall and reverse flow?
void fluxCapacitor() {
  gHue +=3;
  fadeToBlackBy( leds, NUM_LEDS, 20); // TODO: replace with a blend?

  // set a base animation for all leds
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
      leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV(gHue-(j*16),255,beatsin16( 35, 36, 96 ));
    }
  }

  // add some chasing spiraling grains of sand
  for( int i = 0; i < 8; i++) {
    uint16_t column = scaleRange(beat16(80+i), 0, NUM_STRIPS - 1);
    uint16_t row = scaleRange(beat16(10+i), 0, NUM_LEDS_PER_STRIP - 1);
    leds[((NUM_LEDS_PER_STRIP * column) + row)] |= CRGB(255, 255, 255); // TODO: switch to HSV and turn down saturation? does that make white? maybe pulse that value too?
  }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter, CHSV glitterColor) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += glitterColor;
  }
}

void addGlitter(fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 255, 255);

  pulsedHoops(gHue, scaleRange(beatsin16(60), 0, 255));
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  gHue += 3;
  
  for(int i = 0; i < NUM_STRIPS; i++) {
    int pos = beatsin16( 40, 0, NUM_LEDS_PER_STRIP-1 );
    leds[(i*NUM_LEDS_PER_STRIP) + pos] = CHSV( gHue + (i*(255 / NUM_STRIPS)), 255, 255);
    leds[(i*NUM_LEDS_PER_STRIP) + 31 - pos] = CHSV( gHue + (i*(255 / NUM_STRIPS)), 255, 255);
  }

  centerWhiteGlitter();
  pulsedHoops(gHue, 0);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void prideDown() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void prideUp() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
//    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}
