# gen4-tools
 
Some C++ tools to emulate the PRNG mechanics of the Gen 4 Pokémon games.
Very simple implementations of the Linear Congruential Generator and the 32-bit Mersenne Twister, which may be used as part of more complex algorithms, such as methods for generating Pokémon and Trainer IDs.

As an example, here's a program which generates a Trainer ID/Secret ID pair and then finds a shiny using method 1:

```c++
#include "gen4-tools/nds/time.hpp"

#include "gen4-tools/pkm/trainer_id.hpp"
#include "gen4-tools/pkm/pokemon_data.hpp"

#include <iostream>

si32 main()
{
	//Initialise the NDS time and generate the seed
	gen4::nds::Time time { 22, 12, 25, 12, 30, 42 }; //25 December 2022, 12:30:42
	ui32 rng_seed = time.generate_rng_seed(6969); //6969 v-frames since the game was booted

	//Print the seed
	std::cout << "RNG seed: " << std::hex << rng_seed << std::dec << std::endl;

	//Initialise and seed the 32-bit linear congruential generator
	gen4::rng::LCG lc_generator;
	lc_generator.seed(rng_seed);

	//Initialise and seed the 32-bit Merseenne Twister
	gen4::rng::MT19937 mt_generator;
	mt_generator.seed(rng_seed);

	//Generate the trainer IDs
	gen4::pkm::TrainerIds trainer_ids;
	trainer_ids.generate(mt_generator);

	//Print the trainer IDs
	std::cout 
		<< "Trainer ID: " << trainer_ids.get_trainer_id() << std::endl
		<< "Secret ID: "  << trainer_ids.get_secret_id()  << std::endl;

	/*
		Now let's find a shiny using method 1!

		Since method 1 involves 4 advancements (2 for the PID + 2 for the IVs),
		we create a secondary LCG just to simulate those without actually advancing
		the main LCG.

		Then we check the shininess of the newly generated Pokémon using the trainer
		IDs we have just generated.

		If the Pokémon was not shiny, we advance the LCG and continue searching
		until we find one.
	*/
	gen4::pkm::PokemonData pokemon_data;
	gen4::rng::LCG simulated_lcg;
	ui32 lcg_frame = 0;

	//First attempt
	simulated_lcg.seed(lc_generator.get_value());
	pokemon_data.generate_method_1(simulated_lcg);

	//If the first attempt was not successful, keep looking for shinies
	while (!pokemon_data.get_shiny(trainer_ids))
	{
		lc_generator.advance();
		++lcg_frame;

		simulated_lcg.seed(lc_generator.get_value());
		pokemon_data.generate_method_1(simulated_lcg);
	}

	//Success!
	std::cout 
		<< "Successfully found a shiny on frame " << lcg_frame
		<< " of seed " << std::hex << rng_seed << std::dec << "!" << std::endl;

	//Exit
	return 0;
}
```
Here's the output of the program:
```
RNG seed: 740C1B4F
Trainer ID: 20950
Secret ID: 666
Successfully found a shiny on frame 876 of seed 740C1B4F!
```
