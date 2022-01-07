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

#include "utility.h"

 /**
  * @brief [brief description]
  * @details [long description]
  *
  */
float util::steinhart(const uint16_t& raw)
{
	if (raw < NTC_MIN_RAW_VALUE || raw > NTC_MAX_RAW_VALUE)
	{
		return -13.37F;
	}

	float steinhart = constrain(raw, 1, 1022);
	steinhart = NTC_RESISTOR_VAL / (1023.0F / steinhart - 1.0F); // convert raw to ohms
	steinhart = log(steinhart / NTC_NOMINAL_VAL);				 // ln(R/Ro)
	steinhart /= NTC_BCOEFFICIENT;								 // 1/B * ln(R/Ro)
	steinhart += 1.0F / (NTC_NOMINAL_TEMP + 273.15F);			 // + (1/To)
	steinhart = 1.0F / steinhart;								 // invert
	steinhart -= 273.15F;										 // convert to K to C
	return steinhart;
}

#ifdef HAS_ACCELERATION
/**
 * @brief Linear interpolation function
 * @details Linear interpolation on a set of data points (x0, y0), (x1, y1),
 *          is defined as the concatenation of linear interpolants between
 *          each pair of data points. [https://bit.ly/2JP5SIh]
 *
 */
float util::lerp(float const& edge0, float const& edge1, float x)
{
	return (1.0 - x) * edge0 + x * edge1;
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
float util::clamp(float x, float const& lowerlimit, float const& upperlimit)
{
	if (x < lowerlimit)
		x = lowerlimit;
	else if (x > upperlimit)
		x = upperlimit;
	return x;
}

#ifdef USE_SMOOTHSTEP_ACCEL
/**
 * @brief Sigmoid-like interpolation and clamping function
 * @details 3rd-order equation [https://bit.ly/2la5Ek0]
 *
 */
float util::smoothstep(float const& edge0, float const& edge1, float x)
{
	// Scale, and clamp x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	return x * x * (3.0 - 2.0 * x);
}

/**
 * @brief Sigmoid-like interpolation and clamping function
 * @details 5th-order equation [https://bit.ly/2JUMe0I]
 *
 */
float util::smootherstep(float const& edge0, float const& edge1, float x)
{
	// Scale, and clamp x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
}

/**
 * @brief Sigmoid-like interpolation and clamping function
 * @details 7th-order equation [https://bit.ly/2MtinL2]
 *
 */
float util::smootheststep(float const& edge0, float const& edge1, float x)
{
	// Scale, and clamp x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	return x * x * x * x * (x * (x * (-x * 20.0 + 70.0) - 84.0) + 35.0);
}
#endif
#endif
