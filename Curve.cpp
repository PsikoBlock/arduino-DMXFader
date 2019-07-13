/* 
 * File:   Curve.cpp
 * Author: cameron
 * 
 * Created on October 22, 2013, 1:07 AM
 */

#include "Curve.h"

// If this wasn't in program memory, we'd run out of SRAM way to fast.
const uint8_t Curve::etable[256] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8,
  9, 9, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 14,
  14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21,
  22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32, 32,
  33, 34, 35, 35, 36, 37, 38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 49, 50,
  51, 52, 53, 54, 55, 57, 58, 59, 61, 62, 63, 65, 66, 68, 69, 71, 72, 74, 76,
  77, 79, 81, 83, 84, 86, 88, 90, 92, 94, 96, 98, 101, 103, 105, 108, 110, 112,
  115, 117, 120, 123, 125, 128, 131, 134, 137, 140, 143, 146, 150, 153, 156,
  160, 163, 167, 171, 174, 178, 182, 186, 190, 195, 199, 203, 208, 212, 217,
  222, 227, 232, 237, 242, 248, 255
};


uint8_t Curve::exponential(uint8_t i) {
  // If the compiler could build the above table for us, we could have "simply":
  //return !i ? 0 : round(exp(log(MAXOUTPUT) * i / MAXINPUT));

  // Need to read the table in a special way!
  return pgm_read_byte(&etable[i]);
}

uint8_t Curve::linear(uint8_t i) {
  return i;
}

uint8_t Curve::reverse(uint8_t i) {
  return 255-i;
}
