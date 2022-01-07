/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017-2022 João Brázio [joao@brazio.org]
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

#ifndef __MOTOR_DRIVER_H__
#define __MOTOR_DRIVER_H__

#include "version.h"
#include "config.h"

#if defined(MOTOR1_USE_A4988_DRIVER) || defined(MOTOR2_USE_A4988_DRIVER)
#include "a4988.h"
#endif

#if defined(MOTOR1_USE_ULN2003_DRIVER) || defined(MOTOR2_USE_ULN2003_DRIVER)
#include "uln2003.h"
#endif

#if defined(MOTOR1_USE_A4988_DRIVER)
a4988   motor1drv({ MOTOR1_PINOUT });
#elif defined(MOTOR1_USE_ULN2003_DRIVER)
uln2003 motor1drv({ MOTOR1_PINOUT });
#endif

#if defined(MOTOR2_USE_A4988_DRIVER)
a4988   motor2drv({ MOTOR2_PINOUT });
#elif defined(MOTOR2_USE_ULN2003_DRIVER)
uln2003 motor2drv({ MOTOR2_PINOUT });
#endif

#endif
