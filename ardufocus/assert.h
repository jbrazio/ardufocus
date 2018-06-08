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

#ifndef __ASSERT_H__
#define __ASSERT_H__

#if defined(USE_A4988_DRIVER)
  #include "a4988.h"
#elif defined(USE_ULN2003_DRIVER)
  #include "uln2003.h"
#else
  #error No stepper driver selected.
  #error Please review the config.h file.
#endif

#ifndef MOTOR1_PINOUT
  #error A pinout must be defined for the motor.
  #error Please review the config.h file.
#endif

#ifdef ENABLE_REMOTE_RESET
  #warning Remote reset is enabled, make sure your bootloader is updated !
#endif

#endif
