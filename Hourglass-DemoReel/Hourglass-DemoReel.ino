#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define FASTLED_ESP32_I2S
#define NUM_LEDS_PER_STRIP 32
#define NUM_STRIPS 18
#define BRIGHTNESS  255
#define NUM_LEDS 576

#define SHUFFLE_ANIMATIONS false // TODO: shuffle for prod

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
unsigned long startMillis;
unsigned long endMillis;
unsigned long FPS;

bool transitioning = false;
unsigned long transitionEndMillis;
#define TRANSITION_LENGTH_MILLIS 1000;
bool gReverseDirection = false;

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
//SimplePatternList gPatterns = { test }; // TODO: switch away from test pattern for prod
SimplePatternList gPatterns = { IHOTT, breathingPrisim, fluxCapacitor, hourglass, rainbowGlitter, confetti, sinelon, juggle, bpm , prideUp, prideDown , pacifica };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  startMillis = millis();

  if (millis() > transitionEndMillis) transitioning = false;
  
  if (transitioning) {
    fadeToBlackBy( leds, NUM_LEDS, 8);
    FastLED.delay(5);
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
  EVERY_N_SECONDS( 300 ) { nextPattern(); } // change patterns periodically TODO: increase this for prod
  EVERY_N_SECONDS( 342 ) { gReverseDirection = !gReverseDirection; }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // randomize the next animation?
  if ( SHUFFLE_ANIMATIONS ) {
    uint8_t oldPatternNumber = gCurrentPatternNumber;
    gCurrentPatternNumber = random8() % ARRAY_SIZE(gPatterns);
  
    if (ARRAY_SIZE(gPatterns) > 1) {
      while (gCurrentPatternNumber == oldPatternNumber){
        gCurrentPatternNumber = random8() % ARRAY_SIZE(gPatterns);
      }
    }
  }
  else {
    // add one to the current pattern number, and wrap around at the end
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
  }
  
  // add a fade for transition
  transitioning = true;
  transitionEndMillis = millis() + TRANSITION_LENGTH_MILLIS;
}

uint16_t scaleRange( uint16_t value, uint16_t lowest, uint16_t highest ) {
  uint16_t rangewidth = highest - lowest;
  uint16_t scaledbeat = scale16( value, rangewidth);
  return lowest + scaledbeat;
}

uint8_t prisimHue = 0;

void breathingPrisim() {
  prisimHue += 4;
  fadeToBlackBy( leds, NUM_LEDS, 30);

  uint16_t topPadding = scaleRange(beatsin16(50), 0, 6);
  uint16_t botPadding = scaleRange(beatsin16(52), NUM_LEDS_PER_STRIP - 7, NUM_LEDS_PER_STRIP - 1);
  
  for (int i=0; i < NUM_STRIPS; i++){
    leds[((NUM_LEDS_PER_STRIP * i) + topPadding)] = CHSV(60, 80, 255);
    leds[((NUM_LEDS_PER_STRIP * i) + botPadding)] = CHSV(gHue + (i * (255/NUM_STRIPS)), 255, 255);
  }

  // prisim effect particles
  uint8_t numParticles = 2;
  for (int i=0; i < numParticles; i++){
    uint16_t prisimColumn = scaleRange(beat16(120), 0, NUM_STRIPS - 1);
    uint16_t prisimRow = scaleRange(beat16(25), topPadding, NUM_LEDS_PER_STRIP - 1);
    prisimRow = ( prisimRow + (i * ( NUM_LEDS_PER_STRIP / numParticles))) % NUM_LEDS_PER_STRIP;
    
    if ( prisimRow < (NUM_LEDS_PER_STRIP / 2)) { // white sand top
      leds[((NUM_LEDS_PER_STRIP * prisimColumn) + prisimRow)] = CRGB(255, 255, 255);
    }
    else { // rainbow prisim bottom
      if (prisimRow <= botPadding) leds[((NUM_LEDS_PER_STRIP * prisimColumn) + prisimRow)] = CHSV(prisimHue, 255, 255);
    }
  }

  centerWhiteGlitter();
}

void hourglass() {
  gHue += 4;
  fadeToBlackBy( leds, NUM_LEDS, 10);
  uint8_t sandBrightness = 96;
  uint8_t sandSaturationMin = 200;
  uint8_t chanceOfGlitter = 32;
  uint8_t rainSpeed = 80;
  
  // value between 0-65535 that represents the overall animation timer
  uint16_t hourglassTimer = beat16(1); // 1 BPM, 60 seconds for a full animation loop
  uint16_t subTimer = hourglassTimer % 32768;
  uint16_t emptyRows = subTimer / (32768/(NUM_LEDS_PER_STRIP/2));

  if ( 0 < hourglassTimer && hourglassTimer < 32768){ // 0-32767: top draining, bottom accumulating
    // top draining
    for (int row = emptyRows; row < (NUM_LEDS_PER_STRIP/2); row++) {
      uint16_t column = random16(0, NUM_STRIPS);
      leds[ (column * NUM_LEDS_PER_STRIP ) +  row ] += CHSV(60,80,sandBrightness);
//      if( random8() < chanceOfGlitter) leds[ (column * NUM_LEDS_PER_STRIP ) +  row ] = CHSV(66,128,sandBrightness);
    }

    // bottom raining
    uint16_t rainAcceleration = scaleRange(beat16(2), 0, rainSpeed);
    uint16_t rainColumn = scaleRange(beat16(rainSpeed), 0, NUM_STRIPS - 1);
    uint16_t rainRow = scaleRange(beat16(40), (NUM_LEDS_PER_STRIP/2), (NUM_LEDS_PER_STRIP - 1) - emptyRows);
    leds[((NUM_LEDS_PER_STRIP * rainColumn) + rainRow)] = CHSV(gHue, random8(128,255), 255);
    
    // bottom accumulating
    for (int row = (NUM_LEDS_PER_STRIP-1); row > (NUM_LEDS_PER_STRIP-1) - emptyRows; row--) {
      uint16_t column = random16(0, NUM_STRIPS);
      leds[ (column * NUM_LEDS_PER_STRIP ) +  row ] += CHSV(60,80,sandBrightness);
      if( random8() < chanceOfGlitter) leds[ (column * NUM_LEDS_PER_STRIP ) +  row ] = CHSV(random8(),random8(sandSaturationMin,255),sandBrightness);
    }
  }
  else { // 32768-65535: bottom draining, top accumulating
    // bottom draining
    for (int row = (NUM_LEDS_PER_STRIP - 1) - emptyRows; row >= NUM_LEDS_PER_STRIP/2; row--) {
      uint16_t column = random16(0, NUM_STRIPS);
      leds[ (column * NUM_LEDS_PER_STRIP ) +  row ] += CHSV(60,80,sandBrightness);
//      if( random8() < chanceOfGlitter) leds[ (column * NUM_LEDS_PER_STRIP ) +  row ] = CHSV(random8(),random8(sandSaturationMin,255),sandBrightness);
    }

    // top raining
    uint16_t rainAcceleration = scaleRange(beat16(2), 0, rainSpeed);
    uint16_t rainColumn = scaleRange(beat16(rainSpeed), 0, NUM_STRIPS - 1);
    uint16_t rainRow = scaleRange(beat16(40), 0, ((NUM_LEDS_PER_STRIP/2) - 1) - emptyRows);
    rainRow = ((NUM_LEDS_PER_STRIP / 2) - 1) - rainRow;
    leds[((NUM_LEDS_PER_STRIP * rainColumn) + rainRow)] = CHSV(gHue, random8(128,255), 255);
    
    // top accumulating
    for (int row = 0; row < emptyRows; row++) {
      uint16_t column = random16(0, NUM_STRIPS);
      leds[ (column * NUM_LEDS_PER_STRIP ) +  row ] += CHSV(60,80,sandBrightness);
      if( random8() < chanceOfGlitter) leds[ (column * NUM_LEDS_PER_STRIP ) +  row ] = CHSV(random8(),random8(sandSaturationMin,255),sandBrightness);
    }
  }

//  meteorHoops(true);
  pulsedHoops(beat8(10), 255);
}

void rainbowGlitter() {
  addGlitter(200, CHSV(random8(), 255, 255));
  addGlitter(140, CHSV(random8(), random8(), 255));
  
  fadeToBlackBy( leds, NUM_LEDS, 4);
  meteorHoops(true);
}

void IHOTT() {
//  gHue +=3;
  fadeToBlackBy( leds, NUM_LEDS, 20); // TODO: replace with a blend?

  // set a base animation for all leds
  for(int i = 0; i < NUM_STRIPS; i++) {
    uint16_t row = scaleRange(beat16(40+i), 0, (NUM_LEDS_PER_STRIP / 2) - 1);
    
    leds[(i*NUM_LEDS_PER_STRIP) + row] = CHSV(gHue+(row*3),255,255);
    leds[(i*NUM_LEDS_PER_STRIP) + (NUM_LEDS_PER_STRIP - row)] = CHSV(gHue+( (row*3)),255,255);
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
  fadeToBlackBy( leds, NUM_LEDS, 6);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 255, 255);
  pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 255, 255);

  pulsedHoops(gHue, scaleRange(beatsin16(60), 0, 255));
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  gHue += 3;
  
  for(int i = 0; i < NUM_STRIPS; i++) {
    int pos = beatsin16( 20+i, 0, NUM_LEDS_PER_STRIP-1 );
    leds[(i*NUM_LEDS_PER_STRIP) + pos] = CHSV( gHue + (i*(255 / NUM_STRIPS)), 255, 255);
    leds[(i*NUM_LEDS_PER_STRIP) + 31 - pos] = CHSV( gHue + (i*(255 / NUM_STRIPS)), 255, 255);
  }

  centerWhiteGlitter();
  pulsedHoops(gHue, 0);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 60;
  CRGBPalette16 palette = RainbowStripeColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  pulsedHoops(gHue, scaleRange(beatsin16(60), 0, 255));
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    uint8_t dothue = 0;
    for( int j = 0; j < 8; j++) {
      leds[(NUM_LEDS_PER_STRIP * beatsin16( i+j+2, 0, NUM_STRIPS-1 ) + i)] |= CHSV(dothue, 255, 255);
      dothue += 32;
    }
  }
  pulsedHoops(gHue, 0);
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

CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

// In honor of Dan Garcia
void pacifica()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}

//#define COOLING  55
//#define SPARKING 120
//
//void Fire2012()
//{
//// Array of temperature readings at each simulation cell
//  static uint8_t heat[(NUM_LEDS_PER_STRIP / 2)];
//
//  // Step 1.  Cool down every cell a little
//    for( int i = 0; i < (NUM_LEDS_PER_STRIP / 2); i++) {
//      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / (NUM_LEDS_PER_STRIP / 2)) + 2));
//    }
//  
//    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
//    for( int k= (NUM_LEDS_PER_STRIP / 2) - 1; k >= 2; k--) {
//      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
//    }
//    
//    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
//    if( random8() < SPARKING ) {
//      int y = random8(7);
//      heat[y] = qadd8( heat[y], random8(160,255) );
//    }
//
//    // Step 4.  Map from heat cells to LED colors
//    for(int i = 0; i < NUM_STRIPS; i++) {
//      for( int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
//        CRGB color = HeatColor( heat[j]);
//        int pixelnumber;
//        if( gReverseDirection ) {
//          pixelnumber = ((NUM_LEDS_PER_STRIP / 2)-1) - j;
//        } else {
//          pixelnumber = j;
//        }
//        leds[(i*NUM_LEDS_PER_STRIP) + pixelnumber] = color;
//      }
//    }
//}
