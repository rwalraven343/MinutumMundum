//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "RobotFighter.h"

void ROBOT_FIGHTER::mainloop()
{
	bool level1_finished=false;

	begin:

	INPUT::clear();

	while (!INPUT::KEYBOARD::escape() && !INPUT::KEYBOARD::space())
	{
		VIDEO::cls();
		VIDEO::camera(0,0,0,1);

		const char *text1="Robot Fighter!!!";
		const char *text2="Press <space> to begin or <escape> to quit.";

		int dimx,dimy;

		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(-1);}

		VIDEO::TEXT::getdimensions(dimx,dimy,text1);
		VIDEO::TEXT::print_fixed((-VIDEO::width()/2),(-VIDEO::height()/2),VIDEO::width()/dimx,VIDEO::height()/8,text1,1,1,1,1);

		VIDEO::TEXT::getdimensions(dimx,dimy,text2);
		VIDEO::TEXT::print_fixed((-VIDEO::width()/2),(-VIDEO::height()/2)+(VIDEO::height()/8),VIDEO::width()/dimx,VIDEO::height()/8,text2,1,1,1,1);

		AUDIO::listener_topdown(0,0,0,0,0);

		AUDIO::MUSIC::update();
		VIDEO::show();
		INPUT::update();
	}

	if (INPUT::KEYBOARD::escape())
	{
		goto exit;
	}

	INPUT::clear();

	if (!level1_finished)
	{
		level1_finished=mainloop_level(robot_fighter_level1_map_properties,1);

		if (!level1_finished)
		{
			goto skip;
		}
	}

	INPUT::clear();

	while (!INPUT::KEYBOARD::escape() && !INPUT::KEYBOARD::space())
	{
		VIDEO::cls();
		VIDEO::camera(0,0,0,1);

		const char *text1="Game Complete!!!";

		int dimx,dimy;

		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(-1);}

		VIDEO::TEXT::getdimensions(dimx,dimy,text1);
		VIDEO::TEXT::print_fixed((-VIDEO::width()/2),(-VIDEO::height()/16),VIDEO::width()/dimx,VIDEO::height()/8,text1,1,1,1,1);

		AUDIO::listener_topdown(0,0,0,0,0);

		AUDIO::MUSIC::update();
		VIDEO::show();
		INPUT::update();
	}

	INPUT::clear();

	skip:

	goto begin;

	exit:
	;
}
