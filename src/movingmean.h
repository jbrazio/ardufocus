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

#ifndef __SMOOTHING_Mv_H__
#define __SMOOTHING_Mv_H__

template <typename T, uint8_t N> struct movingmean
{
public:
  movingmean() { reset(); }

private:
  volatile T       v_values[N];
  volatile T       v_sum_of_values;
  volatile uint8_t v_index;
  volatile uint8_t v_count;

private:
  inline void reset() {
    v_count = v_index = v_sum_of_values = 0;
    for (size_t i = 0; i < N; i++) { v_values[i] = 0; }
  }

public:
  inline bool full() const {
    return (v_count == N);
  }

  inline T operator()() const {
    if (v_count == 0) { return 0; }
    return v_sum_of_values / v_count;
  }

  inline movingmean& operator=(const T& lhs) {
    reset();
    v_count = v_index = 1;
    v_values[0]       = lhs;
    v_sum_of_values   = lhs;
    return (*this);
  }

  inline movingmean& operator+=(const T& lhs) {
    v_sum_of_values    -= v_values[v_index];
    v_values[v_index++] = lhs;
    v_sum_of_values    += lhs;

    if (v_index >= N) { v_index = 0; }
    else if (v_count <  N) { ++v_count; }

    return (*this);
  }
};

#endif
