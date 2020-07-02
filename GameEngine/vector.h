//Code by Rogier Walraven. Do not distribute.

struct VECTOR
{
	double x,y;

	VECTOR();
	VECTOR(double px,double py);

	void setzero();

	VECTOR operator+(const VECTOR &v) const;
	VECTOR operator-(const VECTOR &v) const;

	VECTOR& operator+=(const VECTOR &v);
	VECTOR& operator-=(const VECTOR &v);

	VECTOR operator-() const;

	VECTOR operator*(double s) const;
	VECTOR operator/(double s) const;

	VECTOR& operator*=(double s);
	VECTOR& operator/=(double s);

	double getdotproduct           (const VECTOR &v) const;
	double getdotproduct_normalized(const VECTOR &v) const;

	double getcrossproduct(const VECTOR &v) const;

	VECTOR getperpend() const;

	double getlength() const;
	double getlength_squared() const;

	VECTOR getnormal() const;

	VECTOR getprojectedon(const VECTOR &v) const;

	VECTOR getrotated(double angle) const;

	double getanglewith(const VECTOR &v) const;

	VECTOR gettransformed_to  (const VECTOR &origin,double angle) const;
	VECTOR gettransformed_from(const VECTOR &origin,double angle) const;
};

inline VECTOR::VECTOR() : x(0),y(0) {}

inline VECTOR::VECTOR(double px,double py) : x(px),y(py) {}

inline void VECTOR::setzero()
{
	x=0;
	y=0;
}

inline VECTOR VECTOR::operator+(const VECTOR &v) const
{
	return(VECTOR(x+v.x,y+v.y));
}

inline VECTOR VECTOR::operator-(const VECTOR &v) const
{
	return(VECTOR(x-v.x,y-v.y));
}

inline VECTOR& VECTOR::operator+=(const VECTOR &v)
{
	x+=v.x;
	y+=v.y;

	return(*this);
}

inline VECTOR& VECTOR::operator-=(const VECTOR &v)
{
	x-=v.x;
	y-=v.y;

	return(*this);
}

inline VECTOR VECTOR::operator-() const
{
	return(VECTOR(-x,-y));
}

inline VECTOR VECTOR::operator*(double s) const
{
	return(VECTOR(x*s,y*s));
}

inline VECTOR VECTOR::operator/(double s) const
{
	return(VECTOR(x/s,y/s));
}

inline VECTOR& VECTOR::operator*=(double s)
{
	x*=s;
	y*=s;

	return(*this);
}

inline VECTOR& VECTOR::operator/=(double s)
{
	x/=s;
	y/=s;

	return(*this);
}

inline double VECTOR::getdotproduct           (const VECTOR &v) const
{
	return(x*v.x + y*v.y);
}

inline double VECTOR::getdotproduct_normalized(const VECTOR &v) const
{
	VECTOR n1=getnormal();
	VECTOR n2=v.getnormal();

	return(n1.x*n2.x + n1.y*n2.y);
}

inline double VECTOR::getcrossproduct(const VECTOR &v) const
{
	return(x*v.y - y*v.x);
}

inline VECTOR VECTOR::getperpend() const
{
	return(VECTOR(-y,x));
}

inline double VECTOR::getlength() const
{
	return(squareroot(x*x + y*y));
}

inline double VECTOR::getlength_squared() const
{
	return(x*x + y*y);
}

inline VECTOR VECTOR::getnormal() const
{
	double l=getlength();

	if (l<math_epsilon)
	{
		l=math_epsilon;
	}

	return(VECTOR(x/l,y/l));
}

inline VECTOR VECTOR::getprojectedon(const VECTOR &v) const
{
	return(v.getnormal()*(getlength()*getdotproduct_normalized(v)));
}

inline VECTOR VECTOR::getrotated(double angle) const
{
	double rad=(math_pi/180)*angle;

	double sinrad=sine(rad);
	double cosrad=cosine(rad);

	return(VECTOR(x*cosrad-y*sinrad,y*cosrad+x*sinrad));
}

inline double VECTOR::getanglewith(const VECTOR &v) const
{
	double dp=getdotproduct_normalized(v);

	if (dp>1) {dp=1;}
	if (dp<-1) {dp=-1;}

	if (getdotproduct(v.getperpend())>0)
	{
		return((-invcosine(dp))*(180/math_pi));
	}

	return(invcosine(dp)*(180/math_pi));
}

inline VECTOR VECTOR::gettransformed_to  (const VECTOR &origin,double angle) const
{
	VECTOR transl=*this-origin;

	VECTOR xaxis=VECTOR(1,0).getrotated(angle);
	VECTOR yaxis=VECTOR(0,1).getrotated(angle);

	return(VECTOR(transl.getdotproduct(xaxis),transl.getdotproduct(yaxis)));
}

inline VECTOR VECTOR::gettransformed_from(const VECTOR &origin,double angle) const
{
	VECTOR xaxis=VECTOR(1,0).getrotated(angle);
	VECTOR yaxis=VECTOR(0,1).getrotated(angle);

	return(origin+(xaxis*x)+(yaxis*y));
}
