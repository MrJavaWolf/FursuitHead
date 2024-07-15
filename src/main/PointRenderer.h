#ifndef POINT_RENDERER_H  // include guard
#define POINT_RENDERER_H

#include <Adafruit_NeoPXL8.h>


class PointRenderer {

public:
  void renderer(Adafruit_NeoPXL8HDR *leds, float position, float size, uint32_t color, float alpha = 1) {

    float middle_posision = position + size / 2;

    for (int i = -2; i < size + 2; i++) {
      int neopixel_index = (int)position + i;
      if (neopixel_index < 0 || neopixel_index >= leds->numPixels()) {
        continue;
      }

      float distance = abs(neopixel_index - middle_posision);
      float ratio = (size / 2 - distance) / size;
      if (ratio < 0) ratio = 0;
      ratio = ease(ratio);
      uint8_t pixel_red = scale_color((uint8_t)(color >> 16), ratio * alpha);
      uint8_t pixel_green = scale_color((uint8_t)(color >> 8), ratio * alpha);
      uint8_t pixel_blue = scale_color((uint8_t)(color), ratio * alpha);
      if (pixel_red == 0 && pixel_green == 0 && pixel_blue == 0) {
        continue;
      }

      uint32_t c = leds->getPixelColor(neopixel_index);
      uint8_t combined_red = add_colors((uint8_t)(c >> 16), pixel_red);
      uint8_t combined_green = add_colors((uint8_t)(c >> 8), pixel_green);
      uint8_t combined_blue = add_colors((uint8_t)(c), pixel_blue);
      leds->setPixelColor(neopixel_index, combined_red, combined_green, combined_blue);
    }
  }

  uint8_t add_colors(uint8_t color1, uint8_t color2) {
    uint16_t final_color = (uint16_t)(color1) + (uint16_t)(color2);
    if (final_color > 255) {
      final_color = 255;
    }
    return (uint8_t)(final_color);
  }

  uint8_t blend_colors(uint8_t a, uint8_t b, float t = 0.5f){
    float blend = (1.0f - t) * a * a + t * b * b;
    blend = sqrt(blend);
      if (blend > 255) {
      blend = 255;
    }
    return (uint8_t)blend;

  }

  uint8_t scale_color(uint8_t color, float ratio) {
    float color_as_float = (float)(color);
    color_as_float = color_as_float * ratio;
    uint8_t color_as_uint = (uint8_t)(color_as_float);
    return color_as_uint;
  }

  float ease(float t) {
    return 1 - pow(2, -8 * t);
  }
};
#endif