//Code by Rogier Walraven. Do not distribute.

struct TRIANGLE
{
	LINE l1,l2,l3;

	VECTOR vel1,vel2,vel3;

	double density;

	double sfriction;
	double dfriction;

	double absorption;
	double refraction;

	TRIANGLE();
	TRIANGLE(const VECTOR &v1,const VECTOR &v2,const VECTOR &v3,bool rdt=false);
	TRIANGLE(const VECTOR &v1,const VECTOR &v2,const VECTOR &v3,double sfric,double dfric,double absorp,double refrac,bool rdt=false);

	TRIANGLE& operator+=(const VECTOR &v);
	TRIANGLE& operator-=(const VECTOR &v);

	TRIANGLE& scale(double sx,double sy);

	TRIANGLE gettransformed_to  (const VECTOR &origin,double angle,const VECTOR &linear_vel,double angular_vel) const;
	TRIANGLE gettransformed_from(const VECTOR &origin,double angle,const VECTOR &linear_vel,double angular_vel) const;

	bool getintersection_seg(VECTOR &i,const LINE &l,double time) const;
	bool getintersection_seg(VECTOR &i,VECTOR &n,const LINE &l,double time) const;

	bool iswithin(const VECTOR &p,double time) const;
	bool iswithin(VECTOR &snorm,VECTOR &svel,const VECTOR &p,double time) const;

	bool getbarycentric_coords(double &c1,double &c2,double &c3,const VECTOR &p,double time) const;

  private:

	VECTOR getnormal(const VECTOR &p,double ct1,double ct2,double ct3,double time) const;
};

inline TRIANGLE::TRIANGLE() : density(1),sfriction(0),dfriction(0),absorption(0),refraction(1) {}

inline TRIANGLE::TRIANGLE(const VECTOR &v1,const VECTOR &v2,const VECTOR &v3,bool rdt) : l1(v1,v2,rdt),l2(v2,v3,rdt),l3(v3,v1,rdt),density(1),sfriction(0),dfriction(0),absorption(0),refraction(1) {}

inline TRIANGLE::TRIANGLE(const VECTOR &v1,const VECTOR &v2,const VECTOR &v3,double sfric,double dfric,double absorp,double refrac,bool rdt) : l1(v1,v2,rdt),l2(v2,v3,rdt),l3(v3,v1,rdt),density(1),sfriction(sfric),dfriction(dfric),absorption(absorp),refraction(refrac) {}

inline TRIANGLE& TRIANGLE::operator+=(const VECTOR &v)
{
	l1+=v;
	l2+=v;
	l3+=v;

	return(*this);
}

inline TRIANGLE& TRIANGLE::operator-=(const VECTOR &v)
{
	l1-=v;
	l2-=v;
	l3-=v;

	return(*this);
}

inline TRIANGLE& TRIANGLE::scale(double sx,double sy)
{
	l1.scale(sx,sy);
	l2.scale(sx,sy);
	l3.scale(sx,sy);

	return(*this);
}

inline TRIANGLE TRIANGLE::gettransformed_to  (const VECTOR &origin,double angle,const VECTOR &linear_vel,double angular_vel) const
{
	VECTOR p1=l1.start.gettransformed_to(origin,angle);
	VECTOR p2=l2.start.gettransformed_to(origin,angle);
	VECTOR p3=l3.start.gettransformed_to(origin,angle);

	VECTOR v1=vel1-linear_vel;
	VECTOR v2=vel2-linear_vel;
	VECTOR v3=vel3-linear_vel;

	double angvelrad=angular_vel*(math_pi/180);

	v1-=((l1.start-origin).getperpend())*angvelrad;
	v2-=((l2.start-origin).getperpend())*angvelrad;
	v3-=((l3.start-origin).getperpend())*angvelrad;

	v1=v1.gettransformed_to(VECTOR(0,0),angle);
	v2=v2.gettransformed_to(VECTOR(0,0),angle);
	v3=v3.gettransformed_to(VECTOR(0,0),angle);

	TRIANGLE triangle(p1,p2,p3);

	triangle.l1.redundant=l1.redundant;
	triangle.l2.redundant=l2.redundant;
	triangle.l3.redundant=l3.redundant;

	triangle.vel1=v1;
	triangle.vel2=v2;
	triangle.vel3=v3;

	triangle.density=density;

	triangle.sfriction=sfriction;
	triangle.dfriction=dfriction;

	triangle.absorption=absorption;
	triangle.refraction=refraction;

	return(triangle);
}

inline TRIANGLE TRIANGLE::gettransformed_from(const VECTOR &origin,double angle,const VECTOR &linear_vel,double angular_vel) const
{
	VECTOR p1=l1.start.gettransformed_from(origin,angle);
	VECTOR p2=l2.start.gettransformed_from(origin,angle);
	VECTOR p3=l3.start.gettransformed_from(origin,angle);

	VECTOR v1=vel1.gettransformed_from(VECTOR(0,0),angle);
	VECTOR v2=vel2.gettransformed_from(VECTOR(0,0),angle);
	VECTOR v3=vel3.gettransformed_from(VECTOR(0,0),angle);

	v1+=linear_vel;
	v2+=linear_vel;
	v3+=linear_vel;

	double angvelrad=angular_vel*(math_pi/180);

	v1+=((p1-origin).getperpend())*angvelrad;
	v2+=((p2-origin).getperpend())*angvelrad;
	v3+=((p3-origin).getperpend())*angvelrad;

	TRIANGLE triangle(p1,p2,p3);

	triangle.l1.redundant=l1.redundant;
	triangle.l2.redundant=l2.redundant;
	triangle.l3.redundant=l3.redundant;

	triangle.vel1=v1;
	triangle.vel2=v2;
	triangle.vel3=v3;

	triangle.density=density;

	triangle.sfriction=sfriction;
	triangle.dfriction=dfriction;

	triangle.absorption=absorption;
	triangle.refraction=refraction;

	return(triangle);
}

inline bool TRIANGLE::getintersection_seg(VECTOR &i,const LINE &l,double time) const
{
	if (l.redundant || (l1.redundant && l2.redundant && l3.redundant))
	{
		return(false);
	}

	LINE lt1=l1;
	LINE lt2=l2;
	LINE lt3=l3;

	lt1.start+=vel1*time;
	lt2.start+=vel2*time;
	lt3.start+=vel3*time;

	lt1.end=lt2.start;
	lt2.end=lt3.start;
	lt3.end=lt1.start;

	bool exists[3];
	VECTOR isect[3];

	exists[0]=lt1.getintersection_seg_seg(isect[0],l);
	exists[1]=lt2.getintersection_seg_seg(isect[1],l);
	exists[2]=lt3.getintersection_seg_seg(isect[2],l);

	int numexisting=0;
	VECTOR existing_isect[3];

	for (int f=0;f<3;f++)
	{
		if (exists[f])
		{
			existing_isect[numexisting]=isect[f];
			numexisting++;
		}
	}

	double d1,d2,d3;

	switch (numexisting)
	{
	case 0:
		return(false);
	break;
	case 1:
		i=existing_isect[0];

		return(true);
	break;
	case 2:
		d1=(existing_isect[0]-l.start).getlength_squared();
		d2=(existing_isect[1]-l.start).getlength_squared();

		if (d1<=d2)
		{
			i=existing_isect[0];

			return(true);
		}

		i=existing_isect[1];

		return(true);
	break;
	case 3:
		d1=(existing_isect[0]-l.start).getlength_squared();
		d2=(existing_isect[1]-l.start).getlength_squared();
		d3=(existing_isect[2]-l.start).getlength_squared();

		if (d1<=d2 && d1<=d3)
		{
			i=existing_isect[0];

			return(true);
		}
		if (d2<=d1 && d2<=d3)
		{
			i=existing_isect[1];

			return(true);
		}
		if (d3<=d1 && d3<=d2)
		{
			i=existing_isect[2];

			return(true);
		}
	break;
	}

	return(false);
}

inline bool TRIANGLE::getintersection_seg(VECTOR &i,VECTOR &n,const LINE &l,double time) const
{
	if (getintersection_seg(i,l,time))
	{
		double c1,c2,c3;

		if (getbarycentric_coords(c1,c2,c3,i,time))
		{
			n=getnormal(i,c1,c2,c3,time);

			return(true);
		}
	}

	return(false);
}

inline bool TRIANGLE::iswithin(const VECTOR &p,double time) const
{
	double c1,c2,c3;

	if (getbarycentric_coords(c1,c2,c3,p,time))
	{
		return(c1>=0 && c2>=0 && c3>=0 && c1<=1 && c2<=1 && c3<=1);
	}

	return(false);
}

inline bool TRIANGLE::iswithin(VECTOR &snorm,VECTOR &svel,const VECTOR &p,double time) const
{
	double c1,c2,c3;

	if (getbarycentric_coords(c1,c2,c3,p,time))
	{
		if (c1>=0 && c2>=0 && c3>=0 && c1<=1 && c2<=1 && c3<=1)
		{
			snorm = getnormal(p,c1,c2,c3,time);
			svel  = vel1*c1 + vel2*c2 + vel3*c3;

			return(true);
		}
	}

	return(false);
}

inline bool TRIANGLE::getbarycentric_coords(double &c1,double &c2,double &c3,const VECTOR &p,double time) const
{
	LINE lt1=l1;
	LINE lt2=l2;
	LINE lt3=l3;

	lt1.start+=vel1*time;
	lt2.start+=vel2*time;
	lt3.start+=vel3*time;

	lt1.end=lt2.start;
	lt2.end=lt3.start;
	lt3.end=lt1.start;

	double a=lt1.start.x - lt3.start.x;
	double b=lt2.start.x - lt3.start.x;
	double c=lt1.start.y - lt3.start.y;
	double d=lt2.start.y - lt3.start.y;

	double e=p.x - lt3.start.x;
	double f=p.y - lt3.start.y;

	double det=a*d - b*c;

	if (!equalszero(det))
	{
		c1=(d*e - b*f)/det;
		c2=(a*f - c*e)/det;
		c3=1-c1-c2;

		return(true);
	}

	return(false);
}

inline VECTOR TRIANGLE::getnormal(const VECTOR &p,double ct1,double ct2,double ct3,double time) const
{
	LINE lt1=l1;
	LINE lt2=l2;
	LINE lt3=l3;

	lt1.start+=vel1*time;
	lt2.start+=vel2*time;
	lt3.start+=vel3*time;

	lt1.end=lt2.start;
	lt2.end=lt3.start;
	lt3.end=lt1.start;

	bool exists[3];
	double dist[3];
	VECTOR norm[3];

	VECTOR center=(lt1.start+lt2.start+lt3.start)/3;

	double cdist1=lt1.getdistance(center).getlength_squared();
	double cdist2=lt2.getdistance(center).getlength_squared();
	double cdist3=lt3.getdistance(center).getlength_squared();

	if (!(lt1.redundant && lt2.redundant && lt3.redundant))
	{
		exists[0]=!lt1.redundant;
		exists[1]=!lt2.redundant;
		exists[2]=!lt3.redundant;
	}
	else
	{
		return(((lt1.start-center)*ct1 + (lt2.start-center)*ct2 + (lt3.start-center)*ct3).getnormal());
	}

	if (exists[0])
	{
		dist[0]=lt1.getdistance(p).getlength_squared();
		norm[0]=-((lt1.end-lt1.start).getperpend().getnormal());

		if (dist[0]>cdist1)
		{
			norm[0]*=-1;
		}
	}

	if (exists[1])
	{
		dist[1]=lt2.getdistance(p).getlength_squared();
		norm[1]=-((lt2.end-lt2.start).getperpend().getnormal());

		if (dist[1]>cdist2)
		{
			norm[1]*=-1;
		}
	}

	if (exists[2])
	{
		dist[2]=lt3.getdistance(p).getlength_squared();
		norm[2]=-((lt3.end-lt3.start).getperpend().getnormal());

		if (dist[2]>cdist3)
		{
			norm[2]*=-1;
		}
	}

	int numexisting=0;
	double existing_dist[3];
	VECTOR existing_norm[3];

	for (int i=0;i<3;i++)
	{
		if (exists[i])
		{
			existing_dist[numexisting]=dist[i];
			existing_norm[numexisting]=norm[i];

			numexisting++;
		}
	}

	switch (numexisting)
	{
	case 0:
		return(VECTOR(0,0));
	break;
	case 1:
		return(existing_norm[0]);
	break;
	case 2:
		if (existing_dist[0]<=existing_dist[1])
		{
			return(existing_norm[0]);
		}

		return(existing_norm[1]);
	break;
	case 3:
		if (existing_dist[0]<=existing_dist[1] && existing_dist[0]<=existing_dist[2])
		{
			return(existing_norm[0]);
		}
		if (existing_dist[1]<=existing_dist[0] && existing_dist[1]<=existing_dist[2])
		{
			return(existing_norm[1]);
		}
		if (existing_dist[2]<=existing_dist[0] && existing_dist[2]<=existing_dist[1])
		{
			return(existing_norm[2]);
		}
	break;
	}

	return(VECTOR(0,0));
}
