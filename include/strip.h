#pragma once
#include <ESP8266WiFi.h>

#include <Adafruit_NeoPixel.h>

#include <config.h>
#include <color.h>


enum setPixelMode {
    GAP_FILL,
    MIDDLE_OUT,
    DEFAULT_PIX
};


void strip_init();
void strip_show();
void strip_set_brightness(uint8_t brightness);
void setPixel(uint16_t num, uint32_t color, setPixelMode mode = DEFAULT_PIX, boolean rainbow = false, uint8_t wheel_color = 0);
