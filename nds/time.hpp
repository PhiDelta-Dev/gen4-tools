#pragma once

#include "gen4-tools/data_types.hpp"

#define NDS_FPS 59.8261

namespace nds
{
	struct Time
	{
		ui32 year { 2000 };
		ui32 month { 01 };
		ui32 day { 01 };
		ui32 hour { 00 };
		ui32 minute { 00 };
		ui32 second { 00 };

		const ui32 generate_rng_seed(const ui32& frame) const
		{
			return ((month * day + minute + second) << 24) | (hour << 16) + (year % 100) + frame;
		}
	};
}