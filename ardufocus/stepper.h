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

#ifndef __STEPPER_H__
#define __STEPPER_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "util.h"
#include "macro.h"
#include "lookuptable.h"

class stepper
{
  public:
    struct position_t
    {
      bool moving;      // When set to true it will start the movement
      uint16_t current, // Absolution position
               target;  // Target value for the step counter

      #ifdef HAS_ACCELERATION
      uint16_t distance,  // Move distance
               relative,  // Relative position
               easein,    // Step where easein stops
               easeout;   // Step where easeout starts
      #endif
    };

  protected:
    uint8_t m_mode;
    volatile uint16_t m_speed;
    volatile uint8_t  m_counter;
    volatile position_t m_position;

  public:
    void tick();

  private:
    #ifdef HAS_ACCELERATION
    __attribute__((always_inline)) inline void update_freq();
    #endif
    __attribute__((always_inline)) inline void update_position(const int8_t&);

  public:
    virtual void init();

    void move();
    bool is_moving();
    virtual void halt();

    uint16_t get_current_position();
    void     set_current_position(const uint16_t&);

    uint16_t get_speed();
    void     set_speed(const uint16_t&);

    uint16_t get_target_position();
    void     set_target_position(const uint16_t&);

    virtual uint8_t       get_step_mode();
    virtual inline void   set_full_step() { ; }
    virtual inline void   set_half_step() { ; }

    virtual inline bool step_cw()  { return false; }
    virtual inline bool step_ccw() { return false; }
};

#endif
