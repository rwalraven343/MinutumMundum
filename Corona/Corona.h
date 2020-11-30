//Code by Rogier Walraven. Do not distribute.

namespace CORONA
{
	typedef SOLID_OBJECT<4> SMALL_SOLID_OBJECT;
	typedef SOLID_OBJECT<6> BIG_SOLID_OBJECT;

	typedef LIQUID_OBJECT<4> SMALL_LIQUID_OBJECT;
	typedef LIQUID_OBJECT<6> BIG_LIQUID_OBJECT;

	typedef WORLD<SOLID_OCTAGON_OBJECT,BIG_SOLID_OBJECT,SMALL_LIQUID_OBJECT,BIG_LIQUID_OBJECT> WORLD;

	typedef FEATURE<WORLD> FEATURE;

	#include "virus_object.h"
	#include "whitebloodcell_object.h"

	#include "corona_level1_map.h"

	bool mainloop_level(const MAP_PROPERTIES &map_properties,int num_level);

	void mainloop();
}
