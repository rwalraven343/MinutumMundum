//Code by Rogier Walraven. Do not distribute.

#include "GameEngine\\GameEngine.h"
#include "SchizoDriver.h"

void SCHIZODRIVER::mainloop()
{
	VECTOR camera1_pos;
	double camera1_angle=0;
	double camera1_zoom =4;

	VECTOR camera2_pos;
	double camera2_angle=0;
	double camera2_zoom =3;

	double video_radius=squareroot(square(VIDEO::width())+square(VIDEO::height()))/2;
	double audio_radius=16777216;

	MAP map(driving_level_map_properties);

	WORLD world(map);

	world.getprimary_layer_object().getbeam(0).activate();

	AUDIO::setspatial_scale(world.getproperties().audio_spatial_scale);

	CAR<CAR_CONSTRUCT_POLICY_PLAYER1,CAR_CONTROL_POLICY_PLAYER1> car1;
	CAR<CAR_CONSTRUCT_POLICY_PLAYER2,CAR_CONTROL_POLICY_PLAYER2> car2;

	car1.main_init(VECTOR(10240,12032),-90,VECTOR(0,0),0,world);
	car2.main_init(VECTOR(10240,11520),-90,VECTOR(0,0),0,world);

	double fuel1_old;
	double fuel1_new;

	double fuel2_old;
	double fuel2_new;

	double camera_pos_multiplay_x[2];
	double camera_pos_multiplay_y[2];

	VIDEO::HUD hud1(-VIDEO::width()/2     ,(VIDEO::height()/2)-192,256,192,12,"Data\\Textures\\Sprites\\hud_border.bmp","Data\\Textures\\Sprites\\hud_display.bmp");
	VIDEO::HUD hud2((VIDEO::width()/2)-256,(VIDEO::height()/2)-192,256,192,12,"Data\\Textures\\Sprites\\hud_border.bmp","Data\\Textures\\Sprites\\hud_display.bmp");

	double hud_radius=(squareroot(square(384)+square(256))/2)*12;

	double kmph_scale =(world.getproperties().meter_scale/1000)/(world.getproperties().second_scale/3600);
	double joule_scale=world.getproperties().kilogram_scale*square(world.getproperties().meter_scale/world.getproperties().second_scale);
	double watt_scale =joule_scale/world.getproperties().second_scale;

	bool inc_score1=false;
	bool inc_score2=false;

	int score1=0;
	int score2=0;

	int audio_mode=1;

	INPUT::clear();

	world.begin_first_frame();

	while (!INPUT::KEYBOARD::escape())
	{
		if (car1.main_exit())
		{
			car1.main_reset(world);
			car1.main_init(VECTOR(10240,12032),-90,VECTOR(0,0),0,world);

			if (inc_score2)
			{
				score2++;

				inc_score2=false;
			}
		}

		if (car2.main_exit())
		{
			car2.main_reset(world);
			car2.main_init(VECTOR(10240,11520),-90,VECTOR(0,0),0,world);

			if (inc_score1)
			{
				score1++;

				inc_score1=false;
			}
		}

		fuel1_old=car1.getfuel();
		fuel2_old=car2.getfuel();

		world.integrate();

		fuel1_new=car1.getfuel();
		fuel2_new=car2.getfuel();

		camera1_angle=car1.getorientation();
		camera1_pos  =VECTOR(VIDEO::width()/2,0).gettransformed_from(car1.getposition(),camera1_angle);
		camera1_zoom =4+(car1.getlinear_velocity().getlength()/256);

		camera2_angle=car2.getorientation();
		camera2_pos  =VECTOR(-VIDEO::width()/2,0).gettransformed_from(car2.getposition(),camera2_angle);
		camera2_zoom =4+(car2.getlinear_velocity().getlength()/256);

		VIDEO::cls();
		VIDEO::enable_cliprect(-VIDEO::width()/2,-VIDEO::height()/2,VIDEO::width()/2,VIDEO::height());
		VIDEO::camera(camera1_pos.x,camera1_pos.y,camera1_angle,camera1_zoom);

		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(-1);}
		world.draw_skyplane  (camera1_pos,video_radius*camera1_zoom);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(0);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_background(camera1_pos,video_radius*camera1_zoom);}
		world.draw_background(camera1_pos,video_radius*camera1_zoom,1);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(1);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_tertiary  (camera1_pos,video_radius*camera1_zoom);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_secondary (camera1_pos,video_radius*camera1_zoom);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_primary   (camera1_pos,video_radius*camera1_zoom);}
		world.draw_tertiary  (camera1_pos,video_radius*camera1_zoom,1);
		world.draw_secondary (camera1_pos,video_radius*camera1_zoom,1);
		world.draw_primary   (camera1_pos,video_radius*camera1_zoom,1);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_objects();}
		world.draw_objects   ();
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(2);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_foreground(camera1_pos,video_radius*camera1_zoom);}
		world.draw_foreground(camera1_pos,video_radius*camera1_zoom,0.75);

		VIDEO::enable_cliprect(0,-VIDEO::height()/2,VIDEO::width()/2,VIDEO::height());
		VIDEO::camera(camera2_pos.x,camera2_pos.y,camera2_angle,camera2_zoom);

		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(-1);}
		world.draw_skyplane  (camera2_pos,video_radius*camera2_zoom);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(0);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(0.5,0.5,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_background(camera2_pos,video_radius*camera2_zoom);}
		world.draw_background(camera2_pos,video_radius*camera2_zoom,1);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(1);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_tertiary  (camera2_pos,video_radius*camera2_zoom);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_secondary (camera2_pos,video_radius*camera2_zoom);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_primary   (camera2_pos,video_radius*camera2_zoom);}
		world.draw_tertiary  (camera2_pos,video_radius*camera2_zoom,1);
		world.draw_secondary (camera2_pos,video_radius*camera2_zoom,1);
		world.draw_primary   (camera2_pos,video_radius*camera2_zoom,1);
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_objects();}
		world.draw_objects   ();
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(2);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,0.25);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {world.light_foreground(camera2_pos,video_radius*camera2_zoom);}
		world.draw_foreground(camera2_pos,video_radius*camera2_zoom,0.75);

		VIDEO::disable_cliprect();

		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::setlayer(3);}
		if (VIDEO::DYNAMIC_LIGHTING::available()) {VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(1,1,1,1);}

		camera1_pos=car1.getposition();
		camera2_pos=car2.getposition();

		hud1.camera(camera1_pos.x,camera1_pos.y,camera1_angle,camera1_zoom);
		hud2.camera(camera2_pos.x,camera2_pos.y,camera2_angle,camera2_zoom);

		hud1.draw_display(0.375);
		hud2.draw_display(0.375);

		world.draw_primary(camera1_pos,hud_radius*camera1_zoom,1,0,1,0.75,hud1);
		world.draw_primary(camera2_pos,hud_radius*camera2_zoom,1,0,1,0.75,hud2);

		world.draw_objects(hud1);
		world.draw_objects(hud2);

		hud1.draw_border(1);
		hud2.draw_border(1);

		if (!car1.hasvapourized() && !car2.hasvapourized())
		{
			VIDEO::TEXT::print_fixed(-VIDEO::width()/2,-VIDEO::height()/2,16,16,"Speed (km/h) :",1,1,1,1);
			VIDEO::TEXT::print_fixed(0                ,-VIDEO::height()/2,16,16,"Speed (km/h) :",1,1,1,1);

			VIDEO::TEXT::print_number_fixed((-VIDEO::width()/2)+15*16,-VIDEO::height()/2,16,16,car1.getlinear_velocity().getlength()*kmph_scale,1,1,1,1);
			VIDEO::TEXT::print_number_fixed(0                  +15*16,-VIDEO::height()/2,16,16,car2.getlinear_velocity().getlength()*kmph_scale,1,1,1,1);

			VIDEO::TEXT::print_fixed(-VIDEO::width()/2,(-VIDEO::height()/2)+16,16,16,"Gear         :",1,1,1,1);
			VIDEO::TEXT::print_fixed(0                ,(-VIDEO::height()/2)+16,16,16,"Gear         :",1,1,1,1);

			VIDEO::TEXT::print_number_fixed((-VIDEO::width()/2)+15*16,(-VIDEO::height()/2)+16,16,16,car1.getgear(),1,1,1,1);
			VIDEO::TEXT::print_number_fixed(0                  +15*16,(-VIDEO::height()/2)+16,16,16,car2.getgear(),1,1,1,1);

			VIDEO::TEXT::print_fixed(-VIDEO::width()/2,(-VIDEO::height()/2)+32,16,16,"Fuel     (J) :",1,1,1,1);
			VIDEO::TEXT::print_fixed(0                ,(-VIDEO::height()/2)+32,16,16,"Fuel     (J) :",1,1,1,1);

			VIDEO::TEXT::print_number_fixed((-VIDEO::width()/2)+15*16,(-VIDEO::height()/2)+32,16,16,car1.getfuel()*joule_scale,1,1,1,1);
			VIDEO::TEXT::print_number_fixed(0                  +15*16,(-VIDEO::height()/2)+32,16,16,car2.getfuel()*joule_scale,1,1,1,1);

			VIDEO::TEXT::print_fixed(-VIDEO::width()/2,(-VIDEO::height()/2)+48,16,16,"Power    (W) :",1,1,1,1);
			VIDEO::TEXT::print_fixed(0                ,(-VIDEO::height()/2)+48,16,16,"Power    (W) :",1,1,1,1);

			VIDEO::TEXT::print_number_fixed((-VIDEO::width()/2)+15*16,(-VIDEO::height()/2)+48,16,16,((fuel1_old-fuel1_new)/(world.gettimestep_in_seconds()*timestep_scale))*watt_scale,1,1,1,1);
			VIDEO::TEXT::print_number_fixed(0                  +15*16,(-VIDEO::height()/2)+48,16,16,((fuel2_old-fuel2_new)/(world.gettimestep_in_seconds()*timestep_scale))*watt_scale,1,1,1,1);

			VIDEO::TEXT::print_fixed(-VIDEO::width()/2,(-VIDEO::height()/2)+64,16,16,"Score        :",1,1,1,1);
			VIDEO::TEXT::print_fixed(0                ,(-VIDEO::height()/2)+64,16,16,"Score        :",1,1,1,1);

			VIDEO::TEXT::print_number_fixed((-VIDEO::width()/2)+15*16,(-VIDEO::height()/2)+64,16,16,score1,1,1,1,1);
			VIDEO::TEXT::print_number_fixed(0                  +15*16,(-VIDEO::height()/2)+64,16,16,score2,1,1,1,1);

			VIDEO::camera(0,0,0,1);

			VECTOR hb1p1(-VIDEO::width()/2,(-VIDEO::height()/2)+80+8);
			VECTOR hb1p2(hb1p1.x+(VIDEO::width()/2)*((double)car1.gethealth()/car1.getmaxhealth()),(-VIDEO::height()/2)+80+8);
			VIDEO::draw_beam(hb1p1.x,hb1p1.y,hb1p2.x,hb1p2.y,0,1,0,1,0,1,16);

			VECTOR hb2p1(0,(-VIDEO::height()/2)+80+8);
			VECTOR hb2p2((VIDEO::width()/2)*((double)car2.gethealth()/car2.getmaxhealth()),(-VIDEO::height()/2)+80+8);
			VIDEO::draw_beam(hb2p1.x,hb2p1.y,hb2p2.x,hb2p2.y,0,0,1,1,0,1,16);

			VIDEO::TEXT::print_fixed(-VIDEO::width()/2,(-VIDEO::height()/2)+80,16,16,"Health",1,1,1,1);
			VIDEO::TEXT::print_fixed(0                ,(-VIDEO::height()/2)+80,16,16,"Health",1,1,1,1);
		}
		else
		{
			if (car1.hasvapourized())
			{
				VIDEO::TEXT::print_fixed(-VIDEO::width()/2,-VIDEO::height()/2,16,16,"Aaaaargh!!!",1,0,0,1);
			}
			else
			{
				VIDEO::TEXT::print_fixed(-VIDEO::width()/2,-VIDEO::height()/2,16,16,"Burn, motherfucker!!!",1,0,0,1);

				inc_score1=true;
			}

			if (car2.hasvapourized())
			{
				VIDEO::TEXT::print_fixed(0                ,-VIDEO::height()/2,16,16,"Aaaaargh!!!",1,0,0,1);
			}
			else
			{
				VIDEO::TEXT::print_fixed(0                ,-VIDEO::height()/2,16,16,"Burn, motherfucker!!!",1,0,0,1);

				inc_score2=true;
			}
		}

		camera_pos_multiplay_x[0]=camera1_pos.x;
		camera_pos_multiplay_y[0]=camera1_pos.y;

		camera_pos_multiplay_x[1]=camera2_pos.x;
		camera_pos_multiplay_y[1]=camera2_pos.y;

		if (audio_mode==1)
		{
			VECTOR pos=car1.getposition();
			VECTOR vel=car1.getlinear_velocity();
			double ang=car1.getorientation();

			AUDIO::listener_topdown(pos.x,pos.y,vel.x,vel.y,ang);
		}
		else if (audio_mode==2)
		{
			VECTOR pos=car2.getposition();
			VECTOR vel=car2.getlinear_velocity();
			double ang=car2.getorientation();

			AUDIO::listener_topdown(pos.x,pos.y,vel.x,vel.y,ang);
		}

		if (INPUT::KEYBOARD::one())
		{
			audio_mode=1;
		}
		else if (INPUT::KEYBOARD::two())
		{
			audio_mode=2;
		}

		world.sound((camera1_pos+camera2_pos)/2,audio_radius);

		AUDIO::MUSIC::update();
		VIDEO::show();
		INPUT::update();

		world.end_frame();
	}

	INPUT::clear();
}
