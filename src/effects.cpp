#include "effects.h"


void Effects::_perlin() {
  if (millis() - effect_tick > 30) {
    for (byte i=0; i < LED_COUNT; i++) {
      uint16_t perl_wheel = inoise8(i*13, perlin_step);
      if (rainbow_mode) {
        perl_wheel += perlin_hue_min;
        if (perl_wheel > 255) {
          setPixel(i, mixColors(wheel(255), 0xFFFFFF, (float)(perl_wheel % 255) / (100*2)));
        } else {
          setPixel(i, wheel(perl_wheel));
        }
      } else {
        if (perl_wheel < PERLIN_MIDPOINT) {
          setPixel(i, mixColors(color, 0x0, (float)(map(perl_wheel, 0, PERLIN_MIDPOINT, 90, 0))/100));
        } else {
          setPixel(i, mixColors(color, 0xFFFFFF, (float)(map(perl_wheel, PERLIN_MIDPOINT, 255, 0, 40))/100));
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
        setPixel(i, 0xFFFFFF);
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

void removeElementFromArray(Comet arr[], uint8_t index) {
  for (uint8_t i = index; i < MAX_COMETS-1; ++i)
    arr[i] = arr[i+1];
}

void removeElementFromArray(Explosion arr[], uint8_t index) {
  for (uint8_t i = index; i < MAX_COMETS-1; ++i)
    arr[i] = arr[i+1];
}

void Effects::_comets() {
  if (millis() - effect_tick > 30) {
    if (comets_count < MAX_COMETS) {
      if (random(100) > 97 || comets_count == 0) {
        // создание новой кометы
        uint32_t color;
        if (random(10) > 7) {
          color = random(1, 0xFFFFFF);
        } else {
          color = wheel(random(256));
        }

        uint8_t min_speed = 0;
        for (uint8_t i = 0; i < comets_count; i++) {
          if (comets[i].speed == 0) {
            min_speed = 1;
          }
        }

        uint16_t pos;
        uint16_t iterations = 0;
        boolean clear_pos;
        // поиск пустой области для кометы 
        while (iterations < 1000) {
          pos = random(LED_COUNT);
          clear_pos = true;
          for (int8_t d = -8; d < 9; d++) {
            if (pos + d >= LED_COUNT)
              continue;
            if (comet_strip[pos+d] != 0) {
              clear_pos = false;
              break;
            }
          }
          if (clear_pos)
            break;
          iterations++;
        }

        if (clear_pos) {
          Comet comet {pos, (uint8_t)random(min_speed, COMET_MAX_SPEED), (boolean)random(2), color};
          comets[comets_count] = comet;
          comets_count++;
        }
      }
    }
    for (uint8_t cycle = 0; cycle < COMET_MAX_SPEED+1; cycle++) {
      // обработка движения comet.speed раз за цикл
      for (uint8_t i = 0; i < comets_count; i++) {
        Comet &comet = comets[i];
        if (cycle < comet.speed) {
          if (random(100) > 98) {
            // случайное изменение скорости кометы
            uint8_t new_speed = constrain(comet.speed + random(-1, 2), 1, COMET_MAX_SPEED);
            for (uint8_t j = 0; j < comets_count; j++) {
              if (comets[j].speed != new_speed && i != j) {
                comet.speed = new_speed;
              }
            }
          }

          // движение кометы
          if (comet.pos == LED_COUNT - 1 && comet.direction) {
            comet.pos = 0;
          } else if (comet.pos == 0 && !comet.direction) {
            comet.pos = LED_COUNT - 1;
          } else {
            comet.pos += comet.direction*2-1;
          }
          for (uint8_t j = 0; j < comets_count; j++) {
            for (uint8_t c = 0; c < explosions_count; c++) {
              if (comets[j].pos == explosions[c].pos && explosions[c].age > 1) {
                // столкновение с ядром взрыва
                removeElementFromArray(comets, j);
                comets_count--;
              }
            }
            if (comets[j].pos == comet.pos && i != j) {
              // столкновение с кометой
              if (explosions_count < MAX_EXPLOSIONS) {
                Explosion explosion {comet.pos};
                explosions[explosions_count] = explosion;
                explosions_count++;
                uint32_t exp_color = mixColors(comet.color, comets[j].color, 0.5);
                if (comets[j].speed == 0 && comet.speed > 1) {
                  exp_color = comets[j].color;
                }
                for (uint8_t n = 0; n < EXPLOSION_DISTANCE; n++) {
                  uint32_t dimmed_exp_color = mixColors(exp_color, 0, (float)n/(EXPLOSION_DISTANCE+1));
                  if (explosion.pos + n < LED_COUNT)
                    comet_strip[explosion.pos + n] = dimmed_exp_color;
                  if (explosion.pos - n >= 0)
                    comet_strip[explosion.pos - n] = dimmed_exp_color;
                }
              }
              if (comets[j].speed != 0 || comet.speed == 1) {
                removeElementFromArray(comets, i);
                comets_count--;
              }
              removeElementFromArray(comets, j);
              comets_count--;
            }
          }
        }
        if (comet_strip[comet.pos] == 0) {
          comet_strip[comet.pos] = comet.color;
        } else {
          comet_strip[comet.pos] = mixColors(comet.color, comet_strip[comet.pos], 0.2);
        }
      }
    }
    for (int i=0; i<LED_COUNT; i++) {
      setPixel(i, comet_strip[i], DEFAULT_PIX);
    }
    for (uint8_t i = 0; i < explosions_count; i++) {
      Explosion &explosion = explosions[i];
      explosion.age++;
      if (explosion.age > 15) {
        removeElementFromArray(explosions, i);
        explosions_count--;
      } else {
        comet_strip[explosion.pos] = 0xDDDDDD;
      }
      // свечение взрыва
      for (uint16_t j = explosion.pos-1; j < explosion.pos+2; j++)
        setPixel(j, 0xDDDDDD, DEFAULT_PIX);
    }
    strip_show();
    for (uint16_t i = 0; i < LED_COUNT; i++) {
      comet_strip[i] = mixColors(comet_strip[i], 0, 0.1);
    }
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
    case 6:
      _comets();
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
