//Code by Rogier Walraven. Do not distribute.

template<int N>
struct MATRIX
{
    double cmp[N*N];

	MATRIX();

    void setzero();
    void setone();

	void setrow(const VECTOR<N> &v,int i);
	void setcolumn(const VECTOR<N> &v,int j);

    VECTOR<N> getrow(int i) const;
    VECTOR<N> getcolumn(int j) const;

	static MATRIX<4> projection(double left,double right,
                                double bottom,double top,
                                double near,double far);

    MATRIX<N> operator+(const MATRIX<N> &m) const;
    MATRIX<N>& operator+=(const MATRIX<N> &m);

    MATRIX<N> operator-(const MATRIX<N> &m) const;
    MATRIX<N>& operator-=(const MATRIX<N> &m);

    MATRIX<N> operator-() const;

    MATRIX<N> operator*(const MATRIX<N> &m) const;
    MATRIX<N>& operator*=(const MATRIX<N> &m);

    MATRIX<N> operator/(const MATRIX<N> &m) const;
    MATRIX<N>& operator/=(const MATRIX<N> &m);

    MATRIX<N> operator*(double s) const;
    MATRIX<N>& operator*=(double s);

    MATRIX<N> operator/(double s) const;
    MATRIX<N>& operator/=(double s);

    VECTOR<N> operator*(const VECTOR<N> &v) const;

    MATRIX<N> getinverse() const;
    void setinverse();

    MATRIX<N> gettranspose() const;
    void settranspose();

    MATRIX<N> getadjugate() const;
    void setadjugate();

    MATRIX<N-1> getminor(int i,int j) const;

    double getdeterminant() const;

    MATRIX<N+1> gethomogeneous() const;
};

template<int N>
inline MATRIX<N>::MATRIX()
{
    setzero();
}

template<int N>
inline void MATRIX<N>::setzero()
{
    for (int i=0;i<N*N;i++)
    {
        cmp[i]=0;
    }
}

template<int N>
inline void MATRIX<N>::setone()
{
    setzero();

    for (int i=0,j=0;i<N*N;i+=N,j++)
    {
        cmp[i+j]=1;
    }
}

template<int N>
inline void MATRIX<N>::setrow(const VECTOR<N> &v,int i)
{
    for (int j=0;j<N;j++)
    {
        cmp[i*N+j]=v.cmp[j];
    }
}

template<int N>
inline void MATRIX<N>::setcolumn(const VECTOR<N> &v,int j)
{
    for (int i=0;i<N*N;i+=N)
    {
        cmp[i+j]=v.cmp[i/N];
    }
}

template<int N>
inline VECTOR<N> MATRIX<N>::getrow(int i) const
{
    VECTOR<N> temp;

    for (int j=0;j<N;j++)
    {
        temp.cmp[j]=cmp[i*N+j];
    }

    return(temp);
}

template<int N>
inline VECTOR<N> MATRIX<N>::getcolumn(int j) const
{
    VECTOR<N> temp;

    for (int i=0;i<N*N;i+=N)
    {
        temp.cmp[i/N]=cmp[i+j];
    }

    return(temp);
}

template<int N>
inline MATRIX<4> MATRIX<N>::projection(double left,double right,
                                       double bottom,double top,
                                       double near,double far)
{
	MATRIX<4> temp;

	temp.cmp[0]=(2*near)/(right-left);
	temp.cmp[1]=0;
	temp.cmp[2]=(left+right)/(right-left);
	temp.cmp[3]=0;
	temp.cmp[4]=0;
	temp.cmp[5]=(2*near)/(top-bottom);
	temp.cmp[6]=(top+bottom)/(top-bottom);
	temp.cmp[7]=0;
	temp.cmp[8]=0;
	temp.cmp[9]=0;
	temp.cmp[10]=-((near+far)/(far-near));
	temp.cmp[11]=-((2*near*far)/(far-near));
	temp.cmp[12]=0;
	temp.cmp[13]=0;
	temp.cmp[14]=-1;
	temp.cmp[15]=0;

	return(temp);
}

template<int N>
inline MATRIX<N> MATRIX<N>::operator+(const MATRIX<N> &m) const
{
    MATRIX<N> temp;

    for (int i=0;i<N*N;i++)
    {
        temp.cmp[i]=cmp[i]+m.cmp[i];
    }

    return(temp);
}

template<int N>
inline MATRIX<N>& MATRIX<N>::operator+=(const MATRIX<N> &m)
{
    for (int i=0;i<N*N;i++)
    {
        cmp[i]+=m.cmp[i];
    }

    return(*this);
}

template<int N>
inline MATRIX<N> MATRIX<N>::operator-(const MATRIX<N> &m) const
{
    MATRIX<N> temp;

    for (int i=0;i<N*N;i++)
    {
        temp.cmp[i]=cmp[i]-m.cmp[i];
    }

    return(temp);
}

template<int N>
inline MATRIX<N>& MATRIX<N>::operator-=(const MATRIX<N> &m)
{
    for (int i=0;i<N*N;i++)
    {
        cmp[i]-=m.cmp[i];
    }

    return(*this);
}

template<int N>
inline MATRIX<N> MATRIX<N>::operator-() const
{
    MATRIX<N> temp;

    for (int i=0;i<N*N;i++)
    {
        temp.cmp[i]=-cmp[i];
    }

    return(temp);
}

template<int N>
inline MATRIX<N> MATRIX<N>::operator*(const MATRIX<N> &m) const
{
    MATRIX<N> temp;

    for (int j=0;j<N;j++)
    {
        for (int i=0;i<N*N;i+=N)
        {
            for (int k=0;k<N;k++)
            {
                temp.cmp[i+j]+=cmp[i+k]*m.cmp[j+k*N];
            }
        }
    }

    return(temp);
}

template<int N>
inline MATRIX<N>& MATRIX<N>::operator*=(const MATRIX<N> &m)
{
	*this=*this*m;

	return(*this);
}

template<int N>
inline MATRIX<N> MATRIX<N>::operator/(const MATRIX<N> &m) const
{
    return(*this*m.getinverse());
}

template<int N>
inline MATRIX<N>& MATRIX<N>::operator/=(const MATRIX<N> &m)
{
	*this*=m.getinverse();

	return(*this);
}

template<int N>
inline MATRIX<N> MATRIX<N>::operator*(double s) const
{
    MATRIX<N> temp;

    for (int i=0;i<N*N;i++)
    {
        temp.cmp[i]=cmp[i]*s;
    }

    return(temp);
}

template<int N>
inline MATRIX<N>& MATRIX<N>::operator*=(double s)
{
    for (int i=0;i<N*N;i++)
    {
        cmp[i]*=s;
    }

    return(*this);
}

template<int N>
inline MATRIX<N> MATRIX<N>::operator/(double s) const
{
    MATRIX<N> temp;

    for (int i=0;i<N*N;i++)
    {
        temp.cmp[i]=cmp[i]/s;
    }

    return(temp);
}

template<int N>
inline MATRIX<N>& MATRIX<N>::operator/=(double s)
{
    for (int i=0;i<N*N;i++)
    {
        cmp[i]/=s;
    }

    return(*this);
}

template<int N>
inline VECTOR<N> MATRIX<N>::operator*(const VECTOR<N> &v) const
{
    VECTOR<N> temp;

    for (int i=0;i<N*N;i+=N)
    {
        for (int k=0;k<N;k++)
        {
            temp.cmp[i/N]+=cmp[i+k]*v.cmp[k];
        }
    }

    return(temp);
}

template<int N>
inline MATRIX<N> MATRIX<N>::getinverse() const
{
    return(getadjugate().gettranspose()/getdeterminant());
}

template<int N>
inline void MATRIX<N>::setinverse()
{
	*this=getadjugate().gettranspose()/getdeterminant();
}

template<int N>
inline MATRIX<N> MATRIX<N>::gettranspose() const
{
    MATRIX<N> temp;

    for (int i=0;i<N*N;i+=N)
    {
        for (int j=0;j<N;j++)
        {
            temp.cmp[i+j]=cmp[(j*N)+(i/N)];
        }
    }

    return(temp);
}

template<int N>
inline void MATRIX<N>::settranspose()
{
    *this=gettranspose();
}

template<int N>
inline MATRIX<N> MATRIX<N>::getadjugate() const
{
    MATRIX<N> temp;

    for (int i=0;i<N*N;i+=N)
    {
        for (int j=0;j<N;j++)
        {
            temp.cmp[i+j]=power(-1,(i/N)+j+2)*getminor(i/N,j).getdeterminant();
        }
    }

    return(temp);
}

template<int N>
inline void MATRIX<N>::setadjugate()
{
	*this=getadjugate();
}

template<int N>
inline MATRIX<N-1> MATRIX<N>::getminor(int i,int j) const
{
    MATRIX<N-1> temp;

    int kl=0;

    for (int il=0;il<N*N;il+=N)
    {
        for (int jl=0;jl<N;jl++)
        {
            if (il!=(i*N) && jl!=j)
            {
                temp.cmp[kl]=cmp[il+jl];

                kl++;
            }
        }
    }

    return(temp);
}

template<int N>
inline double MATRIX<N>::getdeterminant() const
{
    double dm=0;

    for (int j=0;j<N;j++)
    {
        dm+=cmp[j]*power(-1,j)*getminor(0,j).getdeterminant();
    }

    return(dm);
}
template<>
inline double MATRIX<1>::getdeterminant() const
{
    return(cmp[0]);
}

template<int N>
inline MATRIX<N+1> MATRIX<N>::gethomogeneous() const
{
    MATRIX<N+1> temp;

    for (int i=0;i<N*N;i+=N)
    {
        for (int j=0;j<N;j++)
        {
            temp.cmp[((i/N)*(N+1))+j]=cmp[i+j];
        }
    }

    temp.cmp[((N+1)*(N+1))-1]=1;

    return(temp);
}
