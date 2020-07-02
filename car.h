//Code by Rogier Walraven. Do not distribute.

static const SOLID_OBJECT_PROPERTIES car_properties={{256,384,1024,4096,0.25,0.25,0.25,0.25,8},{256,384,2,8,16,1024,-8,8,256,0.25,0.25,1.25},4};

static const PARTICLE_EMITTER_PROPERTIES car_exhaust_emitter_properties={2,6,8,16384,4,8,11.25,0.5,0.25};
static const PARTICLE_EMITTER_PROPERTIES car_tire_squeal_emitter_properties={1,2,8,16384,4,8,22.5,0.5,0.25};
static const PARTICLE_EMITTER_PROPERTIES car_heat_emitter_properties={2,6,64,16384,4,8,11.25,0.5,0.25};

struct CAR_CONSTRUCT_POLICY_PLAYER1
{
	const char *gettexture_filename() const
	{
		return("Data\\Textures\\Sprites\\spacebuggy_green.bmp");
	}

	bool isgreen_hud() const
	{
		return(true);
	}
};

struct CAR_CONSTRUCT_POLICY_PLAYER2
{
	const char *gettexture_filename() const
	{
		return("Data\\Textures\\Sprites\\spacebuggy_blue.bmp");
	}

	bool isgreen_hud() const
	{
		return(false);
	}
};

struct CAR_CONTROL_POLICY_PLAYER1
{
	double getgas() const
	{
		//return(-INPUT::GAMEPAD1::axis_left_y());
		if ( INPUT::KEYBOARD::w() && !INPUT::KEYBOARD::s())
		{
			return(1);
		}

		if (!INPUT::KEYBOARD::w() &&  INPUT::KEYBOARD::s())
		{
			return(-1);
		}

		return(0);
	}

	double getsteer() const
	{
		//return(INPUT::GAMEPAD1::axis_right_x());
		if ( INPUT::KEYBOARD::a() && !INPUT::KEYBOARD::d())
		{
			return(-1);
		}

		if (!INPUT::KEYBOARD::a() &&  INPUT::KEYBOARD::d())
		{
			return(1);
		}

		return(0);
	}

	bool getbreak() const
	{
		//return(INPUT::GAMEPAD1::digital_trigger_right());
		return(INPUT::KEYBOARD::q());
	}

	double getwheelfriction_constant() const
	{
		return(64);
	}

	double getwheelfriction_threshold() const
	{
		return(768);
	}

	double getbreakfriction_constant() const
	{
		return(128);
	}

	double getbreakfriction_threshold() const
	{
		return(512);
	}

	double getmaxforce(int gear) const
	{
		if (gear==1)
		{
			return(16384);
		}

		if (gear==2)
		{
			return(12288);
		}

		if (gear==3)
		{
			return(9216);
		}

		if (gear==4)
		{
			return(6912);
		}

		return(5184);
	}
};

struct CAR_CONTROL_POLICY_PLAYER2
{
	double getgas() const
	{
		if ( INPUT::KEYBOARD::up() && !INPUT::KEYBOARD::down())
		{
			return(1);
		}

		if (!INPUT::KEYBOARD::up() &&  INPUT::KEYBOARD::down())
		{
			return(-1);
		}

		return(0);
	}

	double getsteer() const
	{
		if ( INPUT::KEYBOARD::left() && !INPUT::KEYBOARD::right())
		{
			return(-1);
		}

		if (!INPUT::KEYBOARD::left() &&  INPUT::KEYBOARD::right())
		{
			return(1);
		}

		return(0);
	}

	bool getbreak() const
	{
		return(INPUT::KEYBOARD::b());
	}

	double getwheelfriction_constant() const
	{
		return(64);
	}

	double getwheelfriction_threshold() const
	{
		return(768);
	}

	double getbreakfriction_constant() const
	{
		return(128);
	}

	double getbreakfriction_threshold() const
	{
		return(512);
	}

	double getmaxforce(int gear) const
	{
		if (gear==1)
		{
			return(16384);
		}

		if (gear==2)
		{
			return(12288);
		}

		if (gear==3)
		{
			return(9216);
		}

		if (gear==4)
		{
			return(6912);
		}

		return(5184);
	}
};

template<class CONSTRUCT_POLICY,class CONTROL_POLICY>
class CAR : public BIG_SOLID_OBJECT, public FEATURE
{
	CONSTRUCT_POLICY construct_policy;
	CONTROL_POLICY   control_policy;

	PARTICLE_EMITTER<BIG_SOLID_OBJECT> exhaust_emitter;
	PARTICLE_EMITTER<BIG_SOLID_OBJECT> tire_squeal_emitter;
	PARTICLE_EMITTER<BIG_SOLID_OBJECT> heat_emitter;
	MOTOR<BIG_SOLID_OBJECT> motor;

	VIDEO::TEXTURE texture;
	VIDEO::TEXTURE texture_wheel;

	AUDIO::SOURCE engine_sound_loop;
	AUDIO::SOURCE squealing_tires_sound_loop;
	AUDIO::SOURCE explosion_sound;

	int    gear;
	double gear_inbetween;
	double fuel;

	double tire_squeal1;
	double tire_squeal2;
	double tire_squeal3;
	double tire_squeal4;

	bool main_initialized;

  public:

	CAR() : exhaust_emitter(1024),tire_squeal_emitter(1024),heat_emitter(1024),
            engine_sound_loop("Data\\Samples\\car_engine_loop.wav",1),
			squealing_tires_sound_loop("Data\\Samples\\squealing_tires_loop.wav",1),
            explosion_sound("Data\\Samples\\explosion.wav",1)
	{
		BIG_SOLID_OBJECT::init_reset(car_properties);
		exhaust_emitter.init_reset(car_exhaust_emitter_properties,*this);
		tire_squeal_emitter.init_reset(car_tire_squeal_emitter_properties,*this);
		heat_emitter.init_reset(car_heat_emitter_properties,*this);
		motor.init_reset(*this,fuel);

		texture=VIDEO::loadtexture(construct_policy.gettexture_filename());
		texture_wheel=VIDEO::loadtexture("Data\\Textures\\Sprites\\spacebuggy_wheel.bmp");

		gear          =1;
		gear_inbetween=0;
		fuel          =16777216.0/4;

		tire_squeal1=0;
		tire_squeal2=0;
		tire_squeal3=0;
		tire_squeal4=0;

		main_initialized=false;
	}

	~CAR()
	{
		VIDEO::freetexture(texture);
		VIDEO::freetexture(texture_wheel);
	}

	void reset()
	{
		BIG_SOLID_OBJECT::init_reset(car_properties);
		exhaust_emitter.init_reset(car_exhaust_emitter_properties,*this);
		tire_squeal_emitter.init_reset(car_tire_squeal_emitter_properties,*this);
		heat_emitter.init_reset(car_heat_emitter_properties,*this);
		motor.init_reset(*this,fuel);

		engine_sound_loop.rewind();
		squealing_tires_sound_loop.rewind();
		explosion_sound.rewind();

		gear          =1;
		gear_inbetween=0;
		fuel          =16777216.0/4;

		tire_squeal1=0;
		tire_squeal2=0;
		tire_squeal3=0;
		tire_squeal4=0;
	}

	int getgear() const
	{
		return(gear);
	}

	double getfuel() const
	{
		return(fuel);
	}

	void control(double timestep)
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			double width    =getwidth();
			double height   =getheight();
			VECTOR pos      =getposition();
			double ang      =getorientation();
			VECTOR vel      =getlinear_velocity();
			double angvelrad=getangular_velocity()*(math_pi/180);

			VECTOR wheelpos1=pos + VECTOR((-width/2)*0.75,(-height/2)*0.625).getrotated(ang);
			VECTOR wheelpos2=pos + VECTOR(( width/2)*0.75,(-height/2)*0.625).getrotated(ang);
			VECTOR wheelpos3=pos + VECTOR(( width/2)*0.75,( height/2)*0.625).getrotated(ang);
			VECTOR wheelpos4=pos + VECTOR((-width/2)*0.75,( height/2)*0.625).getrotated(ang);

			VECTOR wheelvel1=vel;
			VECTOR wheelvel2=vel;
			VECTOR wheelvel3=vel;
			VECTOR wheelvel4=vel;

			wheelvel1+=((wheelpos1-pos).getperpend())*angvelrad;
			wheelvel2+=((wheelpos2-pos).getperpend())*angvelrad;
			wheelvel3+=((wheelpos3-pos).getperpend())*angvelrad;
			wheelvel4+=((wheelpos4-pos).getperpend())*angvelrad;

			VECTOR xaxis=VECTOR(1,0).getrotated(ang);
			VECTOR yaxis=VECTOR(0,1).getrotated(ang);

			VECTOR xaxis_steer=VECTOR(1,0).getrotated(45*control_policy.getsteer()).getrotated(ang);
			VECTOR yaxis_steer=VECTOR(0,1).getrotated(45*control_policy.getsteer()).getrotated(ang);

			VECTOR wheelvel1_xaxis_steer=wheelvel1.getprojectedon(xaxis_steer);
			VECTOR wheelvel2_xaxis_steer=wheelvel2.getprojectedon(xaxis_steer);
			VECTOR wheelvel3_xaxis      =wheelvel3.getprojectedon(xaxis);
			VECTOR wheelvel4_xaxis      =wheelvel4.getprojectedon(xaxis);

			VECTOR wheelvel1_yaxis_steer=wheelvel1.getprojectedon(yaxis_steer);
			VECTOR wheelvel2_yaxis_steer=wheelvel2.getprojectedon(yaxis_steer);
			VECTOR wheelvel3_yaxis      =wheelvel3.getprojectedon(yaxis);
			VECTOR wheelvel4_yaxis      =wheelvel4.getprojectedon(yaxis);

			double wheelfriction1_coeff=wheelvel1_xaxis_steer.getlength()*control_policy.getwheelfriction_constant();
			double wheelfriction2_coeff=wheelvel2_xaxis_steer.getlength()*control_policy.getwheelfriction_constant();
			double wheelfriction3_coeff=wheelvel3_xaxis.getlength()      *control_policy.getwheelfriction_constant();
			double wheelfriction4_coeff=wheelvel4_xaxis.getlength()      *control_policy.getwheelfriction_constant();

			if (wheelfriction1_coeff<=control_policy.getwheelfriction_threshold())
			{
				BIG_SOLID_OBJECT::addforce((-wheelvel1_xaxis_steer.getnormal()*wheelfriction1_coeff)/timestep,wheelpos1);

				tire_squeal1=0;
			}
			else
			{
				BIG_SOLID_OBJECT::addforce( -wheelvel1_xaxis_steer.getnormal()*wheelfriction1_coeff          ,wheelpos1);
				tire_squeal_emitter.addforce(wheelvel1_xaxis_steer.getnormal()*wheelfriction1_coeff*4        ,wheelpos1,timestep);

				tire_squeal1=wheelfriction1_coeff;
			}

			if (wheelfriction2_coeff<=control_policy.getwheelfriction_threshold())
			{
				BIG_SOLID_OBJECT::addforce((-wheelvel2_xaxis_steer.getnormal()*wheelfriction2_coeff)/timestep,wheelpos2);

				tire_squeal2=0;
			}
			else
			{
				BIG_SOLID_OBJECT::addforce( -wheelvel2_xaxis_steer.getnormal()*wheelfriction2_coeff          ,wheelpos2);
				tire_squeal_emitter.addforce(wheelvel2_xaxis_steer.getnormal()*wheelfriction2_coeff*4        ,wheelpos2,timestep);

				tire_squeal2=wheelfriction2_coeff;
			}

			if (wheelfriction3_coeff<=control_policy.getwheelfriction_threshold())
			{
				BIG_SOLID_OBJECT::addforce((-wheelvel3_xaxis.getnormal()*wheelfriction3_coeff)/timestep,wheelpos3);

				tire_squeal3=0;
			}
			else
			{
				BIG_SOLID_OBJECT::addforce( -wheelvel3_xaxis.getnormal()*wheelfriction3_coeff          ,wheelpos3);
				tire_squeal_emitter.addforce(wheelvel3_xaxis.getnormal()*wheelfriction3_coeff*4        ,wheelpos3,timestep);

				tire_squeal3=wheelfriction3_coeff;
			}

			if (wheelfriction4_coeff<=control_policy.getwheelfriction_threshold())
			{
				BIG_SOLID_OBJECT::addforce((-wheelvel4_xaxis.getnormal()*wheelfriction4_coeff)/timestep,wheelpos4);

				tire_squeal4=0;
			}
			else
			{
				BIG_SOLID_OBJECT::addforce( -wheelvel4_xaxis.getnormal()*wheelfriction4_coeff          ,wheelpos4);
				tire_squeal_emitter.addforce(wheelvel4_xaxis.getnormal()*wheelfriction4_coeff*4        ,wheelpos4,timestep);

				tire_squeal4=wheelfriction4_coeff;
			}

			if (control_policy.getbreak())
			{
				double breakfriction1_coeff=wheelvel3.getlength()*(control_policy.getbreakfriction_constant());
				double breakfriction2_coeff=wheelvel4.getlength()*(control_policy.getbreakfriction_constant());

				if (breakfriction1_coeff<=control_policy.getbreakfriction_threshold())
				{
					BIG_SOLID_OBJECT::addforce((-wheelvel3.getnormal()*breakfriction1_coeff)/timestep,wheelpos3);
				}
				else
				{
					BIG_SOLID_OBJECT::addforce( -wheelvel3.getnormal()*breakfriction1_coeff          ,wheelpos3);
					tire_squeal_emitter.addforce(wheelvel3.getnormal()*breakfriction1_coeff*4        ,wheelpos3,timestep);

					tire_squeal3+=breakfriction1_coeff;
				}

				if (breakfriction2_coeff<=control_policy.getbreakfriction_threshold())
				{
					BIG_SOLID_OBJECT::addforce((-wheelvel4.getnormal()*breakfriction2_coeff)/timestep,wheelpos4);
				}
				else
				{
					BIG_SOLID_OBJECT::addforce( -wheelvel4.getnormal()*breakfriction2_coeff          ,wheelpos4);
					tire_squeal_emitter.addforce(wheelvel4.getnormal()*breakfriction2_coeff*4        ,wheelpos4,timestep);

					tire_squeal4+=breakfriction2_coeff;
				}
			}

			double meter_scale =0.010416666666;
			double second_scale=0.125;

			double speed=vel.getprojectedon(yaxis).getlength()*((meter_scale/1000)/(second_scale/3600));

			if (speed<50)
			{
				gear=1;
				gear_inbetween=speed/50;
			}
			else if (speed<80)
			{
				gear=2;
				gear_inbetween=speed/80;
			}
			else if (speed<110)
			{
				gear=3;
				gear_inbetween=speed/110;
			}
			else if (speed<140)
			{
				gear=4;
				gear_inbetween=speed/140;
			}
			else
			{
				gear=5;
				gear_inbetween=0;
			}

			if (fuel>0)
			{
				VECTOR engine_force=-yaxis_steer*control_policy.getmaxforce(gear)*control_policy.getgas();

				motor.addforce(engine_force/2,wheelpos1,timestep);
				motor.addforce(engine_force/2,wheelpos2,timestep);

				VECTOR exhaustpos1=pos + (VECTOR( width/2, height/2)*0.625).getrotated(ang);
				VECTOR exhaustpos2=pos + (VECTOR(-width/2, height/2)*0.625).getrotated(ang);

				double engine_force_len=engine_force.getlength();

				exhaust_emitter.addforce(-yaxis*((engine_force_len/2)*4),exhaustpos1,timestep);
				exhaust_emitter.addforce(-yaxis*((engine_force_len/2)*4),exhaustpos2,timestep);
			}
		}
	}

	void addheat(double ht,const VECTOR &pos)
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			BIG_SOLID_OBJECT::addheat(ht*0.5,pos);
			heat_emitter.addheat(ht*0.5,pos);
		}
		else
		{
			BIG_SOLID_OBJECT::addheat(ht,pos);
		}
	}

	void change_origin(const VECTOR &new_origin)
	{
		BIG_SOLID_OBJECT::change_origin(new_origin);
		exhaust_emitter.change_origin(new_origin);
		tire_squeal_emitter.change_origin(new_origin);
		heat_emitter.change_origin(new_origin);
	}

	void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
	{
		BIG_SOLID_OBJECT::addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
		exhaust_emitter.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
		tire_squeal_emitter.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
		heat_emitter.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	}

	void integrate(double timestep)
	{
		BIG_SOLID_OBJECT::integrate(timestep);
		exhaust_emitter.integrate(timestep);
		tire_squeal_emitter.integrate(timestep);
		heat_emitter.integrate(timestep);
	}

	void light() const
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			double width =getwidth();
			double height=getheight();
			VECTOR pos   =getposition();
			double ang   =getorientation();

			VECTOR breaklightpos1=pos + VECTOR(( width/2)*0.5,( height/2)*0.85).getrotated(ang);
			VECTOR breaklightpos2=pos + VECTOR((-width/2)*0.5,( height/2)*0.85).getrotated(ang);

			if (control_policy.getbreak())
			{
				VIDEO::DYNAMIC_LIGHTING::draw_pointlight(breaklightpos1.x,breaklightpos1.y,1,0,0,1,128);
				VIDEO::DYNAMIC_LIGHTING::draw_pointlight(breaklightpos2.x,breaklightpos2.y,1,0,0,1,128);
			}

			VECTOR headlightpos1a=pos + VECTOR(( width/2)*0.5,(-height/2)).getrotated(ang);
			VECTOR headlightpos2a=pos + VECTOR((-width/2)*0.5,(-height/2)).getrotated(ang);
			VECTOR headlightpos1b=pos + VECTOR(( width/2)*0.5,(-height)*4).getrotated(ang);
			VECTOR headlightpos2b=pos + VECTOR((-width/2)*0.5,(-height)*4).getrotated(ang);

			VIDEO::DYNAMIC_LIGHTING::draw_spotlight(headlightpos1a.x,headlightpos1a.y,headlightpos1b.x,headlightpos1b.y,1,1,1,1,512);
			VIDEO::DYNAMIC_LIGHTING::draw_spotlight(headlightpos2a.x,headlightpos2a.y,headlightpos2b.x,headlightpos2b.y,1,1,1,1,512);

			VIDEO::DYNAMIC_LIGHTING::draw_pointlight(headlightpos1a.x,headlightpos1a.y,1,1,1,1,256);
			VIDEO::DYNAMIC_LIGHTING::draw_pointlight(headlightpos2a.x,headlightpos2a.y,1,1,1,1,256);

			VECTOR headlightpos3=(headlightpos1b+headlightpos2b)/2;

			VIDEO::DYNAMIC_LIGHTING::draw_pointlight(headlightpos3.x,headlightpos3.y,1,1,1,1,1024);
		}
		else
		{
			BIG_SOLID_OBJECT::light(1,1,0,1,0,0,1,3);
		}

		heat_emitter.light(1,1,0,1,0,0,0.75,8);
	}

	void draw() const
	{
		exhaust_emitter.draw(1,1,1,0,0,0,0.125,4);
		tire_squeal_emitter.draw(1,1,1,0,0,0,0.125,4);

		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			double width =getwidth();
			double height=getheight();
			VECTOR pos   =getposition();
			double ang   =getorientation();

			VECTOR wheelpos1a=pos + VECTOR((-width/2)*0.75,(-height/2)*0.625).getrotated(ang) + VECTOR(-32,-32).getrotated(45*control_policy.getsteer()).getrotated(ang);
			VECTOR wheelpos1b=pos + VECTOR((-width/2)*0.75,(-height/2)*0.625).getrotated(ang) + VECTOR(+16,-32).getrotated(45*control_policy.getsteer()).getrotated(ang);
			VECTOR wheelpos1c=pos + VECTOR((-width/2)*0.75,(-height/2)*0.625).getrotated(ang) + VECTOR(+16,+32).getrotated(45*control_policy.getsteer()).getrotated(ang);
			VECTOR wheelpos1d=pos + VECTOR((-width/2)*0.75,(-height/2)*0.625).getrotated(ang) + VECTOR(-32,+32).getrotated(45*control_policy.getsteer()).getrotated(ang);

			VECTOR wheelpos2a=pos + VECTOR(( width/2)*0.75,(-height/2)*0.625).getrotated(ang) + VECTOR(-16,-32).getrotated(45*control_policy.getsteer()).getrotated(ang);
			VECTOR wheelpos2b=pos + VECTOR(( width/2)*0.75,(-height/2)*0.625).getrotated(ang) + VECTOR(+32,-32).getrotated(45*control_policy.getsteer()).getrotated(ang);
			VECTOR wheelpos2c=pos + VECTOR(( width/2)*0.75,(-height/2)*0.625).getrotated(ang) + VECTOR(+32,+32).getrotated(45*control_policy.getsteer()).getrotated(ang);
			VECTOR wheelpos2d=pos + VECTOR(( width/2)*0.75,(-height/2)*0.625).getrotated(ang) + VECTOR(-16,+32).getrotated(45*control_policy.getsteer()).getrotated(ang);

			/*VIDEO::draw_point_sized(wheelpos1a.x,wheelpos1a.y,1,1,1,1,4);
			VIDEO::draw_point_sized(wheelpos1b.x,wheelpos1b.y,1,1,1,1,4);
			VIDEO::draw_point_sized(wheelpos1c.x,wheelpos1c.y,1,1,1,1,4);
			VIDEO::draw_point_sized(wheelpos1d.x,wheelpos1d.y,1,1,1,1,4);

			VIDEO::draw_point_sized(wheelpos2a.x,wheelpos2a.y,1,1,1,1,4);
			VIDEO::draw_point_sized(wheelpos2b.x,wheelpos2b.y,1,1,1,1,4);
			VIDEO::draw_point_sized(wheelpos2c.x,wheelpos2c.y,1,1,1,1,4);
			VIDEO::draw_point_sized(wheelpos2d.x,wheelpos2d.y,1,1,1,1,4);*/

			VIDEO::draw_quad_texture(wheelpos1a.x,wheelpos1a.y,wheelpos1b.x,wheelpos1b.y,wheelpos1c.x,wheelpos1c.y,wheelpos1d.x,wheelpos1d.y,texture_wheel,1);
			VIDEO::draw_quad_texture(wheelpos2a.x,wheelpos2a.y,wheelpos2b.x,wheelpos2b.y,wheelpos2c.x,wheelpos2c.y,wheelpos2d.x,wheelpos2d.y,texture_wheel,1);

			BIG_SOLID_OBJECT::draw(texture,1);

			VECTOR breaklightpos1=pos + VECTOR(( width/2)*0.5,( height/2)*0.85).getrotated(ang);
			VECTOR breaklightpos2=pos + VECTOR((-width/2)*0.5,( height/2)*0.85).getrotated(ang);

			VIDEO::draw_point_fluid(breaklightpos1.x,breaklightpos1.y,1,0,0,0.5,32);
			VIDEO::draw_point_fluid(breaklightpos2.x,breaklightpos2.y,1,0,0,0.5,32);
		}
		else
		{
			BIG_SOLID_OBJECT::draw(1,1,0,1,0,0,1);
		}

		heat_emitter.draw(1,1,0,1,0,0,0.75,4);
	}

	void draw(const VIDEO::HUD &hud) const
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			VECTOR pos=BIG_SOLID_OBJECT::getposition();

			if (construct_policy.isgreen_hud())
			{
				hud.draw_point_sized(pos.x,pos.y,0,1,0,1,4);
			}
			else
			{
				hud.draw_point_sized(pos.x,pos.y,0,0,1,1,4);
			}
		}
	}

	void sound() const
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			VECTOR pos=BIG_SOLID_OBJECT::getposition();
			double ori=BIG_SOLID_OBJECT::getorientation();

			double width =BIG_SOLID_OBJECT::getwidth();
			double height=BIG_SOLID_OBJECT::getheight();

			VECTOR spos=(VECTOR(0,height/2)*0.75).gettransformed_from(pos,ori);
			VECTOR svel=BIG_SOLID_OBJECT::getlinear_velocity();

			if (fuel>0)
			{
				double engine_pitch;

				double gear_switch_deadzone=0.0625*control_policy.getmaxforce(gear)/control_policy.getmaxforce(1);

				if (gear_inbetween<gear_switch_deadzone)
				{
					engine_pitch=((gear_inbetween/gear_switch_deadzone)/8)+0.625;
				}
				else if (gear_inbetween>=1-gear_switch_deadzone)
				{
					engine_pitch=(((1-gear_inbetween)/gear_switch_deadzone)/8)+0.625;
				}
				else
				{
					engine_pitch=(gear_inbetween*absolute(control_policy.getgas()))+0.75;
				}

				engine_sound_loop.play(spos.x,spos.y,svel.x,svel.y,engine_pitch,1);
			}
			else
			{
				engine_sound_loop.rewind();
			}

			double meter_scale =0.010416666666;
			double second_scale=0.125;

			double max_speed=80/((meter_scale/1000)/(second_scale/3600));

			double squealing_tires_vol=(tire_squeal1+tire_squeal2+tire_squeal3+tire_squeal4)/(control_policy.getwheelfriction_constant()*max_speed*4);

			if (squealing_tires_vol<0.25) {squealing_tires_vol=0;}
			if (squealing_tires_vol>1)    {squealing_tires_vol=1;}

			squealing_tires_sound_loop.play(spos.x,spos.y,svel.x,svel.y,1,squealing_tires_vol);
		}
		else
		{
			VECTOR pos=BIG_SOLID_OBJECT::getposition();
			double ori=BIG_SOLID_OBJECT::getorientation();

			double width =BIG_SOLID_OBJECT::getwidth();
			double height=BIG_SOLID_OBJECT::getheight();

			VECTOR spos=(VECTOR(0,height/2)*0.75).gettransformed_from(pos,ori);
			VECTOR svel=BIG_SOLID_OBJECT::getlinear_velocity();

			explosion_sound.play(spos.x,spos.y,svel.x,svel.y,1,1);
			engine_sound_loop.rewind();
			squealing_tires_sound_loop.rewind();
		}
	}

	void main_init(const VECTOR &pos,double angle,const VECTOR &vel,double angular_vel,WORLD &world)
	{
		BIG_SOLID_OBJECT::setposition(pos);
		BIG_SOLID_OBJECT::setorientation(angle);
		BIG_SOLID_OBJECT::setlinear_velocity(vel);
		BIG_SOLID_OBJECT::setangular_velocity(angular_vel);

		world.putobject(*this);

		main_initialized=true;
	}

	void main_reset(WORLD &world)
	{
		reset();

		world.removeobject(*this);

		main_initialized=false;
	}

	bool main_wasinit() const
	{
		return(main_initialized);
	}

	bool main_exit() const
	{
		return(fuel<=0 || BIG_SOLID_OBJECT::isgone());
	}

	void main_update(WORLD &world)
	{
	}
};
