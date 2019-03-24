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

#ifndef __ASSERT_H__
#define __ASSERT_H__

// Defaults -------------------------------------------------------------------
#define TIMER0_FREQ 20000 // Hz
#define TIMER0_OCRA ((F_CPU/8) / TIMER0_FREQ)
#define TIMER0_TICK (1000000 / TIMER0_FREQ) // uS

#define DEFAULT_SLEEP_TIMEOUT 15

#define DEFAULT_MAX_SPEED 250
#define DEFAULT_MIN_SPEED  25

// User warnings --------------------------------------------------------------
#if defined(ENABLE_REMOTE_RESET)
  #warning Remote reset is enabled, make sure your bootloader is updated !
#endif

// Invalid configurations -----------------------------------------------------
#if defined(MOTOR1_COMPRESS_STEPS)
  #if defined(MOTOR1_USE_ULN2003_DRIVER) || defined(MOTOR2_USE_ULN2003_DRIVER)
    #error COMPRESS_STEPS is not supported for ULN2003 drivers.
    #error Please review the config.h file.
  #else
    #warning COMPRESS_STEPS is deprecated and will be removed soon.
    #warning Please review the config.h file.
  #endif
#endif

// DRV8825 driver hack --------------------------------------------------------
#if defined(MOTOR1_USE_DRV8825_DRIVER)
  #define MOTOR1_USE_A4988_DRIVER
#endif

// Duplicate driver check -----------------------------------------------------
#if defined(MOTOR1_USE_A4988_DRIVER) && defined(MOTOR1_USE_ULN2003_DRIVER)
  #error More than one stepper driver selected for motor #1
  #error Please review the config.h file.
#elif defined(MOTOR1_USE_A4988_DRIVER) || defined(MOTOR1_USE_ULN2003_DRIVER)
  #define MOTOR1_HAS_DRIVER
#else
  #error No stepper driver selected for motor #1.
  #error Please review the config.h file.
#endif

// DRV8825 driver hack --------------------------------------------------------
#if defined(MOTOR2_USE_DRV8825_DRIVER)
  #define MOTOR2_USE_A4988_DRIVER
#endif

#if defined(MOTOR2_USE_A4988_DRIVER) && defined(MOTOR2_USE_ULN2003_DRIVER)
  #error More than one stepper driver selected for motor #2
  #error Please review the config.h file.
#elif defined(MOTOR2_USE_A4988_DRIVER) || defined(MOTOR2_USE_ULN2003_DRIVER)
  #define MOTOR2_HAS_DRIVER
#elif defined (MOTOR2_PINOUT)
  #error No stepper driver selected for motor #2.
  #error Please review the config.h file.
#else
  // Motor 2 driver is optional if a pinout is undef
#endif

// High resolution mode -------------------------------------------------------
#if defined(MOTOR1_HIGH_RESOLUTION) || defined(MOTOR2_HIGH_RESOLUTION)
  #define HIGH_RESOLUTION_MODE
#endif

// Sanity check for motor one -------------------------------------------------
#ifdef MOTOR1_HAS_DRIVER
  // Speed control ------------------------------------------------------------
  #ifndef MOTOR1_MAX_SPEED
    #define MOTOR1_MAX_SPEED DEFAULT_MAX_SPEED
  #endif

  #ifndef MOTOR1_MIN_SPEED
    #define MOTOR1_MIN_SPEED DEFAULT_MIN_SPEED
  #endif

  // Miscellaneous ------------------------------------------------------------
  #ifndef MOTOR1_INVERT_DIRECTION
    #define MOTOR1_INVERT_DIRECTION false
  #else
    #undef MOTOR1_INVERT_DIRECTION
    #define MOTOR1_INVERT_DIRECTION true
  #endif

  #ifndef MOTOR1_SLEEP_WHEN_IDLE
    #define MOTOR1_SLEEP_WHEN_IDLE false
  #else
    #undef MOTOR1_SLEEP_WHEN_IDLE
    #define MOTOR1_SLEEP_WHEN_IDLE true
  #endif

  #ifndef MOTOR1_SLEEP_TIMEOUT
    #define MOTOR1_SLEEP_TIMEOUT DEFAULT_SLEEP_TIMEOUT
  #endif

  #ifndef MOTOR1_COMPRESS_STEPS
    #define MOTOR1_COMPRESS_STEPS false
  #else
    #undef MOTOR1_COMPRESS_STEPS
    #define MOTOR1_COMPRESS_STEPS true
  #endif
#endif

// Sanity check for motor two -------------------------------------------------
#ifdef MOTOR2_HAS_DRIVER
  // Speed control ------------------------------------------------------------
  #ifndef MOTOR2_MAX_SPEED
    #define MOTOR2_MAX_SPEED DEFAULT_MAX_SPEED
  #endif

  #ifndef MOTOR2_MIN_SPEED
    #define MOTOR2_MIN_SPEED DEFAULT_MIN_SPEED
  #endif

  // Miscellaneous ------------------------------------------------------------
  #ifndef MOTOR2_INVERT_DIRECTION
    #define MOTOR2_INVERT_DIRECTION false
  #else
    #undef MOTOR2_INVERT_DIRECTION
    #define MOTOR2_INVERT_DIRECTION true
  #endif

  #ifndef MOTOR2_SLEEP_WHEN_IDLE
    #define MOTOR2_SLEEP_WHEN_IDLE false
  #else
    #undef MOTOR2_SLEEP_WHEN_IDLE
    #define MOTOR2_SLEEP_WHEN_IDLE true
  #endif

  #ifndef MOTOR2_HIGH_RESOLUTION
    #define MOTOR2_HIGH_RESOLUTION false
  #endif

  #ifndef MOTOR2_COMPRESS_STEPS
    #define MOTOR2_COMPRESS_STEPS false
  #else
    #undef MOTOR2_COMPRESS_STEPS
    #define MOTOR2_COMPRESS_STEPS true
  #endif
#endif

// Acceleration sanity checks -------------------------------------------------
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

#ifndef NTC_ADC_CHANNEL
  #error You must assign a analog channel to the thermistor.
  #error Please review the config.h file.
#else
    #if !defined(NTC_NOMINAL_TEMP) || !defined(NTC_BCOEFFICIENT) || !defined(NTC_NOMINAL_VAL) || !defined(NTC_RESISTOR_VAL)
      #error Thermistor configuration is not valid.
      #error Please review the config.h file.
    #endif

    #ifndef NTC_MIN_RAW_VALUE
      #define NTC_MIN_RAW_VALUE 50
    #endif

    #ifndef NTC_MAX_RAW_VALUE
      #define NTC_MAX_RAW_VALUE 950
    #endif
#endif

#endif
