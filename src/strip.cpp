#include "strip.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


void strip_init() {
  strip.begin();
  strip.show();
  strip.setBrightness(20);
}

void strip_show() {
  strip.show();
}

void strip_set_brightness(uint8_t brightness) {
  strip.setBrightness(brightness);
}


void setPixel(uint16_t num, uint32_t color, setPixelMode mode, boolean rainbow, uint8_t wheel_color) {
  if (rainbow && color != 0) {
    color = wheel(wheel_color);
  }
  switch (mode)
  {
  case DEFAULT_PIX:
    strip.setPixelColor(num, color);
    break;
  case GAP_FILL:
    strip.setPixelColor(num, color);
    if (num == 49 && color != 0) {
      strip.setPixelColor(48, color);
      strip.setPixelColor(47, color);
    }
    break;
  case MIDDLE_OUT:
    uint16_t dist = LED_COUNT/2-abs(LED_COUNT/2-STRIP_MID_POINT)-1;
    strip.setPixelColor(STRIP_MID_POINT + num - dist, color);
    strip.setPixelColor(STRIP_MID_POINT - num + dist, color);
    break;
  }
}
