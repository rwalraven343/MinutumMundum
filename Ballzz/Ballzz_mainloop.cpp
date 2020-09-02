//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Ballzz.h"

void BALLZZ::mainloop()
{
	bool game_finished=false;
	int high_score=500;

	begin:

	INPUT::clear();

	while (!INPUT::KEYBOARD::escape() && !INPUT::KEYBOARD::space())
	{
		VIDEO::cls();
		VIDEO::camera(0,0,0,1);

		const char *text1="Ballzz!!!";
		const char *text2="Press <space> to begin or <escape> to quit.";
		const char *text3="Use <left> and <right> and <b> to control flippers.";

		int dimx,dimy;
		int gw,gh;

		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(-1);}

		VIDEO::TEXT::getdimensions(dimx,dimy,text1);
		VIDEO::TEXT::print_fixed((-VIDEO::width()/2),(-VIDEO::height()/2),VIDEO::width()/dimx,VIDEO::height()/8,text1,1,1,1,1);

		VIDEO::TEXT::getdimensions(dimx,dimy,text2);
		VIDEO::TEXT::print_fixed((-VIDEO::width()/2),(-VIDEO::height()/2)+(VIDEO::height()/8),VIDEO::width()/dimx,VIDEO::height()/8,text2,1,1,1,1);

		VIDEO::TEXT::getdimensions(dimx,dimy,text3);
		VIDEO::TEXT::print_fixed((-VIDEO::width()/2),(-VIDEO::height()/2)+((VIDEO::height()/8)*2),VIDEO::width()/dimx,VIDEO::height()/8,text3,1,1,1,1);

		VIDEO::TEXT::getdimensions_number(dimx,dimy,high_score);
		gw=VIDEO::width()/11;
		gh=VIDEO::height()/11;
		VIDEO::TEXT::print_number_fixed(0-((dimx*gw)/2),0-((dimy*gh)/2),gw,gh,high_score,1,1,1,1);

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

	if (!game_finished)
	{
		game_finished=mainloop_level(high_score);

		if (!game_finished)
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
