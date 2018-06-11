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
  #define MAX_ISR_FREQ 0x3E8 // 2KHz
  #define MIN_ISR_FREQ 0xFA0 // 500Hz

#elif defined(USE_ULN2003_DRIVER)
  #define MAX_ISR_FREQ 0x7D0  // 1KHz
  #define MIN_ISR_FREQ 0x4E20 // 100Hz

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

#if defined(USE_LINEAR_ACCEL) || defined(USE_TRAPEZOID_ACCEL) || defined(USE_SMOOTHSTEP_ACCEL)
  #define HAS_ACCELERATION

  #ifndef ACCEL_DURATION
    #define ACCEL_DURATION 250
  #elif (ACCEL_DURATION < 100)
    #warning The acceleration duration is less than 100 steps.
    #warning Maybe you should consider disabling acceleration at al ?
  #endif

  #ifndef ACCEL_MIN_STEPS
    #define ACCEL_MIN_STEPS 10
  #elif (ACCEL_MIN_STEPS < 1)
    #error ACCEL_MIN_STEPS cannot be lower than 1 step.
    #error Please review the config.h file.
  #endif

  #if (ACCEL_MIN_STEPS >= ACCEL_DURATION)
    #error ACCEL_MIN_STEPS must be lower than ACCEL_DURATION.
    #error Please review the config.h file.
  #endif
#endif

#endif
