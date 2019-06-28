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

// FIXME TODO
// When this class is active the linker was screaming:
// undefined reference to `__cxa_pure_virtual'

extern "C" void __cxa_pure_virtual(void) __attribute__ ((__noreturn__));
extern "C" void __cxa_deleted_virtual(void) __attribute__ ((__noreturn__));

void __cxa_pure_virtual(void) {
  // We might want to write some diagnostics to uart in this case
  //std::terminate();
  abort();
}

void __cxa_deleted_virtual(void) {
  // We might want to write some diagnostics to uart in this case
  //std::terminate();
  abort();
}

// EOF

void uln2003::init()
{
  stepper::init();

  m_sequence     = 0;
  m_stepping_sz  = lookup::uln2003_unipolar_full_sz;
  p_stepping_tbl = lookup::uln2003_unipolar_full;

  IO::set_as_output(m_pinout.A);
  IO::set_as_output(m_pinout.B);
  IO::set_as_output(m_pinout.C);
  IO::set_as_output(m_pinout.D);

  IO::write(m_pinout.A, LOW);
  IO::write(m_pinout.B, LOW);
  IO::write(m_pinout.C, LOW);
  IO::write(m_pinout.D, LOW);
}

void uln2003::halt()
{
  stepper::halt();
  m_sleep_timeout_cnt = ((m_sleep_timeout * 1000000UL) / TIMER0_TICK);
}

bool uln2003::step_cw()
{
  step();
  --m_sequence;
  if (m_sequence < 0) { m_sequence = (m_stepping_sz -1); }
  return true;
}

bool uln2003::step_ccw()
{
  step();
  ++m_sequence;
  if (m_sequence > (int8_t) (m_stepping_sz -1)) { m_sequence = 0; }
  return true;
}

void uln2003::step()
{
  const uint8_t byte = pgm_read_byte( &(*(p_stepping_tbl + m_sequence)) );

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    IO::write(m_pinout.A, ((byte >> 3) & 0x1) ? HIGH : LOW);
    IO::write(m_pinout.B, ((byte >> 2) & 0x1) ? HIGH : LOW);
    IO::write(m_pinout.C, ((byte >> 1) & 0x1) ? HIGH : LOW);
    IO::write(m_pinout.D, ((byte     ) & 0x1) ? HIGH : LOW);
  }
}

void uln2003::sleep()
{
  if(m_sleep_when_idle && m_sleep_timeout_cnt) {
    --m_sleep_timeout_cnt;

    if(!m_sleep_timeout_cnt) {
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        IO::write(m_pinout.A, LOW);
        IO::write(m_pinout.B, LOW);
        IO::write(m_pinout.C, LOW);
        IO::write(m_pinout.D, LOW);
      }
    }
  }
}
