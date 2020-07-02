//Code by Rogier Walraven. Do not distribute.

static const SOLID_OBJECT_PROPERTIES snake_tail_solid_object_properties={{256,384,1024,4096,0.25,0.25,0.25,0.25,8},{256,384,2,8,16,1024,-8,8,256,0.25,0.25,1.25},4};

static const CONSTRAINT_SOLID_PROPERTIES snake_tail_constraint_solid_properties={2048,0.25,0.25,2};

class SNAKE_TAIL_OBJECT : public BIG_SOLID_OBJECT, public FEATURE/*, public BEAM_OBJECT<BIG_SOLID_OBJECT>*/
{
	VIDEO::TEXTURE sprite;

	CONSTRAINT_SOLID<BIG_SOLID_OBJECT,BIG_SOLID_OBJECT> connection;

	VIDEO::TEXTURE connection_sprite;

	AUDIO::SOURCE explosion_snd;

	bool main_initialized;

  public:

	SNAKE_TAIL_OBJECT() : explosion_snd("Data\\Samples\\explosion.wav",1)
	{
		BIG_SOLID_OBJECT::init_reset(snake_tail_solid_object_properties);

		sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\snaketail.bmp");

		connection.init_reset(snake_tail_constraint_solid_properties,0,0,VECTOR(),VECTOR());

		connection_sprite=VIDEO::loadtexture("Data\\Textures\\Sprites\\chain.bmp");

		main_initialized=false;
	}

	~SNAKE_TAIL_OBJECT()
	{
		VIDEO::freetexture(sprite);
		VIDEO::freetexture(connection_sprite);
	}

	void reset()
	{
		BIG_SOLID_OBJECT::init_reset(snake_tail_solid_object_properties);

		connection.init_reset(snake_tail_constraint_solid_properties,0,0,VECTOR(),VECTOR());

		explosion_snd.rewind();
	}

	void control(double timestep)
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			if (!connection.isgone())
			{
				connection.enforce(timestep);
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

		if (!connection.isgone())
		{
			connection.change_origin(new_origin);
		}
	}

	/*int getnumbeams() const
	BEAM<BIG_SOLID_OBJECT>& getbeam(int i)*/
	void integrate(double timestep)
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			if (connection.isgone())
			{
				BIG_SOLID_OBJECT::addheat(1.0e9*timestep,VECTOR());
			}
		}

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
			if (!connection.isgone())
			{
				connection.draw(connection_sprite,1,32,16);
			}

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

	void connection_init_reset(BIG_SOLID_OBJECT &head)
	{
		if (head.wasinit() && !head.isgone() && !head.hasvapourized())
		{
			double conn_len=256;

			double head_width =head.getwidth();
			double head_height=head.getheight();

			VECTOR head_pos=head.getposition();
			double head_ang=head.getorientation();
			VECTOR head_vel=head.getlinear_velocity();

			VECTOR pos1(0,head_height/2);
			VECTOR pos2=pos1+VECTOR(0,conn_len);

			VECTOR this_pos=pos1+pos2;

			pos1=pos1.gettransformed_from(head_pos,head_ang);
			pos2=pos2.gettransformed_from(head_pos,head_ang);

			this_pos=this_pos.gettransformed_from(head_pos,head_ang);

			BIG_SOLID_OBJECT::setposition(this_pos);
			BIG_SOLID_OBJECT::setorientation(head_ang);
			BIG_SOLID_OBJECT::setlinear_velocity(head_vel);

			connection.init_reset(snake_tail_constraint_solid_properties,&head,this,pos1,pos2);
		}
	}

	bool connection_intact() const
	{
		return(connection.wasinit() && !connection.isgone());
	}
};
