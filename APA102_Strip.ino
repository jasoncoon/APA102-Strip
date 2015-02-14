#include <FastLED.h>

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define LED_PIN     1
#define CLOCK_PIN   0
#define COLOR_ORDER GBR
#define CHIPSET     APA102
#define NUM_LEDS    16

uint8_t brightness = 8;

CRGB leds[NUM_LEDS];

CRGB solidColor = CRGB::Red;

uint8_t hue = 0; // rotating "base color" used by many of the patterns

void setup()
{
  //  delay(3000); // sanity delay
  //  Serial.println("ready...");

  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(12)>(leds, NUM_LEDS);
  FastLED.setCorrection(0x80FFFF); // UncorrectedColor TypicalLEDStrip TypicalPixelString
  FastLED.setBrightness(brightness);
}

void loop()
{
  hueCycle();

  FastLED.show(); // display this frame
  FastLED.delay(30);
  
  hue++;
}

// scale the brightness of the screenbuffer down
void dimAll(byte value)
{
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i].nscale8(value);
  }
}

uint16_t showSolidColor() {
  fill_solid(leds, NUM_LEDS, solidColor);

  return 60;
}

uint16_t rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, hue, 7);

  return 8;
}

uint16_t rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
  return 8;
}

void addGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

uint16_t confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(hue + random8(64), 200, 255);
  return 8;
}

uint16_t bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
  }
  return 8;
}

uint16_t juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  return 8;
}

// An animation to play while the crowd goes wild after the big performance
uint16_t applause()
{
  static uint16_t lastPixel = 0;
  fadeToBlackBy(leds, NUM_LEDS, 32);
  leds[lastPixel] = CHSV(random8(HUE_BLUE, HUE_PURPLE), 255, 255);
  lastPixel = random16(NUM_LEDS);
  leds[lastPixel] = CRGB::White;
  return 8;
}

// An "animation" to just fade to black.  Useful as the last track
// in a non-looping performance-oriented playlist.
uint16_t fadeToBlack()
{
  fadeToBlackBy(leds, NUM_LEDS, 10);
  return 8;
}

uint16_t sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV(hue, 255, 192);
  return 8;
}

uint16_t hueCycle() {
  fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 255));
  return 60;
}


