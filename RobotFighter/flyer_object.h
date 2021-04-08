//Code by Rogier Walraven. Do not distribute.

static const SOLID_OCTAGON_OBJECT_PROPERTIES flyer_solid_octagon_object_properties={{128,1024,4096,0.25,8,4,0.25,8},{256,512,2,8,16,1024,-8,8,256,0.25,0.25,1.25},128};

class FLYER_OBJECT : public SOLID_OCTAGON_OBJECT, public FEATURE/*, public BEAM_OBJECT<SOLID_OCTAGON_OBJECT>*/
{
	VIDEO::TEXTURE sprite;

	bool main_initialized;

  public:

	FLYER_OBJECT()
	{
		SOLID_OCTAGON_OBJECT::init_reset(flyer_solid_octagon_object_properties);

		sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\spaceship4.bmp");

		main_initialized=false;
	}

	~FLYER_OBJECT()
	{
		VIDEO::freetexture(sprite);
	}

	void reset()
	{
		SOLID_OCTAGON_OBJECT::init_reset(flyer_solid_octagon_object_properties);
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

	/*int getnumbeams() const
	BEAM<SOLID_OCTAGON_OBJECT>& getbeam(int i)*/
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
