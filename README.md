# gen4-tools
 
Header-only C++ library containing various tools to emulate the PRNG mechanics of the Generation 4 Pokémon games (Diamond and Pearl, Platinum, HeartGold and SoulSilver).

The library provides simple implementations of the two PRNGs used in the games: the 32-bit Linear Congruential Generator and the MT19937.
Implementations of some of the algorithms involved in the generation of Pokémon and Trainer IDs are also provided.

More features are to come!


As an example, here's a small program which, given a certain date, time and vframe, searches for a Chansey holding a Lucky Egg on Route 209 in Pokémon Platinum Version.

```c++
#include <iostream>
#include <iomanip>

#include "gen4-tools/nds/time.hpp"
#include "gen4-tools/pkm/trainer_id.hpp"
#include "gen4-tools/pkm/pokemon_data.hpp"

int main()
{
	//Generate the seed using the startup date and frame
	nds::Time time = nds::Time{ 2023, 1, 1, 12, 0, 0 };
	ui32 seed = time.generate_rng_seed(3000);

	//Initialise the LCG which will be used to search through the encounters
	rng::LCG search_lcg;
	search_lcg.seed(seed);

	//Allocate the LCG which will be used to simulate each encounter
	rng::LCG encounter_lcg;

	//Allocate the structure which will be used to store the data of each generated Pokémon
	pkm::PokemonData pokemon_data;

	//Check the first 2000 frames
	for (ui32 lcg_frame = 0; lcg_frame < 2000; lcg_frame++)
	{
		//Copy the value of the search_lcg to the encounter_lcg
		encounter_lcg.seed(search_lcg.get_value());

		//Generate the encounter slot
		encounter_lcg.advance();
		ui8 encounter_slot = ((encounter_lcg.get_value() >> 16) / 656) & 0xFF;

		//Make sure it's either slot 9 or slot 11, as those are the ones in which Chansey is found
		if (encounter_slot < 94 || (encounter_slot == 98))
		{
			//Keep searching
			search_lcg.advance();
			continue;
		}

		//Generate the Chansey
		pokemon_data.generate_method_j(encounter_lcg, pkm::LeadPokemon::NONE);

		//Generate the item
		encounter_lcg.advance();
		ui8 item_value = (encounter_lcg.get_value() >> 16) % 100;

		//Make sure it's a Lucky Egg
		if (item_value < 95)
		{
			//Keep searching
			search_lcg.advance();
			continue;
		}

		//Success!
		std::cout 
			<< "Found a Chansey holding a Lucky Egg on frame " << std::dec << lcg_frame
			<< " of seed 0x" << std::setfill('0') << std::setw(8) << std::hex << std::uppercase << seed << "!"
			<< std::endl;

		//Keep searching
		search_lcg.advance();
	}

	return 0;
}
```
Here's the output of the program:
```
Found a Chansey holding a Lucky Egg on frame 90 of seed 0x010C0BCF!
Found a Chansey holding a Lucky Egg on frame 362 of seed 0x010C0BCF!
Found a Chansey holding a Lucky Egg on frame 1373 of seed 0x010C0BCF!
Found a Chansey holding a Lucky Egg on frame 1832 of seed 0x010C0BCF!
Found a Chansey holding a Lucky Egg on frame 1995 of seed 0x010C0BCF!
```
