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

#include "macro.h"
#include "struct.h"
#include "lookuptable.h"

class stepper
{
protected:
  uint8_t m_mode = 0x00;
  volatile uint16_t m_speed = 2;
  volatile stepper_position_t m_position = { false, 0, 0, 0, 0, 0, 0 };

public:
  inline bool is_moving()
  {
    CRITICAL_SECTION_START
    const bool b = (m_position.moving);
    CRITICAL_SECTION_END
    return b;
  }

  inline void move()
  {
    CRITICAL_SECTION_START
    m_position.moving = true;
    CRITICAL_SECTION_END
  }

  inline uint16_t get_current_position()
  {
    return m_position.current;
  }

  inline void set_current_position(const uint16_t& target)
  {
    CRITICAL_SECTION_START
    m_position.current  = target;
    m_position.target   = target;
    CRITICAL_SECTION_END
  }

  inline uint16_t get_speed()
  {
    return m_speed;
  }

  inline void set_speed(const uint16_t& target)
  {
    CRITICAL_SECTION_START
    m_speed = target;
    CRITICAL_SECTION_END
  }

  inline uint16_t get_target_position()
  {
    return m_position.target;
  }

  inline void set_target_position(const uint16_t& target)
  {
    CRITICAL_SECTION_START

    m_position.target = target;

    #ifdef USE_LINEAR_ACCEL
      m_position.last = m_position.current;

      if (m_position.target > m_position.current) {
        m_position.delta = m_position.target - m_position.last;
      } else {
        m_position.delta = m_position.last - m_position.target;
      }

      if (m_position.delta < 200 ) {
        m_position.accelin = m_position.delta >> 1;
      } else {
        m_position.accelin = m_position.delta >> 6;
      }

      m_position.accelout = m_position.delta - m_position.accelin;
    #endif

    CRITICAL_SECTION_END
  }

  inline void tick()
  {
    if (m_position.moving) {
      // Set the stepping frequency
      switch(m_speed) {
        case 1:
          OCR1A = lookup::step_freq_table[0]; // 500 Hz
          break;

        case 2:
          OCR1A = lookup::step_freq_table[1]; // 250 Hz
          break;

        case 4:
          OCR1A = lookup::step_freq_table[2]; // 125 Hz
          break;

        case 8:
          OCR1A = lookup::step_freq_table[3]; // 63 Hz
          break;

        case 16:
          OCR1A = lookup::step_freq_table[4]; // 32 Hz
          break;

        case 32:
          OCR1A = lookup::step_freq_table[5]; // 16 Hz
          break;

        default: break;
      }

      #ifdef USE_A4988_DRIVER
        // Double the interrupt rate to archive 50% duty cycle
        OCR1A = (uint16_t) (OCR1A >>1);
      #endif

      #ifdef USE_LINEAR_ACCEL
        // Calculates the current relative position
        const uint16_t pos = (m_position.target < m_position.current)
          ? m_position.last - m_position.current
          : m_position.current - m_position.last;

        // Acceleration control
        uint16_t min = lookup::step_freq_table[5],
                 max = OCR1A;

        if (pos <= m_position.accelin) {
          OCR1A = map(pos, 0, m_position.accelin, min, max);
        }

        else if (pos >= m_position.accelout) {
          OCR1A = map(pos, m_position.accelout, m_position.delta, max, min);
        }
      #endif

      // Move the focus point out
      if (m_position.target > m_position.current) {
        if (
          #ifdef INVERT_MOTOR_DIR
            step_cw()
          #else
            step_ccw()
          #endif
        ) { ++m_position.current; }
      }

      // Move the focus point in
      else if (m_position.target < m_position.current) {
        if (
          #ifdef INVERT_MOTOR_DIR
            step_ccw()
          #else
            step_cw()
          #endif
        ) { --m_position.current; }
      }

      // Stop movement
      else { halt(); }
    }
  }

  virtual inline void halt()
  {
    CRITICAL_SECTION_START
    m_position.target = m_position.current;
    m_position.moving = false;
    CRITICAL_SECTION_END
  }

  virtual inline uint8_t get_step_mode() {
    return m_mode;
  }

  virtual inline void set_full_step() { ; }
  virtual inline void set_half_step() { ; }

  virtual inline bool step_cw()  { return false; }
  virtual inline bool step_ccw() { return false; }
};

#endif
