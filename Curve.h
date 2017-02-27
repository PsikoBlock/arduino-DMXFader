/* 
 * File:   Curve.h
 * Author: cameron
 *
 * Created on October 22, 2013, 1:07 AM
 */

#ifndef CURVE_H
#define	CURVE_H

#include <pgmspace.h>

class Curve {
 static const uint16_t etable[] PROGMEM;
public:
 static uint16_t exponential(uint8_t);
 static uint16_t linear(uint8_t);
 static uint16_t reverse(uint8_t);
};

#endif	/* CURVE_H */

