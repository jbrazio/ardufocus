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

#ifndef __CONDITIONALS_H__
#define __CONDITIONALS_H__

#ifdef USE_ULN2003_DRIVER
  #include "uln2003.h"
  #define MOTOR_DRIVER uln2003
#else
  #include "a4988.h"
  #define MOTOR_DRIVER a4988
#endif

#ifdef ENABLE_REMOTE_RESET
  #warning Remote reset is enabled, make sure your bootloader is updated
#endif

#endif
