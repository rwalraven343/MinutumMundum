//Code by Rogier Walraven. Do not distribute.

struct CIRCLE
{
	VECTOR position;
	double radius;

	VECTOR velocity;
	double density;
	double pressure;
	double viscosity;

	double absorption;
	double refraction;

	CIRCLE();
	CIRCLE(const VECTOR &p,double r);

	CIRCLE& operator+=(const VECTOR &v);
	CIRCLE& operator-=(const VECTOR &v);

	bool getintersection_seg                              (VECTOR &i,const LINE &l,double time) const;
	bool getintersection_seg(bool &b1,bool &b2,VECTOR &i1,VECTOR &i2,const LINE &l,double time) const;
	bool getintersection_inf                  (VECTOR &i1,VECTOR &i2,const LINE &l,double time) const;

	bool getintersection_seg(VECTOR &i,VECTOR &n,const LINE &l,double time) const;

	bool iswithin(const VECTOR &p,double time) const;
	bool iswithin(double &fdens,VECTOR &fpress_diff,double &fvisc,const VECTOR &p,double time) const;
};

inline CIRCLE::CIRCLE() : radius(0),density(1),pressure(0),viscosity(0),absorption(0),refraction(1) {}

inline CIRCLE::CIRCLE(const VECTOR &p,double r) : position(p),radius(r),density(1),pressure(0),viscosity(0),absorption(0),refraction(1) {}

inline CIRCLE& CIRCLE::operator+=(const VECTOR &v)
{
	position+=v;

	return(*this);
}

inline CIRCLE& CIRCLE::operator-=(const VECTOR &v)
{
	position-=v;

	return(*this);
}

inline bool CIRCLE::getintersection_seg(VECTOR &i,const LINE &l,double time) const
{
	if (l.redundant)
	{
		return(false);
	}

	VECTOR isect1;
	VECTOR isect2;

	bool exists1;
	bool exists2;

	if (!getintersection_seg(exists1,exists2,isect1,isect2,l,time))
	{
		return(false);
	}

	if ( exists1 && !exists2)
	{
		i=isect1;

		return(true);
	}

	if (!exists1 &&  exists2)
	{
		i=isect2;

		return(true);
	}

	double d1=(isect1-l.start).getlength_squared();
	double d2=(isect2-l.start).getlength_squared();

	if (d1<=d2)
	{
		i=isect1;

		return(true);
	}

	i=isect2;

	return(true);
}

inline bool CIRCLE::getintersection_seg(bool &b1,bool &b2,VECTOR &i1,VECTOR &i2,const LINE &l,double time) const
{
	if (l.redundant)
	{
		return(false);
	}

	b1=false;
	b2=false;

	if (getintersection_inf(i1,i2,l,time))
	{
		VECTOR m(l.getdeltax()/2,l.getdeltay()/2);

		double mlen=m.getlength_squared();

		LINE l1(l.start+m,i1);
		LINE l2(l.start+m,i2);

		if (l1.getlength_squared()<=mlen)
		{
			b1=true;
		}

		if (l2.getlength_squared()<=mlen)
		{
			b2=true;
		}
	}

	return(b1 || b2);
}

inline bool CIRCLE::getintersection_inf(VECTOR &i1,VECTOR &i2,const LINE &l,double time) const
{
	if (l.redundant || radius<math_epsilon)
	{
		return(false);
	}

	double len=l.getlength_squared();

	if (equalszero(len))
	{
		return(false);
	}

	VECTOR tpos=position + velocity*time;

	LINE lt=l;
	lt-=tpos;

	double dx=lt.getdeltax();
	double dy=lt.getdeltay();

	double det=lt.start.x*lt.end.y - lt.end.x*lt.start.y;

	double dis=len*square(radius) - square(det);

	if (dis>=0)
	{
		i1.x=( dy*det +     sign(dy)*dx*squareroot(dis))/len;
		i1.y=(-dx*det + absolute(dy)   *squareroot(dis))/len;

		i2.x=( dy*det -     sign(dy)*dx*squareroot(dis))/len;
		i2.y=(-dx*det - absolute(dy)   *squareroot(dis))/len;

		i1+=tpos;
		i2+=tpos;

		return(true);
	}

	return(false);
}

inline bool CIRCLE::getintersection_seg(VECTOR &i,VECTOR &n,const LINE &l,double time) const
{
	if (getintersection_seg(i,l,time))
	{
		n=((i-(velocity*time))-position).getnormal();

		return(true);
	}

	return(false);
}

inline bool CIRCLE::iswithin(const VECTOR &p,double time) const
{
	if (radius>=math_epsilon)
	{
		return(((p-(velocity*time))-position).getlength_squared()<=square(radius));
	}

	return(false);
}

inline bool CIRCLE::iswithin(double &fdens,VECTOR &fpress_diff,double &fvisc,const VECTOR &p,double time) const
{
	if (radius>=math_epsilon)
	{
		VECTOR pt=p-(velocity*time);

		VECTOR distance=pt-position;

		if (distance.getlength_squared()<=square(radius))
		{
			fdens       = (density                      ) * (math_pi*square(radius)*sph_kernel         (pt.x,pt.y,position.x,position.y,radius));
			fpress_diff = (distance.getnormal()*pressure) * (math_pi*square(radius)*sph_kernel_grad    (pt.x,pt.y,position.x,position.y,radius));
			fvisc       = (viscosity                    ) * (math_pi*square(radius)*sph_kernel_div_grad(pt.x,pt.y,position.x,position.y,radius));

			return(true);
		}
	}

	return(false);
}
