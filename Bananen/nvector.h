//Code by Rogier Walraven. Do not distribute.

template<int N>
struct VECTOR
{
    double cmp[N];

    VECTOR();

    void setzero();
    void setortho(int i);

	void setall(double val);
    void setrandom(double min,double max);

	bool iszero() const;
    bool isnormal() const;

	VECTOR<N> operator+(const VECTOR<N> &v) const;
	VECTOR<N> operator-(const VECTOR<N> &v) const;

	VECTOR<N>& operator+=(const VECTOR<N> &v);
	VECTOR<N>& operator-=(const VECTOR<N> &v);

	VECTOR<N> operator-() const;

	VECTOR<N> operator*(double s) const;
	VECTOR<N> operator/(double s) const;

	VECTOR<N>& operator*=(double s);
	VECTOR<N>& operator/=(double s);

	double getdotproduct           (const VECTOR<N> &v) const;
	double getdotproduct_normalized(const VECTOR<N> &v) const;

    VECTOR<3> getcrossproduct(const VECTOR<3> &v) const;
    void      setcrossproduct(const VECTOR<3> &v);

	double getlength() const;
	double getlength_squared() const;

    VECTOR<N> getnormal() const;
    void setnormal();

    VECTOR<N+1> gethomogeneous() const;
    VECTOR<N-1> getcartesian() const;

	double operator()(double azimuth,double zenith) const;
};

template<int N>
inline VECTOR<N>::VECTOR()
{
	setzero();
}

template<int N>
inline void VECTOR<N>::setzero()
{
    for (int i=0;i<N;i++)
    {
        cmp[i]=0;
    }
}

template<int N>
inline void VECTOR<N>::setortho(int i)
{
    setzero();

    cmp[i]=1;
}

template<int N>
inline void VECTOR<N>::setall(double val)
{
    for (int i=0;i<N;i++)
    {
        cmp[i]=val;
    }
}

template<int N>
inline void VECTOR<N>::setrandom(double min,double max)
{
    for (int i=0;i<N;i++)
    {
        cmp[i]=RANDOM::global(min,max);
    }
}

template<int N>
inline bool VECTOR<N>::iszero() const
{
	return(equalszero(getlength()));
}

template<int N>
inline bool VECTOR<N>::isnormal() const
{
	return(equalszero(getlength()-1));
}

template<int N>
inline VECTOR<N> VECTOR<N>::operator+(const VECTOR<N> &v) const
{
    VECTOR<N> temp;

    for (int i=0;i<N;i++)
    {
        temp.cmp[i]=cmp[i]+v.cmp[i];
    }

    return(temp);
}

template<int N>
inline VECTOR<N> VECTOR<N>::operator-(const VECTOR<N> &v) const
{
    VECTOR<N> temp;

    for (int i=0;i<N;i++)
    {
        temp.cmp[i]=cmp[i]-v.cmp[i];
    }

    return(temp);
}

template<int N>
inline VECTOR<N>& VECTOR<N>::operator+=(const VECTOR<N> &v)
{
    for (int i=0;i<N;i++)
    {
        cmp[i]+=v.cmp[i];
    }

	return(*this);
}

template<int N>
inline VECTOR<N>& VECTOR<N>::operator-=(const VECTOR<N> &v)
{
    for (int i=0;i<N;i++)
    {
        cmp[i]-=v.cmp[i];
    }

	return(*this);
}

template<int N>
inline VECTOR<N> VECTOR<N>::operator-() const
{
    VECTOR<N> temp;

    for (int i=0;i<N;i++)
    {
        temp.cmp[i]=-cmp[i];
    }

    return(temp);
}

template<int N>
inline VECTOR<N> VECTOR<N>::operator*(double s) const
{
    VECTOR<N> temp;

    for (int i=0;i<N;i++)
    {
        temp.cmp[i]=cmp[i]*s;
    }

    return(temp);
}

template<int N>
inline VECTOR<N> VECTOR<N>::operator/(double s) const
{
    VECTOR<N> temp;

    for (int i=0;i<N;i++)
    {
        temp.cmp[i]=cmp[i]/s;
    }

    return(temp);
}

template<int N>
inline VECTOR<N>& VECTOR<N>::operator*=(double s)
{
    for (int i=0;i<N;i++)
    {
        cmp[i]*=s;
    }

	return(*this);
}

template<int N>
inline VECTOR<N>& VECTOR<N>::operator/=(double s)
{
    for (int i=0;i<N;i++)
    {
        cmp[i]/=s;
    }

	return(*this);
}

template<int N>
inline double VECTOR<N>::getdotproduct           (const VECTOR<N> &v) const
{
    double result=0;

    for (int i=0;i<N;i++)
    {
        result+=cmp[i]*v.cmp[i];
    }

    return(result);
}

template<int N>
inline double VECTOR<N>::getdotproduct_normalized(const VECTOR<N> &v) const
{
	VECTOR<N> n1=getnormal();
	VECTOR<N> n2=v.getnormal();

	return(n1.getdotproduct(n2));
}

template<int N>
inline VECTOR<3> VECTOR<N>::getcrossproduct(const VECTOR<3> &v) const
{
	VECTOR<3> x;
	VECTOR<3> y;
	VECTOR<3> z;

	x.setortho(0);
	y.setortho(1);
	z.setortho(2);

    return
    (
        (x*(cmp[1]*v.cmp[2] - cmp[2]*v.cmp[1])) -
        (y*(cmp[0]*v.cmp[2] - cmp[2]*v.cmp[0])) +
        (z*(cmp[0]*v.cmp[1] - cmp[1]*v.cmp[0]))
    );
}

template<int N>
inline void      VECTOR<N>::setcrossproduct(const VECTOR<3> &v)
{
	*this=getcrossproduct(v);
}

template<int N>
inline double VECTOR<N>::getlength() const
{
	return(squareroot(getdotproduct(*this)));
}

template<int N>
inline double VECTOR<N>::getlength_squared() const
{
	return(getdotproduct(*this));
}

template<int N>
inline VECTOR<N> VECTOR<N>::getnormal() const
{
	double l=getlength();

	if (l<math_epsilon)
	{
		l=math_epsilon;
	}

	return(*this/l);
}

template<int N>
inline void VECTOR<N>::setnormal()
{
	double l=getlength();

	if (l<math_epsilon)
	{
		l=math_epsilon;
	}

	*this/=l;
}

template<int N>
inline VECTOR<N+1> VECTOR<N>::gethomogeneous() const
{
    VECTOR<N+1> temp;

    for (int i=0;i<N;i++)
    {
        temp.cmp[i]=cmp[i];
    }

    temp.cmp[N]=1;

    return(temp);
}

template<int N>
inline VECTOR<N-1> VECTOR<N>::getcartesian() const
{
    VECTOR<N-1> temp;

    for (int i=0;i<N-1;i++)
    {
        temp.cmp[i]=cmp[i]/cmp[N-1];
    }

    return(temp);
}

template<int N>
inline double VECTOR<N>::operator()(double azimuth,double zenith) const
{
    double result=0;

    int l=0;
    int m=0;

    for (int i=0;i<N;i++)
    {
        result += SH(l,m,azimuth,zenith) * cmp[i];

        if (l==m)
        {
            l++;
            m=-l;
        }
        else
        {
            m++;
        }
    }

    return(result);
}
