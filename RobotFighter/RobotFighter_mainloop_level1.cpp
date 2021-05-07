//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "RobotFighter.h"

bool ROBOT_FIGHTER::mainloop_level1()
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

	MAP map(robot_fighter_level1_map_properties);

	WORLD world(map);

	AUDIO::setspatial_scale(world.getproperties().audio_spatial_scale);

	ROBOT_FIGHTER_OBJECT *robot_fighter=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_robot_fighter_1_4(world);

	WALKER_OBJECT *walker_50_5=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_50_5(world);
	WALKER_OBJECT *walker_30_6=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_30_6(world);
	WALKER_OBJECT *walker_60_6=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_60_6(world);
	WALKER_OBJECT *walker_40_10=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_40_10(world);
	WALKER_OBJECT *walker_50_15=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_50_15(world);
	WALKER_OBJECT *walker_30_16=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_30_16(world);
	WALKER_OBJECT *walker_20_17=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_20_17(world);
	WALKER_OBJECT *walker_10_19=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_10_19(world);
	WALKER_OBJECT *walker_40_21=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_40_21(world);
	WALKER_OBJECT *walker_60_22=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_60_22(world);
	WALKER_OBJECT *walker_50_25=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_50_25(world);
	WALKER_OBJECT *walker_30_26=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_30_26(world);
	WALKER_OBJECT *walker_5_30=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_walker_5_30(world);

	walker_50_5->inittargetpos(1024);
	walker_30_6->inittargetpos(1024);
	walker_60_6->inittargetpos(1024);
	walker_40_10->inittargetpos(1024);
	walker_50_15->inittargetpos(1024);
	walker_30_16->inittargetpos(1024);
	walker_20_17->inittargetpos(1024);
	walker_10_19->inittargetpos(1024);
	walker_40_21->inittargetpos(1024);
	walker_60_22->inittargetpos(1024);
	walker_50_25->inittargetpos(1024);
	walker_30_26->inittargetpos(1024);
	walker_5_30->inittargetpos(1024);

	FLYER_OBJECT *flyer_11_0=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_flyer_11_0(world);
	FLYER_OBJECT *flyer_13_0=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_flyer_13_0(world);
	FLYER_OBJECT *flyer_15_0=ROBOT_FIGHTER_LEVEL1_MAP::OBJECTS1::create_flyer_15_0(world);

	KINEMATIC_MOTION motions[2];
	motions[0].deltapos=VECTOR(0,1024);
	motions[0].start=0;
	motions[0].duration=8;
	motions[1].deltapos=VECTOR(0,-1024);
	motions[1].start=8;
	motions[1].duration=8;

	flyer_11_0->init_reset_kinematic_motions(2,motions,16,false,false);
	flyer_13_0->init_reset_kinematic_motions(2,motions,16,false,false);
	flyer_15_0->init_reset_kinematic_motions(2,motions,16,false,false);

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

		VECTOR rfpos=robot_fighter->getposition();

		walker_50_5->settargetpos(rfpos);
		walker_30_6->settargetpos(rfpos);
		walker_60_6->settargetpos(rfpos);
		walker_40_10->settargetpos(rfpos);
		walker_50_15->settargetpos(rfpos);
		walker_30_16->settargetpos(rfpos);
		walker_20_17->settargetpos(rfpos);
		walker_10_19->settargetpos(rfpos);
		walker_40_21->settargetpos(rfpos);
		walker_60_22->settargetpos(rfpos);
		walker_50_25->settargetpos(rfpos);
		walker_30_26->settargetpos(rfpos);
		walker_5_30->settargetpos(rfpos);

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

		VIDEO::TEXT::print_number_fixed((VIDEO::width()/2)-(VIDEO::width()/16),(-VIDEO::height()/2),VIDEO::width()/16,VIDEO::height()/16,1,0,1,1,1);

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
	delete walker_50_5;
	delete walker_30_6;
	delete walker_60_6;
	delete walker_40_10;
	delete walker_50_15;
	delete walker_30_16;
	delete walker_20_17;
	delete walker_10_19;
	delete walker_40_21;
	delete walker_60_22;
	delete walker_50_25;
	delete walker_30_26;
	delete walker_5_30;
	delete flyer_11_0;
	delete flyer_13_0;
	delete flyer_15_0;

	INPUT::clear();

	return(exit_value);
}
