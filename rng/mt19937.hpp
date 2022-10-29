#pragma once

#include "gen4-tools/utility/data_types.hpp"

//The (hardcoded) parameters of the 32-bit MT19937
#define MT_DEGREE 624
#define MT_MIDDLE_WORD 397

#define MT_UPPER_BIT_MASK 0x80000000
#define MT_LOWER_BIT_MASK 0x7fffffff

#define MT_INITIALISATION_COEFFICIENT	0x6C078965
#define MT_TWIST_MATRIX_COEFFICIENT 	0x9908B0DF

#define MT_TEMPERING_BIT_SHIFT_1(value)	value >> 11
#define MT_TEMPERING_BIT_SHIFT_2(value)	value << 7
#define MT_TEMPERING_BIT_SHIFT_3(value)	value << 15
#define MT_TEMPERING_BIT_SHIFT_4(value)	value >> 18
#define MT_TEMPERING_BIT_MASK_1	0xFFFFFFFF
#define MT_TEMPERING_BIT_MASK_2	0x9D2C5680
#define MT_TEMPERING_BIT_MASK_3	0xEFC60000
#define MT_TEMPERING_BIT_MASK_4	0xFFFFFFFF

namespace gen4::rng
{
	//Definition of the MT19937
	class MT19937
	{
	private:
		//Current sequence of values
		ui32 m_values [MT_DEGREE] {};

		//Inedex of the current value
		ui16 m_value_index { 0 };

	private:
		//Apply the twisting matrix to the provided value
		static const ui32 twist_value(const ui32& t_value)
		{
			ui32 twisted_value = (t_value >> 1);
			if (t_value & 1) { twisted_value ^= MT_TWIST_MATRIX_COEFFICIENT; }

			return twisted_value;
		}

		//Apply the tempering matrix to the provided value
		static const ui32 temper_value(const ui32& t_value)
		{
			ui32 tempered_value = t_value;

			tempered_value ^= MT_TEMPERING_BIT_SHIFT_1(tempered_value) & MT_TEMPERING_BIT_MASK_1;
			tempered_value ^= MT_TEMPERING_BIT_SHIFT_2(tempered_value) & MT_TEMPERING_BIT_MASK_2;
			tempered_value ^= MT_TEMPERING_BIT_SHIFT_3(tempered_value) & MT_TEMPERING_BIT_MASK_3;
			tempered_value ^= MT_TEMPERING_BIT_SHIFT_4(tempered_value) & MT_TEMPERING_BIT_MASK_4;

			return tempered_value;
		}

	private:
		//Generate the sequence from the seed
		void initialise_sequence()
		{
			for (ui32 i_value = 1; i_value < MT_DEGREE; ++i_value)
			{
				m_values[i_value] =
				(
					MT_INITIALISATION_COEFFICIENT *
					(
						m_values[i_value - 1] ^ 
						(m_values[i_value - 1] >> (32 - 2))
					) + i_value
				);
			}
		}
	
		//Generate a new sequence by applying the recurrence relation using values from the current one
		void twist_sequence()
		{
			for (ui32 i_value = 0; i_value < MT_DEGREE; ++i_value)
			{
				//Concatenate two consecutive terms of the sequence
				ui32 concatenated_value =
				(
					(m_values[i_value] & MT_UPPER_BIT_MASK) | 
					(m_values[(i_value + 1) % MT_DEGREE] & MT_LOWER_BIT_MASK)
				);
			
				//Apply the recurrence relation
				m_values[i_value] = m_values[(i_value + MT_MIDDLE_WORD) % MT_DEGREE] ^ twist_value(concatenated_value);
			}
		}

	public:
		//Seed and intialise the MT19937
		void seed(const ui32& t_seed)
		{
			m_values[0] = t_seed;
			m_value_index = 0;

			initialise_sequence();
			twist_sequence();
		}

		//Advance the MT19937
		void advance() 
		{
			//Increment the index
			++m_value_index;

			//If all values have already been used, generate new ones
			if (m_value_index >= MT_DEGREE)
			{
				twist_sequence();
				m_value_index = 0;
			}
		}

		//Temper and retrieve the current valute
		const ui32 get_value()
		{
			return temper_value(m_values[m_value_index]);
		}
	};
}