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

#include "a4988.h"

void a4988::init()
{
  stepper::init();

  IO::set_as_output(m_pinout.step);
  IO::set_as_output(m_pinout.sleep);
  IO::set_as_output(m_pinout.direction);

  IO::write(m_pinout.step,       LOW);
  IO::write(m_pinout.direction,  LOW);

  // active low logic
  IO::write(m_pinout.sleep, (m_sleep_when_idle) ? LOW : HIGH);
}

void a4988::halt()
{

  stepper::halt();
  m_sleep_timeout_cnt = ((m_sleep_timeout * 1000000UL) / TIMER0_TICK);
  IO::write(m_pinout.step, LOW);

  // A4988: 400ns, A8825: 1.3us
  util::delay_2us();
}

bool a4988::step_cw()
{
  switch(IO::read(m_pinout.direction))
  {
    case LOW:
      IO::write(m_pinout.direction, HIGH);
      // A4988: 400ns, A8825: 1.3us
      util::delay_2us();

    case HIGH:
      ;

    default:
      return step();
  }
}

bool a4988::step_ccw()
{
  switch(IO::read(m_pinout.direction))
  {
    case LOW:
      ;

    case HIGH:
      IO::write(m_pinout.direction, LOW);
      // A4988: 400ns, A8825: 1.3us
      util::delay_2us();

    default:
      return step();
  }
}

bool a4988::step()
{
  if(m_sleep_when_idle && !IO::read(m_pinout.sleep)) {
    IO::write(m_pinout.sleep, HIGH);
    // A4988: 200ns, A8825: 1.7ms
    util::delay_2ms();
  }

  /*
   * The A4988 driver will physically step the motor when
   * transitioning from a HIGH to LOW signal, the internal
   * position counter should only be updated under this
   * condition.
   */

  // A4988: 1us, A8825: 1.9us
  IO::write(m_pinout.step, HIGH);
  util::delay_2us();

  // A4988: 1us, A8825: 1.9us
  IO::write(m_pinout.step, LOW);
  util::delay_2us();

  return true;
}

void a4988::sleep()
{
  if(m_sleep_when_idle && m_sleep_timeout_cnt) {
    --m_sleep_timeout_cnt;

    if(!m_sleep_timeout_cnt) {
      IO::write(m_pinout.sleep, LOW);
    }
  }
}
