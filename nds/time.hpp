#pragma once

#include "gen4-tools/utility/data_types.hpp"

#define NDS_FPS 59.8261

namespace gen4::nds
{
	//Definition of the NDS time structure (these really are 32-bit integers!)
	struct Time
	{
		ui32 year;
		ui32 month;
		ui32 day;
		ui32 hour;
		ui32 minute;
		ui32 second;

		//Generate the RNG seed from the NDS time and delay
		const ui32 generate_rng_seed(const ui32& t_delay) const
		{
			return ((month * day + minute + second) << 24) | (hour << 16) + year + t_delay;
		}
	};
}