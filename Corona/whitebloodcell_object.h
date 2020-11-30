//Code by Rogier Walraven. Do not distribute.

static const LIQUID_OBJECT_PROPERTIES whitebloodcell_object_properties={{512,768,1,2048,4096*8,1024*8,0.25,0.25,0.25,1.25},{512,768,2,8,16,2048,-8,8,256,0.25,0.25,1.25},4};

class WHITEBLOODCELL_OBJECT : public BIG_LIQUID_OBJECT, public FEATURE
{
	bool main_initialized;

  public:

	WHITEBLOODCELL_OBJECT()
	{
		BIG_LIQUID_OBJECT::init_reset(whitebloodcell_object_properties);

		main_initialized=false;
	}

	~WHITEBLOODCELL_OBJECT()
	{
	}

	void reset()
	{
		BIG_LIQUID_OBJECT::init_reset(whitebloodcell_object_properties);
	}

	void control(double timestep)
	{
		if (!BIG_LIQUID_OBJECT::hasvapourized())
		{
		}
	}

	void addheat(double ht,const VECTOR &pos)
	{
		BIG_LIQUID_OBJECT::addheat(ht,pos);
	}

	void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
	{
		BIG_LIQUID_OBJECT::addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	}

	void change_origin(const VECTOR &new_origin)
	{
		BIG_LIQUID_OBJECT::change_origin(new_origin);
	}

	void integrate(double timestep)
	{
		BIG_LIQUID_OBJECT::integrate(timestep);
	}

	void light() const
	{
		if (!BIG_LIQUID_OBJECT::hasvapourized())
		{
			VECTOR pos=BIG_LIQUID_OBJECT::getposition();

			VIDEO::DYNAMIC_LIGHTING::draw_pointlight(pos.x,pos.y,1,1,1,1,512);

			return;
		}

		BIG_LIQUID_OBJECT::light(1,1,1,1,1,1,1,2);
	}

	void draw() const
	{
		if (!BIG_LIQUID_OBJECT::hasvapourized())
		{
			BIG_LIQUID_OBJECT::draw(1,1,1,0,0,1,1);

			return;
		}

		BIG_LIQUID_OBJECT::draw(1,1,1,0,0,1,1);
	}

	void draw(const VIDEO::HUD &hud) const
	{
	}

	void sound() const
	{
	}

	void main_init(const VECTOR &pos,double angle,const VECTOR &vel,double angular_vel,WORLD &world)
	{
		BIG_LIQUID_OBJECT::setposition(pos);
		BIG_LIQUID_OBJECT::setlinear_velocity(vel);
		BIG_LIQUID_OBJECT::setangular_velocity(angular_vel);

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
		return(BIG_LIQUID_OBJECT::isgone());
	}

	void main_update(WORLD &world)
	{
	}
};
