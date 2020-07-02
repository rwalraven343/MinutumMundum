//Code by Rogier Walraven. Do not distribute.

struct LINE
{
	VECTOR start,end;

	bool redundant;

	LINE();
	LINE(const VECTOR &s,const VECTOR &e,bool rdt=false);

	double getdeltax() const;
	double getdeltay() const;

	LINE& operator+=(const VECTOR &v);
	LINE& operator-=(const VECTOR &v);

	LINE& scale(double sx,double sy);

	LINE getrotatedroundstart(double angle) const;
	LINE getperpend() const;

	double getlength() const;
	double getlength_squared() const;

	VECTOR getcenter() const;

	VECTOR getdistance(const VECTOR &p) const;

	bool getintersection_seg_seg(VECTOR &i,const LINE &l) const;
	bool getintersection_seg_inf(VECTOR &i,const LINE &l) const;
	bool getintersection_inf_seg(VECTOR &i,const LINE &l) const;
	bool getintersection_inf_inf(VECTOR &i,const LINE &l) const;
};

inline LINE::LINE() : redundant(false) {}

inline LINE::LINE(const VECTOR &s,const VECTOR &e,bool rdt) : start(s),end(e),redundant(rdt) {}

inline double LINE::getdeltax() const
{
	return(end.x-start.x);
}

inline double LINE::getdeltay() const
{
	return(end.y-start.y);
}

inline LINE& LINE::operator+=(const VECTOR &v)
{
	start+=v;
	end  +=v;

	return(*this);
}

inline LINE& LINE::operator-=(const VECTOR &v)
{
	start-=v;
	end  -=v;

	return(*this);
}

inline LINE& LINE::scale(double sx,double sy)
{
	start.x*=sx;
	start.y*=sy;

	end.x*=sx;
	end.y*=sy;

	return(*this);
}

inline LINE LINE::getrotatedroundstart(double angle) const
{
	return(LINE(start,start+((end-start).getrotated(angle)),redundant));
}

inline LINE LINE::getperpend() const
{
	return(LINE(start,start+((end-start).getperpend()),redundant));
}

inline double LINE::getlength() const
{
	return((end-start).getlength());
}

inline double LINE::getlength_squared() const
{
	return((end-start).getlength_squared());
}

inline VECTOR LINE::getcenter() const
{
	return((start+end)/2);
}

inline VECTOR LINE::getdistance(const VECTOR &p) const
{
	LINE l1=*this;
	l1.redundant=false;

	LINE l2=l1.getperpend();
	l2+=p-l1.start;

	VECTOR i;

	if (l1.getintersection_seg_inf(i,l2))
	{
		return(i-p);
	}

	VECTOR v1=l1.start-p;
	VECTOR v2=l1.end-p;

	if (v1.getlength_squared()<=v2.getlength_squared())
	{
		return(v1);
	}

	return(v2);
}

inline bool LINE::getintersection_seg_seg(VECTOR &i,const LINE &l) const
{
	if (redundant || l.redundant)
	{
		return(false);
	}

	if (getintersection_inf_inf(i,l))
	{
		VECTOR m1(getdeltax()/2,getdeltay()/2);
		VECTOR m2(l.getdeltax()/2,l.getdeltay()/2);

		LINE l1(start+m1,i);
		LINE l2(l.start+m2,i);

		if (l1.getlength_squared()<=m1.getlength_squared() && l2.getlength_squared()<=m2.getlength_squared())
		{
			return(true);
		}
	}

	return(false);
}

inline bool LINE::getintersection_seg_inf(VECTOR &i,const LINE &l) const
{
	if (redundant || l.redundant)
	{
		return(false);
	}

	if (getintersection_inf_inf(i,l))
	{
		VECTOR m1(getdeltax()/2,getdeltay()/2);

		LINE l1(start+m1,i);

		if (l1.getlength_squared()<=m1.getlength_squared())
		{
			return(true);
		}
	}

	return(false);
}

inline bool LINE::getintersection_inf_seg(VECTOR &i,const LINE &l) const
{
	if (redundant || l.redundant)
	{
		return(false);
	}

	if (getintersection_inf_inf(i,l))
	{
		VECTOR m2(l.getdeltax()/2,l.getdeltay()/2);

		LINE l2(l.start+m2,i);

		if (l2.getlength_squared()<=m2.getlength_squared())
		{
			return(true);
		}
	}

	return(false);
}

inline bool LINE::getintersection_inf_inf(VECTOR &i,const LINE &l) const
{
	if (redundant || l.redundant)
	{
		return(false);
	}

	double a1,a2,b1,b2;

	if (!equalszero(getdeltax()))
	{
		if (!equalszero(l.getdeltax()))
		{
			a1=getdeltay()/getdeltax();
			b1=start.y - a1*start.x;

			a2=l.getdeltay()/l.getdeltax();
			b2=l.start.y - a2*l.start.x;

			if (!equalszero(a1-a2))
			{
				i.x=(b2-b1)/(a1-a2);
				i.y=a1*i.x + b1;

				return(true);
			}

			return(false);
		}

		if (!equalszero(l.getdeltay()))
		{
			a1=getdeltay()/getdeltax();
			b1=start.y - a1*start.x;

			i.x=l.start.x;
			i.y=a1*i.x + b1;

			return(true);
		}

		return(false);
	}

	if (!equalszero(getdeltay()))
	{
		if (!equalszero(l.getdeltax()))
		{
			a2=l.getdeltay()/l.getdeltax();
			b2=l.start.y - a2*l.start.x;

			i.x=start.x;
			i.y=a2*i.x + b2;

			return(true);
		}
	}

	return(false);
}
