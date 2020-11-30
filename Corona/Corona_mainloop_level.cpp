//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Corona.h"

namespace CORONA
{
	struct WHITEBLOODCELL_SPAWNER
	{
		WHITEBLOODCELL_SPAWNER(int mapwidth,int mapheight,int tilewidth,int tileheight,int tilewidth_scale,int tileheight_scale,const int *shape_index,int numwbcs);
		~WHITEBLOODCELL_SPAWNER();

		void update(const VECTOR &vpos,double radius,WORLD &world);

      private:

		int mapwidth,mapheight;
		int tilewidth,tileheight;
		int tilewidth_scale,tileheight_scale;
		const int *shape_index;

		WHITEBLOODCELL_OBJECT *wbcs;
		int numwbcs;

		WHITEBLOODCELL_SPAWNER(const WHITEBLOODCELL_SPAWNER &other) {}
		WHITEBLOODCELL_SPAWNER& operator=(const WHITEBLOODCELL_SPAWNER &other) {}
	};
}

bool CORONA::mainloop_level(const MAP_PROPERTIES &map_properties,int num_level)
{
	bool exit_value=false;

	VECTOR camera_pos;
	VECTOR camera_vel;
	double camera_angle=0;
	double camera_zoom =4;
	bool   camera_rotating=false;

	double video_radius=squareroot(square(VIDEO::width())+square(VIDEO::height()))/2;
	double audio_radius=16777216;

	MAP map(map_properties);

	WORLD world(map);

	AUDIO::setspatial_scale(world.getproperties().audio_spatial_scale);

	VIRUS_OBJECT virus;

	virus.main_init(VECTOR((21*512)+256,(44*512)+256),0,VECTOR(0,0),0,world);

	WHITEBLOODCELL_OBJECT whitebloodcell;

	whitebloodcell.main_init(VECTOR((9*512)+256,(34*512)+256),0,VECTOR(0,0),0,world);

	INPUT::clear();

	world.begin_first_frame();

	while (!INPUT::KEYBOARD::escape() && !virus.main_exit())
	{
		if (INPUT::KEYBOARD::one())
		{
			camera_rotating=false;
		}
		if (INPUT::KEYBOARD::two())
		{
			camera_rotating=true;
		}

		world.integrate();

		camera_pos  =virus.getposition();
		camera_vel  =virus.getlinear_velocity();
		camera_angle=0;
		if (camera_rotating)
		{
			camera_angle=virus.getorientation();
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
		world.draw_primary   (camera_pos,video_radius*camera_zoom,1);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_objects();}
		world.draw_objects   ();
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(2);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_foreground(camera_pos,video_radius*camera_zoom);}
		world.draw_foreground(camera_pos,video_radius*camera_zoom,0.75);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(3);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,1);}

		VIDEO::TEXT::print_number_fixed((VIDEO::width()/2)-(VIDEO::width()/16),(-VIDEO::height()/2),VIDEO::width()/16,VIDEO::height()/16,num_level,0,1,1,1);

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

CORONA::WHITEBLOODCELL_SPAWNER::WHITEBLOODCELL_SPAWNER(int mapwidth,int mapheight,int tilewidth,int tileheight,int tilewidth_scale,int tileheight_scale,const int *shape_index,int numwbcs)
{
	this->mapwidth=mapwidth;
	this->mapheight=mapheight;
	this->tilewidth=tilewidth;
	this->tileheight=tileheight;
	this->tilewidth_scale=tilewidth_scale;
	this->tileheight_scale=tileheight_scale;
	this->shape_index=shape_index;

	wbcs=new WHITEBLOODCELL_OBJECT[numwbcs];
	this->numwbcs=numwbcs;
}

CORONA::WHITEBLOODCELL_SPAWNER::~WHITEBLOODCELL_SPAWNER()
{
	delete[] wbcs;
}

void CORONA::WHITEBLOODCELL_SPAWNER::update(const VECTOR &vpos,double radius,WORLD &world)
{
}
