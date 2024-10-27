// Pinout: https://learn.adafruit.com/adafruit-feather-rp2040-pico/pinouts

#include <Adafruit_NeoPXL8.h>
#include "GameTime.h"
#include "DebouncedSwitch.h"
#include "EarLightAnimation.h"
#include "FireworkAnimation.h"


#define NOSE_BUTTON_PIN 24

#define LED_PIN_LEFT 11
#define LED_PIN_MIDDLE 12
#define LED_PIN_RIGHT 13
#define LED_PIN_BOARD 16

// How many NeoPixels are attached
#define LED_COUNT 50

//int8_t pins[3] = { LED_PIN_LEFT, LED_PIN_MIDDLE, LED_PIN_RIGHT };
int8_t pins[8] = { 6, 7, 9, 8, 13, 12, 11, 10 };
// Declare our NeoPixel strip object:
Adafruit_NeoPXL8HDR leds = Adafruit_NeoPXL8HDR(LED_COUNT, pins, NEO_GRB);

GameTime game_time;
DebouncedSwitch nose_button(NOSE_BUTTON_PIN);  // monitor a switch connected between input D4 and GND

EarLightAnimation ear_light_animation(&leds, &game_time);
FireworkAnimation firework_animation(&leds, &game_time);

uint16_t hue = 0;
uint8_t render = 0;

void setup() {
  Serial.begin(115200);
  pinMode(NOSE_BUTTON_PIN, INPUT_PULLUP);

  //leds.begin();
  //leds.setBrightness(50);
  leds.begin(true, 2, true);
  leds.setBrightness(65535, 2.6);  // 1/8 max duty cycle, 2.6 gamma factor
  leds.show();
}


void loop1() {
  leds.refresh();
}

void loop() {
  //delay(10);
  game_time.loop();
  nose_button.update();

  clear(&leds);
  firework_animation.update();
  firework_animation.renderer();

  if (nose_button.isDown()) {
    firework_animation.spawn_rocket();
  }
  leds.show();
}

void set_to_hue(Adafruit_NeoPXL8HDR *strip, uint16_t hue) {
  uint32_t color = strip->ColorHSV(hue, 255, 255);
  for (int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, color);
  }
  strip->show();
}

void clear(Adafruit_NeoPXL8HDR *strip) {
  uint32_t color = strip->Color(0, 0, 0);
  for (int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, color);
  }
}

void set_to_black(Adafruit_NeoPXL8HDR *strip) {
  uint32_t color = strip->Color(0, 0, 0);
  for (int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, color);
  }
  strip->show();
}
