//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "RobotFighter.h"

bool ROBOT_FIGHTER::mainloop_level(const MAP_PROPERTIES &map_properties,int num_level)
{
	bool exit_value=false;

	VECTOR camera_pos;
	VECTOR camera_vel;
	double camera_angle=0;
	double camera_zoom =3;
	bool   camera_rotating=false;
	bool   camera_zooming=true;

	double video_radius=squareroot(square(VIDEO::width())+square(VIDEO::height()))/2;
	double audio_radius=16777216;

	int numblocks=4;
	int numballs=4;

	MAP map(map_properties);

	WORLD world(map);

	AUDIO::setspatial_scale(world.getproperties().audio_spatial_scale);

	ROBOT_FIGHTER_OBJECT *robot_fighter=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_robot_fighter_1_4(world);

	BLOCK_OBJECT *blocks=new BLOCK_OBJECT[numblocks];
	BALL_OBJECT *balls=new BALL_OBJECT[numballs];
	for (int i=0;i<numblocks;i++)
	{
		blocks[i].main_init(VECTOR(768+(512*6)+(i*512)+512,2304-1024),0,VECTOR(0,0),0,world);
	}
	for (int i=0;i<numballs;i++)
	{
		balls[i].main_init(VECTOR(768+(512*6)+(i*512),2304-1024),0,VECTOR(0,0),0,world);
	}

	INPUT::clear();

	world.begin_first_frame();

	while (!INPUT::KEYBOARD::escape() && !robot_fighter->main_exit())
	{
		if (INPUT::KEYBOARD::one())
		{
			camera_rotating=false;
		}
		if (INPUT::KEYBOARD::two())
		{
			camera_rotating=true;
		}
		if (INPUT::KEYBOARD::three())
		{
			camera_zooming=false;
		}
		if (INPUT::KEYBOARD::four())
		{
			camera_zooming=true;
		}

		world.integrate();

		camera_pos  =robot_fighter->getposition();
		camera_vel  =robot_fighter->getlinear_velocity();
		camera_angle=0;
		camera_zoom =3;
		if (camera_rotating)
		{
			camera_angle=robot_fighter->getorientation();
		}
		if (camera_zooming)
		{
			camera_zoom=2+(robot_fighter->getlinear_velocity().getlength()/256);
		}

		VIDEO::cls();
		VIDEO::camera(camera_pos.x,camera_pos.y,camera_angle,camera_zoom);

		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(-1);}
		world.draw_skyplane  (camera_pos,video_radius*camera_zoom);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(0);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_background(camera_pos,video_radius*camera_zoom);}
		world.draw_background(camera_pos,video_radius*camera_zoom,1);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(1);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_tertiary  (camera_pos,video_radius*camera_zoom);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_secondary (camera_pos,video_radius*camera_zoom);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_primary   (camera_pos,video_radius*camera_zoom);}
		world.draw_tertiary  (camera_pos,video_radius*camera_zoom,1);
		world.draw_secondary (camera_pos,video_radius*camera_zoom,1);
		world.draw_primary   (camera_pos,video_radius*camera_zoom,1);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_objects();}
		world.draw_objects   ();
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_secondary (camera_pos,video_radius*camera_zoom);}
		world.draw_secondary (camera_pos,video_radius*camera_zoom,1);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(2);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_foreground(camera_pos,video_radius*camera_zoom);}
		world.draw_foreground(camera_pos,video_radius*camera_zoom,0.75);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(3);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,1);}

		VIDEO::TEXT::print_number_fixed((VIDEO::width()/2)-(VIDEO::width()/16),(-VIDEO::height()/2),VIDEO::width()/16,VIDEO::height()/16,num_level,0,1,1,1);

		VIDEO::camera(0,0,0,1);

		VECTOR hbp1(-VIDEO::width()/2,(-VIDEO::height()/2)+8);
		VECTOR hbp2(hbp1.x+(VIDEO::width()/2)*((double)robot_fighter->gethealth()/robot_fighter->getmaxhealth()),(-VIDEO::height()/2)+8);
		VIDEO::draw_beam(hbp1.x,hbp1.y,hbp2.x,hbp2.y,0,0,1,1,0,1,16);

		AUDIO::listener_topdown(camera_pos.x,camera_pos.y,camera_vel.x,camera_vel.y,camera_angle);

		world.sound(camera_pos,audio_radius);

		AUDIO::MUSIC::update();
		VIDEO::show();
		INPUT::update();

		world.end_frame();
	}

	delete robot_fighter;
	delete[] blocks;
	delete[] balls;

	INPUT::clear();

	return(exit_value);
}
