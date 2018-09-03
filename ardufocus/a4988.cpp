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

#include "a4988.h"

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void a4988::init()
{
  stepper::init();

  IO::set_as_output(m_pinout.ms1);
  IO::set_as_output(m_pinout.ms2);
  IO::set_as_output(m_pinout.ms3);
  IO::set_as_output(m_pinout.step);
  IO::set_as_output(m_pinout.sleep);
  IO::set_as_output(m_pinout.direction);

  IO::write(m_pinout.ms1,        LOW);
  IO::write(m_pinout.ms2,        LOW);
  IO::write(m_pinout.ms3,        LOW);
  IO::write(m_pinout.step,       LOW);
  IO::write(m_pinout.direction,  LOW);

  // active low logic
  IO::write(m_pinout.sleep, (m_sleep_when_idle) ? LOW : HIGH);
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void a4988::halt()
{
  stepper::halt();

  m_step = 0;
  m_sleep_timeout_cnt = ((m_sleep_timeout * 1000000UL) / TIMER0_TICK);

  IO::write(m_pinout.step, LOW);
  util::delay_2us();
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void a4988::set_full_step()
{
  m_mode = 0x00;
  IO::write(m_pinout.ms1, LOW);
  IO::write(m_pinout.ms2, LOW);
  IO::write(m_pinout.ms3, LOW);
  util::delay_250us();
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void a4988::set_half_step()
{
  m_mode = 0xFF;
  IO::write(m_pinout.ms1, HIGH);
  IO::write(m_pinout.ms2, LOW);
  IO::write(m_pinout.ms3, LOW);
  util::delay_250us();
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
bool a4988::step_cw()
{
  switch(IO::read(m_pinout.direction))
  {
    case LOW:
      IO::write(m_pinout.direction, HIGH);
      util::delay_250us();

    case HIGH:
      ;

    default:
      return step();
  }
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
bool a4988::step_ccw()
{
  switch(IO::read(m_pinout.direction))
  {
    case LOW:
      ;

    case HIGH:
      IO::write(m_pinout.direction, LOW);
      util::delay_250us();

    default:
      return step();
  }
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
bool a4988::step()
{
  if(m_sleep_when_idle && !IO::read(m_pinout.sleep)) {
    IO::write(m_pinout.sleep, HIGH);

    #ifdef MOTOR1_USE_DRV8825_DRIVER
      util::delay_1ms();
    #else
      util::delay_250us();
    #endif
  }

  /*
   * The A4988 driver will physically step the motor when
   * transitioning from a HIGH to LOW signal, the internal
   * position counter should only be updated under this
   * condition.
   */

  if(m_compress_steps) ++m_step %= ((m_mode) ? 4 : 2);
  else ++m_step %= 2;

  switch(m_step)
  {
    case 0:
      IO::write(m_pinout.step, LOW);
      break;

    case 1:
      IO::write(m_pinout.step, HIGH);
      break;

    // TODO
    // Optimize this code, case 2-3 is only required
    // when COMPRESS_STEPS is enabled.
    case 2:
      IO::write(m_pinout.step, LOW);
      break;

    case 3:
      IO::write(m_pinout.step, HIGH);
      break;
  }

  util::delay_2us();
  return (! m_step);
}


/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void a4988::sleep()
{
  if(m_sleep_when_idle && m_sleep_timeout_cnt) {
    --m_sleep_timeout_cnt;

    if(!m_sleep_timeout_cnt)
    {
      IO::write(m_pinout.sleep, LOW);

      #ifdef MOTOR1_USE_DRV8825_DRIVER
        util::delay_1ms();
      #else
        util::delay_250us();
      #endif
    }
  }
}
