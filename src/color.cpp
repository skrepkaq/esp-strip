#include "color.h"


uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return makeColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return makeColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return makeColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t mixColors(uint32_t c1, uint32_t c2, float_t ratio) {
  uint8_t cB1[3], cB2[3];
  cB1[0] = ((c1 & 0x00FF0000) >> 16) * (1 - ratio);
  cB1[1] = ((c1 & 0x0000FF00) >> 8) * (1 - ratio);
  cB1[2] =  (c1 & 0x000000FF) * (1 - ratio);
  cB2[0] = ((c2 & 0x00FF0000) >> 16) * ratio;
  cB2[1] = ((c2 & 0x0000FF00) >> 8) * ratio;
  cB2[2] =  (c2 & 0x000000FF) * ratio;
  return makeColor(cB1[0] + cB2[0], cB1[1] + cB2[1], cB1[2] + cB2[2]);
}
