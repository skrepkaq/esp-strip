#pragma once
#include <ESP8266WiFi.h>

#include <FastLED.h>

#include <config.h>
#include <strip.h>
#include <color.h>


#define PERLIN_MIDPOINT 150
#define PADDLE_DELAY 10
#define PADDLE_BUMP_NUM 25

#define MIN_COLORS_DISTANCE 5
#define MAX_MIX_DISTANCE 4


class Effects {
  public:
    void show();
    void show(uint8_t effect_num, uint32_t effect_color);
    void set_color(uint8_t r, uint8_t g, uint8_t b);
    void set_color(uint32_t c);
    void set_wheel_color(uint8_t pos);
    void set_effect(uint8_t num);
    void set_state(boolean state);
    void set_rainbow_state(boolean state);
    void set_brightness(uint8_t brightness);
    uint32_t get_color();
  private:
    void _fillStrip(uint32_t col);
    void _paddle(uint32_t color);
    void _rainbow();
    void _perlin();
    void _blinding_lights();
    void _color_river();

    void _show_strip(uint8_t effect_num, uint32_t effect_color);

    uint8_t rainbow_j;
    uint32_t color;
    uint32_t rainbow_color_last_tick;
    uint8_t perlin_hue_min;
    uint32_t effect_tick;
    uint16_t perlin_step;
    boolean direction;
    boolean leds_b[LED_COUNT];
    uint8_t rand_dots[LED_COUNT];
    uint32_t river_colors[LED_COUNT];
    uint8_t strip_mode;
    uint16_t x;
    uint32_t paddleBumpColor;
    uint8_t paddleBumpSteps;
    uint16_t paddlebarrier;
    uint8_t rainbow_color_hue;
    boolean rainbow_mode;
    boolean strip_state = 1;
};
