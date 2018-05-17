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

#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

struct stepper_position_t
{
  bool moving;

  uint16_t current,
           target,
           last;

  uint16_t delta,
           accelin,
           accelout;
};

struct stepper_pin_t
{
  uint8_t enable;
  uint8_t ms1;
  uint8_t ms2;
  uint8_t ms3;
  uint8_t reset;
  uint8_t sleep;
  uint8_t step;
  uint8_t direction;
};

#endif