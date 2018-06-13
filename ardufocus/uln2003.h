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

#ifndef __ULN2003_H__
#define __ULN2003_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "lookuptable.h"
#include "stepper.h"
#include "macro.h"
#include "util.h"
#include "io.h"

class uln2003: public stepper
{
  public:
    struct pinout_t
    {
      pin_t A, B, C, D;
    };

  protected:
    const pinout_t m_pinout;

    volatile  int8_t m_sequence;
             uint8_t m_stepping_sz;
    const    uint8_t *p_stepping_tbl;

  private:
    void speed step();

  public:
    virtual void init();
    virtual void halt();
    virtual void set_full_step();
    virtual void set_half_step();
    virtual speed bool step_cw();
    virtual speed bool step_ccw();

    inline uln2003(pinout_t const& pinout) : m_pinout({ pinout }) { ; }
};

#endif
