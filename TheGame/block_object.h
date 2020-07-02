//Code by Rogier Walraven. Do not distribute.

static const SOLID_OBJECT_PROPERTIES block_solid_object_properties={{256,384,1024,4096,0.25,0.25,0.25,0.25,8},{256,384,2,8,16,1024,-8,8,256,0.25,0.25,1.25},4};

class BLOCK_OBJECT : public BIG_SOLID_OBJECT, public FEATURE/*, public BEAM_OBJECT<BIG_SOLID_OBJECT>*/
{
	VIDEO::TEXTURE sprite;

	bool main_initialized;

  public:

	BLOCK_OBJECT()
	{
		BIG_SOLID_OBJECT::init_reset(block_solid_object_properties);

		sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\block.bmp");

		main_initialized=false;
	}

	~BLOCK_OBJECT()
	{
		VIDEO::freetexture(sprite);
	}

	void reset()
	{
		BIG_SOLID_OBJECT::init_reset(block_solid_object_properties);
	}

	void control(double timestep)
	{
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
	}

	/*int getnumbeams() const
	BEAM<BIG_SOLID_OBJECT>& getbeam(int i)*/
	void integrate(double timestep)
	{
		BIG_SOLID_OBJECT::integrate(timestep);
	}

	void light() const
	{
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
