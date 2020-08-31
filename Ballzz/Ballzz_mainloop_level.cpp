//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Ballzz.h"

bool BALLZZ::mainloop_level()
{
	bool exit_value=false;

	VECTOR camera_pos;
	VECTOR camera_vel;
	double camera_angle=0;
	double camera_zoom =4;
	bool   camera_rotating=false;

	double video_radius=squareroot(square(VIDEO::width())+square(VIDEO::height()))/2;
	double audio_radius=16777216;

	MAP map(ballzz_table_map_properties);

	WORLD world(map);

	AUDIO::setspatial_scale(world.getproperties().audio_spatial_scale);

	BALL_OBJECT ball;

	ball.main_init(VECTOR((14*512)+256,(22*512)+256),0,VECTOR(0,-2048),0,world);

	FLIPPER_OBJECT right_flipper;

	right_flipper.main_init(VECTOR(9*512,23*512),0,VECTOR(0,0),0,world);

	right_flipper.connection_init_reset(VECTOR(9*512,23*512),VECTOR(9*512,22*512),
                                        VECTOR(9*512,23*512),VECTOR(10*512,23*512),
                                        VECTOR(9*512,23*512),VECTOR(9*512,24*512),
                                        VECTOR(9*512,23*512),VECTOR(8*512,23*512));

	FLIPPER_OBJECT left_flipper;

	left_flipper.main_init(VECTOR(7*512,23*512),0,VECTOR(0,0),0,world);

	left_flipper.connection_init_reset(VECTOR(7*512,23*512),VECTOR(7*512,22*512),
                                       VECTOR(7*512,23*512),VECTOR(8*512,23*512),
                                       VECTOR(7*512,23*512),VECTOR(7*512,24*512),
                                       VECTOR(7*512,23*512),VECTOR(6*512,23*512));

	FLIPPER_OBJECT xflipper1;

	xflipper1.main_init(VECTOR(2*512,10*512),0,VECTOR(0,0),0,world);

	xflipper1.connection_init_reset(VECTOR(2*512,10*512),VECTOR(2*512,9*512),
                                    VECTOR(2*512,10*512),VECTOR(3*512,10*512),
                                    VECTOR(2*512,10*512),VECTOR(2*512,11*512),
                                    VECTOR(2*512,10*512),VECTOR(1*512,10*512));

	FLIPPER_OBJECT xflipper2;

	xflipper2.main_init(VECTOR(4*512,11*512),0,VECTOR(0,0),0,world);

	xflipper2.connection_init_reset(VECTOR(4*512,11*512),VECTOR(4*512,10*512),
                                    VECTOR(4*512,11*512),VECTOR(5*512,11*512),
                                    VECTOR(4*512,11*512),VECTOR(4*512,12*512),
                                    VECTOR(4*512,11*512),VECTOR(3*512,11*512));

	FLIPPER_OBJECT xflipper3;

	xflipper3.main_init(VECTOR(12*512,12*512),0,VECTOR(0,0),0,world);

	xflipper3.connection_init_reset(VECTOR(12*512,12*512),VECTOR(12*512,11*512),
                                    VECTOR(12*512,12*512),VECTOR(13*512,12*512),
                                    VECTOR(12*512,12*512),VECTOR(12*512,13*512),
                                    VECTOR(12*512,12*512),VECTOR(11*512,12*512));

	INPUT::clear();

	world.begin_first_frame();

	while (!INPUT::KEYBOARD::escape() && !ball.main_exit())
	{
		if (INPUT::KEYBOARD::one())
		{
			camera_rotating=false;
		}
		if (INPUT::KEYBOARD::two())
		{
			camera_rotating=true;
		}

		if (right_flipper.main_exit() || right_flipper.connection_isgone())
		{
			right_flipper.main_reset(world);

			right_flipper.main_init(VECTOR(9*512,23*512),0,VECTOR(0,0),0,world);

			right_flipper.connection_init_reset(VECTOR(9*512,23*512),VECTOR(9*512,22*512),
                                                VECTOR(9*512,23*512),VECTOR(10*512,23*512),
                                                VECTOR(9*512,23*512),VECTOR(9*512,24*512),
                                                VECTOR(9*512,23*512),VECTOR(8*512,23*512));
		}

		if (left_flipper.main_exit() || left_flipper.connection_isgone())
		{
			left_flipper.main_reset(world);

			left_flipper.main_init(VECTOR(7*512,23*512),0,VECTOR(0,0),0,world);

			left_flipper.connection_init_reset(VECTOR(7*512,23*512),VECTOR(7*512,22*512),
                                               VECTOR(7*512,23*512),VECTOR(8*512,23*512),
                                               VECTOR(7*512,23*512),VECTOR(7*512,24*512),
                                               VECTOR(7*512,23*512),VECTOR(6*512,23*512));
		}

		if (xflipper1.main_exit() || xflipper1.connection_isgone())
		{
			xflipper1.main_reset(world);

			xflipper1.main_init(VECTOR(2*512,10*512),0,VECTOR(0,0),0,world);

			xflipper1.connection_init_reset(VECTOR(2*512,10*512),VECTOR(2*512,9*512),
                                            VECTOR(2*512,10*512),VECTOR(3*512,10*512),
                                            VECTOR(2*512,10*512),VECTOR(2*512,11*512),
                                            VECTOR(2*512,10*512),VECTOR(1*512,10*512));
		}

		if (xflipper2.main_exit() || xflipper2.connection_isgone())
		{
			xflipper2.main_reset(world);

			xflipper2.main_init(VECTOR(4*512,11*512),0,VECTOR(0,0),0,world);

			xflipper2.connection_init_reset(VECTOR(4*512,11*512),VECTOR(4*512,10*512),
                                            VECTOR(4*512,11*512),VECTOR(5*512,11*512),
                                            VECTOR(4*512,11*512),VECTOR(4*512,12*512),
                                            VECTOR(4*512,11*512),VECTOR(3*512,11*512));
		}

		if (xflipper3.main_exit() || xflipper3.connection_isgone())
		{
			xflipper3.main_reset(world);

			xflipper3.main_init(VECTOR(12*512,12*512),0,VECTOR(0,0),0,world);

			xflipper3.connection_init_reset(VECTOR(12*512,12*512),VECTOR(12*512,11*512),
                                            VECTOR(12*512,12*512),VECTOR(13*512,12*512),
                                            VECTOR(12*512,12*512),VECTOR(12*512,13*512),
                                            VECTOR(12*512,12*512),VECTOR(11*512,12*512));
		}

		world.integrate();

		camera_pos.setzero();
		camera_pos.x=8*512;
		camera_pos.y=ball.getposition().y;
		camera_vel  =ball.getlinear_velocity();
		camera_angle=0;
		camera_zoom =9;

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
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(2);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_foreground(camera_pos,video_radius*camera_zoom);}
		world.draw_foreground(camera_pos,video_radius*camera_zoom,0.75);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(3);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,1);}

		AUDIO::listener_topdown(camera_pos.x,camera_pos.y,camera_vel.x,camera_vel.y,camera_angle);

		world.sound(camera_pos,audio_radius);

		AUDIO::MUSIC::update();
		VIDEO::show();
		INPUT::update();

		world.end_frame();
	}

	INPUT::clear();

	return(exit_value);
}
