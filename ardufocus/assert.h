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

#ifndef __ASSERT_H__
#define __ASSERT_H__

 // Defaults -------------------------------------------------------------------
#define TIMER0_PSCL 64
#define TIMER0_FREQ 5000L // Hz
#define TIMER0_OCRA (((F_CPU/TIMER0_PSCL) / TIMER0_FREQ)  -1)
#define TIMER0_TICK (1000000L  / TIMER0_FREQ) // uS

#define TIMER2_PSCL 1024
#define TIMER2_FREQ 160L // Hz
#define TIMER2_OCRA (((F_CPU/TIMER0_PSCL) / TIMER0_FREQ)  -1)
#define TIMER2_TICK (1000000L  / TIMER0_FREQ) // uS

#define DEFAULT_MAX_SPEED    100
#define DEFAULT_MIN_SPEED     10
#define DEFAULT_SLEEP_TIMEOUT 60

// Include the user selected config file
#if defined(BOARD_OFFICIAL_PCB)
#include "config/pcb.h"
#elif defined(BOARD_CUSTOM)
#include "config/custom.h"
#else
#error No configuration file selected
#endif

// User warnings --------------------------------------------------------------
#if defined(ENABLE_REMOTE_RESET)
  //#warning Remote reset is enabled, make sure your bootloader is updated !
#endif

// Invalid configurations -----------------------------------------------------
#if defined(ENABLE_DTR_RESET)
#if !defined(DTR_RESET_PINOUT)
#error DTR_RESET_PINOUT is active but DTR_RESET_PINOUT is missing.
#error Please review the config.h file.
#endif

#if !defined(USE_EEPROM)
#error ENABLE_DTR_RESET requiures USE_EEPROM to be active aswell.
#error Please review the config.h file.
#endif
#endif

// DRV8825 driver hack --------------------------------------------------------
#if defined(MOTOR1_USE_DRV8825_DRIVER)
#define MOTOR1_USE_A4988_DRIVER
#endif

#if defined(MOTOR2_USE_DRV8825_DRIVER)
#define MOTOR2_USE_A4988_DRIVER
#endif

// Duplicate driver check -----------------------------------------------------
#if defined(MOTOR1_USE_A4988_DRIVER) && defined(MOTOR1_USE_ULN2003_DRIVER)
#error More than one stepper driver selected for motor #1
#error Please review the config.h file.
#elif defined(MOTOR1_USE_A4988_DRIVER) || defined(MOTOR1_USE_ULN2003_DRIVER)
#define MOTOR1_HAS_DRIVER
#else
  // Motor 1 is optional
#endif

#if defined(MOTOR2_USE_A4988_DRIVER) && defined(MOTOR2_USE_ULN2003_DRIVER)
#error More than one stepper driver selected for motor #2
#error Please review the config.h file.
#elif defined(MOTOR2_USE_A4988_DRIVER) || defined(MOTOR2_USE_ULN2003_DRIVER)
#define MOTOR2_HAS_DRIVER
#else
  // Motor 2 is optional
#endif

#if !defined(MOTOR1_HAS_DRIVER) && !defined(MOTOR2_HAS_DRIVER)
#error At least ONE motor must be defined.
#error Please review the config.h file.
#endif

// Driver and/or Pinout validation  -------------------------------------------
#if !defined(MOTOR1_HAS_DRIVER) && defined(MOTOR1_PINOUT)
#error No motor pinout defined for motor #1.
#error Please review the config.h file.
#elif defined(MOTOR1_HAS_DRIVER) && !defined(MOTOR1_PINOUT)
#error No stepper driver selected for motor #1.
#error Please review the config.h file.
#endif

#if !defined(MOTOR2_HAS_DRIVER) && defined(MOTOR2_PINOUT)
#error No motor pinout defined for motor #2.
#error Please review the config.h file.
#elif defined(MOTOR2_HAS_DRIVER) && !defined(MOTOR2_PINOUT)
#error No stepper driver selected for motor #2.
#error Please review the config.h file.
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

#ifndef MOTOR2_SLEEP_TIMEOUT
#define MOTOR2_SLEEP_TIMEOUT DEFAULT_SLEEP_TIMEOUT
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

	// USER INTERFACE: Keyboard ---------------------------------------------------
#if defined(USE_UI_KAP)
#if defined(UI_KAP_BUTTON_DEBOUNCE) && (UI_KAP_BUTTON_DEBOUNCE < 1 || UI_KAP_BUTTON_DEBOUNCE > 255)
#undef UI_KAP_BUTTON_DEBOUNCE
#endif

#if !defined(UI_KAP_BUTTON_DEBOUNCE)
#define UI_KAP_BUTTON_DEBOUNCE 15
#endif

#if !defined(UI_KAP_FWD_BUTTON_PIN) || !defined(UI_KAP_BWD_BUTTON_PIN)
#error KAP configuration is not valid.
#error Please define UI_KAP_FWD_BUTTON_PIN and UI_KAP_BWD_BUTTON_PIN
#endif
#endif

#endif
