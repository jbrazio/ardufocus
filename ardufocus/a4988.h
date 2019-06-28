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

#ifndef __A4988_H__
#define __A4988_H__

#include "config.h"

#include "stepper.h"
#include "macro.h"
#include "utility.h"

class a4988: public stepper
{
  public:
    struct pinout_t
    {
      pin_t sleep, step, direction;
    };

    /*
     *
     *   A4988 STEP STICK PINOUT
     *
     *       .-----------.
     * !ENA |o           o| VMOT
     *  MS1 |o  -----    o| GND
     *  MS2 |o |     |   o| 2B
     *  MS3 |o |     |   o| 2A
     * !RST |o  -----    o| 1A
     * !SLP |o      ---  o| 1B
     *  STP |o     | O | o| VDD
     *  DIR |o      ---  o| GND
     *       `-----------´
     *
     */

  protected:
    const pinout_t m_pinout;

  private:
    force_inline speed bool step();

  public:
    virtual void init();
    virtual void halt();
    virtual void sleep();
    virtual speed bool step_cw();
    virtual speed bool step_ccw();

    inline a4988(pinout_t const& pinout) : m_pinout({ pinout }) { ; }
};

#endif
