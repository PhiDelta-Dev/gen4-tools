#pragma once

#include "gen4-tools/rng/lcg.hpp"
#include "gen4-tools/pkm/pokemon_enum.hpp"
#include "gen4-tools/pkm/trainer_id.hpp"

namespace pkm
{
	enum class LeadPokemon : ui8
	{
		NONE,
		SYNCHRONIZE
	};

	class PokemonData
	{
		struct IndividualValues
		{
			ui8 hp { 0 };
			ui8 attack { 0 };
			ui8 defense { 0 };
			ui8 speed { 0 };
			ui8 special_attack { 0 };
			ui8 special_defense { 0 };
		};

		struct HiddenPower
		{
			Type type { Type::FIGHTING };
			ui8 power { 0 };
		};

	public:
		ui32 personality_value { 0x00000000 };
		Nature nature { Nature::HASTY };
		bool ability_id { false };
		ui8 gender_value { 0x00 };
		ui16 shiny_value { 0x0000 };
		IndividualValues individual_values {};
		HiddenPower hidden_power {};

	private:
		void generate_personality_value_method_1(rng::LCG& lcg);
		void generate_personality_value_methods_jk(rng::LCG& lcg);
		void generate_nature_method_j(rng::LCG& lcg, const LeadPokemon& lead_pokemon, const ui8& lead_parameter);
		void generate_nature_method_k(rng::LCG& lcg, const LeadPokemon& lead_pokemon, const ui8& lead_parameter);
		void generate_individual_values(rng::LCG& lcg);
		void calculate_hidden_power();

	public:
		void generate_method_1(rng::LCG& lcg);
		void generate_method_j(rng::LCG& lcg, const LeadPokemon& lead_pokemon = LeadPokemon::NONE, const ui8& lead_parameter = 0);
		void generate_method_k(rng::LCG& lcg, const LeadPokemon& lead_pokemon = LeadPokemon::NONE, const ui8& lead_parameter = 0);

		const Gender get_gender(const GenderRatio& gender_ratio) const;
		const bool is_shiny(const TrainerId& trainer_id) const;
	};

	void PokemonData::generate_personality_value_method_1(rng::LCG& lcg)
	{
		lcg.advance();
		reinterpret_cast<ui16*>(&personality_value)[0] = lcg.get_value() >> 16;
		lcg.advance();
		reinterpret_cast<ui16*>(&personality_value)[1] = lcg.get_value() >> 16;

		nature = static_cast<Nature>(personality_value % 25);
		ability_id = personality_value & 1;
		gender_value = personality_value & 0xFF;
		shiny_value = reinterpret_cast<ui16*>(&personality_value)[0] ^ reinterpret_cast<ui16*>(&personality_value)[1];
	}

	void PokemonData::generate_personality_value_methods_jk(rng::LCG& lcg)
	{
		do
		{
			lcg.advance();
			reinterpret_cast<ui16*>(&personality_value)[0] = lcg.get_value() >> 16;
			lcg.advance();
			reinterpret_cast<ui16*>(&personality_value)[1] = lcg.get_value() >> 16;
		} 
		while (personality_value % 25 != static_cast<ui8>(nature));

		ability_id = personality_value & 1;
		gender_value = personality_value & 0xFF;
		shiny_value = reinterpret_cast<ui16*>(&personality_value)[0] ^ reinterpret_cast<ui16*>(&personality_value)[1];
	}

	void PokemonData::generate_nature_method_j(rng::LCG& lcg, const LeadPokemon& lead_pokemon, const ui8& lead_parameter)
	{
		bool synchronize_nature = false;
		if (lead_pokemon == LeadPokemon::SYNCHRONIZE)
		{
			lcg.advance();
			synchronize_nature = !(lcg.get_value() >> 31);
		}

		if (synchronize_nature)
		{
			nature = static_cast<Nature>(lead_parameter);
		}
		else
		{
			lcg.advance();
			nature = static_cast<Nature>((lcg.get_value() >> 16) / 0xa3e);
		}
	}

	void PokemonData::generate_nature_method_k(rng::LCG& lcg, const LeadPokemon& lead_pokemon, const ui8& lead_parameter)
	{
		bool synchronize_nature = false;
		if (lead_pokemon == LeadPokemon::SYNCHRONIZE)
		{
			lcg.advance();
			synchronize_nature = !((lcg.get_value() >> 16) & 1);
		}

		if (synchronize_nature)
		{
			nature = static_cast<Nature>(lead_parameter);
		}
		else
		{
			lcg.advance();
			nature = static_cast<Nature>((lcg.get_value() >> 16) % 25);
		}
	}

	void PokemonData::calculate_hidden_power()
	{
		ui8 hidden_power_type_id = 0;
		hidden_power.power = 0;
		for (ui8 i_individual_value = 0; i_individual_value < 6; i_individual_value++)
		{
			const ui8& individual_value = reinterpret_cast<const ui8*>(&individual_values)[i_individual_value];
			hidden_power_type_id += (individual_value & 1) << i_individual_value;
			hidden_power.power += ((individual_value >> 1) & 1) << i_individual_value;
		}
		hidden_power.type = static_cast<Type>(hidden_power_type_id * 15 / 0x3F);
		hidden_power.power = hidden_power.power * 40 / 0x3F + 30;
	}

	void PokemonData::generate_individual_values(rng::LCG& lcg)
	{
		lcg.advance();
		ui16 individual_values_first = lcg.get_value() >> 16;
		lcg.advance();
		ui16 individual_values_second = lcg.get_value() >> 16;

		for (ui8 i_individual_value = 0; i_individual_value < 3; i_individual_value++)
		{
			reinterpret_cast<ui8*>(&individual_values)[i_individual_value] = (individual_values_first >> (5 * i_individual_value)) & 0x1F;
			reinterpret_cast<ui8*>(&individual_values)[i_individual_value + 3] = (individual_values_second >> (5 * i_individual_value)) & 0x1F;
		}

		calculate_hidden_power();
	}

	void PokemonData::generate_method_1(rng::LCG& lcg)
	{
		generate_personality_value_method_1(lcg);
		generate_individual_values(lcg);
	}

	void PokemonData::generate_method_j(rng::LCG& lcg, const LeadPokemon& lead_pokemon, const ui8& lead_parameter)
	{
		generate_nature_method_j(lcg, lead_pokemon, lead_parameter);
		generate_personality_value_methods_jk(lcg);
		generate_individual_values(lcg);
	}

	void PokemonData::generate_method_k(rng::LCG& lcg, const LeadPokemon& lead_pokemon, const ui8& lead_parameter)
	{
		generate_nature_method_k(lcg, lead_pokemon, lead_parameter);
		generate_personality_value_methods_jk(lcg);
		generate_individual_values(lcg);
	}

	const Gender PokemonData::get_gender(const GenderRatio& gender_ratio) const
	{
		switch (gender_ratio)
		{
		case GenderRatio::MALE_100: return Gender::MALE;
		case GenderRatio::MALE_0: return Gender::FEMALE;
		case GenderRatio::GENDERLESS: return Gender::GENDERLESS;
		default: return static_cast<Gender>(gender_value < static_cast<ui8>(gender_ratio));
		}
	}

	const bool PokemonData::is_shiny(const TrainerId& trainer_id) const
	{
		return (trainer_id.main_id ^ trainer_id.secret_id ^ shiny_value) < 8;
	}
}
