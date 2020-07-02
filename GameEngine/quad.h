//Code by Rogier Walraven. Do not distribute.

struct QUAD
{
	LINE l1,l2,l3,l4;

	VECTOR vel1,vel2,vel3,vel4;

	double density;

	double sfriction;
	double dfriction;

	double absorption;
	double refraction;

	QUAD();
	QUAD(const VECTOR &v1,const VECTOR &v2,const VECTOR &v3,const VECTOR &v4,bool rdt=false);
	QUAD(const VECTOR &v1,const VECTOR &v2,const VECTOR &v3,const VECTOR &v4,double sfric,double dfric,double absorp,double refrac,bool rdt=false);

	QUAD& operator+=(const VECTOR &v);
	QUAD& operator-=(const VECTOR &v);

	QUAD& scale(double sx,double sy);

	QUAD gettransformed_to  (const VECTOR &origin,double angle,const VECTOR &linear_vel,double angular_vel) const;
	QUAD gettransformed_from(const VECTOR &origin,double angle,const VECTOR &linear_vel,double angular_vel) const;

	bool getintersection_seg(VECTOR &i,VECTOR &n,const LINE &l,double time) const;

	bool iswithin(const VECTOR &p,double time) const;
	bool iswithin(VECTOR &snorm,VECTOR &svel,const VECTOR &p,double time) const;
};

inline QUAD::QUAD() : density(1),sfriction(0),dfriction(0),absorption(0),refraction(1) {}

inline QUAD::QUAD(const VECTOR &v1,const VECTOR &v2,const VECTOR &v3,const VECTOR &v4,bool rdt) : l1(v1,v2,rdt),l2(v2,v3,rdt),l3(v3,v4,rdt),l4(v4,v1,rdt),density(1),sfriction(0),dfriction(0),absorption(0),refraction(1) {}

inline QUAD::QUAD(const VECTOR &v1,const VECTOR &v2,const VECTOR &v3,const VECTOR &v4,double sfric,double dfric,double absorp,double refrac,bool rdt) : l1(v1,v2,rdt),l2(v2,v3,rdt),l3(v3,v4,rdt),l4(v4,v1,rdt),density(1),sfriction(sfric),dfriction(dfric),absorption(absorp),refraction(refrac) {}

inline QUAD& QUAD::operator+=(const VECTOR &v)
{
	l1+=v;
	l2+=v;
	l3+=v;
	l4+=v;

	return(*this);
}

inline QUAD& QUAD::operator-=(const VECTOR &v)
{
	l1-=v;
	l2-=v;
	l3-=v;
	l4-=v;

	return(*this);
}

inline QUAD& QUAD::scale(double sx,double sy)
{
	l1.scale(sx,sy);
	l2.scale(sx,sy);
	l3.scale(sx,sy);
	l4.scale(sx,sy);

	return(*this);
}

inline QUAD QUAD::gettransformed_to  (const VECTOR &origin,double angle,const VECTOR &linear_vel,double angular_vel) const
{
	TRIANGLE t1(l1.start,l2.start,l3.start);
	TRIANGLE t2(l3.start,l4.start,l1.start);

	t1.vel1=vel1;
	t1.vel2=vel2;
	t1.vel3=vel3;

	t2.vel1=vel3;
	t2.vel2=vel4;
	t2.vel3=vel1;

	t1=t1.gettransformed_to(origin,angle,linear_vel,angular_vel);
	t2=t2.gettransformed_to(origin,angle,linear_vel,angular_vel);

	QUAD quad(t1.l1.start,t1.l2.start,t1.l3.start,t2.l2.start);

	quad.l1.redundant=l1.redundant;
	quad.l2.redundant=l2.redundant;
	quad.l3.redundant=l3.redundant;
	quad.l4.redundant=l4.redundant;

	quad.vel1=t1.vel1;
	quad.vel2=t1.vel2;
	quad.vel3=t1.vel3;
	quad.vel4=t2.vel2;

	quad.density=density;

	quad.sfriction=sfriction;
	quad.dfriction=dfriction;

	quad.absorption=absorption;
	quad.refraction=refraction;

	return(quad);
}

inline QUAD QUAD::gettransformed_from(const VECTOR &origin,double angle,const VECTOR &linear_vel,double angular_vel) const
{
	TRIANGLE t1(l1.start,l2.start,l3.start);
	TRIANGLE t2(l3.start,l4.start,l1.start);

	t1.vel1=vel1;
	t1.vel2=vel2;
	t1.vel3=vel3;

	t2.vel1=vel3;
	t2.vel2=vel4;
	t2.vel3=vel1;

	t1=t1.gettransformed_from(origin,angle,linear_vel,angular_vel);
	t2=t2.gettransformed_from(origin,angle,linear_vel,angular_vel);

	QUAD quad(t1.l1.start,t1.l2.start,t1.l3.start,t2.l2.start);

	quad.l1.redundant=l1.redundant;
	quad.l2.redundant=l2.redundant;
	quad.l3.redundant=l3.redundant;
	quad.l4.redundant=l4.redundant;

	quad.vel1=t1.vel1;
	quad.vel2=t1.vel2;
	quad.vel3=t1.vel3;
	quad.vel4=t2.vel2;

	quad.density=density;

	quad.sfriction=sfriction;
	quad.dfriction=dfriction;

	quad.absorption=absorption;
	quad.refraction=refraction;

	return(quad);
}

inline bool QUAD::getintersection_seg(VECTOR &i,VECTOR &n,const LINE &l,double time) const
{
	if (l.redundant || (l1.redundant && l2.redundant && l3.redundant && l4.redundant))
	{
		return(false);
	}

	TRIANGLE t1(l1.start,l2.start,l3.start);
	TRIANGLE t2(l3.start,l4.start,l1.start);

	t1.l3.redundant=true;
	t2.l3.redundant=true;

	t1.l1.redundant=l1.redundant;
	t1.l2.redundant=l2.redundant;

	t2.l1.redundant=l3.redundant;
	t2.l2.redundant=l4.redundant;

	t1.vel1=vel1;
	t1.vel2=vel2;
	t1.vel3=vel3;

	t2.vel1=vel3;
	t2.vel2=vel4;
	t2.vel3=vel1;

	VECTOR isect1;
	VECTOR isect2;

	VECTOR norm1;
	VECTOR norm2;

	bool exists1=t1.getintersection_seg(isect1,norm1,l,time);
	bool exists2=t2.getintersection_seg(isect2,norm2,l,time);

	if (!exists1 && !exists2)
	{
		return(false);
	}

	if (!exists1 &&  exists2)
	{
		i=isect2;
		n=norm2;

		return(true);
	}

	if ( exists1 && !exists2)
	{
		i=isect1;
		n=norm1;

		return(true);
	}

	double d1=(isect1-l.start).getlength_squared();
	double d2=(isect2-l.start).getlength_squared();

	if (d1<=d2)
	{
		i=isect1;
		n=norm1;

		return(true);
	}

	i=isect2;
	n=norm2;

	return(true);
}

inline bool QUAD::iswithin(const VECTOR &p,double time) const
{
	TRIANGLE t1(l1.start,l2.start,l3.start);
	TRIANGLE t2(l3.start,l4.start,l1.start);

	t1.vel1=vel1;
	t1.vel2=vel2;
	t1.vel3=vel3;

	t2.vel1=vel3;
	t2.vel2=vel4;
	t2.vel3=vel1;

	return(t1.iswithin(p,time) || t2.iswithin(p,time));
}

inline bool QUAD::iswithin(VECTOR &snorm,VECTOR &svel,const VECTOR &p,double time) const
{
	TRIANGLE t1(l1.start,l2.start,l3.start);
	TRIANGLE t2(l3.start,l4.start,l1.start);

	t1.l3.redundant=true;
	t2.l3.redundant=true;

	t1.l1.redundant=l1.redundant;
	t1.l2.redundant=l2.redundant;

	t2.l1.redundant=l3.redundant;
	t2.l2.redundant=l4.redundant;

	t1.vel1=vel1;
	t1.vel2=vel2;
	t1.vel3=vel3;

	t2.vel1=vel3;
	t2.vel2=vel4;
	t2.vel3=vel1;

	return(t1.iswithin(snorm,svel,p,time) || t2.iswithin(snorm,svel,p,time));
}
