/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017 João Brázio [joao@brazio.org]
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

#include "ardufocus.h"

millis_t utils::millis()
{
  #ifdef ARDUINO
    return ::millis();
  #else
    unsigned long m;
    uint8_t oldSREG = SREG;

    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer0_millis)
    cli();

    m = timer0_millis;
    SREG = oldSREG;
    return m;
  #endif
}

void utils::delay(uint32_t ms)
{
  uint32_t start = micros();

  while (ms > 0) {
    wdt_reset();
    while (ms > 0 && (micros() - start) >= 1000) {
      ms--;
      start += 1000;
    }
  }
}

float utils::steinhart(const uint16_t& raw)
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

uint16_t utils::hexstr2long(const char* str)
{
  uint16_t n = 0;
  n = strtol(str, NULL, 16);
  return (n);
}
