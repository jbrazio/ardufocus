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

#ifndef __A4988_H__
#define __A4988_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "stepper.h"
#include "util.h"
#include "io.h"

class a4988: public stepper
{
  public:
    struct pinout_t
    {
      pin_t ms1, ms2, ms3;
      pin_t sleep, step, direction;
    };

  protected:
    uint8_t m_step = 0;
    const pinout_t m_pinout;

  public:
    inline a4988(pinout_t const& pinout) : m_pinout({ pinout }) { init(); }

    virtual void init();
    virtual void halt();
    virtual void set_full_step();
    virtual void set_half_step();
    virtual bool step_cw();
    virtual bool step_ccw();

  private:
    bool step();
};

#endif
