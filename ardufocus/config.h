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
//
//                   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//                   !! VERY IMPORTANT INFORMATION FOLLOWS !!
//                   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// The wiring of the focuser has changed since commit [8375767], you should remove
// any physical connections between the AREF and the 3.3V pin, not doing so may
// damage your board. For more information read the README.md file or the source
// code commit history.
//
// [8375767]: https://bit.ly/2HKUXgV

// ----------------------------------------------------------------------------
// MISCELLANEOUS --------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// Remote reset is a non standard feature added to the Moonlite protocol which
// allows you to reset the micro controller inside the focuser. Please test
// this feature on your bench before deploying on the field. For this feature
// to work the default boot loader must be changed otherwise the uC will enter
// in a infinite loop state. For more information visit the [bug report].
//
// [bug report]: https://github.com/arduino/Arduino/issues/4492
//#define ENABLE_REMOTE_RESET


// ----------------------------------------------------------------------------
// MOTOR #1 CONFIGURATION -----------------------------------------------------
// ----------------------------------------------------------------------------
//
// You should only enable *ONE* of the following drivers
// The ULN2003 shall be used with the unmodded version of 28BYJ-48 or any other
// Unipolar stepper motor. The A4988 driver should be used with Bipolar stepper
// motors or the modded version of the 28BYJ-48 (see the doc/ folder).
#define MOTOR1_USE_A4988_DRIVER
//#define MOTOR1_USE_DRV8825_DRIVER
//#define MOTOR1_USE_ULN2003_DRIVER

// Driver pin-out definition
// Define bellow the pin-out for your specific driver.
//
// Example for ULN2003 driver board
//
//                     IN1, IN2, IN3, IN4
//#define MOTOR1_PINOUT  2,   3,   4,   5
//
// Example for A4988 driver board
//
//                      MS1, MS2, MS3, SLEEP,  STEP, DIR
#define MOTOR1_PINOUT     7,   8,   9,    10,   11,  12

// Activate the following directive if you'd like to invert the motor rotation
// changing the focus direction.
//#define MOTOR1_INVERT_DIRECTION

// When active Ardufocus will cut the stepper motor current when idle, in theory
// this could lead to less accuracy between movements but will keep the motor
// cool. When disabling this flag make sure your motor does not overheat.
#define MOTOR1_SLEEP_WHEN_IDLE

// When active each two half-steps will increase one unit on the step counter
// thus making distance-per-step constant between the two stepping modes, full
// and half.
//
// Enable this flag if you're using a cheap stepper motor such as 28BYJ-48 and
// verify if the overall system accuracy increases.
//#define MOTOR1_COMPRESS_STEPS


// ----------------------------------------------------------------------------
// ACCELERATION PROFILE -------------------------------------------------------
// ----------------------------------------------------------------------------
// When active Ardufocus will apply the selected acceleration profile to the
// motor's speed. The objective is to help the system cope with heavier loads
// such as FF + FW + CCD combos.
//
//
// Linear Acceleration   Trapezoid Acceleration   Smooth Step Acceleration
//                                                        (S-Curve)
//
//   |   /\                |   ___________           |     __---__
// V |  /  \             V |  /           \        V |    -       -
//   | /    \              | /             \         |   -         -
//   |/      \             |/               \        |__-           -__
//   +----------------     +-------------------      +-------------------
//         T                        T                         T
//
//#define USE_LINEAR_ACCEL
//#define USE_TRAPEZOID_ACCEL
#define USE_SMOOTHSTEP_ACCEL

// The acceleration profile, independent of the method used, has at least two
// main periods: the ramp-up period when the motor is gaining speed and the
// ramp-down period when the motor is losing speed. This setting controls the
// duration of each one of those periods, the default value is 250 steps for
// each period if left undefined.
//#define ACCEL_DURATION 250

// When acceleration control is active this setting controls the minimum
// required number of steps on a movement for the algorithm to kick in. Any
// movement with less steps than this will be done at minimum speed without
// any acceleration control. The default value is 10 steps of left undefined.
//#define ACCEL_MIN_STEPS 10


// ----------------------------------------------------------------------------
// TEMPERATURE SENSOR ---------------------------------------------------------
// ----------------------------------------------------------------------------
#define THERMISTOR_ADC_CHANNEL          0
#define THERMISTOR_NOMINAL_TEMP      25.0F
#define THERMISTOR_BCOEFFICIENT    3950.0F
#define THERMISTOR_NOMINAL_VAL    10000.0F
#define THERMISTOR_SERIESRESISTOR 10000.0F

// According to the Moonlite protocol the temperature probe should only be read
// when the command ":C#" is received but some applications, such as SGP, seems
// not to respect this and only call the get temperature command ":GT#" which
// means the temperature will never get updated and the last read value is always
// returned, either it is valid or not. Enabling the following option will force
// the temperature gathering process on every temperature read command.
// #define START_TEMP_CONVERSION_ON_EVERY_GET


// ----------------------------------------------------------------------------
// DO NOT EDIT ANYTHING BELLOW THIS HEADER ------------------------------------
// ----------------------------------------------------------------------------
#include "assert.h"

#endif
