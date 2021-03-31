//Code by Rogier Walraven. Do not distribute.

static const SOLID_OBJECT_PROPERTIES robot_fighter_solid_object_properties={{256,256,1024,4096,0.25,0.25,0.25,0.25,8},{256,384,2,8,16,1024,-8,8,256,0.25,0.25,1.25},16};

static const CONSTRAINT_SOLID_PROPERTIES robot_fighter_stabilizer_constraint_solid_properties={4096,0.25,0.75,2};

static const BEAM_PROPERTIES robot_fighter_beam_properties={2048,268435456,64};

class ROBOT_FIGHTER_OBJECT : public BIG_SOLID_OBJECT, public FEATURE, public BEAM_OBJECT<BIG_SOLID_OBJECT>
{
	int numframes;
	VIDEO::TEXTURE *frames;

	CONSTRAINT_SOLID<BIG_SOLID_OBJECT,BIG_SOLID_OBJECT> stabilizer_constraint1;
	CONSTRAINT_SOLID<BIG_SOLID_OBJECT,BIG_SOLID_OBJECT> stabilizer_constraint2;
	CONSTRAINT_SOLID<BIG_SOLID_OBJECT,BIG_SOLID_OBJECT> stabilizer_constraint3;
	CONSTRAINT_SOLID<BIG_SOLID_OBJECT,BIG_SOLID_OBJECT> stabilizer_constraint4;

	AUDIO::SOURCE explosion_snd;

	bool onsolidground;
	bool facingright;
	bool movingleftright;
	bool scatterfiremode;

	double onsolidground_timer;
	double facingright_timer;

	double anim_totaltime_walk;
	double anim_totaltime_jump;
	double anim_totaltime_turn;
	double anim_currenttime_walk;

	double scatterfire_beam_angle;

	BEAM<BIG_SOLID_OBJECT> beam;

	bool main_initialized;

  public:

	ROBOT_FIGHTER_OBJECT() : explosion_snd("Data\\Samples\\explosion.wav",1),beam(32)
	{
		BIG_SOLID_OBJECT::init_reset(robot_fighter_solid_object_properties);

		numframes=80;
		frames=VIDEO::loadtileset("Data\\Textures\\Sprites\\robot_fighter.bmp",2048,2560,256,256,0,0,numframes);

		stabilizer_constraint1.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		stabilizer_constraint2.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		stabilizer_constraint3.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		stabilizer_constraint4.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());

		onsolidground=false;
		facingright=true;
		movingleftright=false;
		scatterfiremode=false;

		onsolidground_timer=0;
		facingright_timer=0;

		anim_totaltime_walk=0.75*8;
		anim_totaltime_jump=0.25*8;
		anim_totaltime_turn=0.25*8;
		anim_currenttime_walk=0;

		scatterfire_beam_angle=0;

		beam.init_reset(robot_fighter_beam_properties,VECTOR(96,-44),90,*this);

		main_initialized=false;
	}

	~ROBOT_FIGHTER_OBJECT()
	{
		VIDEO::freetileset(frames,numframes);
	}

	void reset()
	{
		BIG_SOLID_OBJECT::init_reset(robot_fighter_solid_object_properties);

		stabilizer_constraint1.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		stabilizer_constraint2.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		stabilizer_constraint3.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		stabilizer_constraint4.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());

		explosion_snd.rewind();

		onsolidground=false;
		facingright=true;
		movingleftright=false;
		scatterfiremode=false;

		onsolidground_timer=0;
		facingright_timer=0;

		anim_totaltime_walk=0.75*8;
		anim_totaltime_jump=0.25*8;
		anim_totaltime_turn=0.25*8;
		anim_currenttime_walk=0;

		scatterfire_beam_angle=0;

		beam.init_reset(robot_fighter_beam_properties,VECTOR(96,-44),90,*this);
	}

	void control(double timestep)
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			double width=BIG_SOLID_OBJECT::getwidth();
			double height=BIG_SOLID_OBJECT::getheight();
			VECTOR pos=BIG_SOLID_OBJECT::getposition();
			double ang=BIG_SOLID_OBJECT::getorientation();

			VECTOR pos1_1(-width/2,-height/2);
			VECTOR pos1_2( width/2,-height/2);
			VECTOR pos1_3( width/2, height/2);
			VECTOR pos1_4(-width/2, height/2);

			VECTOR pos2_1=pos1_1*2;
			VECTOR pos2_2=pos1_2*2;
			VECTOR pos2_3=pos1_3*2;
			VECTOR pos2_4=pos1_4*2;

			pos1_1=pos1_1.gettransformed_from(pos,ang);
			pos1_2=pos1_2.gettransformed_from(pos,ang);
			pos1_3=pos1_3.gettransformed_from(pos,ang);
			pos1_4=pos1_4.gettransformed_from(pos,ang);
			pos2_1=pos2_1.gettransformed_from(pos,ang);
			pos2_2=pos2_2.gettransformed_from(pos,ang);
			pos2_3=pos2_3.gettransformed_from(pos,ang);
			pos2_4=pos2_4.gettransformed_from(pos,ang);

			stabilizer_constraint1.init_reset(robot_fighter_stabilizer_constraint_solid_properties,this,0,pos1_1,pos2_1);
			stabilizer_constraint2.init_reset(robot_fighter_stabilizer_constraint_solid_properties,this,0,pos1_2,pos2_2);
			stabilizer_constraint3.init_reset(robot_fighter_stabilizer_constraint_solid_properties,this,0,pos1_3,pos2_3);
			stabilizer_constraint4.init_reset(robot_fighter_stabilizer_constraint_solid_properties,this,0,pos1_4,pos2_4);

			stabilizer_constraint1.rotate_worldspace_coords_around_point(pos,-ang);
			stabilizer_constraint2.rotate_worldspace_coords_around_point(pos,-ang);
			stabilizer_constraint3.rotate_worldspace_coords_around_point(pos,-ang);
			stabilizer_constraint4.rotate_worldspace_coords_around_point(pos,-ang);

			stabilizer_constraint1.enforce(timestep);
			stabilizer_constraint2.enforce(timestep);
			stabilizer_constraint3.enforce(timestep);
			stabilizer_constraint4.enforce(timestep);

			double leg_checklength=64;
			double leg_beginshift=32;
			double leg_widthshift=96;

			VECTOR i1,i2;
			VECTOR n1,n2;
			double a1,a2;
			void *obj1,*obj2;

			VECTOR pos3( (width/2)-leg_widthshift, height/2);
			VECTOR pos4((-width/2)+leg_widthshift, height/2);

			pos3=pos3.gettransformed_from(pos,ang);
			pos4=pos4.gettransformed_from(pos,ang);

			LINE l1(pos4-VECTOR(0,leg_beginshift).getrotated(ang),pos4+VECTOR(0,leg_checklength).getrotated(ang));
			LINE l2(pos3-VECTOR(0,leg_beginshift).getrotated(ang),pos3+VECTOR(0,leg_checklength).getrotated(ang));

			bool touch1=clist.getintersection_seg(i1,n1,a1,&obj1,l1,0);
			bool touch2=clist.getintersection_seg(i2,n2,a2,&obj2,l2,0);

			bool oldonsolidground=onsolidground;

			bool turning=facingright_timer<anim_totaltime_turn;

			bool oldscatterfiremode=scatterfiremode;

			if (INPUT::KEYBOARD::n())
			{
				scatterfiremode=true;
			}
			else
			{
				scatterfiremode=false;
			}

			if (touch1 && touch2)
			{
				onsolidground=true;

				if (!scatterfiremode && !turning && INPUT::KEYBOARD::up())
				{
					BIG_SOLID_OBJECT::addforce((VECTOR(0,-32)*BIG_SOLID_OBJECT::getmass())/timestep);
				}
			}
			else
			{
				onsolidground=false;

				scatterfiremode=false;
			}

			double force_constant=16384*2;

			VECTOR left=VECTOR(-1,0).getrotated(ang);

			bool oldfacingright=facingright;

			movingleftright=false;

			if (!scatterfiremode && !turning && !(INPUT::KEYBOARD::left() && INPUT::KEYBOARD::right()))
			{
				if (INPUT::KEYBOARD::left())
				{
					facingright=false;
					movingleftright=true;

					if (onsolidground)
					{
						BIG_SOLID_OBJECT::addforce(left*force_constant);
					}
					else
					{
						BIG_SOLID_OBJECT::addforce(left*(force_constant/4));
					}
				}

				if (INPUT::KEYBOARD::right())
				{
					facingright=true;
					movingleftright=true;

					if (onsolidground)
					{
						BIG_SOLID_OBJECT::addforce(-left*force_constant);
					}
					else
					{
						BIG_SOLID_OBJECT::addforce(-left*(force_constant/4));
					}
				}
			}
			else if (scatterfiremode && !(INPUT::KEYBOARD::left() && INPUT::KEYBOARD::right()))
			{
				if (INPUT::KEYBOARD::left())
				{
					scatterfire_beam_angle-=45*timestep;
				}

				if (INPUT::KEYBOARD::right())
				{
					scatterfire_beam_angle+=45*timestep;
				}
			}

			if (onsolidground!=oldonsolidground)
			{
				onsolidground_timer=0;
			}

			if (facingright!=oldfacingright)
			{
				facingright_timer=0;

				bool beamactive=beam.isactive();

				if (facingright)
				{
					beam.init_reset(robot_fighter_beam_properties,VECTOR(96,-44),90,*this);

					if (beamactive)
					{
						beam.activate();
					}
				}
				else
				{
					beam.init_reset(robot_fighter_beam_properties,VECTOR(-96,-44),-90,*this);

					if (beamactive)
					{
						beam.activate();
					}
				}
			}
			else if (scatterfiremode)
			{
				if (scatterfiremode!=oldscatterfiremode)
				{
					scatterfire_beam_angle=0;
				}

				if (facingright)
				{
					beam.init_reset(robot_fighter_beam_properties,VECTOR(96,-44),scatterfire_beam_angle+90,*this);
				}
				else
				{
					beam.init_reset(robot_fighter_beam_properties,VECTOR(-96,-44),scatterfire_beam_angle-90,*this);
				}

				beam.activate();
			}

			if (!scatterfiremode && scatterfiremode!=oldscatterfiremode)
			{
				bool beamactive=beam.isactive();

				if (facingright)
				{
					beam.init_reset(robot_fighter_beam_properties,VECTOR(96,-44),90,*this);

					if (beamactive)
					{
						beam.activate();
					}
				}
				else
				{
					beam.init_reset(robot_fighter_beam_properties,VECTOR(-96,-44),-90,*this);

					if (beamactive)
					{
						beam.activate();
					}
				}
			}

			if (!scatterfiremode)
			{
				if (INPUT::KEYBOARD::b() || INPUT::KEYBOARD::n())
				{
					if (!beam.isgone() && !beam.isactive())
					{
						beam.activate();
					}
				}
				else
				{
					if (!beam.isgone() && beam.isactive())
					{
						beam.deactivate();
					}
				}

				if (turning || (movingleftright && onsolidground))
				{
					if (!beam.isgone() && beam.isactive())
					{
						beam.deactivate();
					}
				}
			}

			if (INPUT::KEYBOARD::x())
			{
				BIG_SOLID_OBJECT::sethealth(0);
			}

			return;
		}

		stabilizer_constraint1.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		stabilizer_constraint2.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		stabilizer_constraint3.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());
		stabilizer_constraint4.init_reset(robot_fighter_stabilizer_constraint_solid_properties,0,0,VECTOR(),VECTOR());

		onsolidground=false;
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

		if (!stabilizer_constraint1.isgone())
		{
			stabilizer_constraint1.change_origin(new_origin);
		}

		if (!stabilizer_constraint2.isgone())
		{
			stabilizer_constraint2.change_origin(new_origin);
		}

		if (!stabilizer_constraint3.isgone())
		{
			stabilizer_constraint3.change_origin(new_origin);
		}

		if (!stabilizer_constraint4.isgone())
		{
			stabilizer_constraint4.change_origin(new_origin);
		}

		if (!beam.isgone())
		{
			beam.change_origin(new_origin);
		}
	}

	int getnumbeams() const
	{
		return(1);
	}

	BEAM<BIG_SOLID_OBJECT>& getbeam(int i)
	{
		return(beam);
	}

	void integrate(double timestep)
	{
		BIG_SOLID_OBJECT::integrate(timestep);

		onsolidground_timer+=timestep;
		facingright_timer+=timestep;
		anim_currenttime_walk=modulus(anim_currenttime_walk+timestep,anim_totaltime_walk);
	}

	void light() const
	{
		if (!BIG_SOLID_OBJECT::hasvapourized())
		{
			if (!beam.isgone() && beam.isactive())
			{
				beam.light(1,1,1,1,64);
			}

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
			if (!beam.isgone() && beam.isactive())
			{
				beam.draw(1,0,0,0.75,32);
			}

			double width=BIG_SOLID_OBJECT::getwidth();
			double height=BIG_SOLID_OBJECT::getheight();
			VECTOR pos=BIG_SOLID_OBJECT::getposition();
			double ang=BIG_SOLID_OBJECT::getorientation();

			VECTOR pos1(-width/2,-height/2);
			VECTOR pos2( width/2,-height/2);
			VECTOR pos3( width/2, height/2);
			VECTOR pos4(-width/2, height/2);

			pos1=(pos1*1.125).gettransformed_from(pos,ang);
			pos2=(pos2*1.125).gettransformed_from(pos,ang);
			pos3=(pos3*1.125).gettransformed_from(pos,ang);
			pos4=(pos4*1.125).gettransformed_from(pos,ang);

			bool turning=facingright_timer<anim_totaltime_turn;

			int anim_frameidx_walk=(int)((anim_currenttime_walk/anim_totaltime_walk)*16);
			int anim_frameidx_turn=(int)((facingright_timer/anim_totaltime_turn)*8);
			int anim_frameidx_jump=(int)((onsolidground_timer/anim_totaltime_jump)*8); if (anim_frameidx_jump>7) {anim_frameidx_jump=7;}
			int anim_frameidx_rot =(int)((modulus(scatterfire_beam_angle,360)/360)*16);

			if (!scatterfiremode)
			{

			if (onsolidground)
			{
				if (facingright)
				{
					if (!turning)
					{
						if (movingleftright)
						{
							VIDEO::draw_quad_texture(pos1.x,pos1.y,pos2.x,pos2.y,pos3.x,pos3.y,pos4.x,pos4.y,frames[anim_frameidx_walk],1);
						}
						else
						{
							VIDEO::draw_quad_texture(pos1.x,pos1.y,pos2.x,pos2.y,pos3.x,pos3.y,pos4.x,pos4.y,frames[40],1);
						}
					}
					else
					{
							VIDEO::draw_quad_texture(pos2.x,pos2.y,pos1.x,pos1.y,pos4.x,pos4.y,pos3.x,pos3.y,frames[32+anim_frameidx_turn],1);
					}
				}
				else
				{
					if (!turning)
					{
						if (movingleftright)
						{
							VIDEO::draw_quad_texture(pos2.x,pos2.y,pos1.x,pos1.y,pos4.x,pos4.y,pos3.x,pos3.y,frames[anim_frameidx_walk],1);
						}
						else
						{
							VIDEO::draw_quad_texture(pos2.x,pos2.y,pos1.x,pos1.y,pos4.x,pos4.y,pos3.x,pos3.y,frames[40],1);
						}
					}
					else
					{
							VIDEO::draw_quad_texture(pos1.x,pos1.y,pos2.x,pos2.y,pos3.x,pos3.y,pos4.x,pos4.y,frames[32+anim_frameidx_turn],1);
					}
				}
			}
			else
			{
				if (facingright)
				{
					if (!turning)
					{
						VIDEO::draw_quad_texture(pos1.x,pos1.y,pos2.x,pos2.y,pos3.x,pos3.y,pos4.x,pos4.y,frames[40+anim_frameidx_jump],1);
					}
					else
					{
						VIDEO::draw_quad_texture(pos2.x,pos2.y,pos1.x,pos1.y,pos4.x,pos4.y,pos3.x,pos3.y,frames[48+anim_frameidx_turn],1);
					}
				}
				else
				{
					if (!turning)
					{
						VIDEO::draw_quad_texture(pos2.x,pos2.y,pos1.x,pos1.y,pos4.x,pos4.y,pos3.x,pos3.y,frames[40+anim_frameidx_jump],1);
					}
					else
					{
						VIDEO::draw_quad_texture(pos1.x,pos1.y,pos2.x,pos2.y,pos3.x,pos3.y,pos4.x,pos4.y,frames[48+anim_frameidx_turn],1);
					}
				}
			}

			}
			else
			{
				if (facingright)
				{
					VIDEO::draw_quad_texture(pos1.x,pos1.y,pos2.x,pos2.y,pos3.x,pos3.y,pos4.x,pos4.y,frames[40],1);
				}
				else
				{
					VIDEO::draw_quad_texture(pos2.x,pos2.y,pos1.x,pos1.y,pos4.x,pos4.y,pos3.x,pos3.y,frames[40],1);
				}
			}

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
		return(BIG_SOLID_OBJECT::isgone());
	}

	void main_update(WORLD &world)
	{
	}
};
