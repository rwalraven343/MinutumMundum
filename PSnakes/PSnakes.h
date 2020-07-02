//Code by Rogier Walraven. Do not distribute.

namespace PSNAKES
{
	typedef SOLID_OBJECT<4> SMALL_SOLID_OBJECT;
	typedef SOLID_OBJECT<6> BIG_SOLID_OBJECT;

	typedef LIQUID_OBJECT<4> SMALL_LIQUID_OBJECT;
	typedef LIQUID_OBJECT<6> BIG_LIQUID_OBJECT;

	typedef WORLD<SMALL_SOLID_OBJECT,BIG_SOLID_OBJECT,SMALL_LIQUID_OBJECT,BIG_LIQUID_OBJECT> WORLD;

	typedef FEATURE<WORLD> FEATURE;

	#include "snake_head_object.h"
	#include "snake_tail_object.h"

	#include "psnakes_level1_map.h"
	#include "psnakes_level2_map.h"
	#include "psnakes_level3_map.h"

	bool mainloop_level(const MAP_PROPERTIES &map_properties,int num_level);

	void mainloop();
}
