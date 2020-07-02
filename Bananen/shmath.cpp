//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Bananen.h"

long long BANANEN::fact(long long n)
{
    if (n==0)
    {
        return(1);
    }

    return(n*fact(n-1));
}

long long BANANEN::factfact(long long n)
{
    if (n==0 || n==-1)
    {
        return(1);
    }

    return(n*factfact(n-2));
}

static double Pmm(int m,double x)
{
	return(power(-1,m) * BANANEN::factfact(2*m-1) * power(1-(x*x),(double)m/2));
}

static double Pmp1m(int m,double x)
{
	return(x * (2*m+1) * Pmm(m,x));
}

double BANANEN::Klm(int l,int m,double s)
{
	return(squareroot(((2*l+1)*fact(l-m))/(s*fact(l+m))));
}

double BANANEN::Plm(int l,int m,double x)
{
	if (l==m)
	{
		return(Pmm(m,x));
	}

	if (l==m+1)
	{
		return(Pmp1m(m,x));
	}

	double Pl_1m=Pmp1m(m,x);
	double Pl_2m=Pmm(m,x);

	double plm;

	for (int ll=m+2;ll<=l;ll++)
	{
		plm=(x*(2*ll-1)*Pl_1m - (ll+m-1)*Pl_2m)/(ll-m);

		Pl_2m=Pl_1m;
		Pl_1m=plm;
	}

	return(plm);
}

double BANANEN::SH(int l,int m,double azimuth,double zenith)
{
	double V2=squareroot(2);
	double Pi4=math_pi*4;

	if (m>0)
	{
		return( V2 * cosine( m*azimuth) * Plm(l, m,cosine(zenith)) * Klm(l, m,Pi4) );
	}
	else if (m<0)
	{
		return( V2 *   sine(-m*azimuth) * Plm(l,-m,cosine(zenith)) * Klm(l,-m,Pi4) );
	}

	return( Plm(l,0,cosine(zenith)) * Klm(l,0,Pi4) );
}
