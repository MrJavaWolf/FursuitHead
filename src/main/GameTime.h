#ifndef GAME_TIME_H  // include guard
#define GAME_TIME_H


class GameTime {

public:

  // The latest millis time
  unsigned long last_millis_time = 0;

  // Total time in milliseconds  since the start of the game
  unsigned long total_time_ms = 0;

  // Total time in seconds  since the start of the game
  float total_time_s = 0;

  // The interval in milliseconds from the last frame to the current one
  unsigned long delta_time_ms = 0;

  // The interval in seconds from the last frame to the current one
  float delta_time_s = 0;

  // The current frame we are rendering
  unsigned long frame_count = 0;

  // Whether it should print stats periodically
  bool enable_print_stats = true;

  // When was the last time in milliseconds it printet stats
  unsigned long last_print_stats_time = 0;

  // How often it should print stats in milliseconds
  unsigned long print_stats_interval = 1000;

  // The number of frames at the last stat print time in milliseconds
  unsigned long last_print_stat_frame_count = 0;

  // True on the frame when a new second occoured
  bool new_second_frame = false;


  GameTime() {

    last_millis_time = millis();
  }

  void loop() {
    unsigned long new_time = millis();
    delta_time_ms = new_time - last_millis_time;
    last_millis_time = new_time;
    total_time_ms += delta_time_ms;
    total_time_s = ((float)(total_time_ms)) / 1000.0;

    delta_time_s = delta_time_ms / (float)1000;
    frame_count++;
    if (enable_print_stats && last_print_stats_time + print_stats_interval < total_time_ms) {
      print_stats();
      new_second_frame = true;
    } else {
      new_second_frame = false;
    }
  }

  void print_stats() {
    unsigned long time_since_last_print = total_time_ms - last_print_stats_time;
    unsigned long frames_since_last_print = frame_count - last_print_stat_frame_count;
    float fps = ((float)frames_since_last_print / (float)time_since_last_print) * 1000;
    Serial.print("fps: ");
    Serial.print(fps);
    Serial.print(", total_time_ms: ");
    Serial.print(total_time_ms);
    Serial.print(", delta_time_ms: ");
    Serial.print(delta_time_ms);
    Serial.print(", frame_count: ");
    Serial.print(frame_count);
    Serial.print(", time_since_last_print: ");
    Serial.print(time_since_last_print);
    Serial.print(", frames_since_last_print: ");
    Serial.print(frames_since_last_print);
    Serial.println("");

    last_print_stats_time = total_time_ms;
    last_print_stat_frame_count = frame_count;
  }

  float get_easing_value(float speed_multiplier) {
    float time = total_time_s * speed_multiplier;
    float easing_value = time - (int)time;
    if (easing_value > 0.5) {
      easing_value = 0.5 - (easing_value - 0.5);
    }
    easing_value = easing_value * 2;
    return easing_value;
  }

};


#endif