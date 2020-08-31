//Code by Rogier Walraven. Do not distribute.

static const SOLID_OBJECT_PROPERTIES flipper_object_properties={{768,256,1024,4096,0.25,0.25,0.25,0.25,8},{256,384,2,8,16,1024,-8,8,256,0.25,0.25,1.25},1024};

static const CONSTRAINT_SOLID_PROPERTIES flipper_constraint_solid_properties={8192,0.25,0.25,2};

class FLIPPER_OBJECT : public BIG_SOLID_OBJECT, public FEATURE
{
	VIDEO::TEXTURE sprite;

	AUDIO::SOURCE explosion_snd;

	CONSTRAINT_SOLID<BIG_SOLID_OBJECT,BIG_SOLID_OBJECT> conn1,conn2,conn3,conn4;

	bool main_initialized;

  public:

	FLIPPER_OBJECT() : explosion_snd("Data\\Samples\\explosion.wav",1)
	{
		BIG_SOLID_OBJECT::init_reset(flipper_object_properties);

		sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\block.bmp");

		conn1.init_reset(flipper_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		conn2.init_reset(flipper_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		conn3.init_reset(flipper_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		conn4.init_reset(flipper_constraint_solid_properties,0,0,VECTOR(),VECTOR());

		main_initialized=false;
	}

	~FLIPPER_OBJECT()
	{
		VIDEO::freetexture(sprite);
	}

	void reset()
	{
		BIG_SOLID_OBJECT::init_reset(flipper_object_properties);

		explosion_snd.rewind();

		conn1.init_reset(flipper_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		conn2.init_reset(flipper_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		conn3.init_reset(flipper_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		conn4.init_reset(flipper_constraint_solid_properties,0,0,VECTOR(),VECTOR());
	}

	void control(double timestep)
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			if (!INPUT::KEYBOARD::b())
			{
				if (INPUT::KEYBOARD::right())
				{
					BIG_SOLID_OBJECT::addtorque(4.096e9);
				}
				if (INPUT::KEYBOARD::left())
				{
					BIG_SOLID_OBJECT::addtorque(-4.096e9);
				}
			}
			else
			{
				BIG_SOLID_OBJECT::addtorque(-BIG_SOLID_OBJECT::getangular_momentum()/timestep);
			}

			if (!conn1.isgone())
			{
				conn1.enforce(timestep);
			}
			if (!conn2.isgone())
			{
				conn2.enforce(timestep);
			}
			if (!conn3.isgone())
			{
				conn3.enforce(timestep);
			}
			if (!conn4.isgone())
			{
				conn4.enforce(timestep);
			}
		}
	}

	void addheat(double ht,const VECTOR &pos)
	{
		BIG_SOLID_OBJECT::addheat(ht,pos);
	}

	void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
	{
		BIG_SOLID_OBJECT::addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	}

	void change_origin(const VECTOR &new_origin)
	{
		BIG_SOLID_OBJECT::change_origin(new_origin);

		if (!conn1.isgone())
		{
			conn1.change_origin(new_origin);
		}
		if (!conn2.isgone())
		{
			conn2.change_origin(new_origin);
		}
		if (!conn3.isgone())
		{
			conn3.change_origin(new_origin);
		}
		if (!conn4.isgone())
		{
			conn4.change_origin(new_origin);
		}
	}

	void integrate(double timestep)
	{
		BIG_SOLID_OBJECT::integrate(timestep);
	}

	void light() const
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			VECTOR pos=BIG_SOLID_OBJECT::getposition();

			VIDEO::DYNAMIC_LIGHTING::draw_pointlight(pos.x,pos.y,1,1,1,1,512);

			return;
		}

		BIG_SOLID_OBJECT::light(1,1,1,1,1,1,1,2);
	}

	void draw() const
	{
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
		if (BIG_SOLID_OBJECT::hasvapourized())
		{
			if (!explosion_snd.stopped())
			{
				VECTOR pos=BIG_SOLID_OBJECT::getposition();
				VECTOR vel=BIG_SOLID_OBJECT::getlinear_velocity();

				explosion_snd.play(pos.x,pos.y,vel.x,vel.y,1,1);
			}
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
		return(BIG_SOLID_OBJECT::isgone());
	}

	void main_update(WORLD &world)
	{
	}

	void connection_init_reset(const VECTOR &p1a,const VECTOR &p1b,
                               const VECTOR &p2a,const VECTOR &p2b,
                               const VECTOR &p3a,const VECTOR &p3b,
                               const VECTOR &p4a,const VECTOR &p4b)
	{
		conn1.init_reset(flipper_constraint_solid_properties,this,0,p1a,p1b);
		conn2.init_reset(flipper_constraint_solid_properties,this,0,p2a,p2b);
		conn3.init_reset(flipper_constraint_solid_properties,this,0,p3a,p3b);
		conn4.init_reset(flipper_constraint_solid_properties,this,0,p4a,p4b);
	}

	bool connection_isgone() const
	{
		return(conn1.isgone() || conn2.isgone() || conn3.isgone() || conn4.isgone());
	}
};
