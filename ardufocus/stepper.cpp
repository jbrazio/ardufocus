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
  CRITICAL_SECTION_START
    m_speed = 2;
    m_ovf_counter = 0;
  CRITICAL_SECTION_END
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::move()
{
  CRITICAL_SECTION_START
    m_ovf_counter = 0;
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
  CRITICAL_SECTION_END
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
uint16_t stepper::get_current_position()
{
  CRITICAL_SECTION_START
    const uint16_t c = m_position.current;
  CRITICAL_SECTION_END

  return c;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::set_current_position(const uint16_t& target)
{
  CRITICAL_SECTION_START
    m_position.current = target;
    m_position.target  = target;
  CRITICAL_SECTION_END
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
uint16_t stepper::get_speed()
{
  CRITICAL_SECTION_START
    const uint16_t s = m_speed;
  CRITICAL_SECTION_END

  return s;
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
  CRITICAL_SECTION_START
    const uint16_t t = m_position.target;
  CRITICAL_SECTION_END

  return t;
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

    #ifdef HAS_ACCELERATION
      m_position.relative = 0;

      m_position.distance = (m_position.current > target)
        ? m_position.current - target
        : target - m_position.current;

      if (m_position.distance >= ACCEL_MIN_STEPS)
      {
        #if defined(USE_LINEAR_ACCEL)
          m_position.easein  = m_position.distance >> 1;
          m_position.easeout = m_position.easein;

        #elif defined(USE_TRAPEZOID_ACCEL) || defined(USE_SMOOTHSTEP_ACCEL)
          if (m_position.distance < (ACCEL_DURATION << 1))
          {
            m_position.easein  = m_position.distance >> 1;
            m_position.easeout = m_position.easein;
          }
          else
          {
            m_position.easein  = ACCEL_DURATION;
            m_position.easeout = m_position.distance - ACCEL_DURATION;
          }
        #endif
      }
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
  // Movement guard
  if (! m_position.moving) { sleep(); return; }

  // Step frequency generator
  if((m_ovf_counter++) < (TIMER0_FREQ / (m_set_speed<<1)) -1) { return; }
  m_ovf_counter = 0;

  // Speed control
  // We're not 100% moonlite compatible here: the PPS speed value you select
  // on the driver acts as a divider of *_MAX_SPEED set on the config file.
  static uint8_t counter = 0;
  if((counter++) % (m_speed>>1) != 0) { return; }

  // Move outwards
  if (m_position.target > m_position.current) {
    if ((m_invert_direction) ? step_cw() : step_ccw()) { update_position(1); }
  }

  // Move inwards
  else if (m_position.target < m_position.current) {
    if ((m_invert_direction) ? step_ccw() : step_cw()) { update_position(-1); }
  }

  // Stop movement
  else { halt(); }
}


#ifdef HAS_ACCELERATION
  /**
   * @brief [brief description]
   * @details [long description]
   *
   */
  void stepper::update_freq()
  {
    if (m_position.distance >= ACCEL_MIN_STEPS)
    {
      #if defined(USE_LINEAR_ACCEL)
        const float s = (m_position.relative <= m_position.easein)
          ? m_position.relative : m_position.distance - m_position.relative;
        const float f = map(s, 0, m_position.easein, 0.0, 1.0);

      #elif defined(USE_TRAPEZOID_ACCEL)
        const float s = (m_position.relative <= m_position.easein)
          ? m_position.relative : (m_position.relative >= m_position.easeout)
          ? m_position.distance - m_position.relative : ACCEL_DURATION;
        const float f = map(s, 0, ACCEL_DURATION, 0.0, 1.0);

      #elif defined(USE_SMOOTHSTEP_ACCEL)
        const float f = util::smootheststep(0, m_position.easein, m_position.relative) *
          (1.0 - util::smootheststep(m_position.easeout, m_position.distance, m_position.relative));
      #endif

      m_set_speed = util::lerp(m_min_speed, m_max_speed, f);

    } else { m_set_speed = m_min_speed; }
  }
#endif

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void stepper::update_position(const int8_t &direction)
{
  CRITICAL_SECTION_START
    m_position.current += direction;          // Update the global position

    #ifdef HAS_ACCELERATION
      ++m_position.relative;  // Update the relative position
      update_freq();          // Update the stepping frequency
    #endif
  CRITICAL_SECTION_END
}
