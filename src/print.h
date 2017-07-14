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

#ifndef __PRINT_H__
#define __PRINT_H__

#include "ardufocus.h"
#include "serial.h"

namespace serial
{
  namespace print
  {
    inline void string(const char* s)
    {
      while (*s)
        serial::write(*s++);
    }

    inline void PGM(const char* s)
    {
      char c;
      while ((c = pgm_read_byte(s))) {
        serial::write(c);
        ++s;
      }
    }

    void banner();

    namespace chr
    {
      inline void eol()
      {
        serial::print::PGM(string_eol);
      }

      inline void space()
      {
        serial::print::PGM(string_space);
      }

      inline void comma()
      {
        serial::print::PGM(string_comma);
      }

      inline void colon()
      {
        serial::print::PGM(string_colon);
      }

      inline void minus()
      {
        serial::print::PGM(string_minus);
      }

      inline void zero()
      {
        serial::print::PGM(string_zero);
      }
    }

    extern void number(uint32_t n, const uint8_t& base, const uint8_t& digits);

    inline void number(const uint32_t& n, const uint8_t& base)
    {
      if (!n) {
        serial::print::chr::zero();
        return;
      }

      uint32_t c = n;
      uint8_t digits = 0;
      while (c) { c /= 10; digits++; }
      serial::print::number(n, base, digits);
    }

    // uint8: 0 to 255
    inline void uint8(const uint8_t& n)
    {
      uint8_t digits = 3;
      if (n < 10) { digits = 1; }
      else if (n < 100) { digits = 2; }
      serial::print::number(n, 10, digits);
    }

    // int8: -127 to 127
    inline void int8(const int8_t& n)
    {
      if (n < 0) {
        serial::print::chr::minus();
        serial::print::uint8(-n);
      } else serial::print::uint8(n);
    }

    // uint16: 0 to 65535
    inline void uint16(const uint16_t& n)
    {
      serial::print::number(n, 10);
    }

    // int16: -32768 to 32767
    inline void int16(const int16_t& n)
    {
      if (n < 0) {
        serial::print::chr::minus();
        serial::print::uint16(-n);
      } else serial::print::uint16(n);
    }

    // uint32: 0 to 4294967295L
    inline void uint32(const uint32_t& n)
    {
      serial::print::number(n, 10);
    }

    // int32: -2147483648L to 2147483647L
    inline void int32(const int32_t& n)
    {
      if (n < 0) {
        serial::print::chr::minus();
        serial::print::uint32(-n);
      } else serial::print::uint32(n);
    }

    extern void float32(float n, const uint8_t& decimal_places);

    namespace base2
    {
      inline void uint8(const uint8_t& n)
      {
        serial::print::number(n, 2, 8);
      }

      inline void int8(const int8_t& n)
      {
        serial::print::number(n, 2, 8);
      }

      inline void uint16(const uint16_t& n)
      {
        serial::print::number(n, 2, 16);
      }

      inline void int16(const int16_t& n)
      {
        serial::print::number(n, 2, 16);
      }

      inline void uint32(const uint32_t& n)
      {
        serial::print::number(n, 2, 32);
      }

      inline void int32(const int32_t& n)
      {
        serial::print::number(n, 2, 32);
      }
    }

    namespace pair
    {
      inline void uint8(const char* s, const uint8_t& n)
      {
        serial::print::PGM(s);
        serial::print::chr::colon();
        serial::print::chr::space();
        serial::print::uint8(n);
        serial::print::chr::eol();
      }

      inline void int8(const char* s, const int8_t& n)
      {
        serial::print::PGM(s);
        serial::print::chr::colon();
        serial::print::chr::space();
        serial::print::int8(n);
        serial::print::chr::eol();
      }

      inline void uint16(const char* s, const uint16_t& n)
      {
        serial::print::PGM(s);
        serial::print::chr::colon();
        serial::print::chr::space();
        serial::print::uint16(n);
        serial::print::chr::eol();
      }

      inline void int16(const char* s, const int16_t& n)
      {
        serial::print::PGM(s);
        serial::print::chr::colon();
        serial::print::chr::space();
        serial::print::int16(n);
        serial::print::chr::eol();
      }

      inline void uint32(const char* s, const uint32_t& n)
      {
        serial::print::PGM(s);
        serial::print::chr::colon();
        serial::print::chr::space();
        serial::print::uint32(n);
        serial::print::chr::eol();
      }

      inline void int32(const char* s, const int32_t& n)
      {
        serial::print::PGM(s);
        serial::print::chr::colon();
        serial::print::chr::space();
        serial::print::int32(n);
        serial::print::chr::eol();
      }

      inline void float32(const char* s, float n, const uint8_t& decimal_places)
      {
        serial::print::PGM(s);
        serial::print::chr::colon();
        serial::print::chr::space();
        serial::print::float32(n, decimal_places);
        serial::print::chr::eol();
      }
    }
  }

  namespace println
  {
    inline void string(const char* s)
    {
      serial::print::string(s);
      serial::print::chr::eol();
    }

    inline void PGM(const char* s)
    {
      serial::print::PGM(s);
      serial::print::chr::eol();
    }

    inline void number(uint32_t n, const uint8_t& base, const uint8_t& digits)
    {
      serial::print::number(n, base, digits);
      serial::print::chr::eol();
    }

    inline void number(const uint32_t& n, const uint8_t& base)
    {
      serial::print::number(n, base);
      serial::print::chr::eol();
    }

    inline void uint8(const uint8_t& n)
    {
      serial::print::uint8(n);
      serial::print::chr::eol();
    }

    inline void int8(const int8_t& n)
    {
      serial::print::int8(n);
      serial::print::chr::eol();
    }

    inline void uint16(const uint16_t& n)
    {
      serial::print::uint16(n);
      serial::print::chr::eol();
    }

    inline void int16(const int16_t& n)
    {
      serial::print::int16(n);
      serial::print::chr::eol();
    }

    inline void uint32(const uint32_t& n)
    {
      serial::print::uint32(n);
      serial::print::chr::eol();
    }

    inline void int32(const int32_t& n)
    {
      serial::print::int32(n);
      serial::print::chr::eol();
    }

    inline void float32(float n, const uint8_t& decimal_places)
    {
      serial::print::float32(n, decimal_places);
      serial::print::chr::eol();
    }

    namespace base2
    {
      inline void uint8(const uint8_t& n)
      {
        serial::print::number(n, 2, 8);
        serial::print::chr::eol();
      }

      inline void int8(const int8_t& n)
      {
        serial::print::number(n, 2, 8);
        serial::print::chr::eol();
      }

      inline void uint16(const uint16_t& n)
      {
        serial::print::number(n, 2, 16);
        serial::print::chr::eol();
      }

      inline void int16(const int16_t& n)
      {
        serial::print::number(n, 2, 16);
        serial::print::chr::eol();
      }

      inline void uint32(const uint32_t& n)
      {
        serial::print::number(n, 2, 32);
        serial::print::chr::eol();
      }

      inline void int32(const int32_t& n)
      {
        serial::print::number(n, 2, 32);
        serial::print::chr::eol();
      }
    }
  }
};

#endif
