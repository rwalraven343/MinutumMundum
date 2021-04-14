//Code by Rogier Walraven. Do not distribute.

namespace ROBOT_FIGHTER
{
	typedef SOLID_OBJECT<4> SMALL_SOLID_OBJECT;
	typedef SOLID_OBJECT<6> BIG_SOLID_OBJECT;

	typedef LIQUID_OBJECT<4> SMALL_LIQUID_OBJECT;
	typedef LIQUID_OBJECT<6> BIG_LIQUID_OBJECT;

	typedef WORLD<SMALL_SOLID_OBJECT,BIG_SOLID_OBJECT,SOLID_OCTAGON_OBJECT,BIG_LIQUID_OBJECT> WORLD;

	typedef FEATURE<WORLD> FEATURE;

	#include "robot_fighter_object.h"
	#include "block_object.h"
	#include "ball_object.h"
	#include "flyer_object.h"

	#include "robot_fighter_level1_map.h"

	bool mainloop_level1();

	void mainloop();
}
