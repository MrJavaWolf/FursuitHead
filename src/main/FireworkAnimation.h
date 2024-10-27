#ifndef FIREWORK_ANIMATION_H  // include guard
#define FIREWORK_ANIMATION_H

#include "GameTime.h"
#include "PointRenderer.h"
#include "easing.h"

#define NUMBER_OF_SPARK_LIGHTS 100
#define NUMBER_OF_ROCKET_LIGHTS 20

struct BaseFireWorkLightPoint {
  bool enabled = false;
  float position = 0;
  float size = 5;
  uint32_t color = 0;
  float alfa = 1;
};

struct Rocket : BaseFireWorkLightPoint {
  float start_time = 0;
  float life_time = 0;
  float speed = 0.5;
  float start_speed = 0.5;
};

struct Spark : BaseFireWorkLightPoint {
  float start_time = 0;
  float life_time = 0;
  float speed = 0.5;
  float start_speed = 0.5;
};

class FireworkAnimation {

  PointRenderer point_renderer;

  Spark sparks[NUMBER_OF_SPARK_LIGHTS];
  Rocket rockets[NUMBER_OF_ROCKET_LIGHTS];

  Adafruit_NeoPXL8HDR *leds;
  GameTime *game_time;
  float last_spawn_fire_time = 0;
  float max_fire_spawn_rate = 0.5;

public:
  FireworkAnimation(Adafruit_NeoPXL8HDR *Leds, GameTime *Game_Time) {
    leds = Leds;
    game_time = Game_Time;
  }

  void spawn_rocket() {
    if (last_spawn_fire_time + max_fire_spawn_rate > game_time->total_time_s) {
      return;
    }

    for (int i = 0; i < NUMBER_OF_ROCKET_LIGHTS; i++) {
      if (rockets[i].enabled)
        continue;
      last_spawn_fire_time = game_time->total_time_s;
      rockets[i].color = leds->ColorHSV(i * 7000, 150, 150);
      rockets[i].enabled = true;

      if (random(0, 2) == 0) {
        rockets[i].position = 200;
      } else {
        rockets[i].position = 250;
      }

      rockets[i].start_time = game_time->total_time_s;
      rockets[i].life_time = 1.5;
      rockets[i].size = 3;
      rockets[i].speed = 15 + random(0, 15);
      rockets[i].start_speed = rockets[i].speed;
      return;
    }
  }

  void spawn_spark(float position, uint32_t color) {
    for (int i = 0; i < NUMBER_OF_SPARK_LIGHTS; i++) {
      if (sparks[i].enabled)
        continue;
      sparks[i].color = color;
      sparks[i].enabled = true;
      sparks[i].position = position;
      sparks[i].start_time = game_time->total_time_s;
      sparks[i].life_time = 0.5 + (random(0, 75) / 100.0f);
      sparks[i].size = 3;
      sparks[i].speed = 10 + random(0, 10);
      if (random(0, 2) == 0) {
        sparks[i].speed = -sparks[i].speed;
      }
      sparks[i].start_speed = sparks[i].speed;
      return;
    }
  }

  void update() {
    update_rockets();
    update_sparks();
  }

  void update_rockets() {
    for (int i = 0; i < NUMBER_OF_ROCKET_LIGHTS; i++) {
      if (rockets[i].enabled == false)
        continue;
      if (game_time->total_time_s > rockets[i].start_time + rockets[i].life_time) {
        rockets[i].enabled = false;
        spawn_spark(rockets[i].position, rockets[i].color);
        spawn_spark(rockets[i].position, rockets[i].color);
        spawn_spark(rockets[i].position, rockets[i].color);
        spawn_spark(rockets[i].position, rockets[i].color);
        continue;
      }

      rockets[i].position += rockets[i].speed * game_time->delta_time_s;
      float life_ratio = (game_time->total_time_s - rockets[i].start_time) / rockets[i].life_time;
      easingFunction ef = getEasingFunction(EaseOutCubic);
      rockets[i].speed = rockets[i].start_speed * ef(1 - life_ratio);
      rockets[i].alfa = 1 - life_ratio;
    }
  }

  void update_sparks() {
    for (int i = 0; i < NUMBER_OF_SPARK_LIGHTS; i++) {

      if (game_time->total_time_s > sparks[i].start_time + sparks[i].life_time) {
        sparks[i].enabled = false;
      }

      sparks[i].position += sparks[i].speed * game_time->delta_time_s;
      float life_ratio = (game_time->total_time_s - sparks[i].start_time) / sparks[i].life_time;
      easingFunction ef = getEasingFunction(EaseOutCubic);
      sparks[i].speed = sparks[i].start_speed * ef(1 - life_ratio);
      sparks[i].alfa = 1 - life_ratio;
    }
  }

  void renderer() {
    for (int i = 0; i < NUMBER_OF_SPARK_LIGHTS; i++) {
      renderer_light_point(&(sparks[i]));
    }

    for (int i = 0; i < NUMBER_OF_ROCKET_LIGHTS; i++) {
      renderer_light_point(&(rockets[i]));
    }
  }

  void renderer_light_point(BaseFireWorkLightPoint *light_point) {
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