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

#include "uln2003.h"

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void uln2003::init()
{
  stepper::init();

  m_sequence = 0;
  set_full_step();

  IO::set_as_output(m_pinout.A);
  IO::set_as_output(m_pinout.B);
  IO::set_as_output(m_pinout.C);
  IO::set_as_output(m_pinout.D);

  IO::write(m_pinout.A, LOW);
  IO::write(m_pinout.B, LOW);
  IO::write(m_pinout.C, LOW);
  IO::write(m_pinout.D, LOW);
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void uln2003::halt()
{
  stepper::halt();

  #ifdef MOTOR_SLEEP_WHEN_IDLE
    IO::write(m_pinout.A, LOW);
    IO::write(m_pinout.B, LOW);
    IO::write(m_pinout.C, LOW);
    IO::write(m_pinout.D, LOW);
  #endif
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void uln2003::set_full_step()
{
  m_mode         = 0x00;
  m_stepping_sz  = lookup::uln2003_unipolar_full_sz;
  p_stepping_tbl = lookup::uln2003_unipolar_full;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void uln2003::set_half_step()
{
  m_mode         = 0xFF;
  m_stepping_sz  = lookup::uln2003_unipolar_half_sz;
  p_stepping_tbl = lookup::uln2003_unipolar_half;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
bool uln2003::step_cw()
{
  uln2003::step();
  ++m_sequence;
  if (m_sequence > (int8_t) (m_stepping_sz -1)) { m_sequence = 0; }
  return true;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
bool uln2003::step_ccw()
{
  uln2003::step();
  --m_sequence;
  if (m_sequence < 0) { m_sequence = (m_stepping_sz -1); }
  return true;
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void uln2003::step()
{
  const uint8_t byte = pgm_read_byte( &(*(p_stepping_tbl + m_sequence)) );

  CRITICAL_SECTION_START
  IO::write(m_pinout.A, ((byte >> 3) & 0x1) ? HIGH : LOW);
  IO::write(m_pinout.B, ((byte >> 2) & 0x1) ? HIGH : LOW);
  IO::write(m_pinout.C, ((byte >> 1) & 0x1) ? HIGH : LOW);
  IO::write(m_pinout.D, ((byte     ) & 0x1) ? HIGH : LOW);
  CRITICAL_SECTION_END
}
