#pragma once

#include "gen4-tools/data_types.hpp"

#define MT_DEGREE 624
#define MT_MIDDLE_WORD 397

#define MT_UPPER_BIT_MASK 0x80000000
#define MT_LOWER_BIT_MASK 0x7fffffff

#define MT_INITIALISATION_COEFFICIENT	0x6C078965
#define MT_TWIST_MATRIX_COEFFICIENT 	0x9908B0DF

#define MT_TEMPERING_BIT_SHIFT_1(value)	(value >> 11)
#define MT_TEMPERING_BIT_SHIFT_2(value)	(value << 7)
#define MT_TEMPERING_BIT_SHIFT_3(value)	(value << 15)
#define MT_TEMPERING_BIT_SHIFT_4(value)	(value >> 18)
#define MT_TEMPERING_BIT_MASK_1	0xFFFFFFFF
#define MT_TEMPERING_BIT_MASK_2	0x9D2C5680
#define MT_TEMPERING_BIT_MASK_3	0xEFC60000
#define MT_TEMPERING_BIT_MASK_4	0xFFFFFFFF

namespace rng
{
	class MT19937
	{
	private:
		ui32 values[MT_DEGREE] {};

		ui16 current_value_index { 0 };

	private:
		static const ui32 twist_value(const ui32& value)
		{
			ui32 twisted_value = (value >> 1);
			if (value & 1) { twisted_value ^= MT_TWIST_MATRIX_COEFFICIENT; }

			return twisted_value;
		}

		static const ui32 temper_value(const ui32& value)
		{
			ui32 tempered_value = value;

			tempered_value ^= MT_TEMPERING_BIT_SHIFT_1(tempered_value) & MT_TEMPERING_BIT_MASK_1;
			tempered_value ^= MT_TEMPERING_BIT_SHIFT_2(tempered_value) & MT_TEMPERING_BIT_MASK_2;
			tempered_value ^= MT_TEMPERING_BIT_SHIFT_3(tempered_value) & MT_TEMPERING_BIT_MASK_3;
			tempered_value ^= MT_TEMPERING_BIT_SHIFT_4(tempered_value) & MT_TEMPERING_BIT_MASK_4;

			return tempered_value;
		}

	private:
		void initialise_sequence()
		{
			for (ui32 i_value = 1; i_value < MT_DEGREE; i_value++)
			{
				values[i_value] = MT_INITIALISATION_COEFFICIENT * (values[i_value - 1] ^ (values[i_value - 1] >> (32 - 2))) + i_value;
			}
		}
	
		void twist_sequence()
		{
			for (ui32 i_value = 0; i_value < MT_DEGREE; i_value++)
			{
				ui32 concatenated_value = (values[i_value] & MT_UPPER_BIT_MASK) | (values[(i_value + 1) % MT_DEGREE] & MT_LOWER_BIT_MASK);

				values[i_value] = values[(i_value + MT_MIDDLE_WORD) % MT_DEGREE] ^ twist_value(concatenated_value);
			}
		}

	public:
		void seed(const ui32& seed)
		{
			values[0] = seed;
			current_value_index = 0;

			initialise_sequence();
			twist_sequence();
		}

		void advance() 
		{
			current_value_index++;

			if (current_value_index >= MT_DEGREE)
			{
				twist_sequence();

				current_value_index = 0;
			}
		}

		void rewind()
		{
			if (current_value_index == 0)
			{
				//TO BE ADDED

				current_value_index = MT_DEGREE;
			}

			--current_value_index;
		}

		const ui32 get_value() const
		{
			return temper_value(values[current_value_index]);
		}
	};
}