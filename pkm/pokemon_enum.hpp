#pragma once

#include "gen4-tools/data_types.hpp"

namespace pkm
{
	enum class Gender : ui8
	{
		MALE = 0,
		FEMALE = 1,
		GENDERLESS = 2
	};

	enum class GenderRatio : ui8
	{
		MALE_100 = 0x00,
		MALE_87 = 0x1F,
		MALE_75 = 0x3F,
		MALE_50 = 0x7F,
		MALE_25 = 0xBF,
		MALE_0 = 0xFE,
		GENDERLESS = 0xFF
	};

	enum class Nature : ui8
	{
		HARDY, LONELY, BRAVE, ADAMANT, NAUGHTY,
		BOLD, DOCILE, RELAXED, IMPISH, LAX,
		TIMID, HASTY, SERIOUS, JOLLY, NAIVE,
		MODEST, MILD, QUIET, BASHFUL, RASH,
		CALM, GENTLE, SASSY, CAREFUL, QUIRKY
	};

	enum class Type : ui8
	{
		FIGHTING,
		FLYING,
		POISON,
		GROUND,
		ROCK,
		BUG,
		GHOST,
		STEEL,
		FIRE,
		WATER,
		GRASS,
		ELECTRIC,
		PSYCHIC,
		ICE,
		DRAGON,
		DARK
	};
}