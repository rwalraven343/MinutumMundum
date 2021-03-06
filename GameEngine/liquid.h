//Code by Rogier Walraven. Do not distribute.

struct LIQUID_PROPERTIES
{
	double radius;
	double max_radius;
	double smoothfactor;
	double mass;
	double rkonst;
	double akonst;
	double dkonst;
	double viscosity;
	double absorption;
	double refraction;
};

template<int N>
struct LIQUID
{
	LIQUID<N> derive(double time) const;
	LIQUID<N> operator+(const LIQUID<N> &delta) const;
	LIQUID<N> operator*(double timestep) const;

	LIQUID();

	void init_reset(const LIQUID_PROPERTIES &props);

	bool wasinit() const;
	bool isgone() const;

	void reset_offending_particles();

	double getbounding_radius() const;

	void setposition        (const VECTOR &pos);
	void setlinear_velocity (const VECTOR &vel);
	void setangular_velocity(double angular_vel);

	VECTOR getposition() const;
	VECTOR getlinear_velocity() const;
	double getangular_velocity() const;

	VECTOR getlinear_momentum() const;
	double getangular_momentum() const;
	double getangular_momentum_origin() const;

	double getmass() const;
	double getangular_mass() const;

	void clearforces();
	void addforce (const VECTOR &frc);
	void addforce (const VECTOR &frc,const VECTOR &pos);
	void addtorque(double trq);

	void addheat(double ht);

	void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc);

	void change_origin(const VECTOR &new_origin);

	double gettotal_kinetic_energy() const;
	double getlinear_kinetic_energy() const;
	double getangular_kinetic_energy() const;
	double getinternal_kinetic_energy() const;

	double gettemperature() const;

	void fillclist(COLLISIONLIST &clist,const VECTOR &other_pos,double other_radius) const;

	void setclist(const COLLISIONLIST &clist);

	void integrate(double timestep);

	void draw(double r1,double g1,double b1,double r2,double g2,double b2,double a) const;

  private:

	bool initialized;
	POINTMASS_LIQUID pointmasses[N*N];
	const LIQUID_PROPERTIES *properties;
	double mindist,maxdist;
	double cached_timestep;
	const COLLISIONLIST *clist;
	double heat_to_add;

	void addheat_pointmasses();
};

template<int N>
LIQUID<N> LIQUID<N>::derive(double time) const
{
	LIQUID<N> delta;

	for (int i=0;i<N*N;i++)
	{
		for (int j=0;j<N*N;j++)
		{
			if (i!=j)
			{
				delta.pointmasses[i].velocity+=DVDT::vanderwaals_liquid(pointmasses[i].position,pointmasses[j].position,pointmasses[i].velocity,pointmasses[j].velocity,
                                                                        pointmasses[i].mass,mindist,maxdist,properties->rkonst,properties->akonst,properties->dkonst);
			}
		}
	}

	double density;

	for (int i=0;i<N*N;i++)
	{
		delta.pointmasses[i].position=pointmasses[i].velocity;

		delta.pointmasses[i].velocity+=DVDT::force(pointmasses[i].force,pointmasses[i].mass);

		if (clist)
		{
			delta.pointmasses[i].velocity+=DVDT::friction(pointmasses[i].velocity,pointmasses[i].mass,clist->friction);

			density=pointmasses[i].mass/(math_pi*square(pointmasses[i].sph_radius));

			if (density<math_epsilon)
			{
				density=math_epsilon;
			}

			delta.pointmasses[i].velocity+=DVDT::collision(pointmasses[i].position,*clist,
                                                           pointmasses[i].velocity,delta.pointmasses[i].velocity,
                                                           pointmasses[i].mass,density,
                                                           false,0,0,
                                                           time,cached_timestep);
		}
	}

	return(delta);
}

template<int N>
LIQUID<N> LIQUID<N>::operator+(const LIQUID<N> &delta) const
{
	LIQUID<N> temp;

	for (int i=0;i<N*N;i++)
	{
		temp.pointmasses[i]=pointmasses[i]+delta.pointmasses[i];
	}

	temp.initialized    =initialized;
	temp.properties     =properties;
	temp.mindist        =mindist;
	temp.maxdist        =maxdist;
	temp.cached_timestep=cached_timestep;
	temp.clist          =clist;
	temp.heat_to_add    =heat_to_add;

	return(temp);
}

template<int N>
LIQUID<N> LIQUID<N>::operator*(double timestep) const
{
	LIQUID<N> temp;

	for (int i=0;i<N*N;i++)
	{
		temp.pointmasses[i]=pointmasses[i]*timestep;
	}

	return(temp);
}

template<int N>
LIQUID<N>::LIQUID() : initialized(false) {}

template<int N>
void LIQUID<N>::init_reset(const LIQUID_PROPERTIES &props)
{
	mindist=(props.radius*2)/(N-1);
	maxdist=props.max_radius;

	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].position.x=RANDOM::global.compact_normal(-props.radius,props.radius);
		pointmasses[i].position.y=RANDOM::global.compact_normal(-props.radius,props.radius);
		pointmasses[i].velocity.setzero();
		pointmasses[i].mass=props.mass/(N*N);
		pointmasses[i].force.setzero();
		pointmasses[i].sph_radius=mindist*props.smoothfactor;
	}

	properties=&props;

	cached_timestep=0;

	clist=0;

	heat_to_add=0;

	initialized=true;
}

template<int N>
bool LIQUID<N>::wasinit() const
{
	return(initialized);
}

template<int N>
bool LIQUID<N>::isgone() const
{
	VECTOR pos=getposition();

	for (int i=0;i<N*N;i++)
	{
		if ((pointmasses[i].position-pos).getlength_squared()>square(properties->max_radius))
		{
			return(true);
		}
	}

	return(false);
}

template<int N>
void LIQUID<N>::reset_offending_particles()
{
	VECTOR pos=getposition();
	VECTOR vel=getlinear_velocity();
	double angvelrad=getangular_velocity()*(math_pi/180);

	for (int i=0;i<N*N;i++)
	{
		if ((pointmasses[i].position-pos).getlength_squared()>square(properties->max_radius))
		{
			pointmasses[i].position.x=pos.x+RANDOM::global.compact_normal(-properties->radius,properties->radius);
			pointmasses[i].position.y=pos.y+RANDOM::global.compact_normal(-properties->radius,properties->radius);
			pointmasses[i].velocity=vel+(((pointmasses[i].position-pos).getperpend())*angvelrad);
			pointmasses[i].force.setzero();
		}
	}
}

template<int N>
double LIQUID<N>::getbounding_radius() const
{
	return(properties->max_radius+(mindist*properties->smoothfactor));
}

template<int N>
void LIQUID<N>::setposition        (const VECTOR &pos)
{
	VECTOR opos=getposition();

	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].position+=pos-opos;
		pointmasses[i].velocity.setzero();
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

template<int N>
void LIQUID<N>::setlinear_velocity (const VECTOR &vel)
{
	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].velocity=vel;
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

template<int N>
void LIQUID<N>::setangular_velocity(double angular_vel)
{
	double angvelrad=angular_vel*(math_pi/180);

	VECTOR pos=getposition();

	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].velocity+=((pointmasses[i].position-pos).getperpend())*angvelrad;
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

template<int N>
VECTOR LIQUID<N>::getposition() const
{
	VECTOR pos;

	for (int i=0;i<N*N;i++)
	{
		pos+=pointmasses[i].position;
	}

	pos/=N*N;

	return(pos);
}

template<int N>
VECTOR LIQUID<N>::getlinear_velocity() const
{
	VECTOR vel;

	for (int i=0;i<N*N;i++)
	{
		vel+=pointmasses[i].velocity;
	}

	vel/=N*N;

	return(vel);
}

template<int N>
double LIQUID<N>::getangular_velocity() const
{
	double angvel=0;

	VECTOR pos=getposition();

	VECTOR r;
	double rlen2;

	int num=0;
	for (int i=0;i<N*N;i++)
	{
		r=pointmasses[i].position-pos;
		rlen2=r.getlength_squared();

		if (!equalszero(rlen2))
		{
			angvel+=(r.getcrossproduct(pointmasses[i].velocity))/rlen2;
			num++;
		}
	}

	if (num>0)
	{
		angvel/=num;

		return(angvel*(180/math_pi));
	}

	return(0);
}

template<int N>
VECTOR LIQUID<N>::getlinear_momentum() const
{
	VECTOR result;

	for (int i=0;i<N*N;i++)
	{
		result+=pointmasses[i].velocity*pointmasses[i].mass;
	}

	return(result);
}

template<int N>
double LIQUID<N>::getangular_momentum() const
{
	double result=0;

	VECTOR pos=getposition();

	for (int i=0;i<N*N;i++)
	{
		result+=(pointmasses[i].position-pos).getcrossproduct(pointmasses[i].velocity*pointmasses[i].mass);
	}

	return(result*(180/math_pi));
}

template<int N>
double LIQUID<N>::getangular_momentum_origin() const
{
	double result=0;

	for (int i=0;i<N*N;i++)
	{
		result+=(pointmasses[i].position).getcrossproduct(pointmasses[i].velocity*pointmasses[i].mass);
	}

	return(result*(180/math_pi));
}

template<int N>
double LIQUID<N>::getmass() const
{
	return(properties->mass);
}

template<int N>
double LIQUID<N>::getangular_mass() const
{
	double result=0;

	VECTOR pos=getposition();

	for (int i=0;i<N*N;i++)
	{
		result+=((pointmasses[i].position-pos).getlength_squared())*pointmasses[i].mass;
	}

	return(result);
}

template<int N>
void LIQUID<N>::clearforces()
{
	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

template<int N>
void LIQUID<N>::addforce (const VECTOR &frc)
{
	VECTOR pmfrc=frc/(N*N);

	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].force+=pmfrc;
	}
}

template<int N>
void LIQUID<N>::addforce (const VECTOR &frc,const VECTOR &pos)
{
	addforce(frc);
	addtorque((pos-getposition()).getcrossproduct(frc)*(180/math_pi));
}

template<int N>
void LIQUID<N>::addtorque(double trq)
{
	double trqrad=trq*(math_pi/180);

	VECTOR pos=getposition();

	VECTOR r;
	double rlen;

	for (int i=0;i<N*N;i++)
	{
		r=pointmasses[i].position-pos;
		rlen=r.getlength();

		if (!equalszero(rlen))
		{
			pointmasses[i].force+=((r.getperpend().getnormal())*(trqrad/rlen))/(N*N);
		}
	}
}

template<int N>
void LIQUID<N>::addheat(double ht)
{
	heat_to_add+=ht;
}

template<int N>
void LIQUID<N>::addheat_pointmasses()
{
	double pmht=                 heat_to_add/(N*N);
	double pmke=getinternal_kinetic_energy()/(N*N);

	VECTOR pos=getposition();

	double deltav;

	for (int i=0;i<N*N;i++)
	{
		deltav=squareroot(((pmht*2)/pointmasses[i].mass)+((pmke*2)/pointmasses[i].mass)) - squareroot((pmke*2)/pointmasses[i].mass);

		pointmasses[i].velocity+=(pointmasses[i].position-pos).getnormal()*deltav;
	}
}

template<int N>
void LIQUID<N>::addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
{
	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].force+=-(linear_acc*pointmasses[i].mass);

		pointmasses[i].force+=-((pointmasses[i].velocity.getperpend())*angular_vel*(math_pi/180)*pointmasses[i].mass*2);

		pointmasses[i].force+=(pointmasses[i].position-axle_pos)*square(angular_vel*(math_pi/180))*pointmasses[i].mass;

		pointmasses[i].force+=-(((pointmasses[i].position-axle_pos).getperpend())*angular_acc*(math_pi/180)*pointmasses[i].mass);
	}
}

template<int N>
void LIQUID<N>::change_origin(const VECTOR &new_origin)
{
	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].position-=new_origin;
	}
}

template<int N>
double LIQUID<N>::gettotal_kinetic_energy() const
{
	double result=0;

	for (int i=0;i<N*N;i++)
	{
		result+=(pointmasses[i].velocity.getlength_squared())*pointmasses[i].mass*0.5;
	}

	return(result);
}

template<int N>
double LIQUID<N>::getlinear_kinetic_energy() const
{
	return((getlinear_velocity().getlength_squared())*getmass()*0.5);
}

template<int N>
double LIQUID<N>::getangular_kinetic_energy() const
{
	return(square(getangular_velocity()*(math_pi/180))*getangular_mass()*0.5);
}

template<int N>
double LIQUID<N>::getinternal_kinetic_energy() const
{
	double result=gettotal_kinetic_energy()-getlinear_kinetic_energy()-getangular_kinetic_energy();

	if (result<0)
	{
		result=0;
	}

	return(result);
}

template<int N>
double LIQUID<N>::gettemperature() const
{
	return(getinternal_kinetic_energy()/(N*N));
}

template<int N>
void LIQUID<N>::fillclist(COLLISIONLIST &clist,const VECTOR &other_pos,double other_radius) const
{
	VECTOR pos   =getposition();
	double radius=getbounding_radius();

	if ((other_pos-pos).getlength_squared()<=square((radius*collision_bounding_radius_mul)+(other_radius*collision_bounding_radius_mul)))
	{
		clist.shapes[clist.numshapes].position=pos;
		clist.shapes[clist.numshapes].radius  =radius;

		clist.shapes[clist.numshapes].numtriangles=0;
		clist.shapes[clist.numshapes].numquads    =0;
		clist.shapes[clist.numshapes].numcircles  =N*N;

		VECTOR vel        =getlinear_velocity();
		double angvelrad  =getangular_velocity()*(math_pi/180);
		double temperature=gettemperature();

		for (int i=0;i<N*N;i++)
		{
			clist.shapes[clist.numshapes].circles[i].position  =pointmasses[i].position;
			clist.shapes[clist.numshapes].circles[i].radius    =pointmasses[i].sph_radius;
			clist.shapes[clist.numshapes].circles[i].velocity  =vel+(((pointmasses[i].position-pos).getperpend())*angvelrad);
			clist.shapes[clist.numshapes].circles[i].density   =pointmasses[i].mass/(math_pi*square(pointmasses[i].sph_radius));
			clist.shapes[clist.numshapes].circles[i].pressure  =temperature/(math_pi*square(pointmasses[i].sph_radius));
			clist.shapes[clist.numshapes].circles[i].viscosity =properties->viscosity;
			clist.shapes[clist.numshapes].circles[i].absorption=properties->absorption;
			clist.shapes[clist.numshapes].circles[i].refraction=properties->refraction;
		}

		clist.numshapes++;
	}
}

template<int N>
void LIQUID<N>::setclist(const COLLISIONLIST &clist)
{
	this->clist=&clist;
}

template<int N>
void LIQUID<N>::integrate(double timestep)
{
	cached_timestep=timestep;

	addheat_pointmasses();

	*this=integrate_RK4(*this,timestep);
}

template<int N>
void LIQUID<N>::draw(double r1,double g1,double b1,double r2,double g2,double b2,double a) const
{
	double cmul1;
	double cmul2;

	VIDEO::BATCH::draw_point_fluid_start();

	for (int i=0;i<N*N;i++)
	{
		cmul1=(double)i/((N*N)-1);
		cmul2=1-cmul1;

		VIDEO::BATCH::draw_point_fluid(pointmasses[i].position.x,pointmasses[i].position.y,r1*cmul1 + r2*cmul2,
                                                                                           g1*cmul1 + g2*cmul2,
                                                                                           b1*cmul1 + b2*cmul2,a,pointmasses[i].sph_radius);
	}

	VIDEO::BATCH::draw_point_fluid_end();
}
