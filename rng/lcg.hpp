#pragma once

#include "gen4-tools/utility/data_types.hpp"

//The (hardcoded) parameters of the LCG
#define LCG_MULTIPLIER	0x41C64E6D
#define LCG_INCREMENT	0x00006073

namespace gen4::rng
{
	//Definition of the LCG
	class LCG
	{
	private:
		//Current value
		ui32 m_value;

	public:
		//Seed the LCG
		void seed(const ui32& t_seed) { m_value = t_seed; }

		//Advancement function
		void advance()
		{ 
			//Use the current value to obtain a new one, which is stored in its place
			m_value = LCG_MULTIPLIER * m_value + LCG_INCREMENT; 
		}

		//Get the current value
		const ui32& get_value() const { return m_value; }
	};
}