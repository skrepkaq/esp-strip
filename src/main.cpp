#include <ESP8266WiFi.h>
#include <network.h>
#include <effects.h>


Effects effects;
Network network(effects);


void setup() {
  ESP.wdtDisable();
  Serial.begin(115200);
  randomSeed(analogRead(A0));
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  strip_init();
  effects.set_effect(0);
  effects.set_wheel_color(random(255));
  network.wifi_init();
  network.handle_mqtt();
}

void loop() {
  ESP.wdtFeed();
  
  network.handle_wifi();
  network.handle_mqtt();

  effects.show();
}
