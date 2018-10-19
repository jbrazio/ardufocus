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

#include "eeprom.h"

#ifdef USE_EEPROM
  void eeprom_init(eeprom_map_t * ptr_config)
  {
    eeprom_busy_wait();
    eeprom_load(ptr_config);

    if ((*ptr_config).header != EEPROM_MAGIC_HEADER)
    {
      (*ptr_config) = {
        .header = EEPROM_MAGIC_HEADER,
        .position_m1 = 0,
        .position_m2 = 0
      };
    }

    eeprom_save(ptr_config);
  }


  void eeprom_load(eeprom_map_t * ptr_config)
  {
    eeprom_read_block(ptr_config, EEPROM_START_ADDRESS, sizeof(eeprom_map_t));
  }

  void eeprom_save(eeprom_map_t * ptr_config)
  {
    eeprom_update_block(ptr_config, EEPROM_START_ADDRESS, sizeof(eeprom_map_t));
  }
#endif
