#include "effects.h"


void Effects::_perlin() {
  if (millis() - effect_tick > 30) {
    for (byte i=0; i < LED_COUNT; i++) {
      uint16_t perl_wheel = inoise8(i*13, perlin_step);
      if (rainbow_mode) {
        perl_wheel += perlin_hue_min;
        if (perl_wheel > 255) {
          setPixel(i, mixColors(wheel(255), makeColor(255, 255, 255), (float)(perl_wheel % 255) / (100*2)));
        } else {
          setPixel(i, wheel(perl_wheel));
        }
      } else {
        if (perl_wheel < PERLIN_MIDPOINT) {
          setPixel(i, mixColors(color, makeColor(0, 0, 0), (float)(map(perl_wheel, 0, PERLIN_MIDPOINT, 90, 0))/100));
        } else {
          setPixel(i, mixColors(color, makeColor(255, 255, 255), (float)(map(perl_wheel, PERLIN_MIDPOINT, 255, 0, 40))/100));
        }
      }
    }
    if (perlin_hue_min == 0 || (perlin_hue_min >= 100 && direction)) {
      direction = !direction;
    }
    perlin_hue_min += direction*2 - 1;
    strip_show();
    perlin_step += 5;
    effect_tick = millis();
  }
}


void Effects::_paddle(uint32_t color) {
  if (millis() - effect_tick > PADDLE_DELAY) {
    uint8_t size = constrain(LED_COUNT / 6, 3, 10); 
    memset(leds_b, 0, sizeof(leds_b));
    for (int i = 0; i < size; i++) {
      leds_b[x+i] = true;
    }
    if (x + size == LED_COUNT || x == 0 || x + size == paddlebarrier || x == paddlebarrier) {
      if (x + size == LED_COUNT || x == 0) {
        paddleBumpSteps = PADDLE_BUMP_NUM;
      } else {
        paddleBumpSteps = PADDLE_BUMP_NUM / 1.5;
      }
      if (x + size == paddlebarrier || x == paddlebarrier || paddlebarrier == 0) {
        paddlebarrier = 0;
        if (random(10) > 2) {
          paddlebarrier = random(1,LED_COUNT);
          while (x - 5 <= paddlebarrier && paddlebarrier <= x + size + 5) {
            paddlebarrier = random(1,LED_COUNT);
          }
        }
      }
      direction = !direction;
      paddleBumpColor = wheel(random(256));
    }
    uint32_t p_color;
    if (paddleBumpSteps > 0) {
      p_color = mixColors(color, paddleBumpColor, (float)paddleBumpSteps/PADDLE_BUMP_NUM);
      paddleBumpSteps--;
      delay(paddleBumpSteps);
    } else {
      p_color = color;
    }
    for (int i=0; i < LED_COUNT; i++) {
      if (leds_b[i]) {
        setPixel(i, p_color, GAP_FILL, rainbow_mode, rainbow_color_hue);
      } else {
        setPixel(i, 0);
      }
    }
    if (paddlebarrier != 0) {
      setPixel(paddlebarrier, paddleBumpColor);
    }
    strip_show();
    x += direction*2 - 1;
    effect_tick = millis();
  }
}


void Effects::_blinding_lights() {
  if (millis() - effect_tick > 30) {
    for (int i = 0; i<LED_COUNT; i++) {
      uint8_t dot = rand_dots[i];
      if (dot == 0) {
        if (random(1000)>990) {
          rand_dots[i] = 255;
        }
      } else {
        uint8_t speed = 15;
        if (rainbow_mode) speed = 10;
        if (dot >= speed) {
          rand_dots[i] -= speed;
        } else {
          rand_dots[i] = 0;
        }
      }
      if (dot == 255) {
        setPixel(i, makeColor(255, 255, 255));
      } else if (dot == 0) {
        setPixel(i, 0);
      } else if (rainbow_mode) {
        if (dot < 100) {
          setPixel(i, mixColors(0, wheel(dot), (float)dot/100));
        } else {
          setPixel(i, wheel(dot));
        }
      } else {
        setPixel(i, mixColors(0, color, (float)dot/254));
      }
    }
    strip_show();
    effect_tick = millis();
  }
}

void Effects::_rainbow() {
  if (millis() - effect_tick > 10) {
    uint16_t i;
    for(i=0; i < LED_COUNT; i++) {
      setPixel(i, wheel(((i * 256 * 2 / LED_COUNT) + rainbow_j) & 255));
    }
    strip_show();
    rainbow_j++;
    effect_tick = millis();
  }
}

void Effects::_fillStrip(uint32_t col) {
  for (int i=0; i<LED_COUNT; i++) {
    setPixel(i, col, DEFAULT_PIX, rainbow_mode, rainbow_color_hue);
  }
  strip_show();
}

void Effects::_color_river() {
  if (millis() - effect_tick > 30) {
    uint16_t first_dot;
    for (first_dot = 0; first_dot < LED_COUNT; first_dot++) {
      if (river_colors[first_dot] != 0) break;
    }
    uint32_t new_color = 0; 
    if (first_dot == LED_COUNT) {
      new_color = wheel(random(256));
    } else if (first_dot > MIN_COLORS_DISTANCE && random(100) > 75) {

      if (random(10) > 8) {
        new_color = random(1, 0xFFFFFF);
      } else {
        new_color = wheel(random(256));
      }
    }
    if (new_color) {
      uint8_t mix_lenght = first_dot - 1;
      if (mix_lenght > MAX_MIX_DISTANCE) mix_lenght = MAX_MIX_DISTANCE;
      uint8_t buff_lenght = first_dot-mix_lenght-1;
      for (uint16_t i = 1; i < first_dot; i++) {
        if (i <= buff_lenght/2+buff_lenght%2) {
          river_colors[i] = new_color;
        } else if (i>=first_dot-buff_lenght/2) {
          river_colors[i] = river_colors[first_dot];
        } else {
          uint8_t coef_i = i-(buff_lenght/2+buff_lenght%2)-1;
          river_colors[i] = mixColors(new_color, river_colors[first_dot], (float)(coef_i+1)/(mix_lenght+1));
        }
      }
      river_colors[0] = new_color;
    }

    if (!new_color) {
      for (uint16_t i = 0; i < first_dot; i++) {
        setPixel(i, river_colors[first_dot], MIDDLE_OUT);
      }
    }
    for (int i=0; i<LED_COUNT; i++) {
      if (river_colors[i]) {
        setPixel(i, river_colors[i], MIDDLE_OUT);
      }
    }
    strip_show();
    for (uint16_t i = LED_COUNT-1; i>0; i--) {
      river_colors[i] = river_colors[i-1];
    }
    river_colors[0] = 0;
    effect_tick = millis();
  }
}

void Effects::_show_strip(uint8_t effect_num, uint32_t effect_color) {

  if (!strip_state) {
    _fillStrip(0);
    return;
  }

  switch (effect_num) {
    case 0:
      _fillStrip(effect_color);
      break;  
    case 1:
      _paddle(effect_color);
      break;
    case 2:
      _rainbow();
      break;
    case 3:
      _perlin();
      break;
    case 4:
      _blinding_lights();
      break;
    case 5:
      _color_river();
      break;
  }
  if (rainbow_mode && millis() - rainbow_color_last_tick > 20) {
    rainbow_color_hue++;
    rainbow_color_last_tick = millis();
  }
}

void Effects::show() {
  _show_strip(strip_mode, color);
}

void Effects::show(uint8_t effect_num, uint32_t effect_color) {
  _show_strip(effect_num, effect_color);
}

void Effects::set_color(uint8_t r, uint8_t g, uint8_t b) {
  color = makeColor(r, g, b);
}

void Effects::set_color(uint32_t c) {
  color = c;
}

void Effects::set_wheel_color(uint8_t pos) {
  color = wheel(pos);
}

void Effects::set_effect(uint8_t num) {
  strip_mode = num;
}

void Effects::set_rainbow_state(boolean state) {
  rainbow_mode = state;
}

void Effects::set_brightness(uint8_t brightness) {
  strip_set_brightness(brightness);
}

void Effects::set_state(boolean state) {
  strip_state = state;
}

uint32_t Effects::get_color() {
  return color;
}
