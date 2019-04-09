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

#include "eeprom.h"

#ifdef USE_EEPROM
  void eeprom_init(eeprom_map_t * ptr)
  {
    eeprom_busy_wait();
    eeprom_load(ptr);

    if ((*ptr).header != EEPROM_MAGIC_HEADER)
    {
      memset(ptr, 0, sizeof(eeprom_map_t));
      (*ptr).header = EEPROM_MAGIC_HEADER;
      eeprom_save(ptr);
    }
  }

  void eeprom_load(eeprom_map_t * ptr)
  {
    eeprom_read_block(ptr, EEPROM_START_ADDRESS, sizeof(eeprom_map_t));
  }

  void eeprom_save(eeprom_map_t * ptr)
  {
    eeprom_update_block(ptr, EEPROM_START_ADDRESS, sizeof(eeprom_map_t));
  }
#endif
