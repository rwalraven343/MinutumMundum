//Code by Rogier Walraven. Do not distribute.

struct SOLID_PROPERTIES
{
	double width;
	double height;
	double mass;
	double skonst;
	double dkonst;
	double sfriction;
	double dfriction;
	double absorption;
	double refraction;
};

template<int N>
struct SOLID
{
	SOLID<N> derive(double time) const;
	SOLID<N> operator+(const SOLID<N> &delta) const;
	SOLID<N> operator*(double timestep) const;

	SOLID();

	void init_reset(const SOLID_PROPERTIES &props);

	bool wasinit() const;
	bool isgone() const;

	double getwidth() const;
	double getheight() const;

	double getbounding_radius() const;

	void setposition        (const VECTOR &pos);
	void setorientation     (double angle);
	void setlinear_velocity (const VECTOR &vel);
	void setangular_velocity(double angular_vel);

	VECTOR getposition() const;
	double getorientation() const;
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

	QUAD getquad() const;

	void fillclist(COLLISIONLIST &clist,const VECTOR &other_pos,double other_radius) const;

	void setclist(const COLLISIONLIST &clist);

	void integrate(double timestep);

	void draw(VIDEO::TEXTURE texture,double alpha) const;

  private:

	bool initialized;
	POINTMASS_SOLID pointmasses[N*N];
	const SOLID_PROPERTIES *properties;
	double restlx,restly,restld;
	double density;
	double cached_timestep;
	const COLLISIONLIST *clist;
	double heat_to_add;

	void addheat_pointmasses();

	void draw_polygon(const POINTMASS_SOLID &a,const POINTMASS_SOLID &b,const POINTMASS_SOLID &c,
                      double tsx,double tex,double tsy,double tey,double alpha) const;
};

template<int N>
SOLID<N> SOLID<N>::derive(double time) const
{
	SOLID<N> delta;

	int i1,i2;
	VECTOR dvdt_a,dvdt_b;

	for (int y=0;y<N-1;y++)
	{
		for (int x=0;x<N-1;x++)
		{
			i1=(x  )+(y  )*N;
			i2=(x+1)+(y  )*N;
			DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                       pointmasses[i1].velocity,pointmasses[i2].velocity,
                                       pointmasses[i1].mass,    pointmasses[i2].mass,
                                       restlx,properties->skonst,properties->dkonst);
			delta.pointmasses[i1].velocity+=dvdt_a;
			delta.pointmasses[i2].velocity+=dvdt_b;

			i1=(x  )+(y  )*N;
			i2=(x  )+(y+1)*N;
			DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                       pointmasses[i1].velocity,pointmasses[i2].velocity,
                                       pointmasses[i1].mass,    pointmasses[i2].mass,
                                       restly,properties->skonst,properties->dkonst);
			delta.pointmasses[i1].velocity+=dvdt_a;
			delta.pointmasses[i2].velocity+=dvdt_b;

			i1=(x  )+(y  )*N;
			i2=(x+1)+(y+1)*N;
			DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                       pointmasses[i1].velocity,pointmasses[i2].velocity,
                                       pointmasses[i1].mass,    pointmasses[i2].mass,
                                       restld,properties->skonst,properties->dkonst);
			delta.pointmasses[i1].velocity+=dvdt_a;
			delta.pointmasses[i2].velocity+=dvdt_b;

			i1=(x+1)+(y  )*N;
			i2=(x  )+(y+1)*N;
			DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                       pointmasses[i1].velocity,pointmasses[i2].velocity,
                                       pointmasses[i1].mass,    pointmasses[i2].mass,
                                       restld,properties->skonst,properties->dkonst);
			delta.pointmasses[i1].velocity+=dvdt_a;
			delta.pointmasses[i2].velocity+=dvdt_b;
		}
	}

	for (int i=0;i<N-1;i++)
	{
		i1=(i  )+(N-1)*N;
		i2=(i+1)+(N-1)*N;
		DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                   pointmasses[i1].velocity,pointmasses[i2].velocity,
                                   pointmasses[i1].mass,    pointmasses[i2].mass,
                                   restlx,properties->skonst,properties->dkonst);
		delta.pointmasses[i1].velocity+=dvdt_a;
		delta.pointmasses[i2].velocity+=dvdt_b;

		i1=(N-1)+(i  )*N;
		i2=(N-1)+(i+1)*N;
		DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                   pointmasses[i1].velocity,pointmasses[i2].velocity,
                                   pointmasses[i1].mass,    pointmasses[i2].mass,
                                   restly,properties->skonst,properties->dkonst);
		delta.pointmasses[i1].velocity+=dvdt_a;
		delta.pointmasses[i2].velocity+=dvdt_b;
	}

	for (int i=0;i<N*N;i++)
	{
		delta.pointmasses[i].position=pointmasses[i].velocity;

		delta.pointmasses[i].velocity+=DVDT::force(pointmasses[i].force,pointmasses[i].mass);

		if (clist)
		{
			delta.pointmasses[i].velocity+=DVDT::friction(pointmasses[i].velocity,pointmasses[i].mass,clist->friction);

			delta.pointmasses[i].velocity+=DVDT::collision(pointmasses[i].position,*clist,
                                                           pointmasses[i].velocity,delta.pointmasses[i].velocity,
                                                           pointmasses[i].mass,density,
                                                           true,properties->sfriction,properties->dfriction,
                                                           time,cached_timestep);
		}
	}

	return(delta);
}

template<int N>
SOLID<N> SOLID<N>::operator+(const SOLID<N> &delta) const
{
	SOLID<N> temp;

	for (int i=0;i<N*N;i++)
	{
		temp.pointmasses[i]=pointmasses[i]+delta.pointmasses[i];
	}

	temp.initialized    =initialized;
	temp.properties     =properties;
	temp.restlx         =restlx;
	temp.restly         =restly;
	temp.restld         =restld;
	temp.density        =density;
	temp.cached_timestep=cached_timestep;
	temp.clist          =clist;
	temp.heat_to_add    =heat_to_add;

	return(temp);
}

template<int N>
SOLID<N> SOLID<N>::operator*(double timestep) const
{
	SOLID<N> temp;

	for (int i=0;i<N*N;i++)
	{
		temp.pointmasses[i]=pointmasses[i]*timestep;
	}

	return(temp);
}

template<int N>
SOLID<N>::SOLID() : initialized(false) {}

template<int N>
void SOLID<N>::init_reset(const SOLID_PROPERTIES &props)
{
	restlx=props.width/(N-1);
	restly=props.height/(N-1);
	restld=squareroot(square(restlx)+square(restly));

	int i;
	for (int y=0;y<N;y++)
	{
		for (int x=0;x<N;x++)
		{
			i=x+y*N;

			pointmasses[i].position.x=x*restlx;
			pointmasses[i].position.y=y*restly;
			pointmasses[i].velocity.setzero();
			pointmasses[i].mass=props.mass/(N*N);
			pointmasses[i].force.setzero();
		}
	}

	properties=&props;

	density=props.mass/(props.width*props.height);

	cached_timestep=0;

	clist=0;

	heat_to_add=0;

	initialized=true;
}

template<int N>
bool SOLID<N>::wasinit() const
{
	return(initialized);
}

template<int N>
bool SOLID<N>::isgone() const
{
	VECTOR pos=getposition();
	double ang=getorientation();

	pos+=VECTOR(-getwidth()/2,-getheight()/2).getrotated(ang);

	VECTOR xaxis=VECTOR(1,0).getrotated(ang);
	VECTOR yaxis=VECTOR(0,1).getrotated(ang);

	VECTOR refpos;

	int i;
	for (int y=0;y<N;y++)
	{
		for (int x=0;x<N;x++)
		{
			i=x+y*N;

			refpos=pos+(xaxis*(x*restlx))+(yaxis*(y*restly));

			if (absolute((pointmasses[i].position-refpos).getdotproduct(xaxis))>=restlx ||
				absolute((pointmasses[i].position-refpos).getdotproduct(yaxis))>=restly)
			{
				return(true);
			}
		}
	}

	return(false);
}

template<int N>
double SOLID<N>::getwidth() const
{
	return(properties->width);
}

template<int N>
double SOLID<N>::getheight() const
{
	return(properties->height);
}

template<int N>
double SOLID<N>::getbounding_radius() const
{
	return(squareroot(square(properties->width)+square(properties->height))/2);
}

template<int N>
void SOLID<N>::setposition        (const VECTOR &pos)
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
void SOLID<N>::setorientation     (double angle)
{
	VECTOR pos=getposition();

	VECTOR start(pos.x-(properties->width/2),pos.y-(properties->height/2));

	int i;
	for (int y=0;y<N;y++)
	{
		for (int x=0;x<N;x++)
		{
			i=x+y*N;

			pointmasses[i].position.x=start.x+(x*restlx);
			pointmasses[i].position.y=start.y+(y*restly);
			pointmasses[i].position=LINE(pos,pointmasses[i].position).getrotatedroundstart(angle).end;
			pointmasses[i].velocity.setzero();
			pointmasses[i].force.setzero();
		}
	}

	heat_to_add=0;
}

template<int N>
void SOLID<N>::setlinear_velocity (const VECTOR &vel)
{
	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].velocity=vel;
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

template<int N>
void SOLID<N>::setangular_velocity(double angular_vel)
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
VECTOR SOLID<N>::getposition() const
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
double SOLID<N>::getorientation() const
{
	VECTOR start,end;

	for (int y=0;y<N-1;y++)
	{
		for (int x=0;x<N;x++)
		{
			start+=pointmasses[x+(y+1)*N].position;
			end  +=pointmasses[x+(y  )*N].position;
		}
	}

	LINE line;

	for (int y=0;y<N;y++)
	{
		for (int x=0;x<N-1;x++)
		{
			line.start+=pointmasses[(x+1)+y*N].position;
			line.end  +=pointmasses[(x  )+y*N].position;
		}
	}

	line=line.getrotatedroundstart(90);

	return(VECTOR(0,-1).getanglewith((end+line.end)-(start+line.start)));
}

template<int N>
VECTOR SOLID<N>::getlinear_velocity() const
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
double SOLID<N>::getangular_velocity() const
{
	double angvel=0;

	VECTOR pos=getposition();

	VECTOR r;
	double rlen2;

	int num=0;
	for (int i=0;i<N*N;i++)
	{
		if ( !((N%2)!=0 && i==(((N*N)-1)/2)) )
		{
			r=pointmasses[i].position-pos;
			rlen2=r.getlength_squared();

			if (!equalszero(rlen2))
			{
				angvel+=(r.getcrossproduct(pointmasses[i].velocity))/rlen2;
				num++;
			}
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
VECTOR SOLID<N>::getlinear_momentum() const
{
	VECTOR result;

	for (int i=0;i<N*N;i++)
	{
		result+=pointmasses[i].velocity*pointmasses[i].mass;
	}

	return(result);
}

template<int N>
double SOLID<N>::getangular_momentum() const
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
double SOLID<N>::getangular_momentum_origin() const
{
	double result=0;

	for (int i=0;i<N*N;i++)
	{
		result+=(pointmasses[i].position).getcrossproduct(pointmasses[i].velocity*pointmasses[i].mass);
	}

	return(result*(180/math_pi));
}

template<int N>
double SOLID<N>::getmass() const
{
	return(properties->mass);
}

template<int N>
double SOLID<N>::getangular_mass() const
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
void SOLID<N>::clearforces()
{
	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

template<int N>
void SOLID<N>::addforce (const VECTOR &frc)
{
	VECTOR pmfrc=frc/(N*N);

	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].force+=pmfrc;
	}
}

template<int N>
void SOLID<N>::addforce (const VECTOR &frc,const VECTOR &pos)
{
	addforce(frc);
	addtorque((pos-getposition()).getcrossproduct(frc)*(180/math_pi));
}

template<int N>
void SOLID<N>::addtorque(double trq)
{
	double trqrad=trq*(math_pi/180);

	VECTOR pos=getposition();

	VECTOR r;
	double rlen;

	for (int i=0;i<N*N;i++)
	{
		if ( !((N%2)!=0 && i==(((N*N)-1)/2)) )
		{
			r=pointmasses[i].position-pos;
			rlen=r.getlength();

			if (!equalszero(rlen))
			{
				pointmasses[i].force+=((r.getperpend().getnormal())*(trqrad/rlen))/((N*N)-(N%2));
			}
		}
	}
}

template<int N>
void SOLID<N>::addheat(double ht)
{
	heat_to_add+=ht;
}

template<int N>
void SOLID<N>::addheat_pointmasses()
{
	double pmht=                 heat_to_add/((N*N)-(N%2));
	double pmke=getinternal_kinetic_energy()/((N*N)-(N%2));

	VECTOR pos=getposition();

	double deltav;

	for (int i=0;i<N*N;i++)
	{
		if ( !((N%2)!=0 && i==(((N*N)-1)/2)) )
		{
			deltav=squareroot(((pmht*2)/pointmasses[i].mass)+((pmke*2)/pointmasses[i].mass)) - squareroot((pmke*2)/pointmasses[i].mass);

			pointmasses[i].velocity+=(pointmasses[i].position-pos).getnormal()*deltav;
		}
	}
}

template<int N>
void SOLID<N>::addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
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
void SOLID<N>::change_origin(const VECTOR &new_origin)
{
	for (int i=0;i<N*N;i++)
	{
		pointmasses[i].position-=new_origin;
	}
}

template<int N>
double SOLID<N>::gettotal_kinetic_energy() const
{
	double result=0;

	for (int i=0;i<N*N;i++)
	{
		result+=(pointmasses[i].velocity.getlength_squared())*pointmasses[i].mass*0.5;
	}

	return(result);
}

template<int N>
double SOLID<N>::getlinear_kinetic_energy() const
{
	return((getlinear_velocity().getlength_squared())*getmass()*0.5);
}

template<int N>
double SOLID<N>::getangular_kinetic_energy() const
{
	return(square(getangular_velocity()*(math_pi/180))*getangular_mass()*0.5);
}

template<int N>
double SOLID<N>::getinternal_kinetic_energy() const
{
	double result=gettotal_kinetic_energy()-getlinear_kinetic_energy()-getangular_kinetic_energy();

	if (result<0)
	{
		result=0;
	}

	return(result);
}

template<int N>
double SOLID<N>::gettemperature() const
{
	return(getinternal_kinetic_energy()/(N*N));
}

template<int N>
QUAD SOLID<N>::getquad() const
{
	double width    =getwidth();
	double height   =getheight();
	VECTOR pos      =getposition();
	double ang      =getorientation();
	VECTOR vel      =getlinear_velocity();
	double angvelrad=getangular_velocity()*(math_pi/180);

	VECTOR c1=pos + VECTOR(-width/2,-height/2).getrotated(ang);
	VECTOR c2=pos + VECTOR( width/2,-height/2).getrotated(ang);
	VECTOR c3=pos + VECTOR( width/2, height/2).getrotated(ang);
	VECTOR c4=pos + VECTOR(-width/2, height/2).getrotated(ang);

	QUAD quad(c1,c2,c3,c4);

	quad.vel1=vel;
	quad.vel2=vel;
	quad.vel3=vel;
	quad.vel4=vel;

	quad.vel1+=((quad.l1.start-pos).getperpend())*angvelrad;
	quad.vel2+=((quad.l2.start-pos).getperpend())*angvelrad;
	quad.vel3+=((quad.l3.start-pos).getperpend())*angvelrad;
	quad.vel4+=((quad.l4.start-pos).getperpend())*angvelrad;

	quad.density=density;

	quad.sfriction=properties->sfriction;
	quad.dfriction=properties->dfriction;

	quad.absorption=properties->absorption;
	quad.refraction=properties->refraction;

	return(quad);
}

template<int N>
void SOLID<N>::fillclist(COLLISIONLIST &clist,const VECTOR &other_pos,double other_radius) const
{
	VECTOR pos   =getposition();
	double radius=getbounding_radius();

	if ((other_pos-pos).getlength_squared()<=square((radius*collision_bounding_radius_mul)+(other_radius*collision_bounding_radius_mul)))
	{
		clist.shapes[clist.numshapes].position=pos;
		clist.shapes[clist.numshapes].radius  =radius;

		clist.shapes[clist.numshapes].numtriangles=0;
		clist.shapes[clist.numshapes].numquads    =1;
		clist.shapes[clist.numshapes].numcircles  =0;

		clist.shapes[clist.numshapes].quads[0]=getquad();

		clist.numshapes++;
	}
}

template<int N>
void SOLID<N>::setclist(const COLLISIONLIST &clist)
{
	this->clist=&clist;
}

template<int N>
void SOLID<N>::integrate(double timestep)
{
	cached_timestep=timestep;

	addheat_pointmasses();

	*this=integrate_RK4(*this,timestep);
}

template<int N>
void SOLID<N>::draw(VIDEO::TEXTURE texture,double alpha) const
{
	VIDEO::BATCH::draw_polygon_texture_start(texture);

	for (int y=0;y<N-1;y++)
	{
		for (int x=0;x<N-1;x++)
		{
			draw_polygon(pointmasses[(x  )+((y  )*N)],
                         pointmasses[(x+1)+((y  )*N)],
                         pointmasses[(x+1)+((y+1)*N)],
                         ((double)x  )/(N-1),
                         ((double)x+1)/(N-1),
                         ((double)y  )/(N-1),
                         ((double)y+1)/(N-1),alpha);
		}
	}

	for (int y=0;y<N-1;y++)
	{
		for (int x=0;x<N-1;x++)
		{
			draw_polygon(pointmasses[(x+1)+((y+1)*N)],
                         pointmasses[(x  )+((y+1)*N)],
                         pointmasses[(x  )+((y  )*N)],
                         ((double)x+1)/(N-1),
                         ((double)x  )/(N-1),
                         ((double)y+1)/(N-1),
                         ((double)y  )/(N-1),alpha);
		}
	}

	VIDEO::BATCH::draw_polygon_texture_end();
}

template<int N>
inline void SOLID<N>::draw_polygon(const POINTMASS_SOLID &a,const POINTMASS_SOLID &b,const POINTMASS_SOLID &c,
                                   double tsx,double tex,double tsy,double tey,double alpha) const
{
	VIDEO::BATCH::draw_polygon_texture(a.position.x,a.position.y,b.position.x,b.position.y,c.position.x,c.position.y,
                                       tsx,tex,tsy,tey,alpha);
}
