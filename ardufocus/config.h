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

#ifndef __CONFIG_H__
#define __CONFIG_H__

 // ----------------------------------------------------------------------------
 // OFFICIAL PCB ---------------------------------------------------------------
 // ----------------------------------------------------------------------------
 // Enable this configuration file if you are using on the of the official PCB
 // boards, bellow you can fine tune the firmware to your specific configuration.
 //
 // Version: PCB 2019-05-rev1
 // Documentation: TODO
#define BOARD_OFFICIAL_PCB

//#define ENABLE_THE_SECOND_MOTOR_OUTPUT
//#define ENABLE_MANUAL_MOTOR_CONTROL
//#define ENABLE_HIGH_RESOLUTION_MODE
//#define ENABLE_MOTOR1_INVERT_DIRECTION
//#define ENABLE_MOTOR2_INVERT_DIRECTION


// ----------------------------------------------------------------------------
// CUSTOM BOARD ---------------------------------------------------------------
// ----------------------------------------------------------------------------
// This configuration file will give you the most flexibility and control over
// the Ardufocus firmware.
//
//#define BOARD_CUSTOM


// ----------------------------------------------------------------------------
// DO NOT EDIT ANYTHING BELLOW THIS HEADER ------------------------------------
// ----------------------------------------------------------------------------
#include "assert.h"

#endif
