//Code by Rogier Walraven. Do not distribute.

struct PARTICLE_EMITTER_PROPERTIES
{
	double min_lifetime;
	double max_lifetime;
	double pmass;
	double std_speed;
	int    min_spawn;
	int    max_spawn;
	double spread;
	double speedvar;
	double friction;
};

struct PARTICLE
{
	VECTOR position;
	VECTOR velocity;

	double mass;
	double friction;

	VECTOR force;

	double lifetime;
	double lifeleft;

	PARTICLE derive(double time) const
	{
		PARTICLE delta;

		delta.position=velocity;

		delta.velocity+=DVDT::force(force,mass);
		delta.velocity+=DVDT::friction(velocity,mass,friction);

		return(delta);
	}

	PARTICLE operator+(const PARTICLE &delta) const
	{
		PARTICLE temp;

		temp.position=position+delta.position;
		temp.velocity=velocity+delta.velocity;

		temp.mass    =mass;
		temp.friction=friction;
		temp.force   =force;
		temp.lifetime=lifetime;
		temp.lifeleft=lifeleft;

		return(temp);
	}

	PARTICLE operator*(double timestep) const
	{
		PARTICLE temp;

		temp.position=position*timestep;
		temp.velocity=velocity*timestep;

		return(temp);
	}
};

template<class O>
struct PARTICLE_EMITTER
{
	PARTICLE_EMITTER(int num);
	~PARTICLE_EMITTER();

	void init_reset(const PARTICLE_EMITTER_PROPERTIES &props);
	void init_reset(const PARTICLE_EMITTER_PROPERTIES &props,const O &object);

	bool wasinit() const;
	bool isgone() const;

	void addforce(const VECTOR &frc,const VECTOR &pos,double timestep);

	void addheat(double ht,const VECTOR &pos);

	void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc);

	void change_origin(const VECTOR &new_origin);

	void integrate(double timestep);

	void light(double r1,double g1,double b1,double r2,double g2,double b2,double a,double size) const;

	void draw(double r1,double g1,double b1,double r2,double g2,double b2,double a,double size) const;

  private:

	bool initialized;
	int numparticles;
	PARTICLE *particles;
	const O *object;
	const PARTICLE_EMITTER_PROPERTIES *properties;

	PARTICLE_EMITTER(const PARTICLE_EMITTER<O> &other) {}
	PARTICLE_EMITTER<O>& operator=(const PARTICLE_EMITTER<O> &other) {}
};

template<class O>
PARTICLE_EMITTER<O>::PARTICLE_EMITTER(int num) : initialized(false),numparticles(num),particles(new PARTICLE[num]) {}

template<class O>
PARTICLE_EMITTER<O>::~PARTICLE_EMITTER()
{
	delete[] particles;
}

template<class O>
void PARTICLE_EMITTER<O>::init_reset(const PARTICLE_EMITTER_PROPERTIES &props)
{
	for (int i=0;i<numparticles;i++)
	{
		particles[i].mass=props.pmass;
		particles[i].friction=props.friction;
		particles[i].lifeleft=0;
	}

	object=0;

	properties=&props;

	initialized=true;
}

template<class O>
void PARTICLE_EMITTER<O>::init_reset(const PARTICLE_EMITTER_PROPERTIES &props,const O &object)
{
	for (int i=0;i<numparticles;i++)
	{
		particles[i].mass=props.pmass;
		particles[i].friction=props.friction;
		particles[i].lifeleft=0;
	}

	this->object=&object;

	properties=&props;

	initialized=true;
}

template<class O>
bool PARTICLE_EMITTER<O>::wasinit() const
{
	if (initialized)
	{
		if (object)
		{
			return(object->wasinit());
		}

		return(true);
	}

	return(false);
}

template<class O>
bool PARTICLE_EMITTER<O>::isgone() const
{
	if (object)
	{
		return(object->isgone());
	}

	return(false);
}

template<class O>
void PARTICLE_EMITTER<O>::addforce(const VECTOR &frc,const VECTOR &pos,double timestep)
{
	double deltamom=frc.getlength()*timestep;

	if (equalszero(deltamom))
	{
		return;
	}

	double speed=properties->std_speed;

	int spawned=appround(deltamom/(speed*properties->pmass));

	if (spawned<properties->min_spawn)
	{
		speed=deltamom/(properties->min_spawn*properties->pmass);

		spawned=properties->min_spawn;
	}
	else if (spawned>properties->max_spawn)
	{
		speed=deltamom/(properties->max_spawn*properties->pmass);

		spawned=properties->max_spawn;
	}

	VECTOR refvel;

	if (object)
	{
		VECTOR objpos      =object->getposition();
		VECTOR objvel      =object->getlinear_velocity();
		double objangvelrad=object->getangular_velocity()*(math_pi/180);

		refvel=objvel+(((pos-objpos).getperpend())*objangvelrad);
	}

	VECTOR exitnormal=-frc.getnormal();

	for (int i=0;i<spawned;i++)
	{
		for (int j=0;j<numparticles;j++)
		{
			if (particles[j].lifeleft<=0)
			{
				particles[j].position=pos;
				particles[j].velocity=exitnormal.getrotated(RANDOM::global(-properties->spread,properties->spread));
				particles[j].velocity*=speed*RANDOM::global(1-properties->speedvar,1+properties->speedvar);
				particles[j].velocity+=refvel;
				particles[j].force.setzero();
				particles[j].lifetime=RANDOM::global(properties->min_lifetime,properties->max_lifetime);
				particles[j].lifeleft=particles[j].lifetime;

				goto exit;
			}
		}

		exit:
		;
	}
}

template<class O>
void PARTICLE_EMITTER<O>::addheat(double ht,const VECTOR &pos)
{
	if (equalszero(ht))
	{
		return;
	}

	double speed=properties->std_speed;

	int spawned=appround(ht/(square(speed)*properties->pmass*0.5));

	if (spawned<properties->min_spawn)
	{
		speed=squareroot((ht*2)/(properties->min_spawn*properties->pmass));

		spawned=properties->min_spawn;
	}
	else if (spawned>properties->max_spawn)
	{
		speed=squareroot((ht*2)/(properties->max_spawn*properties->pmass));

		spawned=properties->max_spawn;
	}

	VECTOR refvel;

	if (object)
	{
		VECTOR objpos      =object->getposition();
		VECTOR objvel      =object->getlinear_velocity();
		double objangvelrad=object->getangular_velocity()*(math_pi/180);

		refvel=objvel+(((pos-objpos).getperpend())*objangvelrad);
	}

	for (int i=0;i<spawned;i++)
	{
		for (int j=0;j<numparticles;j++)
		{
			if (particles[j].lifeleft<=0)
			{
				particles[j].position=pos;
				particles[j].velocity=VECTOR(0,-1).getrotated(RANDOM::global(0,360));
				particles[j].velocity*=speed*RANDOM::global(1-properties->speedvar,1+properties->speedvar);
				particles[j].velocity+=refvel;
				particles[j].force.setzero();
				particles[j].lifetime=RANDOM::global(properties->min_lifetime,properties->max_lifetime);
				particles[j].lifeleft=particles[j].lifetime;

				goto exit;
			}
		}

		exit:
		;
	}
}

template<class O>
void PARTICLE_EMITTER<O>::addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
{
	for (int i=0;i<numparticles;i++)
	{
		if (particles[i].lifeleft>0)
		{
			particles[i].force.setzero();

			particles[i].force+=-(linear_acc*particles[i].mass);

			particles[i].force+=-((particles[i].velocity.getperpend())*angular_vel*(math_pi/180)*particles[i].mass*2);

			particles[i].force+=(particles[i].position-axle_pos)*square(angular_vel*(math_pi/180))*particles[i].mass;

			particles[i].force+=-(((particles[i].position-axle_pos).getperpend())*angular_acc*(math_pi/180)*particles[i].mass);
		}
	}
}

template<class O>
void PARTICLE_EMITTER<O>::change_origin(const VECTOR &new_origin)
{
	for (int i=0;i<numparticles;i++)
	{
		if (particles[i].lifeleft>0)
		{
			particles[i].position-=new_origin;
		}
	}
}

template<class O>
void PARTICLE_EMITTER<O>::integrate(double timestep)
{
	for (int i=0;i<numparticles;i++)
	{
		if (particles[i].lifeleft>0)
		{
			particles[i]=integrate_RK4(particles[i],timestep);

			particles[i].lifeleft-=timestep;
		}
	}
}

template<class O>
void PARTICLE_EMITTER<O>::light(double r1,double g1,double b1,double r2,double g2,double b2,double a,double size) const
{
	double cmul1;
	double cmul2;

	VIDEO::DYNAMIC_LIGHTING::BATCH::draw_sparklight_start(size);

	for (int i=0;i<numparticles;i++)
	{
		if (particles[i].lifeleft>0)
		{
			cmul1=particles[i].lifeleft/particles[i].lifetime;
			cmul2=1-cmul1;

			VIDEO::DYNAMIC_LIGHTING::BATCH::draw_sparklight(particles[i].position.x,particles[i].position.y,r1*cmul1 + r2*cmul2,
                                                                                                            g1*cmul1 + g2*cmul2,
                                                                                                            b1*cmul1 + b2*cmul2,a*cmul1);
		}
	}

	VIDEO::DYNAMIC_LIGHTING::BATCH::draw_sparklight_end();
}

template<class O>
void PARTICLE_EMITTER<O>::draw(double r1,double g1,double b1,double r2,double g2,double b2,double a,double size) const
{
	double cmul1;
	double cmul2;

	VIDEO::BATCH::draw_point_sized_start(size);

	for (int i=0;i<numparticles;i++)
	{
		if (particles[i].lifeleft>0)
		{
			cmul1=particles[i].lifeleft/particles[i].lifetime;
			cmul2=1-cmul1;

			VIDEO::BATCH::draw_point_sized(particles[i].position.x,particles[i].position.y,r1*cmul1 + r2*cmul2,
                                                                                           g1*cmul1 + g2*cmul2,
                                                                                           b1*cmul1 + b2*cmul2,a*cmul1);
		}
	}

	VIDEO::BATCH::draw_point_sized_end();
}
