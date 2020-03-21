#include "NeoPixel_Strobe.h"

NeoPixel_Strobe::NeoPixel_Strobe(uint16_t n, uint16_t pin, neoPixelType type) :
  Adafruit_NeoPixel(n, pin, type),
  strobe(false)
{
};

void NeoPixel_Strobe::setPixelColor(uint16_t n, uint32_t c)
{
  colorPixels[n] = c;
  
}

void NeoPixel_Strobe::clear()
{
  for ( int n = 0; n < NUM_STROBE_PIXELS; n++ ) {
   colorPixels[n] = 0;
  }
}

void NeoPixel_Strobe::show()
{
  for ( int n = 0; n < NUM_STROBE_PIXELS; n++ ) {
    uint32_t col = colorPixels[n];

    if (strobe) {
      if ( wOffset == rOffset) { // RGB device
        col = (0xFF << 16) | (0xFF < 8) || 0xFF;
      } else { // RGBW device
        col |= (strobe_brightness << 24);
      }
    }
    
    Adafruit_NeoPixel::setPixelColor(n, col);
  }
  
  Adafruit_NeoPixel::show();
}

void NeoPixel_Strobe::setStrobe(boolean val)
{
  strobe = val;
  show();
}
