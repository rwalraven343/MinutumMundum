//Code by Rogier Walraven. Do not distribute.

struct QUATERNION
{
	double real;
	VECTOR<3> imag;

	QUATERNION();
	QUATERNION(double r,const VECTOR<3> &i);
	QUATERNION(double a,double b,double c,double d);

    void setzero();
    void setangleaxis(double angle,const VECTOR<3> &axis);

	bool iszero() const;
    bool isnormal() const;

    QUATERNION operator+(const QUATERNION &q) const;
    QUATERNION& operator+=(const QUATERNION &q);

    QUATERNION operator-(const QUATERNION &q) const;
    QUATERNION& operator-=(const QUATERNION &q);

    QUATERNION operator-() const;

    QUATERNION operator*(const QUATERNION &q) const;
    QUATERNION& operator*=(const QUATERNION &q);

    QUATERNION operator*(double s) const;
    QUATERNION& operator*=(double s);

    QUATERNION operator/(double s) const;
    QUATERNION& operator/=(double s);

    double getdotproduct(const QUATERNION &q) const;

    QUATERNION getinverse() const;
    void setinverse();

    QUATERNION getconjugate() const;
    void setconjugate();

    double getlength() const;

    QUATERNION getnormal() const;
    void setnormal();

    MATRIX<3> getrotationmatrix() const;
};

inline QUATERNION::QUATERNION() : real(0) {}

inline QUATERNION::QUATERNION(double r,const VECTOR<3> &i) : real(r),imag(i) {}

inline QUATERNION::QUATERNION(double a,double b,double c,double d)
{
	real=a;
	imag.cmp[0]=b;
	imag.cmp[1]=c;
	imag.cmp[2]=d;
}

inline void QUATERNION::setzero()
{
	real=0;
	imag.setzero();
}

inline void QUATERNION::setangleaxis(double angle,const VECTOR<3> &axis)
{
	double arad=angle*(math_pi/180);

    real=cosine(arad/2);
    imag=axis*sine(arad/2);
}

inline bool QUATERNION::iszero() const
{
	return(equalszero(getlength()));
}

inline bool QUATERNION::isnormal() const
{
	return(equalszero(getlength()-1));
}

inline QUATERNION QUATERNION::operator+(const QUATERNION &q) const
{
    return(QUATERNION(real+q.real,imag+q.imag));
}

inline QUATERNION& QUATERNION::operator+=(const QUATERNION &q)
{
    real+=q.real;
    imag+=q.imag;

    return(*this);
}

inline QUATERNION QUATERNION::operator-(const QUATERNION &q) const
{
    return(QUATERNION(real-q.real,imag-q.imag));
}

inline QUATERNION& QUATERNION::operator-=(const QUATERNION &q)
{
    real-=q.real;
    imag-=q.imag;

    return(*this);
}

inline QUATERNION QUATERNION::operator-() const
{
    return(QUATERNION(-real,-imag));
}

inline QUATERNION QUATERNION::operator*(const QUATERNION &q) const
{
	return(QUATERNION(real*q.real - imag.getdotproduct(q.imag),
           imag*q.real + q.imag*real + imag.getcrossproduct(q.imag)));
}

inline QUATERNION& QUATERNION::operator*=(const QUATERNION &q)
{
	*this=*this*q;

	return(*this);
}

inline QUATERNION QUATERNION::operator*(double s) const
{
    return(QUATERNION(real*s,imag*s));
}

inline QUATERNION& QUATERNION::operator*=(double s)
{
    real*=s;
    imag*=s;

    return(*this);
}

inline QUATERNION QUATERNION::operator/(double s) const
{
    return(QUATERNION(real/s,imag/s));
}

inline QUATERNION& QUATERNION::operator/=(double s)
{
    real/=s;
    imag/=s;

    return(*this);
}

inline double QUATERNION::getdotproduct(const QUATERNION &q) const
{
    return(real*q.real + imag.getdotproduct(q.imag));
}

inline QUATERNION QUATERNION::getinverse() const
{
    return(getconjugate()/getdotproduct(*this));
}

inline void QUATERNION::setinverse()
{
    *this=getinverse();
}

inline QUATERNION QUATERNION::getconjugate() const
{
    return(QUATERNION(real,-imag));
}

inline void QUATERNION::setconjugate()
{
	imag*=-1;
}

inline double QUATERNION::getlength() const
{
	return(squareroot(getdotproduct(*this)));
}

inline QUATERNION QUATERNION::getnormal() const
{
	double l=getlength();

	if (l<math_epsilon)
	{
		l=math_epsilon;
	}

	return(*this/l);
}

inline void QUATERNION::setnormal()
{
	double l=getlength();

	if (l<math_epsilon)
	{
		l=math_epsilon;
	}

	*this/=l;
}

inline MATRIX<3> QUATERNION::getrotationmatrix() const
{
    MATRIX<3> temp;

    double a=real;
    double b=imag.cmp[0];
    double c=imag.cmp[1];
    double d=imag.cmp[2];

    temp.cmp[0]=a*a + b*b - c*c - d*d;
    temp.cmp[1]=2*b*c - 2*a*d;
    temp.cmp[2]=2*a*c + 2*b*d;
    temp.cmp[3]=2*a*d + 2*b*c;
    temp.cmp[4]=a*a - b*b + c*c - d*d;
    temp.cmp[5]=2*c*d - 2*a*b;
    temp.cmp[6]=2*b*d - 2*a*c;
    temp.cmp[7]=2*a*b + 2*c*d;
    temp.cmp[8]=a*a - b*b - c*c + d*d;

    return(temp);
}
