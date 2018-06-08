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

#include "stepper.h"

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::init()
{
  m_mode    = 0;
  m_speed   = 2;
  m_counter = 0;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::move()
{
  CRITICAL_SECTION_START
  m_position.moving = true;
  CRITICAL_SECTION_END
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
bool stepper::is_moving()
{
  CRITICAL_SECTION_START
  const bool b = (m_position.moving);
  CRITICAL_SECTION_END
  return b;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::halt()
{
  CRITICAL_SECTION_START
  m_position.target = m_position.current;
  m_position.moving = false;
  m_counter         = 0;
  CRITICAL_SECTION_END
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
uint16_t stepper::get_current_position()
{
  return m_position.current;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::set_current_position(const uint16_t& target)
{
  CRITICAL_SECTION_START
  m_position.current  = target;
  m_position.target   = target;
  CRITICAL_SECTION_END
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
uint16_t stepper::get_speed()
{
  return m_speed;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::set_speed(const uint16_t& target)
{
  CRITICAL_SECTION_START
  m_speed = target;
  CRITICAL_SECTION_END
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
uint16_t stepper::get_target_position()
{
  return m_position.target;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::set_target_position(const uint16_t& target)
{
  CRITICAL_SECTION_START

  m_position.target = target;

  #ifdef USE_LINEAR_ACCEL
  /*
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
    */
  #endif

  CRITICAL_SECTION_END
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::tick()
{
  if (m_position.moving) {
    // Set the stepping frequency
    if((m_counter++) % m_speed != 0) { return; }

    #ifdef USE_LINEAR_ACCEL
      #error USE_LINEAR_ACCEL feature is currently not supported.
    /*
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
    */
    #endif

    // Move outwards
    if (m_position.target > m_position.current) {
      if (
        #ifdef INVERT_MOTOR_DIR
          step_cw()
        #else
          step_ccw()
        #endif
      ) { ++m_position.current; }
    }

    // Move inwards
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


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
uint8_t stepper::get_step_mode() {
  return m_mode;
}
