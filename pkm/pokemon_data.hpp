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

}