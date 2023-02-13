#pragma once

#include "gen4-tools/rng/mt19937.hpp"

namespace pkm
{
	struct TrainerId
	{
		ui16 main_id { 0x0000 };
		ui16 secret_id { 0x0000 };

		void generate(rng::MT19937& mt19937)
		{
			mt19937.advance();
			*(reinterpret_cast<ui32*>(&main_id)) = mt19937.get_value();
		}
	};
}
