#include "NeoPixel_Strobe.h"

#include "stdlib.h"

#define PIN_PORT_RING 12
#define PIN_STAR_RING A2
#define LEG_STRIP_1 7
#define LEG_STRIP_2 4
#define LEG_STRIP_3 3
#define LEG_STRIP_4 2

#define RING_PIXELS 16
#define LEG_PIXELS 16

#define BRIGHTNESS 255 // Max 255
#define LOOP_DELAY 10 // 100Hz main loop

enum MODE {
  INIT,
  RUNNING,
  ARMED,
  LANDING,
  TAKEOFF,
  COLLISION_AVIODANCE,
  ERROR,
  MUCH_ERROR
};


// LED Strips
NeoPixel_Strobe port_ring_strip = NeoPixel_Strobe(RING_PIXELS, PIN_PORT_RING, NEO_GRBW + NEO_KHZ800);
NeoPixel_Strobe star_ring_strip = NeoPixel_Strobe(RING_PIXELS, PIN_STAR_RING, NEO_GRBW + NEO_KHZ800);

NeoPixel_Strobe leg_1_strip = NeoPixel_Strobe(LEG_PIXELS, LEG_STRIP_1, NEO_GRB + NEO_KHZ800);
NeoPixel_Strobe leg_2_strip = NeoPixel_Strobe(LEG_PIXELS, LEG_STRIP_2, NEO_GRB + NEO_KHZ800);
NeoPixel_Strobe leg_3_strip = NeoPixel_Strobe(LEG_PIXELS, LEG_STRIP_3, NEO_GRB + NEO_KHZ800);
NeoPixel_Strobe leg_4_strip = NeoPixel_Strobe(LEG_PIXELS, LEG_STRIP_4, NEO_GRB + NEO_KHZ800);


void setup()
{
  initialiseNeoPixel( &port_ring_strip );
  initialiseNeoPixel( &star_ring_strip );

  initialiseNeoPixel( &leg_1_strip );
  initialiseNeoPixel( &leg_2_strip );
  initialiseNeoPixel( &leg_3_strip );
  initialiseNeoPixel( &leg_4_strip );
}


void loop()
{
  static uint32_t sLoopCount = 0;

  static uint8_t sMode = INIT;
  static boolean sModeChanged = true;

  if ( sModeChanged )
    sLoopCount = 0;

  mode_strobe(sLoopCount, sModeChanged);

  switch (sMode)
  {
  case INIT:
    mode_init(sLoopCount, sModeChanged);
    break;

  case RUNNING:
    mode_running(sLoopCount, sModeChanged);
    break;
  case ARMED:
    mode_armed(sLoopCount, sModeChanged);
    break;

  case LANDING:
    mode_landing(sLoopCount, sModeChanged);
    break;

  case TAKEOFF:
    mode_takeoff(sLoopCount, sModeChanged);
    break;

  case COLLISION_AVIODANCE:
    mode_collisionAvoid(sLoopCount, sModeChanged);
    break;

  case ERROR:
    mode_error(sLoopCount, sModeChanged);
    break;

  case MUCH_ERROR:
    mode_muchError(sLoopCount, sModeChanged);
    break;
  
  default:
    sMode = INIT;
    break;
  }

  if ( sModeChanged )
    sModeChanged = false;

  sLoopCount++;
  delay(LOOP_DELAY); // 100Hz counter

  if (sLoopCount % 400 == 0) {
    sMode++;
    sModeChanged = true;
  }
}

// Modes
void mode_init( uint32_t loopCount, boolean modeChanged ) {
  static uint32_t sCWOffset = 0;
  static uint32_t sCCWOffset = 0;

  if ( modeChanged ) {
    sCWOffset = 0;
    sCCWOffset = 0;
  }

  fillRainbow(&leg_1_strip, sCCWOffset);
  fillRainbow(&leg_2_strip, sCCWOffset);
  fillRainbow(&leg_3_strip, sCCWOffset);
  fillRainbow(&leg_4_strip, sCCWOffset);

  fillRainbow(&star_ring_strip, sCWOffset);
  fillRainbow(&port_ring_strip, sCCWOffset);

  sCWOffset += 500;
  sCCWOffset -= 500;
}

void mode_running(uint32_t loopCount, boolean modeChanged) {
  static boolean direction = true;
  static int16_t brightness = 0;

  if (modeChanged) {
    direction = true;
    brightness = 0;
  }
  
  uint32_t color = colorRGB(0, brightness, 0);

  fill(&leg_1_strip, color);
  fill(&leg_2_strip, color);
  fill(&leg_3_strip, color);
  fill(&leg_4_strip, color);

  fill(&star_ring_strip, color);
  fill(&port_ring_strip, color);

  if (direction)
    brightness += 2;
  else
    brightness -= 2;

  if (brightness < 0 ) {
    brightness = 0;
    direction = true;
  } else if (brightness > 255 ) {
    brightness = 255;
    direction = false;
  }
}

void mode_armed(uint32_t loopCount, boolean modeChanged) {
  if (modeChanged) {
    uint32_t color = colorRGB(0, 255, 0);

    fill(&leg_1_strip, color);
    fill(&leg_2_strip, color);
    fill(&leg_3_strip, color);
    fill(&leg_4_strip, color);

    fill(&star_ring_strip, color);
    fill(&port_ring_strip, color);
  }
}

void mode_landing( uint32_t loopCount, boolean modeChanged) {
  uint8_t offset = (loopCount / 20);
  uint32_t color = colorRGB(0, 0, 255);

  rain(&leg_1_strip, offset, color, 3, false);
  rain(&leg_2_strip, offset, color, 3, false);
  rain(&leg_3_strip, offset, color, 3, false);
  rain(&leg_4_strip, offset, color, 3, false);
  rain(&star_ring_strip, offset, color, 2, true);
  rain(&port_ring_strip, offset, color, 2, false);
}

void mode_takeoff( uint32_t loopCount, boolean modeChanged) {
  uint8_t offset = (loopCount / 20);
  uint32_t color = colorRGB(0, 255, 0);

  rain(&leg_1_strip, offset, color, 3, true);
  rain(&leg_2_strip, offset, color, 3, true);
  rain(&leg_3_strip, offset, color, 3, true);
  rain(&leg_4_strip, offset, color, 3, true);
  rain(&star_ring_strip, offset, color, 2, false);
  rain(&port_ring_strip, offset, color, 2, true);
}

void mode_collisionAvoid(uint32_t loopCount, boolean modeChanged) {
  uint8_t offset = (loopCount / 20);
  uint32_t color = colorRGB(128, 0, 128);

  rain(&leg_1_strip, offset, color, 2, true);
  rain(&leg_2_strip, offset, color, 2, true);
  rain(&leg_3_strip, offset, color, 2, true);
  rain(&leg_4_strip, offset, color, 2, true);
  rain(&star_ring_strip, offset, color, 2, true);
  rain(&port_ring_strip, offset, color, 2, true);
}

void mode_error(uint32_t loopCount, boolean modeChanged) {
  uint8_t strobeCount = loopCount % 40;
  uint32_t color;
  
  if (strobeCount > 20)
    color = colorRGB(0, 0, 0);
  else
    color = colorRGB(255, 0, 0);

  fill(&leg_1_strip, color);
  fill(&leg_2_strip, color);
  fill(&leg_3_strip, color);
  fill(&leg_4_strip, color);

  fill(&star_ring_strip, color);
  fill(&port_ring_strip, color);
}

void mode_muchError(uint32_t loopCount, boolean modeChanged) {
  uint8_t strobeCount = loopCount % 40;
  uint32_t color;
  
  if ((strobeCount > 3 && strobeCount < 8) \
    || (strobeCount > 12 && strobeCount < 17) \
    || (strobeCount > 21 && strobeCount < 16))
    color = colorRGB(255, 0, 0);
  else
    color = colorRGB(0, 0, 0);

  fill(&leg_1_strip, color);
  fill(&leg_2_strip, color);
  fill(&leg_3_strip, color);
  fill(&leg_4_strip, color);

  fill(&star_ring_strip, color);
  fill(&port_ring_strip, color);
}

void mode_strobe( uint32_t loopCount, boolean modeChanged ) {
  uint8_t strobeCount = loopCount % 100;
  if ( strobeCount == 0 || strobeCount == 1 \
     || strobeCount == 7 || strobeCount == 8 \
     || strobeCount == 14 || strobeCount == 15 )
  {
    star_ring_strip.setStrobe(true);
    port_ring_strip.setStrobe(true);
  } 
  else
  {
    star_ring_strip.setStrobe(false);
    port_ring_strip.setStrobe(false);
  }
}


// LED Strips
void initialiseNeoPixel( NeoPixel_Strobe* strip )
{
  strip->begin();
  strip->setBrightness(BRIGHTNESS);
  strip->show();
}

void clearNeopixel( NeoPixel_Strobe* strip )
{
  strip->clear();
  strip->show();
}



// Light styles
void fill(NeoPixel_Strobe* strip, uint32_t color) {
  for (int8_t i = 0; i < strip->numPixels(); i++ ) {
    strip->setPixelColor(i, color);
  }
  strip->show();
}

void fillRainbow(NeoPixel_Strobe* strip, uint32_t rainbowOffset) {
  for (int8_t i = 0; i < strip->numPixels(); i++ ) {
    uint32_t c;
    c = colorRainbow(i, rainbowOffset, strip->numPixels());
    strip->setPixelColor(i, c);
  }
  strip->show();
}

void colorWipe(NeoPixel_Strobe* strip, uint32_t loopCount, uint8_t offset, uint32_t c)
{
  const uint8_t startPause = 1;
  const uint8_t endPause = 1;
  const uint8_t loopLength = strip->numPixels() + startPause + endPause;

  int16_t count = loopCount >> 3; // Scale to 25Hz
  uint8_t i = count % loopLength;

  // Loop lifecycle managment
  if (i < startPause)
  {
    strip->clear();
    strip->show();
  }

  // Set LED values
  int8_t ledIndex = i - startPause;

  if ( ledIndex >= strip->numPixels() )
  {
    ledIndex = strip->numPixels();
  }

  for (int8_t j = offset; j < ledIndex + offset; j++)
  {
    if ( j >= strip->numPixels() )
    {
      strip->setPixelColor(j % strip->numPixels(), c);
    } else {
      strip->setPixelColor(j, c);
    }
  }

  strip->show();
}

void rain(NeoPixel_Strobe* strip, uint32_t loopCount, uint32_t color, uint8_t spread, boolean direction) {
  uint8_t offset = loopCount % spread;

  strip->clear();
  for (uint8_t i = 0; i < strip->numPixels(); i++ ) {
    int8_t j = direction ? i + offset : i - offset;

    if (j % spread == 0) {
      strip->setPixelColor(i, color);
    }
  }
  strip->show();
}


// Colour styles
uint32_t colorRainbow(uint32_t i, uint32_t rainbowOffset, int8_t numPixels) {
  uint32_t pixelHue = rainbowOffset + (i * 65536L / numPixels);

  return colorHSV(pixelHue, 255, 255);
}

uint32_t colorRGB(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

int32_t colorHSV(uint16_t hue, uint8_t sat, uint8_t val) {
  uint8_t r, g, b;

  hue = (hue * 1530L + 32768) / 65536;
  
  // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
  if(hue < 510) {         // Red to Green-1
    b = 0;
    if(hue < 255) {       //   Red to Yellow-1
      r = 255;
      g = hue;            //     g = 0 to 254
    } else {              //   Yellow to Green-1
      r = 510 - hue;      //     r = 255 to 1
      g = 255;
    }
  } else if(hue < 1020) { // Green to Blue-1
    r = 0;
    if(hue <  765) {      //   Green to Cyan-1
      g = 255;
      b = hue - 510;      //     b = 0 to 254
    } else {              //   Cyan to Blue-1
      g = 1020 - hue;     //     g = 255 to 1
      b = 255;
    }
  } else if(hue < 1530) { // Blue to Red-1
    g = 0;
    if(hue < 1275) {      //   Blue to Magenta-1
      r = hue - 1020;     //     r = 0 to 254
      b = 255;
    } else {              //   Magenta to Red-1
      r = 255;
      b = 1530 - hue;     //     b = 255 to 1
    }
  } else {                // Last 0.5 Red (quicker than % operator)
    r = 255;
    g = b = 0;
  }

  // Apply saturation and value to R,G,B, pack into 32-bit result:
  uint32_t v1 =   1 + val; // 1 to 256; allows >>8 instead of /255
  uint16_t s1 =   1 + sat; // 1 to 256; same reason
  uint8_t  s2 = 255 - sat; // 255 to 0
  return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
          (((((g * s1) >> 8) + s2) * v1) & 0xff00)       |
         ( ((((b * s1) >> 8) + s2) * v1)           >> 8);
}
