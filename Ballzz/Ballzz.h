//Code by Rogier Walraven. Do not distribute.

namespace BALLZZ
{
	typedef SOLID_OBJECT<4> SMALL_SOLID_OBJECT;
	typedef SOLID_OBJECT<6> BIG_SOLID_OBJECT;

	typedef LIQUID_OBJECT<4> SMALL_LIQUID_OBJECT;
	typedef LIQUID_OBJECT<6> BIG_LIQUID_OBJECT;

	typedef WORLD<SOLID_OCTAGON_OBJECT,BIG_SOLID_OBJECT,SMALL_LIQUID_OBJECT,BIG_LIQUID_OBJECT> WORLD;

	typedef FEATURE<WORLD> FEATURE;

	#include "ball_object.h"
	#include "flipper_object.h"

	#include "ballzz_table_map.h"

	bool mainloop_level(int &high_score);

	void mainloop();
}
