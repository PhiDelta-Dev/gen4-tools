#pragma once

#include "gen4-tools/data_types.hpp"

#define LCG_MULTIPLIER 0x41C64E6D
#define LCG_INVERSE_MULTIPLIER 0xEEB9EB65
#define LCG_INCREMENT 0x00006073

namespace rng
{
	class LCG
	{
	private:
		ui32 value { 0 };
		ui32 advancements{ 0 };

	public:
		void seed(const ui32& seed) 
		{ 
			value = seed;
			advancements = 0;
		}

		void advance()
		{ 
			value = LCG_MULTIPLIER * value + LCG_INCREMENT; 
			advancements++;
		}

		void rewind()
		{
			value = LCG_INVERSE_MULTIPLIER * (value - LCG_INCREMENT);
			advancements--;
		}

		const ui32& get_value() const 
		{
			return value;
		}
		
		const ui32& get_advancements() const
		{
			return advancements;
		}
	};
}