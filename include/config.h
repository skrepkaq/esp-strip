#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define MQTT_BROKER "MQTT_BROKER"
#define MQTT_USERNAME "MQTT_USERNAME"
#define MQTT_PASSWORD "MQTT_PASSWORD"
#define MQTT_PORT 1883

#define DNS_HOST "esp8266strip"
#define UPDATE_PATH "/update"
#define UPDATE_USERNAME "admin"
#define UPDATE_PASSWORD "admin?"

#define RELAY_PIN 0
#define LED_PIN 3
#define LED_COUNT 60
#define STRIP_MID_POINT 30


/*fail save - при невозможности подключиться к wifi,
создаёт свою wifi точку к которой можно подключиться и загрузить прошивку
(адрес обычно 192.168.1.4)*/
#define HOST_WIFI_AP 1
#if HOST_WIFI_AP
#define HOSTED_WIFI_SSID     "Esp8266-strip-wifi"
#define HOSTED_WIFI_PASSWORD "my_esp_passwd"
#endif
