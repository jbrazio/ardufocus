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

#include "ui_keybd.h"

#ifdef USE_UI_KAP

motor_t Keybd::motor = MOTOR_ONE;
bool Keybd::inited = false;

void Keybd::setup()
{
  if(inited) { return; }
  inited = true;

  IO::set_as_input(UI_KAP_FWD_BUTTON_PIN);
  IO::set_as_input(UI_KAP_BWD_BUTTON_PIN);

  #ifdef UI_KAP_SWT_BUTTON_PIN
    IO::set_as_input(UI_KAP_SWT_BUTTON_PIN);
  #endif

  #ifndef UI_KAP_INVERT_BUTTON_LOGIC
    // Activates the internal pullup resistor
    IO::write(UI_KAP_FWD_BUTTON_PIN, HIGH);
    IO::write(UI_KAP_BWD_BUTTON_PIN, HIGH);
    #ifdef UI_KAP_SWT_BUTTON_PIN
      IO::write(UI_KAP_SWT_BUTTON_PIN, HIGH);
    #endif
  #endif

  #ifdef UI_KAP_FWD_BUTTON_LED_PIN
    IO::set_as_output(UI_KAP_FWD_BUTTON_LED_PIN);
    IO::write(UI_KAP_FWD_BUTTON_LED_PIN, LOW);
  #endif

  #ifdef UI_KAP_BWD_BUTTON_LED_PIN
    IO::set_as_output(UI_KAP_BWD_BUTTON_LED_PIN);
    IO::write(UI_KAP_BWD_BUTTON_LED_PIN, LOW);
  #endif

  #ifdef UI_KAP_SWT_BUTTON_LED_PIN
    IO::set_as_output(UI_KAP_SWT_BUTTON_LED_PIN);
    IO::write(UI_KAP_SWT_BUTTON_LED_PIN, LOW);
  #endif
}

void Keybd::tick()
{
  setup();

  // Get button state
  bool button_fwd_state = (bool)(IO::read(UI_KAP_FWD_BUTTON_PIN) == LOW); // fwd
  bool button_bwd_state = (bool)(IO::read(UI_KAP_BWD_BUTTON_PIN) == LOW); // back

  #ifdef UI_KAP_INVERT_BUTTON_LOGIC
    button_fwd_state ^= true;
    button_bwd_state ^= true;
  #endif

  static uint8_t counter[3] = { 0 };
  static bool previous_state[3] = { false };

  // Debounce routine
  bool button_fwd_trigger = false;
  debounce(button_fwd_state, previous_state[0], button_fwd_trigger, counter[0], UI_KAP_BUTTON_DEBOUNCE);

  bool button_bwd_trigger = false;
  debounce(button_bwd_state, previous_state[1], button_bwd_trigger, counter[1], UI_KAP_BUTTON_DEBOUNCE);

  // Visual feedback when the forward button is pressed
  #ifdef UI_KAP_FWD_BUTTON_LED_PIN
  IO::write(UI_KAP_FWD_BUTTON_LED_PIN, (button_fwd_state) ? HIGH : LOW);
  #endif

  // Visual feedback when the backward button is pressed
  #ifdef UI_KAP_BWD_BUTTON_LED_PIN
  IO::write(UI_KAP_BWD_BUTTON_LED_PIN, (button_bwd_state) ? HIGH : LOW);
  #endif

  static uint8_t old_motor_speed = 0;

  // Invalid state when two buttons are pushed at the same time
  if(button_fwd_state && button_bwd_state) { return; }

  if(button_fwd_state || button_bwd_state) {
    if(button_fwd_trigger || button_bwd_trigger) {
      old_motor_speed = api::motor_get_speed(motor);
    }

    // It's important that the "new_motor_speed" value not to
    // be lower than 2, because at the stepper tick routine it
    // will be divivded by two.
    uint8_t new_motor_speed = map(Analog::read(UI_KAP_ADC_CHANNEL), 0, 1023, 2, 64);
    api::motor_set_speed(motor, new_motor_speed);

    if(! api::motor_is_moving(motor)) {
      api::motor_set_target(motor, (button_fwd_state) ? -1 : 0);
      api::motor_start(motor);
    }
  } else {
    if(button_fwd_trigger || button_bwd_trigger) {
      api::motor_stop(motor);
      api::motor_set_speed(motor, old_motor_speed);
    }

    #if defined(UI_KAP_SWT_BUTTON_PIN) && defined(MOTOR2_HAS_DRIVER)
      bool button_swt_state = (bool)(IO::read(UI_KAP_SWT_BUTTON_PIN) == LOW); // motor switch

      #ifdef UI_KAP_INVERT_BUTTON_LOGIC
      button_swt_state ^= true;
      #endif

      bool button_swt_trigger = false;
      debounce(button_swt_state, previous_state[2], button_swt_trigger, counter[2], UI_KAP_BUTTON_DEBOUNCE);

      #ifdef UI_KAP_SWT_BUTTON_LED_PIN
      IO::write(UI_KAP_SWT_BUTTON_LED_PIN, (motor == MOTOR_ONE) ? HIGH : LOW);
      #endif

      if(button_swt_state && button_swt_trigger) {
        if(! api::motor_is_moving(motor)) {
          switch(motor) {
            case MOTOR_ONE:
            comms.reply("1");
              motor = MOTOR_TWO;
              break;

            case MOTOR_TWO:
            comms.reply("2");
              motor = MOTOR_ONE;
              break;
          }
        }
      }
    #endif
  }
}

void Keybd::debounce(bool& current_state, bool& previous_state, bool& trigger_event, uint8_t& counter, const uint8_t& threshold)
{
  setup();

  if(current_state == previous_state) {
    trigger_event = false;
    counter = 0;
    return;
  }

  if(counter < threshold) {
    current_state = previous_state;
    trigger_event = false;
    ++counter;
    return;
  }

  previous_state = current_state;
  trigger_event = true;
  counter = 0;
  return;
}
#endif
