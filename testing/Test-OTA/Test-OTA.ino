#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FastLED.h>

#define FASTLED_ESP32_I2S
#define NUM_LEDS_PER_STRIP 32
#define NUM_STRIPS 18
#define BRIGHTNESS  255
#define NUM_LEDS 576

const char* ssid = "Electromagnetic Farts 2.4GHz";
const char* password = "nevertrustafart";
// 192.168.1.138

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
unsigned long startMillis;
unsigned long endMillis;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  // OTA Setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
   ArduinoOTA.setHostname("IHOTT Hourglass");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP addressssss: ");
  Serial.println(WiFi.localIP());

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

void loop() {
  ArduinoOTA.handle();

  startMillis = millis();
//  Serial.println("Loop....");
  
//  static uint8_t hue = 0;
//  for(int i = 0; i < NUM_STRIPS; i++) {
//    for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
////      leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32*i) + hue+j,192,255);
//      leds[(i*NUM_LEDS_PER_STRIP) + j] = CRGB(255,255,255);
//    }
//  }
//
//  hue++;
  if (millis() % 20000 > 10000) prideDown();
  else prideUp();

  FastLED.show();

  // Print FPS over serial
  endMillis = millis();
  unsigned long loopMillis = endMillis - startMillis;
  Serial.print("FPS: ");
  Serial.println(1000 / loopMillis);
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
