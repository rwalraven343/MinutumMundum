//Code by Rogier Walraven. Do not distribute.

static const SOLID_OCTAGON_OBJECT_PROPERTIES ball_object_properties={{128,1024,4096,0.25,0.25,0.25,0.25,8},{256,384,2,8,16,1024,-8,8,256,0.25,0.25,1.25},1024};

class BALL_OBJECT : public SOLID_OCTAGON_OBJECT, public FEATURE
{
	VIDEO::TEXTURE sprite;

	AUDIO::SOURCE explosion_snd;

	bool main_initialized;

  public:

	BALL_OBJECT() : explosion_snd("Data\\Samples\\explosion.wav",1)
	{
		SOLID_OCTAGON_OBJECT::init_reset(ball_object_properties);

		sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\ball3.bmp");

		main_initialized=false;
	}

	~BALL_OBJECT()
	{
		VIDEO::freetexture(sprite);
	}

	void reset()
	{
		SOLID_OCTAGON_OBJECT::init_reset(ball_object_properties);

		explosion_snd.rewind();
	}

	void control(double timestep)
	{
		if (!SOLID_OCTAGON_OBJECT::hasvapourized())
		{
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
	}

	void light() const
	{
		if (!SOLID_OCTAGON_OBJECT::hasvapourized())
		{
			VECTOR pos=SOLID_OCTAGON_OBJECT::getposition();

			VIDEO::DYNAMIC_LIGHTING::draw_pointlight(pos.x,pos.y,1,1,1,1,512);

			return;
		}

		SOLID_OCTAGON_OBJECT::light(1,1,1,1,1,1,1,2);
	}

	void draw() const
	{
		if (!SOLID_OCTAGON_OBJECT::hasvapourized())
		{
			SOLID_OCTAGON_OBJECT::draw(sprite,1);

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

	void main_init(const VECTOR &pos,double angle,const VECTOR &vel,double angular_vel,WORLD &world)
	{
		SOLID_OCTAGON_OBJECT::setposition(pos);
		SOLID_OCTAGON_OBJECT::setorientation(angle);
		SOLID_OCTAGON_OBJECT::setlinear_velocity(vel);
		SOLID_OCTAGON_OBJECT::setangular_velocity(angular_vel);

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
		return(SOLID_OCTAGON_OBJECT::isgone());
	}

	void main_update(WORLD &world)
	{
	}
};
