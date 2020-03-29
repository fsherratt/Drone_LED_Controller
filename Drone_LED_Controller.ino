#include <Adafruit_NeoPixel.h>
#include "NeoPixel_Strobe.h"

#define PIN_PORT_RING 12
#define PIN_STAR_RING A2
#define LEG_STRIP_1 7
#define LEG_STRIP_2 4
#define LEG_STRIP_3 3
#define LEG_STRIP_4 2

#define RING_PIXELS 16
#define LEG_PIXELS 16

#define BRIGHTNESS 10 // Max 255
#define LOOP_DELAY 10 // 100Hz main loop

#define DEFAULT 0
#define OTHER 1

NeoPixel_Strobe port_ring_strip = NeoPixel_Strobe(RING_PIXELS, PIN_PORT_RING, NEO_GRBW + NEO_KHZ800);
NeoPixel_Strobe star_ring_strip = NeoPixel_Strobe(RING_PIXELS, PIN_STAR_RING, NEO_GRBW + NEO_KHZ800);

NeoPixel_Strobe leg_1_strip = NeoPixel_Strobe(LEG_PIXELS, LEG_STRIP_1, NEO_GRB + NEO_KHZ800);
NeoPixel_Strobe leg_2_strip = NeoPixel_Strobe(LEG_PIXELS, LEG_STRIP_2, NEO_GRB + NEO_KHZ800);
NeoPixel_Strobe leg_3_strip = NeoPixel_Strobe(LEG_PIXELS, LEG_STRIP_3, NEO_GRB + NEO_KHZ800);
NeoPixel_Strobe leg_4_strip = NeoPixel_Strobe(LEG_PIXELS, LEG_STRIP_4, NEO_GRB + NEO_KHZ800);

uint16_t gLoopCount;
uint8_t mode;

void setup()
{
  mode = DEFAULT;
  gLoopCount = 0;

  initialiseNeoPixel( &port_ring_strip );
  initialiseNeoPixel( &star_ring_strip );

  initialiseNeoPixel( &leg_1_strip );
  initialiseNeoPixel( &leg_2_strip );
  initialiseNeoPixel( &leg_3_strip );
  initialiseNeoPixel( &leg_4_strip );
}

void loop()
{
  static boolean modeChanged = false;

  if ( modeChanged )
  {
    modeChanged = false;
    gLoopCount = 0;
  }

  colorWipe(&leg_1_strip, leg_1_strip.Color(0,0,255));
  colorWipe(&leg_2_strip, leg_2_strip.Color(255,0,0));

  colorWipe(&star_ring_strip, star_ring_strip.Color(0,0,255));
  colorWipe(&port_ring_strip, port_ring_strip.Color(255,0,0));

  
  uint8_t strobeCount = gLoopCount % 100;
  if ( strobeCount == 0 || strobeCount == 1 || strobeCount == 10 || strobeCount == 11 )
  {
    star_ring_strip.setStrobe(true);
    port_ring_strip.setStrobe(true);
  } 
  else
  {
    star_ring_strip.setStrobe(false);
    port_ring_strip.setStrobe(false);
  }

  gLoopCount++;
  delay(LOOP_DELAY); // 100Hz counter
}


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


boolean colorWipe(NeoPixel_Strobe* strip, uint32_t c)
{
  const uint8_t startPause = 1;
  const uint8_t endPause = 1;
  const uint8_t loopLength = strip->numPixels() + startPause + endPause;

  boolean finished = false;

  int16_t count = gLoopCount >> 3; // Scale to 25Hz
  uint8_t i = count % loopLength;

  if (i < startPause)
  {
    strip->clear();
    strip->show();
    return false;
  }

  if ( i-startPause < strip->numPixels() )
  {
    strip->setPixelColor(i-startPause, c);
    strip->show();
  }

  if ( i == loopLength)
  {
    finished = true;
  }

  return finished;
}
