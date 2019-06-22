/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017-2019 João Brázio [joao@brazio.org]
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

#include "analog.h"

/**
 * @brief Static class member initialization
 */
uint16_t Analog::s_cache[4] = { 0, 0, 0, 0 };
Analog::buffer_t Analog::s_buffer = { 0, 255, {} };

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
ISR(ADC_vect)
{
  // store the raw value from the ADC
  Analog::s_buffer.raw[Analog::s_buffer.n++] = ADCW;

  // after reaching the sample count filter the results
  if(Analog::s_buffer.n == asizeof(Analog::s_buffer.raw)) {
    // we could shave a few uS from the ISR by using a static inline
    // function instead of the lambda, but who cares ?
    qsort(Analog::s_buffer.raw, asizeof(Analog::s_buffer.raw), sizeof(uint16_t),
      [](const void* a, const void* b) {
        return ( *(int*)a - *(int*)b );
      }
    );

    // uses a basic 95th percentile filter with an additional discarding
    // of the lower 5% results.
    uint32_t sum = 0;
    const uint8_t start = asizeof(Analog::s_buffer.raw) * 0.05f; // lower 5%
    const uint8_t end   = asizeof(Analog::s_buffer.raw) - start; // sort of 95th percentile
    for(uint8_t i = start; i < end; i++) { sum += Analog::s_buffer.raw[i]; }
    const uint16_t avg = (uint16_t) (sum / (end - start));

    // Store the average analog reading into the cache
    Analog::s_cache[Analog::s_buffer.chan] = avg;
    Analog::s_buffer.chan  = 255;

    // Disable the async analog read ISR
    ADCSRA &= ~(bit (ADSC) | bit (ADIE));
  }

  else { ADCSRA |= bit(ADSC) | bit(ADIE); }
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void Analog::read_async(const uint8_t& channel)
{
  if(channel > 3) { return; }
  if(Analog::s_buffer.chan != 255) { return; }

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    // erase any data on the buffer
    for(uint8_t i = 0; i < asizeof(Analog::s_buffer.raw); i++) {
      Analog::s_buffer.raw[i] = 0;
    }

    Analog::s_buffer.n = 0;
    Analog::s_buffer.chan = channel;

    // select the internal 1.1V aref and the target analog channel
    ADMUX = bit (REFS1) | bit (REFS0) | (channel & 0x07);

    // start the async analog read
    ADCSRA |= bit(ADSC) | bit(ADIE);
  }
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void Analog::setup()
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    // clear adc prescaler bits
    ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));

    #ifdef ADC_PRESCALER16
      // set ADC prescaler select to 16 (upto 76,923 conversions/sec)
      ADCSRA |= bit (ADPS2);
    #else
      // set ADC prescaler select to 128 (upto 9,615 conversions/sec)
      ADCSRA |= bit(ADPS2) | bit(ADPS1) | bit(ADPS0);
    #endif

    // set ADC Enable
    ADCSRA |= bit(ADEN);

    // set ADC multiplexer selection to internal 1.1V
    ADMUX  |= bit(REFS1) | bit(REFS0);

    // set digital input disable register to A4-A5
    DIDR0 |= bit(ADC5D) | bit(ADC4D);
  }

  // wait a bit
  util::delay_5ms();
}
