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
// SAFE NOTICE ----------------------------------------------------------------
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
#define INVERT_MOTOR_DIR

// When active ardufocus will apply a trapezoidal acceleration profile to the
// motor's speed, this feature is EXPERIMENTAL. The objective is to help the
// system to cope with heavier focuser loads such as FF + FW + CCD combos.
//#define USE_LINEAR_ACCEL


// ----------------------------------------------------------------------------
// Temperature sensor configuration -------------------------------------------
// ----------------------------------------------------------------------------
#define THERMISTOR_ADC_CHANNEL          0
#define THERMISTOR_NOMINAL_TEMP      25.0F
#define THERMISTOR_BCOEFFICIENT    3950.0F
#define THERMISTOR_NOMINAL_VAL    10000.0F
#define THERMISTOR_SERIESRESISTOR 10000.0F

#endif
