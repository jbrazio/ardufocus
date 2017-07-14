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

void serial::print::number(uint32_t n, const uint8_t& base, const uint8_t& digits)
{
  unsigned char buf[digits];
  uint8_t i = 0;

  for (; i < digits; i++) {
    buf[i] = n % base;
    n /= base;
  }

  for (; i > 0; i--) {
    if (i < digits && base == 2 && !((digits - i) % 4))
      serial::print::chr::space();
    serial::write('0' + buf[i - 1]);
  }
}

void serial::print::float32(float n, const uint8_t& decimal_places)
{
  if (n < 0) {
    serial::print::chr::minus();
    n = -n;
  }

  uint8_t decimals = decimal_places;

  // Quickly convert values expected to be E0 to E-4.
  while (decimals >= 2) {
    n *= 100;
    decimals -= 2;
  }

  if (decimals) { n *= 10; }
  n += 0.5; // Add rounding factor. Ensures carryover through entire value.

  // Generate digits backwards and store in string.
  unsigned char buf[10];
  uint8_t i = 0;
  uint32_t a = (long)n;

  // Place decimal point, even if decimal places are zero.
  buf[decimal_places] = '.';

  while(a > 0) {
    if (i == decimal_places) { i++; } // Skip decimal point location
    buf[i++] = (a % 10) + '0'; // Get digit
    a /= 10;
  }

  // Fill in zeros to decimal point for (n < 1)
  while (i < decimal_places)
    buf[i++] = '0';

  // Fill in leading zero, if needed.
  if (i == decimal_places) {
    i++;
    buf[i++] = '0';
  }

  // Print the generated string.
  for (; i > 0; i--)
    serial::write(buf[i-1]);
}
