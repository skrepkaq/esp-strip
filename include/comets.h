#pragma once
#include <ESP8266WiFi.h>


struct Comet {
    uint16_t pos;
    uint8_t speed;
    boolean direction;
    uint32_t color;
};

struct Explosion
{
    uint16_t pos;
    uint8_t age;
};
