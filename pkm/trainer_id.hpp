#pragma once

#include "gen4-tools/rng/mt19937.hpp"

namespace gen4::pkm
{
	class TrainerIds
	{
	private:
		ui32 m_raw_id { 0 };

	public:
		void generate(rng::MT19937& t_mt)
		{
			t_mt.advance();
			m_raw_id = t_mt.get_value();
		}

		inline const ui16 get_trainer_id() const { return static_cast<ui16>(m_raw_id); }
		inline const ui16 get_secret_id() const { return m_raw_id >> 16; }
	};
}
