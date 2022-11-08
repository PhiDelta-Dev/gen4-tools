#pragma once

#include "gen4-tools/rng/lcg.hpp"
#include "gen4-tools/pkm/trainer_id.hpp"

namespace gen4::pkm
{
	class PokemonData
	{
		struct IVs
		{
			ui8 hp				{ 0 };
			ui8 attack			{ 0 };
			ui8 defense			{ 0 };
			ui8 special_attack	{ 0 };
			ui8 special_defense	{ 0 };
			ui8 speed			{ 0 };
		};

		struct HiddenPower
		{
			ui8 type_id { 0 };
			ui8 power	{ 0 };
		};

	private:
		ui32 m_personality_value { 0 };
		ui8 m_nature_id { 0 };
		bool m_ability_id { 0 };
		ui8 m_gender_value { 0 };
		ui16 m_shiny_value { 0 };

		IVs m_ivs {};
		HiddenPower m_hidden_power {};

	public:
		void generate_method_1(rng::LCG& t_lcg);

		inline const ui32& get_personality_value() const { return m_personality_value; }
		inline const ui8& get_nautre_id() const { return m_nature_id; }
		inline const bool& get_ability_id() const { return m_ability_id; }
		inline const ui8& get_gender_value() const { return m_gender_value; }
		inline const ui16& get_shiny_value() const { return m_shiny_value; }
		inline const IVs& get_ivs() const { return m_ivs; }
		inline const HiddenPower& get_hidden_power() const { return m_hidden_power; }

		const ui8 get_gender_id(const ui8& t_gender_ratio) const;
		const bool get_shiny(const TrainerIds& t_trainer_ids) const;
	};

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