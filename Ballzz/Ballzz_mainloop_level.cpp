//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Ballzz.h"

namespace BALLZZ
{
	struct SPECIAL
	{
		SPECIAL();
		~SPECIAL();

		void init_reset();

		bool wasinit() const;
		bool isgone() const;

		bool iswithin(const VECTOR &pos) const;

		void light() const;

		void draw() const;

      private:

		bool initialized;

		CIRCLE circle;

		VIDEO::TEXTURE sprite;

		double start_time;
		double life_time;

		SPECIAL(const SPECIAL &other) {}
		SPECIAL& operator=(const SPECIAL &other) {}
	};
}

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

	int max_specials=8;

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

	SPECIAL *specials=new SPECIAL[max_specials];

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

		ball.clearspecialforces();

		for (int i=0;i<max_specials;i++)
		{
			if (!specials[i].wasinit() || specials[i].isgone())
			{
				specials[i].init_reset();
			}

			if (specials[i].iswithin(ball.getposition()))
			{
				VECTOR f(0,-4.096e6);

				ball.addspecialforce(f.getrotated(RANDOM::global(-45,45)));

				specials[i].init_reset();
			}
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
		for (int i=0;i<max_specials;i++)
		{
			if (specials[i].wasinit() && !specials[i].isgone())
			{
				if (VIDEO::DYNAMIC_LIGHTING::available())
				{
					specials[i].light();
				}

				specials[i].draw();
			}
		}
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

	delete[] specials;

	INPUT::clear();

	return(exit_value);
}

BALLZZ::SPECIAL::SPECIAL()
{
	sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\arrow.bmp");

	initialized=false;
}

BALLZZ::SPECIAL::~SPECIAL()
{
	VIDEO::freetexture(sprite);
}

void BALLZZ::SPECIAL::init_reset()
{
	circle=CIRCLE(VECTOR(RANDOM::global(0,13*128*4),RANDOM::global(0,24*128*4)),256);

	start_time=SYSTEM::seconds_since_startup();

	life_time=RANDOM::global(15,30);

	initialized=true;
}

bool BALLZZ::SPECIAL::wasinit() const
{
	return(initialized);
}

bool BALLZZ::SPECIAL::isgone() const
{
	return(SYSTEM::seconds_since_startup()>(start_time+life_time));
}

bool BALLZZ::SPECIAL::iswithin(const VECTOR &pos) const
{
	double t=SYSTEM::seconds_since_startup()-start_time;

	if (t>=life_time/4 && t<=(life_time*3)/4)
	{
		return(circle.iswithin(pos,0));
	}

	return(false);
}

void BALLZZ::SPECIAL::light() const
{
	VIDEO::DYNAMIC_LIGHTING::draw_pointlight(circle.position.x,circle.position.y,1,1,1,compact_gaussian(SYSTEM::seconds_since_startup()-start_time,life_time,false),1024);
}

void BALLZZ::SPECIAL::draw() const
{
	VECTOR p1=circle.position+VECTOR(-circle.radius,-circle.radius);
	VECTOR p2=circle.position+VECTOR( circle.radius,-circle.radius);
	VECTOR p3=circle.position+VECTOR( circle.radius, circle.radius);
	VECTOR p4=circle.position+VECTOR(-circle.radius, circle.radius);

	VIDEO::draw_quad_texture(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,p4.x,p4.y,sprite,compact_gaussian(SYSTEM::seconds_since_startup()-start_time,life_time,false));
}
