/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017-2018 João Brázio [joao@brazio.org]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "util.h"

float util::steinhart(const uint16_t& raw)
{
  float ret = constrain(raw, 1, 1022);
  ret  = THERMISTOR_SERIESRESISTOR / (1023.0 / ret - 1);  // convert raw to ohms
  ret  = ret / THERMISTOR_NOMINAL_VAL;                    // (R/Ro)
  ret  = log(ret);                                        // ln(R/Ro)
  ret /= THERMISTOR_BCOEFFICIENT;                         // 1/B * ln(R/Ro)
  ret += 1.0 / (THERMISTOR_NOMINAL_TEMP + 273.15);        // + (1/To)
  ret  = 1.0 / ret;                                       // invert
  ret -= 273.15;                                          // convert to K to C
  return ret;
}

uint16_t util::hexstr2long(const char* str)
{
  uint16_t n = 0;
  n = strtol(str, NULL, 16);
  return (n);
}

#ifdef HAS_ACCELERATION
  float util::lerp(float const &edge0, float const &edge1, float x) {
    return (1.0 - x) * edge0 + x * edge1;
  }

  float util::clamp(float x, float const &lowerlimit, float const &upperlimit)
  {
    if      (x < lowerlimit) x = lowerlimit;
    else if (x > upperlimit) x = upperlimit;
    return x;
  }

  #ifdef USE_SMOOTHSTEP_ACCEL
    // 5th-order equation
    float util::smoothstep(float const &edge0, float const &edge1, float x)
    {
      // Scale, and clamp x to 0..1 range
      x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
      // Evaluate polynomial
      return x * x * (3.0 - 2.0 * x);
    }

    // 5th-order equation
    float util::smootherstep(float const &edge0, float const &edge1, float x)
    {
      // Scale, and clamp x to 0..1 range
      x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
      // Evaluate polynomial
      return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
    }

    // 7th-order equation
    float util::smootheststep(float const &edge0, float const &edge1, float x)
    {
      // Scale, and clamp x to 0..1 range
      x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
      // Evaluate polynomial
      return x * x * x * x * ( x * (x * (- x * 20.0  + 70.0) - 84.0) + 35.0);
    }
  #endif
#endif
