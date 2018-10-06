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

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "version.h"
#include "config.h"

#include "macro.h"
#include "ringbuf.h"

class Serial
{
  /**
   * Disable the creation of an instance of this object.
   * This class should be used as a static class.
   */
  private:
     Serial() {;}
    ~Serial() {;}

  protected:
    typedef void (*callbackfunc_t)(const char*);

  public:
    // Being a bit lazy here, this buffer should be private to the class
    // and have a set of wrappers around it.. adding it to the TODO list.
    static struct buffer_t
    {
      Ringbuf<char, SERIAL_RXBUF_SZ> rx;
      Ringbuf<char, SERIAL_TXBUF_SZ> tx;
    } s_buffer;

  public:
    static char read();
    static void flush();
    static void setup();
    static bool available();
    static void write(const char&);
    static speed void process(callbackfunc_t);
};

#endif
