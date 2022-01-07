/**
 * Ardufocus - Moonlite                                                                                                                                                                                                                                                                                                                                                                                                                                atible focuser
 * Copyright (C) 2017-2022 João Brázio [joao@brazio.org]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __PCB_CONFIG_H__
#define __PCB_CONFIG_H__

 //
 // Version: PCB 2019-05-rev1
 //

 //#define ENABLE_SECOND_OUTPUT
 //#define ENABLE_MANUAL_MOTOR_CONTROL
 //#define ENABLE_HIGH_RESOLUTION_MODE
 //#define ENABLE_MOTOR1_INVERT_DIRECTION
 //#define ENABLE_MOTOR2_INVERT_DIRECTION


 // ----------------------------------------------------------------------------
 // DO NOT EDIT ANYTHING BELLOW THIS HEADER ------------------------------------
 // ----------------------------------------------------------------------------
#define USE_EEPROM
#define USE_SMOOTHSTEP_ACCEL

#ifdef ENABLE_HIGH_RESOLUTION_MODE
#define HIGH_RESOLUTION_MODE
#endif

#ifdef ENABLE_MOTOR1_INVERT_DIRECTION
#define MOTOR1_INVERT_DIRECTION
#endif

#ifdef ENABLE_MOTOR2_INVERT_DIRECTION
#define MOTOR2_INVERT_DIRECTION
#endif

#define MOTOR1_PINOUT   6, 5, 4
#define MOTOR1_USE_A4988_DRIVER
#define MOTOR1_SLEEP_WHEN_IDLE
#define MOTOR1_SLEEP_TIMEOUT 60
#define MOTOR1_MAX_SPEED    250
#define MOTOR1_MIN_SPEED     10

#ifdef ENABLE_SECOND_OUTPUT
#define MOTOR2_PINOUT   9, 8, 7
#define MOTOR2_USE_A4988_DRIVER
#define MOTOR2_SLEEP_WHEN_IDLE
#define MOTOR2_SLEEP_TIMEOUT 60
#define MOTOR2_MAX_SPEED    250
#define MOTOR2_MIN_SPEED     10
#endif

#define NTC_ADC_CHANNEL          0
#define NTC_NOMINAL_TEMP      25.0F
#define NTC_BCOEFFICIENT    3950.0F
#define NTC_NOMINAL_VAL    10000.0F
#define NTC_RESISTOR_VAL   10000.0F

#ifdef ENABLE_MANUAL_MOTOR_CONTROL
#define USE_UI_KAP
#define UI_KAP_FWD_BUTTON_PIN  2 // AUX6
#define UI_KAP_BWD_BUTTON_PIN  3 // AUX7
#define UI_KAP_SWT_BUTTON_PIN 16 // AUX2
#define UI_KAP_ADC_CHANNEL     1 // AUX1
#endif

#endif
