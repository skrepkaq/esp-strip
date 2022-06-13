#include "network.h"


WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;


Network::Network(Effects &effects): effects(effects) {
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->_callback(topic, payload, length); });
}


boolean Network::_wifi_connect() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  uint32_t wifi_try = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    effects.show(1, 0xFF0000);
    ESP.wdtFeed();
    if (millis() - wifi_try > 15000) {
      wifi_tries++;
      WiFi.disconnect();
      delay(100);
      #if HOST_WIFI_AP
      if (wifi_tries > 3) {
        WiFi.mode(WIFI_AP);
        delay(100);
        WiFi.softAP(HOSTED_WIFI_SSID, HOSTED_WIFI_PASSWORD);
        //IPAddress ip = WiFi.softAPIP();
        MDNS.begin(DNS_HOST);
        httpUpdater.setup(&httpServer, UPDATE_PATH, UPDATE_USERNAME, UPDATE_PASSWORD);
        httpServer.begin();
        MDNS.addService("http", "tcp", 80);
        while (true) {
          httpServer.handleClient();
        }
      }
      #endif
      return false;
    }
  }
  wifi_tries = 0;
  return true;
}


void Network::handle_wifi() {
  while (WiFi.status() != WL_CONNECTED) {
    if (_wifi_connect()) {
      uint32_t wait_for = millis() + 1000;
      while (millis() < wait_for) {
        ESP.wdtFeed();
        effects.show(1, 0xFF8000);
      }
      MDNS.begin(DNS_HOST);

      httpUpdater.setup(&httpServer, UPDATE_PATH, UPDATE_USERNAME, UPDATE_PASSWORD);
      httpServer.begin();

      MDNS.addService("http", "tcp", 80);
    }
  }
  httpServer.handleClient();
  MDNS.update();
}


void Network::handle_mqtt() {
  if (WiFi.status() != WL_CONNECTED) return;
  while (!client.connected()) {
    effects.show(1, 0xFF8000);
    ESP.wdtFeed();
    mqtt_tries++;
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    if (client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      client.subscribe("wall/#");
      client.subscribe("bathroom/light/#");
      uint32_t wait_for = millis() + 1000;
      while (millis() < wait_for) {
        ESP.wdtFeed();
        effects.show(1, 0x00FF00);
      }

      client.publish("wall/color/stat", String(effects.get_color()).c_str());
      client.publish("bathroom/light/state/stat", String(digitalRead(RELAY_PIN)).c_str());
      client.publish("wall/state/stat", "1");
      client.publish("wall/brightness/stat", "8");
      client.publish("wall/mode/stat", "0");
      randomSeed(micros());
      mqtt_tries = 0;
    } else {
      #if WIFI_RECONECT_WHEN_MQTT_FAIL
      if (mqtt_tries > 5) {
        mqtt_tries = 0;
        WiFi.disconnect();
        delay(100);
      }
      #endif
      Serial.println("MQTT FAIL!");
      delay(10);
    }
  }

  client.loop();
}


void Network::wifi_init() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  handle_wifi();
}


void Network::_callback(char *t, byte *payload, unsigned int length) {
  uint32_t int_inp = 0;
  for (uint8_t i = 0; i < length; i++) {
    int_inp *= 10;
    int_inp += (int)payload[i] - '0';
  }
  String topic = t;
  if (String(topic) == "wall/color") {
    if (int_inp == 16729907) {
      effects.set_rainbow_state(true);
      return;
    }
    effects.set_color(int_inp);
    effects.set_state(true);
    effects.set_rainbow_state(false);
  } else if (topic == "wall/state") {
    if ((char)payload[0] == '1') {
      effects.set_state(true);
    } else {
      effects.set_state(false);
    }
  } else if (topic == "wall/brightness") {
    effects.set_brightness(map(int_inp, 0, 100, 0, 255));
    effects.set_state(true);
  } else if (topic == "wall/mode") {
    effects.set_effect(int_inp);
    effects.set_state(true);
  } else if (topic == "bathroom/light/state") {
    if ((char)payload[0] == '0') {
      digitalWrite(RELAY_PIN, LOW);
    } else {
      digitalWrite(RELAY_PIN, HIGH);
    }
  }
}
