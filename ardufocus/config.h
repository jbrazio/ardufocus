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

#ifndef __CONFIG_H__
#define __CONFIG_H__

// ----------------------------------------------------------------------------
// SAFETY NOTICE --------------------------------------------------------------
// ----------------------------------------------------------------------------

// THIS IS VERY IMPORTANT INFORMATION
//
// The wiring of the focuser has changed since commit [8375767], you should remove
// any physical conections between the AREF and the 3.3V pin, not doing so may
// damage your board. For more information read the README.md file or the source
// code commit history.
//
// [8375767]: https://github.com/jbrazio/ardufocus/commit/8375767da8008305e1cb2a93d049970c49c1482d

// ----------------------------------------------------------------------------
// Remote reset ---------------------------------------------------------------
// ----------------------------------------------------------------------------

// Remote reset is a non standard feature added to the Moonlite protocol which
// allows you to reset the micro controller inside the focuser. Please test
// this feature on your bench before deploying on the field. For this feature
// to work the default bootloader must be changed otherwise the uC will enter
// in a infinite loop state. For more information visit the [bug report].
//
// [bug report]: https://github.com/arduino/Arduino/issues/4492
//#define ENABLE_REMOTE_RESET

// ----------------------------------------------------------------------------
// Motor driver configuration -------------------------------------------------
// ----------------------------------------------------------------------------

// You should only enable *ONE* of the following drivers
// The ULN2003 shall be used with the unmodded version of 28BYJ-48 or any other
// Unipolar stepper motor. The A4988 driver should be used with Bipolar stepper
// motors or the modded version of the 28BYJ-48 (see the doc/ folder).
#define USE_A4988_DRIVER
//#define USE_ULN2003_DRIVER

// Pin definition
//#define MOTOR1_PINOUT 0x0f, &PORTB // Example for ULN2003 (pin D8 to D11)

//                      DIR, STEP, SLEEP, MS1  // Example for A4988 and alike
//#define MOTOR1_PINOUT   6,   11,    10,   7  // (short pinout)

//                    MS1, MS2, MS3, SLEEP, STEP, DIR  // Example for A4988 and alike
#define MOTOR1_PINOUT   7,   8,   9,    10,   11,  12  // (no RST nor ENA pib)

//                      ENA, MS1, MS2, MS3, RESET, SLEEP, STEP, DIR  // Example for A4988 and alike
//#define MOTOR1_PINOUT   5,   6,   7,   8,     9,     10,   11, 12  // (full pinout)

// Activate the following directive if you'd like to invert the motor rotation
// changing the focus direction.
//#define INVERT_MOTOR_DIR

// When active ardufocus will apply a trapezoidal acceleration profile to the
// motor's speed, this feature is EXPERIMENTAL. The objective is to help the
// system to cope with heavier focuser loads such as FF + FW + CCD combos.
//#define USE_LINEAR_ACCEL

// When active ardufocus will cut the stepper motor current when idle, in theory
// this could lead to less accuracy betwen movements but will keep the motor
// cool. When disabling this flag make sure your motor does not overheat.
#define MOTOR_SLEEP_WHEN_IDLE

// When active each two half-steps will increase one unit on the step counter
// thus making distance-per-step constant between the two stepping modes, full
// and half.
//
// Enable this flag if you're using a cheap stepper motor such as 28BYJ-48 and
// verify if the overall system accuracy increases.
//#define COMPRESS_HALF_STEPS


// According to the Moonlite protocol the temperature probe should only be read
// when the command ":C#" is received but some applications, such as SGP, seems
// not to respect this and only call the get temperature command ":GT#" which
// means the temperature will never get updated and the last value is always
// returned, either it is valid or not. Enabling the following option will force
// the temperature gathering process on every temperature read command.
// #define START_TEMP_CONVERSION_ON_EVERY_GET

// ----------------------------------------------------------------------------
// Temperature sensor configuration -------------------------------------------
// ----------------------------------------------------------------------------
#define THERMISTOR_ADC_CHANNEL          0
#define THERMISTOR_NOMINAL_TEMP      25.0F
#define THERMISTOR_BCOEFFICIENT    3950.0F
#define THERMISTOR_NOMINAL_VAL    10000.0F
#define THERMISTOR_SERIESRESISTOR 10000.0F

#include "conditionals.h"
#endif
