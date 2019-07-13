/*
 * DMXFader.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: jeremy
 *  Modified by PsikoBlock on 2017-07-23
 */

#include "DMXFader.h"


DMXFader::DMXFader(DMXESPSerial &dmx_universe, uint16_t dmx_channel) {
  universe = &dmx_universe;
  channel = dmx_channel;
  value = 0;
  to_value = 0;
  last_step_time = 0;
  interval = 0;
  duration = 0;
  percent_done = 0;
  curve = (curve_function)0;
}

void DMXFader::set_universe(DMXESPSerial &dmx_universe) {
  universe = &dmx_universe;
}

DMXESPSerial& DMXFader::get_universe(){
  return *universe;
}

void DMXFader::set_channel(uint16_t dmx_channel) {
  channel = dmx_channel;
}

uint16_t DMXFader::get_channel(){
  return channel;
}

void DMXFader::set_value(uint8_t new_value) {
  if (!channel || !universe) return;
  value = new_value;
  if (curve) {
    universe->write(channel, curve(value)); 
  } else {
    universe->write(channel, value); 
  }
}

uint8_t DMXFader::get_value() {
  return value;
}

uint8_t DMXFader::get_target_value() {
  return to_value;
}
    
// Set curve to transform output
void DMXFader::set_curve(curve_function c) {
  curve = c;
}

// Get the current curve function pointer
DMXFader::curve_function DMXFader::get_curve() {
  return curve;
}

void DMXFader::slower(int by) {
  float cached_percent = percent_done;
  duration += by;
  fade(to_value, duration);
  percent_done = cached_percent;
}

void DMXFader::faster(int by) {
  float cached_percent = percent_done;

  // Ends the fade
  if (duration <= by) {
    stop_fade();
    set_value(to_value);
  } else {
    duration -= by;
    fade(to_value, duration);
  }
  percent_done = cached_percent;
}

void DMXFader::fade(uint8_t new_value, unsigned int time) {
  stop_fade();
  percent_done = 0;

  // No channel defined
  if (!channel || !universe) {
    return;
  }

  // value hasn't changed
  if (new_value == value) {
    return;
  }

  if (time <= MIN_INTERVAL) {
    set_value(new_value);
    return;
  }

  duration = time;
  to_value = new_value;

  // Figure out what the interval should be so that we're changing the value by at least 1 each cycle
  // (minimum interval is MIN_INTERVAL)
  float value_diff = abs(value - to_value);
  interval = round((float)duration / value_diff);
  if (interval < MIN_INTERVAL) {
    interval = MIN_INTERVAL;
  }

  last_step_time = millis();
}

bool DMXFader::is_fading() {
  if (!channel || !universe) {
    return false;
  }
  
  if (duration > 0) {
    return true;
  }
  
  return false;
}

void DMXFader::stop_fade() {
  percent_done = 100;
  duration = 0;
}

uint8_t DMXFader::get_progress() {
  return round(percent_done);
}

bool DMXFader::update() {

  // No channel defined
  if (!channel || !universe) {
    return false;
  }

  // No fade
  if (duration == 0) {
    return false;
  }

  unsigned long now = millis();
  unsigned int time_diff = now - last_step_time;

  // Interval hasn't passed yet
  if (time_diff < interval) {
    return true;
  }

  // How far along have we gone since last update
  float percent = (float)time_diff / (float)duration;
  percent_done += percent;

  // We've hit 100%, set channel to the final value
  if (percent >= 1) {
    stop_fade();
    set_value(to_value);
    return false;
  }

  // Move value to where it should be
  int value_diff = to_value - value;
  int increment = round(value_diff * percent);

  set_value(value + increment);

  // Update time and finish
  duration -= time_diff;
  last_step_time = millis();
  return true;
}
