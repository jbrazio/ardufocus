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

#ifndef __ISR_H__
#define __ISR_H__

#include "config.h"

#include <avr/interrupt.h>
#include "stepper.h"
#include "eeprom.h"
#include "analog.h"
#include "macro.h"

#ifdef MOTOR1_HAS_DRIVER
extern stepper* g_motor1;
#endif

#ifdef MOTOR2_HAS_DRIVER
extern stepper* g_motor2;
#endif

#endif
