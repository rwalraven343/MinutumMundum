//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "PSnakes.h"

namespace PSNAKES
{
	struct SNAKE_FOOD
	{
		SNAKE_FOOD();
		~SNAKE_FOOD();

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

		SNAKE_FOOD(const SNAKE_FOOD &other) {}
		SNAKE_FOOD& operator=(const SNAKE_FOOD &other) {}
	};
}

bool PSNAKES::mainloop_level(const MAP_PROPERTIES &map_properties,int num_level)
{
	bool exit_value=false;

	VECTOR camera_pos;
	VECTOR camera_vel;
	double camera_angle=0;
	double camera_zoom =4;
	bool   camera_rotating=false;

	double video_radius=squareroot(square(VIDEO::width())+square(VIDEO::height()))/2;
	double audio_radius=16777216;

	int max_snake_length=7;
	int max_snake_foods=16;

	int segments_to_add=0;

	int food_eaten=0;

	bool complete_snake=false;

	MAP map(map_properties);

	WORLD world(map);

	AUDIO::setspatial_scale(world.getproperties().audio_spatial_scale);

	SNAKE_HEAD_OBJECT *snake_head=PSNAKES_LEVEL1_MAP::OBJECTS1::create_snake_head_8_8(world);

	SNAKE_TAIL_OBJECT *snake_tails=new SNAKE_TAIL_OBJECT[max_snake_length];

	SNAKE_FOOD *snake_foods=new SNAKE_FOOD[max_snake_foods];

	AUDIO::SOURCE ping_snd("Data\\Samples\\metallic_swing.wav",1,true);

	INPUT::clear();

	world.begin_first_frame();

	while (!INPUT::KEYBOARD::escape() && !snake_head->main_exit() && !complete_snake)
	{
		if (INPUT::KEYBOARD::one())
		{
			camera_rotating=false;
		}
		if (INPUT::KEYBOARD::two())
		{
			camera_rotating=true;
		}

		for (int i=0;i<max_snake_foods;i++)
		{
			if (!snake_foods[i].wasinit() || snake_foods[i].isgone())
			{
				snake_foods[i].init_reset();
			}
		}

		segments_to_add=0;

		if (snake_head->wasinit() && !snake_head->isgone() && !snake_head->hasvapourized())
		{
			for (int i=0;i<max_snake_foods;i++)
			{
				if (snake_foods[i].wasinit() && !snake_foods[i].isgone())
				{
					if (snake_foods[i].iswithin(snake_head->getposition()))
					{
						snake_foods[i].init_reset();

						segments_to_add++;
					}
				}
			}
		}

		if (ping_snd.stopped())
		{
			ping_snd.rewind();
		}

		for (int i=0;i<max_snake_length;i++)
		{
			if (snake_tails[i].main_wasinit() && snake_tails[i].main_exit())
			{
				snake_tails[i].main_reset(world);

				food_eaten--;
			}
		}

		for (int s=0;s<segments_to_add;s++)
		{
			for (int i=0;i<max_snake_length;i++)
			{
				if (!snake_tails[i].main_wasinit())
				{
					for (int j=i+1;j<max_snake_length;j++)
					{
						if (snake_tails[j].main_wasinit())
						{
							goto exit1;
						}
					}

					snake_tails[i].main_init(VECTOR(-4352,-4352),0,VECTOR(0,0),0,world);

					if (i==0)
					{
						snake_tails[i].connection_init_reset(*snake_head);
					}
					else
					{
						snake_tails[i].connection_init_reset(snake_tails[i-1]);
					}

					food_eaten++;

					if (ping_snd.initial())
					{
						ping_snd.play(0,0,0,0,1,1);
					}

					goto exit2;
				}
			}

			complete_snake=true;

			exit2:
			;
		}

		exit1:

		world.integrate();

		camera_pos  =snake_head->getposition();
		camera_vel  =snake_head->getlinear_velocity();
		camera_angle=0;
		if (camera_rotating)
		{
			camera_angle=snake_head->getorientation();
		}
		camera_zoom =4;

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
		for (int i=0;i<max_snake_foods;i++)
		{
			if (snake_foods[i].wasinit() && !snake_foods[i].isgone())
			{
				if (VIDEO::DYNAMIC_LIGHTING::available())
				{
					snake_foods[i].light();
				}

				snake_foods[i].draw();
			}
		}
		world.draw_primary   (camera_pos,video_radius*camera_zoom,1);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_objects();}
		world.draw_objects   ();
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(2);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_foreground(camera_pos,video_radius*camera_zoom);}
		world.draw_foreground(camera_pos,video_radius*camera_zoom,0.75);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(3);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,1);}

		VIDEO::TEXT::print_number_fixed((-VIDEO::width()/2),(-VIDEO::height()/2),VIDEO::width()/16,VIDEO::height()/16,food_eaten,1,0,1,1);
		VIDEO::TEXT::print_fixed((-VIDEO::width()/2)+(VIDEO::width()/16),(-VIDEO::height()/2),VIDEO::width()/16,VIDEO::height()/16," of 8",1,1,1,1);
		VIDEO::TEXT::print_number_fixed((VIDEO::width()/2)-(VIDEO::width()/16),(-VIDEO::height()/2),VIDEO::width()/16,VIDEO::height()/16,num_level,0,1,1,1);

		AUDIO::listener_topdown(camera_pos.x,camera_pos.y,camera_vel.x,camera_vel.y,camera_angle);

		world.sound(camera_pos,audio_radius);

		AUDIO::MUSIC::update();
		VIDEO::show();
		INPUT::update();

		if (complete_snake)
		{
			complete_snake=false;

			if (snake_head->wasinit() && !snake_head->isgone() && !snake_head->hasvapourized())
			{
				int result=0;

				for (int i=0;i<max_snake_length;i++)
				{
					if (snake_tails[i].main_wasinit() && !snake_tails[i].main_exit() && !snake_tails[i].hasvapourized() && snake_tails[i].connection_intact())
					{
						result++;
					}
				}

				if (result==max_snake_length)
				{
					complete_snake=true;
				}
			}
		}

		world.end_frame();
	}

	if (!INPUT::KEYBOARD::escape())
	{
		if (!snake_head->main_exit())
		{
			INPUT::clear();

			while (!INPUT::KEYBOARD::escape() && !INPUT::KEYBOARD::space())
			{
				VIDEO::cls();
				VIDEO::camera(0,0,0,1);

				const char *text1="Snake Complete!!!";

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

			exit_value=true;
		}
		else
		{
			INPUT::clear();

			while (!INPUT::KEYBOARD::escape() && !INPUT::KEYBOARD::space())
			{
				VIDEO::cls();
				VIDEO::camera(0,0,0,1);

				const char *text1="Snake Head is dead!!!";

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
		}
	}

	delete snake_head;
	delete[] snake_tails;
	delete[] snake_foods;

	INPUT::clear();

	return(exit_value);
}

PSNAKES::SNAKE_FOOD::SNAKE_FOOD()
{
	int r=RANDOM::global(3);

	switch (r)
	{
	case 0:
		sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\ball1.bmp");
	break;
	case 1:
		sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\ball2.bmp");
	break;
	case 2:
		sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\ball3.bmp");
	break;
	}

	initialized=false;
}

PSNAKES::SNAKE_FOOD::~SNAKE_FOOD()
{
	VIDEO::freetexture(sprite);
}

void PSNAKES::SNAKE_FOOD::init_reset()
{
	circle=CIRCLE(VECTOR(RANDOM::global(0,16*128*4),RANDOM::global(0,16*128*4)),256);

	start_time=SYSTEM::seconds_since_startup();

	life_time=RANDOM::global(15,30);

	initialized=true;
}

bool PSNAKES::SNAKE_FOOD::wasinit() const
{
	return(initialized);
}

bool PSNAKES::SNAKE_FOOD::isgone() const
{
	return(SYSTEM::seconds_since_startup()>(start_time+life_time));
}

bool PSNAKES::SNAKE_FOOD::iswithin(const VECTOR &pos) const
{
	double t=SYSTEM::seconds_since_startup()-start_time;

	if (t>=life_time/4 && t<=(life_time*3)/4)
	{
		return(circle.iswithin(pos,0));
	}

	return(false);
}

void PSNAKES::SNAKE_FOOD::light() const
{
	VIDEO::DYNAMIC_LIGHTING::draw_pointlight(circle.position.x,circle.position.y,1,1,1,compact_gaussian(SYSTEM::seconds_since_startup()-start_time,life_time,false),1024);
}

void PSNAKES::SNAKE_FOOD::draw() const
{
	VECTOR p1=circle.position+VECTOR(-circle.radius,-circle.radius);
	VECTOR p2=circle.position+VECTOR( circle.radius,-circle.radius);
	VECTOR p3=circle.position+VECTOR( circle.radius, circle.radius);
	VECTOR p4=circle.position+VECTOR(-circle.radius, circle.radius);

	VIDEO::draw_quad_texture(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,p4.x,p4.y,sprite,compact_gaussian(SYSTEM::seconds_since_startup()-start_time,life_time,false));
}
