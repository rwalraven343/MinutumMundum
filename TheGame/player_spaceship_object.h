//Code by Rogier Walraven. Do not distribute.

static const SOLID_OBJECT_PROPERTIES player_spaceship_solid_object_properties={{256,384,1024,4096,0.25,0.25,0.25,0.25,8},{256,384,2,8,16,1024,-8,8,256,0.25,0.25,1.25},4};

static const PARTICLE_EMITTER_PROPERTIES player_spaceship_engine_emitter_properties={2,6,8,16384,4,8,11.25,0.5,0.25};

class PLAYER_SPACESHIP_OBJECT : public BIG_SOLID_OBJECT, public FEATURE/*, public BEAM_OBJECT<BIG_SOLID_OBJECT>*/
{
	PARTICLE_EMITTER<BIG_SOLID_OBJECT> engine_emitter1;
	PARTICLE_EMITTER<BIG_SOLID_OBJECT> engine_emitter2;
	PARTICLE_EMITTER<BIG_SOLID_OBJECT> engine_emitter3;
	PARTICLE_EMITTER<BIG_SOLID_OBJECT> engine_emitter4;

	MOTOR<BIG_SOLID_OBJECT> motor;

	double fuel;

	VIDEO::TEXTURE sprite;

	bool main_initialized;

  public:

	PLAYER_SPACESHIP_OBJECT()
	: engine_emitter1(1024),engine_emitter2(1024),engine_emitter3(1024),engine_emitter4(1024)
	{
		BIG_SOLID_OBJECT::init_reset(player_spaceship_solid_object_properties);

		engine_emitter1.init_reset(player_spaceship_engine_emitter_properties,*this);
		engine_emitter2.init_reset(player_spaceship_engine_emitter_properties,*this);
		engine_emitter3.init_reset(player_spaceship_engine_emitter_properties,*this);
		engine_emitter4.init_reset(player_spaceship_engine_emitter_properties,*this);

		fuel=16777216;

		motor.init_reset(*this,fuel);

		sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\spaceship1.bmp");

		main_initialized=false;
	}

	~PLAYER_SPACESHIP_OBJECT()
	{
		VIDEO::freetexture(sprite);
	}

	void reset()
	{
		BIG_SOLID_OBJECT::init_reset(player_spaceship_solid_object_properties);

		engine_emitter1.init_reset(player_spaceship_engine_emitter_properties,*this);
		engine_emitter2.init_reset(player_spaceship_engine_emitter_properties,*this);
		engine_emitter3.init_reset(player_spaceship_engine_emitter_properties,*this);
		engine_emitter4.init_reset(player_spaceship_engine_emitter_properties,*this);

		fuel=16777216;

		motor.init_reset(*this,fuel);
	}

	void control(double timestep)
	{
		if (!BIG_SOLID_OBJECT::hasvapourized() && fuel>0)
		{
			double width    =getwidth();
			double height   =getheight();
			VECTOR pos      =getposition();
			double ang      =getorientation();
			VECTOR vel      =getlinear_velocity();
			double angvelrad=getangular_velocity()*(math_pi/180);

			VECTOR engine_pos1=pos + VECTOR((-width/2)*0.75,(-height/2)*0.625).getrotated(ang);
			VECTOR engine_pos2=pos + VECTOR(( width/2)*0.75,(-height/2)*0.625).getrotated(ang);
			VECTOR engine_pos3=pos + VECTOR(( width/2)*0.75,( height/2)*0.625).getrotated(ang);
			VECTOR engine_pos4=pos + VECTOR((-width/2)*0.75,( height/2)*0.625).getrotated(ang);

			double thrust=-INPUT::GAMEPAD1::axis_left_y();
			double steer=INPUT::GAMEPAD1::axis_right_x();

			VECTOR engine_force1;
			VECTOR engine_force2;
			VECTOR engine_force3;
			VECTOR engine_force4;

			if (thrust>0)
			{
				engine_force3+=(VECTOR(0,-1).getrotated(ang)*thrust)*16384;
				engine_force4+=(VECTOR(0,-1).getrotated(ang)*thrust)*16384;
			}
			else if (thrust<0)
			{
				engine_force1+=(VECTOR(0,-1).getrotated(ang)*thrust)*16384;
				engine_force2+=(VECTOR(0,-1).getrotated(ang)*thrust)*16384;
			}

			if (steer>0)
			{
				engine_force2+=(VECTOR(0,1).getrotated(ang)*steer)*16384;
				engine_force4+=(VECTOR(0,-1).getrotated(ang)*steer)*16384;
			}
			else if (steer<0)
			{
				engine_force1+=(VECTOR(0,-1).getrotated(ang)*steer)*16384;
				engine_force3+=(VECTOR(0,1).getrotated(ang)*steer)*16384;
			}

			motor.addforce(engine_force1,engine_pos1,timestep);
			motor.addforce(engine_force2,engine_pos2,timestep);
			motor.addforce(engine_force3,engine_pos3,timestep);
			motor.addforce(engine_force4,engine_pos4,timestep);

			engine_emitter1.addforce(engine_force1,engine_pos1,timestep);
			engine_emitter2.addforce(engine_force2,engine_pos2,timestep);
			engine_emitter3.addforce(engine_force3,engine_pos3,timestep);
			engine_emitter4.addforce(engine_force4,engine_pos4,timestep);
		}
	}

	void addheat(double ht,const VECTOR &pos)
	{
		BIG_SOLID_OBJECT::addheat(ht,pos);
	}

	void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
	{
		BIG_SOLID_OBJECT::addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);

		engine_emitter1.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
		engine_emitter2.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
		engine_emitter3.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
		engine_emitter4.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	}

	void change_origin(const VECTOR &new_origin)
	{
		BIG_SOLID_OBJECT::change_origin(new_origin);

		engine_emitter1.change_origin(new_origin);
		engine_emitter2.change_origin(new_origin);
		engine_emitter3.change_origin(new_origin);
		engine_emitter4.change_origin(new_origin);
	}

	/*int getnumbeams() const
	BEAM<BIG_SOLID_OBJECT>& getbeam(int i)*/
	void integrate(double timestep)
	{
		BIG_SOLID_OBJECT::integrate(timestep);

		engine_emitter1.integrate(timestep);
		engine_emitter2.integrate(timestep);
		engine_emitter3.integrate(timestep);
		engine_emitter4.integrate(timestep);
	}

	void light() const
	{
		engine_emitter1.light(1,1,0,1,0,0,0.75,8);
		engine_emitter2.light(1,1,0,1,0,0,0.75,8);
		engine_emitter3.light(1,1,0,1,0,0,0.75,8);
		engine_emitter4.light(1,1,0,1,0,0,0.75,8);
	}

	void draw() const
	{
		engine_emitter1.draw(1,1,0,1,0,0,0.75,4);
		engine_emitter2.draw(1,1,0,1,0,0,0.75,4);
		engine_emitter3.draw(1,1,0,1,0,0,0.75,4);
		engine_emitter4.draw(1,1,0,1,0,0,0.75,4);

		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			BIG_SOLID_OBJECT::draw(sprite,1);

			return;
		}

		BIG_SOLID_OBJECT::draw(1,1,0,1,0,0,1);
	}

	void draw(const VIDEO::HUD &hud) const
	{
	}

	void sound() const
	{
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
		return(BIG_SOLID_OBJECT::isgone());
	}

	void main_update(WORLD &world)
	{
	}
};
