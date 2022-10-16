# gen4-tools
 
Some C++ tools to emulate the PRNG mechanics of the Gen 4 Pokémon games.
Very simple implementations of the Linear Congruential Generator and the 32-bit Mersenne Twister, which can be included as part of more complex algorithms, such as methods for generating Pokémon and Trainer IDs.

```
#include "gen4-tools/nds/time.hpp"

#include "gen4-tools/rng/lcg.hpp"
#include "gen4-tools/rng/mt19937.hpp"

int main()
{
	gen4::nds::Time time { 22, 12, 25, 11, 42, 3 }; //Initialise the DS time and delay
	ui32 delay = 5982; //Define a delay of about 100 seconds
	ui32 seed = time.generate_rng_seed(5982); //Generate the rng seed from the time

	gen4::rng::LCG lc_generator; //Initialise the Linear Congruential Generator
	lc_generator.seed(seed); //Seed the LCG
	lc_generator.advance(); //Advance the LCG

	ui32 lc_value = lc_generator.get_value(); //Get the value of the LCG
 
	gen4::rng::MT19937 mt_generator; //Initialise the 32-bit Mersenne Twister
	mt_generator.seed(seed); //Seed the MT
	mt_generator.advance(); //Advance the MT

	ui32 mt_value = mt_generator.get_value(); //Get the value of the MT

	...
}
```
