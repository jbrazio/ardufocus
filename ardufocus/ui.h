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

#ifndef __UI_H__
#define __UI_H__

#include "config.h"

#ifdef USE_UI_KAP
  #include "ui_keybd.h"
#endif

namespace UI
{
  inline void setup()
  {
    #ifdef USE_UI_KAP
      Keybd::setup();
    #endif
  }

  inline void update_display()
  {
    ;
  }

  inline void fetch_key_state()
  {
    #ifdef USE_UI_KAP
      Keybd::tick();
    #endif
  }
}

#endif
