//Code by Rogier Walraven. Do not distribute.

static const SOLID_OCTAGON_OBJECT_PROPERTIES virus_object_properties={{128,1024,4096,0.25,0.25,0.25,0.25,8},{256,384,2,8,16,1024,-8,8,256,0.25,0.25,1.25},1024};

static const BEAM_PROPERTIES virus_beam_properties={2048,8.192e9,256};

class VIRUS_OBJECT : public SOLID_OCTAGON_OBJECT, public FEATURE, public BEAM_OBJECT<SOLID_OCTAGON_OBJECT>
{
	VIDEO::TEXTURE *sprites;

	AUDIO::SOURCE explosion_snd;

	double anim_currenttime;
	double anim_totaltime;

	BEAM<SOLID_OCTAGON_OBJECT> beam;

	bool main_initialized;

  public:

	VIRUS_OBJECT() : explosion_snd("Data\\Samples\\explosion.wav",1),beam(32)
	{
		SOLID_OCTAGON_OBJECT::init_reset(virus_object_properties);

		sprites=VIDEO::loadtileset("Data\\Textures\\Sprites\\coronavirus.bmp",1024,1024,256,256,0,0,16);

		anim_currenttime=0;
		anim_totaltime=16;

		beam.init_reset(virus_beam_properties,VECTOR(0,0),0,*this);

		main_initialized=false;
	}

	~VIRUS_OBJECT()
	{
		VIDEO::freetileset(sprites,16);
	}

	void reset()
	{
		SOLID_OCTAGON_OBJECT::init_reset(virus_object_properties);

		anim_currenttime=0;
		anim_totaltime=16;

		explosion_snd.rewind();

		beam.init_reset(virus_beam_properties,VECTOR(0,0),0,*this);
	}

	void control(double timestep)
	{
		if (!SOLID_OCTAGON_OBJECT::hasvapourized())
		{
			double force_constant=16384*2;
			double torque_constant=16777216*8;

			double angle=SOLID_OCTAGON_OBJECT::getorientation();

			VECTOR force_dir=VECTOR(0,-1).getrotated(angle);

			if (INPUT::KEYBOARD::up())
			{
				SOLID_OCTAGON_OBJECT::addforce(force_dir*force_constant);
			}

			if (INPUT::KEYBOARD::down())
			{
				SOLID_OCTAGON_OBJECT::addforce(-force_dir*force_constant);
			}

			if (INPUT::KEYBOARD::left())
			{
				SOLID_OCTAGON_OBJECT::addtorque(-torque_constant);
			}

			if (INPUT::KEYBOARD::right())
			{
				SOLID_OCTAGON_OBJECT::addtorque(torque_constant);
			}

			if (INPUT::KEYBOARD::b())
			{
				if (!beam.isactive())
				{
					beam.activate();
				}
			}
			else
			{
				if (beam.isactive())
				{
					beam.deactivate();
				}
			}

			return;
		}

		if (beam.isactive())
		{
			beam.deactivate();
		}
	}

	void addheat(double ht,const VECTOR &pos)
	{
		SOLID_OCTAGON_OBJECT::addheat(ht,pos);
	}

	void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
	{
		SOLID_OCTAGON_OBJECT::addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	}

	void change_origin(const VECTOR &new_origin)
	{
		SOLID_OCTAGON_OBJECT::change_origin(new_origin);
	}

	void integrate(double timestep)
	{
		SOLID_OCTAGON_OBJECT::integrate(timestep);

		anim_currenttime=modulus(anim_currenttime+timestep,anim_totaltime);
	}

	void light() const
	{
		if (!SOLID_OCTAGON_OBJECT::hasvapourized())
		{
			VECTOR pos=SOLID_OCTAGON_OBJECT::getposition();

			VIDEO::DYNAMIC_LIGHTING::draw_pointlight(pos.x,pos.y,1,1,1,1,512);

			if (beam.isactive())
			{
				beam.light(1,1,1,1,64);
			}

			return;
		}

		SOLID_OCTAGON_OBJECT::light(1,1,1,1,1,1,1,2);
	}

	void draw() const
	{
		if (!SOLID_OCTAGON_OBJECT::hasvapourized())
		{
			if (beam.isactive())
			{
				beam.draw(0,1,0,1,32);
			}

			SOLID_OCTAGON_OBJECT::draw(sprites[(int)((anim_currenttime/anim_totaltime)*16)],1);

			return;
		}

		SOLID_OCTAGON_OBJECT::draw(1,1,0,1,0,0,1);
	}

	void draw(const VIDEO::HUD &hud) const
	{
	}

	void sound() const
	{
		if (SOLID_OCTAGON_OBJECT::hasvapourized())
		{
			if (!explosion_snd.stopped())
			{
				VECTOR pos=SOLID_OCTAGON_OBJECT::getposition();
				VECTOR vel=SOLID_OCTAGON_OBJECT::getlinear_velocity();

				explosion_snd.play(pos.x,pos.y,vel.x,vel.y,1,1);
			}
		}
	}

	int getnumbeams() const
	{
		return(1);
	}

	BEAM<SOLID_OCTAGON_OBJECT>& getbeam(int i)
	{
		return(beam);
	}

	void main_init(const VECTOR &pos,double angle,const VECTOR &vel,double angular_vel,WORLD &world)
	{
		SOLID_OCTAGON_OBJECT::setposition(pos);
		SOLID_OCTAGON_OBJECT::setorientation(angle);
		SOLID_OCTAGON_OBJECT::setlinear_velocity(vel);
		SOLID_OCTAGON_OBJECT::setangular_velocity(angular_vel);

		world.putobject(*this);
		world.putbeam_object(*this);

		main_initialized=true;
	}

	void main_reset(WORLD &world)
	{
		reset();

		world.removeobject(*this);
		world.removebeam_object(*this);

		main_initialized=false;
	}

	bool main_wasinit() const
	{
		return(main_initialized);
	}

	bool main_exit() const
	{
		return(SOLID_OCTAGON_OBJECT::isgone());
	}

	void main_update(WORLD &world)
	{
	}
};
