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

#include "utility.h"

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
float util::steinhart(const uint16_t& raw)
{
  float ret = constrain(raw, 1, 1022);
  ret  = THERMISTOR_SERIESRESISTOR / (1023.0 / ret - 1.0);  // convert raw to ohms
  ret  = ret / THERMISTOR_NOMINAL_VAL;                      // (R/Ro)
  ret  = log(ret);                                          // ln(R/Ro)
  ret /= THERMISTOR_BCOEFFICIENT;                           // 1/B * ln(R/Ro)
  ret += 1.0 / (THERMISTOR_NOMINAL_TEMP + 273.15);          // + (1/To)
  ret  = 1.0 / ret;                                         // invert
  ret -= 273.15;                                            // convert to K to C
  return ret;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
uint16_t util::hexstr2long(const char* str)
{
  uint16_t n = 0;
  n = strtol(str, NULL, 16);
  return (n);
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
uint16_t util::get_timer_prescaler(const uint8_t &idx)
{
  switch(idx)
  {
    case 0:
      return timer_prescaler[(TCCR0B & 0x6F)][1];

    case 1:
      return timer_prescaler[(TCCR1B & 0x6F)][1];

    case 2:
      return timer_prescaler[(TCCR2B & 0x6F)][1];

    default: return 0;
  }
}

#ifdef HAS_ACCELERATION
  /**
   * @brief Linear interpolation function
   * @details Linear interpolation on a set of data points (x0, y0), (x1, y1),
   *          is defined as the concatenation of linear interpolants between
   *          each pair of data points. [https://bit.ly/2JP5SIh]
   *
   */
  float util::lerp(float const &edge0, float const &edge1, float x) {
    return (1.0 - x) * edge0 + x * edge1;
  }

  /**
   * @brief [brief description]
   * @details [long description]
   *
   */
  float util::clamp(float x, float const &lowerlimit, float const &upperlimit)
  {
    if      (x < lowerlimit) x = lowerlimit;
    else if (x > upperlimit) x = upperlimit;
    return x;
  }

  #ifdef USE_SMOOTHSTEP_ACCEL
    /**
     * @brief Sigmoid-like interpolation and clamping function
     * @details 3rd-order equation [https://bit.ly/2la5Ek0]
     *
     */
    float util::smoothstep(float const &edge0, float const &edge1, float x)
    {
      // Scale, and clamp x to 0..1 range
      x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
      // Evaluate polynomial
      return x * x * (3.0 - 2.0 * x);
    }


    /**
     * @brief Sigmoid-like interpolation and clamping function
     * @details 5th-order equation [https://bit.ly/2JUMe0I]
     *
     */
    float util::smootherstep(float const &edge0, float const &edge1, float x)
    {
      // Scale, and clamp x to 0..1 range
      x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
      // Evaluate polynomial
      return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
    }


    /**
     * @brief Sigmoid-like interpolation and clamping function
     * @details 7th-order equation [https://bit.ly/2MtinL2]
     *
     */
    float util::smootheststep(float const &edge0, float const &edge1, float x)
    {
      // Scale, and clamp x to 0..1 range
      x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
      // Evaluate polynomial
      return x * x * x * x * ( x * (x * (- x * 20.0  + 70.0) - 84.0) + 35.0);
    }
  #endif
#endif
