#pragma once
#include <ESP8266WiFi.h>

#include <config.h>


uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);
uint32_t wheel(byte WheelPos);
uint32_t mixColors(uint32_t color1, uint32_t color2, float_t ratio);
