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

#ifndef __STEPPER_H__
#define __STEPPER_H__

#include "config.h"

#include <stdint.h>
#include <util/atomic.h>
#include "macro.h"
#include "io.h"
#include "utility.h"

class stepper
{
  public:
    struct position_t
    {
      bool moving;      // When set to true it will start the movement

      uint32_t current, // Absolution position
               target;  // Target value for the step counter

      #ifdef HAS_ACCELERATION
      uint32_t distance,  // Move distance
               relative;  // Relative position

      uint32_t easein,    // Step where easein stops
               easeout;   // Step where easeout starts
      #endif
    };

  protected:
    volatile uint16_t m_speed;       // Stepping speed
    volatile position_t m_position;  // Absolute motor position
    volatile uint16_t m_ovf_counter; // Overflow counter

    bool m_sleep_when_idle;          // Disable power to the motor when idle
    bool m_invert_direction;         // Invert the direction of rotation

    uint16_t m_max_speed = DEFAULT_MAX_SPEED;
    uint16_t m_set_speed = DEFAULT_MAX_SPEED;
    uint16_t m_min_speed = DEFAULT_MIN_SPEED;

    uint8_t  m_sleep_timeout     = DEFAULT_SLEEP_TIMEOUT;
    uint32_t m_sleep_timeout_cnt = 0;

  protected:
    #ifdef HAS_ACCELERATION
    inline speed void update_freq();
    #endif
    inline speed void update_position(const int8_t&);

  public:
    virtual void init();
    virtual void halt();

    virtual inline void sleep()                        { ; }
    virtual inline bool step_cw()                      { return false;              }
    virtual inline bool step_ccw()                     { return false;              }

    inline bool get_sleep_when_idle()                  { return m_sleep_when_idle;  }
    inline void set_sleep_when_idle(const bool& b)     { m_sleep_when_idle = b;     }

    inline bool get_invert_direction()                 { return m_invert_direction; }
    inline void set_invert_direction(const bool& b)    { m_invert_direction = b;    }

    inline uint16_t get_max_speed()                    { return m_max_speed;        }
    inline void     set_max_speed(uint16_t const& s)   { m_max_speed = s;           }

    inline uint16_t get_min_speed()                    { return m_min_speed;        }
    inline void     set_min_speed(uint16_t const& s)   { m_min_speed = s;           }

    inline uint8_t get_sleep_timeout()                 { return m_sleep_timeout;    }
    inline void    set_sleep_timeout(uint8_t const& t) { m_sleep_timeout = t;       }

    void move();
    bool is_moving();
    void speed tick();

    uint16_t get_speed();
    void     set_speed(const uint16_t&);

    uint32_t get_current_position();
    void     set_current_position(const uint32_t&);
    uint32_t get_target_position ();
    void     set_target_position (const uint32_t&);
};

#endif
