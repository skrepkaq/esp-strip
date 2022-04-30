# Лента.
Ну, это лента на адресных светодиодах WS2812B управляемая esp8266 по MQTT и заточенная под работу с Алисой через [yandex2mqtt](https://github.com/munrexio/yandex2mqtt). Я пишу для неё разные режимы, ну и всё вообщем то.

Проект этот чисто для себя, но вот решил выложить. Тапками за архитектуру и код просьба не кидать, у меня лапки

## Режимы
0. fill - Полное заполнение ленты одним цветом
1. paddle - Понг (попробуйте сами крч)
2. rainbow - Бегущая радуга
3. perlin - Штука на шуме Перлина
4. blinding_lights - Тип звёздочки такие
5. color_river - Случные, плавно сменяющие друг друга цвета, бегущие из центра ленты


P.S. У некоторых режимов (1, 2, 4, 5) можно включить радужный режим. Для этого нужно установить 16729907 цвет на лете (коралловый в Алисе) 

## Подключение
1. Подключите ленту к ```LED_PIN``` пину
2. Опционально, реле (для управления светом например) к ```RELAY_PIN``` пину

## Прошивка
1. Установите [PlatformIO](https://platformio.org/install/integration/)
2. Откройте проект в IDE
3. Введите данные вашего WIFI и MQTT в include/config.h файле
```cpp
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define MQTT_BROKER "MQTT_BROKER"
#define MQTT_USERNAME "MQTT_USERNAME"
#define MQTT_PASSWORD "MQTT_PASSWORD"
#define MQTT_PORT 1883
```
4. Скомпилируйте и загрузите прошивку в ESP

## Управление
#### MQTT TOPIC wall/ - управление лентой

wall/state - 1/0

wall/color - 32 битный цвет

wall/mode - номер режима

wall/brightness - яркость 0-100

#### MQTT TOPIC bathroom/light/ - управление реле

bathroom/light/state - 1/0
