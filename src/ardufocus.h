/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017 João Brázio [joao@brazio.org]
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

#ifndef __ARDUFOCUS_H__
#define __ARDUFOCUS_H__

// ----------------------------------------------------------------------------
// Program info and version ---------------------------------------------------
// ----------------------------------------------------------------------------
#define ARDUFOCUS_VERSION "01"
#define ARDUFOCUS_VERSION_BUILD "20170613"


// ----------------------------------------------------------------------------
// Motor driver configuration -------------------------------------------------
// ----------------------------------------------------------------------------

// You should only enable *ONE* of the following drivers
// The ULN2003 shall be used with the unmodded version of 28BYJ-48 or any other
// Unipolar stepper motor. The A4988 driver should be used with Bipolar stepper
// motors or the modded version of the 28BYJ-48 (see the doc/ folder).
#define USE_A4988_DRIVER
//#define USE_ULN2003_DRIVER

// Activate the following directive if you'd like to invert the motor rotation
// changing the focus direction.
#define INVERT_MOTOR_DIR


// ----------------------------------------------------------------------------
// SERIAL PORT configuration --------------------------------------------------
// ----------------------------------------------------------------------------

// Defines the max number of received chars over the serial line that can be
// waiting to be processed, if the buffer gets full incoming data will be lost.
#define SERIAL_RX_BUFFER_SIZE 16

// Defines the max number of chars waiting to be sent over the serial line.
#define SERIAL_TX_BUFFER_SIZE 64

// Defines the speed at which the serial line will operate.
// The default settings are: 8-bit, no parity, 1 stop bit.
#define SERIAL_BAUDRATE       9600

// Defines the char which will represent a null buffer value, do not change this.
#define SERIAL_NO_DATA        0xff

// Defines the max length of a received command string.
#define COMMAND_BUFFER_SIZE   16


// ----------------------------------------------------------------------------
// Temperature sensor configuration -------------------------------------------
// ----------------------------------------------------------------------------
#define THERMISTOR_NOMINAL_TEMP      25.0
#define THERMISTOR_BCOEFFICIENT    3950.0
#define THERMISTOR_NOMINAL_VAL    10000.0
#define THERMISTOR_SERIESRESISTOR 10000.0


// ----------------------------------------------------------------------------
// Includes -------------------------------------------------------------------
// ----------------------------------------------------------------------------
#include <Arduino.h>
#include <avr/wdt.h>

#include "macros.h"
#include "utils.h"

#include "types.h"
#include "struct.h"
#include "strings.h"

#include "lookuptables.h"

#include "ringbuf.h"
#include "serial.h"
#include "print.h"
#include "stepper.h"
#include "a4988.h"
#include "uln2003.h"
#include "cmd.h"
#include "movingmean.h"


// ----------------------------------------------------------------------------
// Global objects -------------------------------------------------------------
// ----------------------------------------------------------------------------
extern movingmean<uint16_t, 10> temp;

#ifdef USE_ULN2003_DRIVER
  extern uln2003 motor1;
#else
  extern a4988 motor1;
#endif

#endif
