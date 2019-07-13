/*
 * DMXFader.h
 *
 *  Created on: Sep 24, 2013
 *      Author: jeremy
 *  Modified by PsikoBlock on 2017-07-23
 */

#include "Arduino.h"
#include <ESPDMX.h>

#ifndef DMXFader_H_
#define DMXFader_H_

// The minimum time (milliseconds) the program will wait between channel adjustments
// adjust this to modify performance.
// max DMX frequency is 44 Hz ~ 23 ms
#define MIN_INTERVAL 50

class DMXFader {
  public:
    // Who likes dealing with function pointers? (Ok, I do, but no one else does)
    typedef uint8_t (*curve_function)(uint8_t);

  private:
    DMXESPSerial *universe;
    uint16_t channel;
    unsigned long last_step_time;
    unsigned int interval;
    uint8_t value;
    uint8_t to_value;
    unsigned int duration;
    float percent_done;
    curve_function curve;

  public:
    // Create a new DMX Fader for a channel
    DMXFader(DMXESPSerial &dmx_universe, uint16_t dmx_channel = 1);

    // Set the DMX universe (instance of ESPDMX)
    void set_universe(DMXESPSerial &dmx_universe);
    DMXESPSerial& get_universe();

    // Set the DMX channel
    void set_channel(uint16_t dmx_channel);
    uint16_t get_channel();

    // Set an channel to an absolute value
    void set_value(uint8_t value);

    // Get the current channel value
    uint8_t get_value();

    // Get the value we're fading to
    uint8_t get_target_value();
    
    // Set curve to transform output
    void set_curve(curve_function);
    
    // Get the current curve function pointer
    curve_function get_curve();

    // Fade an channel to a value over a duration of time (milliseconds)
    void fade(uint8_t newValue, unsigned int time);

    // Returns TRUE if there is an active fade process
    bool is_fading();

    // Stop the current fade where it's at
    void stop_fade();

    // Update the channels along the fade
    // Returns TRUE if a fade is still in process
    bool update();

    // Decrease the current fading speed by a number of milliseconds
    void slower(int by_seconds);

    // Increase the current fading speed by a number of milliseconds
    void faster(int by_seconds);

    // Returns how much of the fade is complete in a percentage between 0 - 100
    uint8_t get_progress();
};

#endif /* DMXFader_H_ */
