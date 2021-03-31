//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "RobotFighter.h"

int main()
{
	SYSTEM::init();
	VIDEO::init(1024,768);
	VIDEO::DYNAMIC_LIGHTING::init(4);
	VIDEO::TEXT::init("Data\\Tilesets\\codepage437.bmp");
	AUDIO::init();
	AUDIO::MUSIC::init("Data\\Music\\free_space.ogg");
	INPUT::init();

	ROBOT_FIGHTER::mainloop();

	INPUT::quit();
	AUDIO::MUSIC::quit();
	AUDIO::quit();
	VIDEO::TEXT::quit();
	VIDEO::DYNAMIC_LIGHTING::quit();
	VIDEO::quit();
	SYSTEM::quit();
}
