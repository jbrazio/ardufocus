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

#include "log.h"

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
    uint8_t m_mode;                  // Stepping mode (1/1, 1/2, ..)
    volatile uint16_t m_speed;       // Stepping speed
    volatile position_t m_position;  // Absolute motor position
    volatile uint16_t m_ovf_counter; // Overflow counter

    bool m_sleep_when_idle;          // Disable power to the motor when idle
    bool m_invert_direction;         // Invert the direction of rotation
    bool m_compress_steps;           // Convert micro-stepping counting to full steps

    uint16_t m_max_speed = DEFAULT_MAX_SPEED;
    uint16_t m_set_speed = DEFAULT_MAX_SPEED;
    uint16_t m_min_speed = DEFAULT_MIN_SPEED;

  protected:
    #ifdef HAS_ACCELERATION
    inline speed void update_freq();
    #endif
    inline speed void update_position(const int8_t&);

  public:
    virtual void init();
    virtual void halt();

    virtual uint8_t       get_step_mode();
    virtual inline void   set_full_step() { ; }
    virtual inline void   set_half_step() { ; }

    virtual inline bool step_cw()  { return false; }
    virtual inline bool step_ccw() { return false; }

    inline bool get_sleep_when_idle() { return m_sleep_when_idle; }
    inline void set_sleep_when_idle(const bool& b) { m_sleep_when_idle = b; }

    inline bool get_invert_direction() { return m_invert_direction; }
    inline void set_invert_direction(const bool& b) { m_invert_direction = b; }

    inline bool get_compress_steps() { return m_compress_steps; }
    inline void set_compress_steps(const bool& b) { m_compress_steps = b; }

    void move();
    bool is_moving();
    void speed tick();

    uint16_t get_current_position();
    void     set_current_position(const uint16_t&);

    uint16_t get_speed();
    void     set_speed(const uint16_t&);

    uint16_t get_target_position();
    void     set_target_position(const uint16_t&);
};

#endif
