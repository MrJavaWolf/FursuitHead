#ifndef EAR_LIGHT_ANIMATION_H  // include guard
#define EAR_LIGHT_ANIMATION_H

#include "GameTime.h"
#include "PointRenderer.h"
#include "easing.h"


#define NUMBER_OF_EXPLOSION_LIGHTS 8
#define NUMBER_OF_FLOATING_LIGHTS 8
#define NUMBER_OF_NOSE_BOOP_FIRE 20


struct BaseLightPoint {

  bool enabled = false;
  float position = 0;
  float size = 5;
  uint32_t color = 0;
  float alfa = 1;
};

struct LightPoint : BaseLightPoint {
  float speed = 0.5;
};

struct NoseBoopFirePoint : BaseLightPoint {
  float start_time = 0;
  float life_time = 0;
};


class EarLightAnimation {

  PointRenderer point_renderer;
  LightPoint ear_light_blob;

  LightPoint explosion_lights[NUMBER_OF_EXPLOSION_LIGHTS];
  LightPoint light_blob[NUMBER_OF_FLOATING_LIGHTS];
  NoseBoopFirePoint nose_boop_fire[NUMBER_OF_NOSE_BOOP_FIRE];

  Adafruit_NeoPXL8HDR *leds;
  GameTime *game_time;
  float last_spawn_fire_time = 0;
  float max_fire_spawn_rate = 0.2;


  int explosion_state = 0;
  float explosion_size = 26;
  float explosion_start_time = -1;
  bool is_exploding = false;
  float explosion_duration = 5;
  float ear_light_start_size = 4;


public:

  EarLightAnimation(Adafruit_NeoPXL8HDR *Leds, GameTime *Game_Time) {
    leds = Leds;
    game_time = Game_Time;
    initialize_ear_blob();
    for (int i = 0; i < NUMBER_OF_FLOATING_LIGHTS; i++) {
      light_blob[i].color = leds->ColorHSV(i * 7000);
      light_blob[i].position = 200 + i * 50;
      light_blob[i].speed = i + 1;
      light_blob[i].size = 4;
    }
  }

  void initialize_ear_blob() {
    ear_light_blob.color = leds->ColorHSV(1500, 200, 150);
    ear_light_blob.size = ear_light_start_size;
    ear_light_blob.enabled = true;
    ear_light_blob.position = 226;
  }

  void spawn_nose_boop_fire() {
    if (last_spawn_fire_time + max_fire_spawn_rate > game_time->total_time_s) {
      return;
    }

    if (is_exploding) {
      return;
    }

    for (int i = 0; i < NUMBER_OF_NOSE_BOOP_FIRE; i++) {
      if (nose_boop_fire[i].enabled) continue;
      last_spawn_fire_time = game_time->total_time_s;
      nose_boop_fire[i].color = leds->ColorHSV(i * 7000, 150, 150);
      nose_boop_fire[i].enabled = true;
      nose_boop_fire[i].position = 200;
      nose_boop_fire[i].start_time = game_time->total_time_s;
      nose_boop_fire[i].life_time = 1.5;
      nose_boop_fire[i].size = 3;
      return;
    }
  }

  void update() {
    update_nose_boop_fire();
    update_light_blobs();
    update_ear_animation();
    if (is_exploding) {
      exploding_loop();
    }
  }

  void exploding_start() {
    is_exploding = true;
    explosion_start_time = game_time->total_time_s;
  }

  void exploding_loop() {

    if (explosion_state == 0) {
      float time_used_in_explosion = game_time->total_time_s - explosion_start_time;
      float ratio = time_used_in_explosion / explosion_duration;
      if (explosion_duration < time_used_in_explosion) {
        //explosion_state = 1;
        is_exploding = false;
        initialize_ear_blob();
      }
    } else if (explosion_state == 1) {
      //explosion_state = 0;
    }
  }

  void update_nose_boop_fire() {
    for (int i = 0; i < NUMBER_OF_NOSE_BOOP_FIRE; i++) {
      if (nose_boop_fire[i].enabled == false) continue;
      if (game_time->total_time_s > nose_boop_fire[i].start_time + nose_boop_fire[i].life_time) {
        nose_boop_fire[i].enabled = false;
        ear_light_blob.size += 0.8;
        ear_light_blob.position -= 0.4;

        if (ear_light_blob.size > explosion_size && !is_exploding) {
          exploding_start();
        }
        continue;
      }
      float life_ratio = (game_time->total_time_s - nose_boop_fire[i].start_time) / nose_boop_fire[i].life_time;
      easingFunction ef = getEasingFunction(EaseOutCubic);
      float position = 200 + ef(life_ratio) * ((ear_light_blob.position - 200) + ear_light_blob.size / 2);
      nose_boop_fire[i].position = position;
      nose_boop_fire[i].alfa = 1 - life_ratio;
    }
  }

  void update_light_blobs() {
    for (int i = 0; i < NUMBER_OF_FLOATING_LIGHTS; i++) {
      light_blob[i].position += light_blob[i].speed * game_time->delta_time_s;
      if (light_blob[i].position > (float)350.0) {
        light_blob[i].position = 200 - light_blob[i].size;
      }
    }
  }

  void update_ear_animation() {
  }


  void renderer() {
    for (int i = 0; i < NUMBER_OF_FLOATING_LIGHTS; i++) {
      renderer_light_point(&(light_blob[i]));
    }

    for (int i = 0; i < NUMBER_OF_NOSE_BOOP_FIRE; i++) {
      renderer_light_point(&(nose_boop_fire[i]));
    }
    renderer_light_point(&ear_light_blob);
  }

  void renderer_light_point(BaseLightPoint *light_point) {
    if (!light_point->enabled) {
      return;
    }
    point_renderer.renderer(
      leds,
      light_point->position,
      light_point->size,
      light_point->color,
      light_point->alfa);
  }
};

#endif