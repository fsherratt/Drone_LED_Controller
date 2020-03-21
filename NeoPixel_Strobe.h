#include <Adafruit_NeoPixel.h>

#pragma once

#define NUM_STROBE_PIXELS 16

class NeoPixel_Strobe : public Adafruit_NeoPixel
{
  public:
    NeoPixel_Strobe(uint16_t n, uint16_t pin=6, neoPixelType type=NEO_GRB + NEO_KHZ800);

    void setPixelColor(uint16_t n, uint32_t c);
    void clear();
    void show();

    void setStrobe(boolean val);

    const uint8_t strobe_brightness = 0xFF;

  private:
    boolean strobe;
    uint32_t colorPixels[NUM_STROBE_PIXELS];
};
