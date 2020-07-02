//Code by Rogier Walraven. Do not distribute.

namespace SCHIZODRIVER
{
	typedef SOLID_OBJECT<4> SMALL_SOLID_OBJECT;
	typedef SOLID_OBJECT<6> BIG_SOLID_OBJECT;

	typedef LIQUID_OBJECT<4> SMALL_LIQUID_OBJECT;
	typedef LIQUID_OBJECT<6> BIG_LIQUID_OBJECT;

	typedef WORLD<SMALL_SOLID_OBJECT,BIG_SOLID_OBJECT,SMALL_LIQUID_OBJECT,BIG_LIQUID_OBJECT> WORLD;

	typedef FEATURE<WORLD> FEATURE;

	#include "Maps.h"
	#include "Objects.h"

	void mainloop();
}
