#pragma once
#include <ESP8266WiFi.h>

#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

#include <config.h>
#include <effects.h>
#include <color.h>


class Network {
  public:
    Network(Effects &effects);
    void handle_wifi();
    void handle_mqtt();
    void wifi_init();
  private:
    boolean _wifi_connect();
    void _callback(char *topic, byte *payload, unsigned int length);

    uint8_t wifi_tries, mqtt_tries;
    Effects &effects;
};
