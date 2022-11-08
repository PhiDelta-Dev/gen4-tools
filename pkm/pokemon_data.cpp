#include "pokemon_data.hpp"

namespace gen4::pkm
{
	void PokemonData::generate_method_1(rng::LCG& t_lcg)
	{
		//Generate the personality value
		t_lcg.advance();
		m_personality_value = t_lcg.get_value() >> 16;
		t_lcg.advance();
		m_personality_value |= t_lcg.get_value() & 0xFFFF0000;

		//Calculate the nature id, abilty id, gender value and shiny value from the personality value
		m_nature_id = m_personality_value % 25;
		m_ability_id = m_personality_value & 1;
		m_gender_value = m_personality_value & 0xFF;
		m_shiny_value = (m_personality_value >> 16) ^ (m_personality_value & 0xFFFF);

		//Generate the IVs
		t_lcg.advance();
		m_ivs.hp				= (t_lcg.get_value() >> 16) & 0x1F;
		m_ivs.attack			= (t_lcg.get_value() >> 21) & 0x1F;
		m_ivs.defense			= (t_lcg.get_value() >> 26) & 0x1F;
		t_lcg.advance();
		m_ivs.special_attack	= (t_lcg.get_value() >> 21) & 0x1F;
		m_ivs.special_defense	= (t_lcg.get_value() >> 26) & 0x1F;
		m_ivs.speed				= (t_lcg.get_value() >> 16) & 0x1F;

		//Calculate the HP type from the IVs
		m_hidden_power.type_id = 
		(
			((m_ivs.hp				& 1) << 0) + 
			((m_ivs.attack			& 1) << 1) +
			((m_ivs.defense			& 1) << 2) +
			((m_ivs.speed			& 1) << 3) +
			((m_ivs.special_attack	& 1) << 4) +
			((m_ivs.special_defense	& 1) << 5)
		) * 15 / 0x3F;

		//Calculate the HP power from the IVs
		m_hidden_power.power =
		(
			(((m_ivs.hp					>> 1) & 1) << 0) +
			(((m_ivs.attack				>> 1) & 1) << 1) +
			(((m_ivs.defense			>> 1) & 1) << 2) +
			(((m_ivs.speed				>> 1) & 1) << 3) +
			(((m_ivs.special_attack		>> 1) & 1) << 4) +
			(((m_ivs.special_defense	>> 1) & 1) << 5)
		) * 40 / 0x3F + 30;
	}

	const ui8 PokemonData::get_gender_id(const ui8& t_gender_ratio) const
	{
		switch (t_gender_ratio)
		{
		case 0x00: return 0;	//Always male
		case 0xFE: return 1;	//Always female
		case 0xFF: return 2;	//Always genderless
		default: return static_cast<ui8>(m_gender_value < t_gender_ratio);
		}
	}

	const bool PokemonData::get_shiny(const TrainerIds& t_trainer_ids) const
	{
		return (t_trainer_ids.get_trainer_id() ^ t_trainer_ids.get_secret_id() ^ m_shiny_value) < 8;
	}
}