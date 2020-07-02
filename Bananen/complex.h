//Code by Rogier Walraven. Do not distribute.

struct COMPLEX
{
    double real;
    double imag;

    COMPLEX();
    COMPLEX(double r,double i);

    void setzero();
    void setangle(double a);

	bool iszero() const;
    bool isnormal() const;

    COMPLEX operator+(const COMPLEX &c) const;
    COMPLEX& operator+=(const COMPLEX &c);

    COMPLEX operator-(const COMPLEX &c) const;
    COMPLEX& operator-=(const COMPLEX &c);

    COMPLEX operator-() const;

    COMPLEX operator*(const COMPLEX &c) const;
    COMPLEX& operator*=(const COMPLEX &c);

    COMPLEX operator/(const COMPLEX &c) const;
    COMPLEX& operator/=(const COMPLEX &c);

    COMPLEX operator*(double s) const;
    COMPLEX& operator*=(double s);

    COMPLEX operator/(double s) const;
    COMPLEX& operator/=(double s);

    double getdotproduct(const COMPLEX &c) const;

    COMPLEX getinverse() const;
    void setinverse();

    COMPLEX getconjugate() const;
    void setconjugate();

    double getlength() const;

    COMPLEX getnormal() const;
    void setnormal();
};

inline COMPLEX::COMPLEX() : real(0),imag(0) {}

inline COMPLEX::COMPLEX(double r,double i) : real(r),imag(i) {}

inline void COMPLEX::setzero()
{
	real=0;
	imag=0;
}

inline void COMPLEX::setangle(double a)
{
	double arad=a*(math_pi/180);

    real=cosine(arad);
    imag=sine(arad);
}

inline bool COMPLEX::iszero() const
{
	return(equalszero(getlength()));
}

inline bool COMPLEX::isnormal() const
{
	return(equalszero(getlength()-1));
}

inline COMPLEX COMPLEX::operator+(const COMPLEX &c) const
{
    return(COMPLEX(real+c.real,imag+c.imag));
}

inline COMPLEX& COMPLEX::operator+=(const COMPLEX &c)
{
    real+=c.real;
    imag+=c.imag;

    return(*this);
}

inline COMPLEX COMPLEX::operator-(const COMPLEX &c) const
{
    return(COMPLEX(real-c.real,imag-c.imag));
}

inline COMPLEX& COMPLEX::operator-=(const COMPLEX &c)
{
    real-=c.real;
    imag-=c.imag;

    return(*this);
}

inline COMPLEX COMPLEX::operator-() const
{
    return(COMPLEX(-real,-imag));
}

inline COMPLEX COMPLEX::operator*(const COMPLEX &c) const
{
    return(COMPLEX(real*c.real - imag*c.imag,real*c.imag + imag*c.real));
}

inline COMPLEX& COMPLEX::operator*=(const COMPLEX &c)
{
    *this=*this*c;

    return(*this);
}

inline COMPLEX COMPLEX::operator/(const COMPLEX &c) const
{
    return(*this*c.getinverse());
}

inline COMPLEX& COMPLEX::operator/=(const COMPLEX &c)
{
	*this*=c.getinverse();

	return(*this);
}

inline COMPLEX COMPLEX::operator*(double s) const
{
    return(COMPLEX(real*s,imag*s));
}

inline COMPLEX& COMPLEX::operator*=(double s)
{
    real*=s;
    imag*=s;

    return(*this);
}

inline COMPLEX COMPLEX::operator/(double s) const
{
    return(COMPLEX(real/s,imag/s));
}

inline COMPLEX& COMPLEX::operator/=(double s)
{
    real/=s;
    imag/=s;

    return(*this);
}

inline double COMPLEX::getdotproduct(const COMPLEX &c) const
{
    return(real*c.real + imag*c.imag);
}

inline COMPLEX COMPLEX::getinverse() const
{
    return(getconjugate()/getdotproduct(*this));
}

inline void COMPLEX::setinverse()
{
    *this=getinverse();
}

inline COMPLEX COMPLEX::getconjugate() const
{
    return(COMPLEX(real,-imag));
}

inline void COMPLEX::setconjugate()
{
    imag*=-1;
}

inline double COMPLEX::getlength() const
{
	return(squareroot(getdotproduct(*this)));
}

inline COMPLEX COMPLEX::getnormal() const
{
	double l=getlength();

	if (l<math_epsilon)
	{
		l=math_epsilon;
	}

	return(*this/l);
}

inline void COMPLEX::setnormal()
{
	double l=getlength();

	if (l<math_epsilon)
	{
		l=math_epsilon;
	}

	*this/=l;
}
