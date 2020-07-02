//Code by Rogier Walraven. Do not distribute.

#define _USE_MATH_DEFINES
#include <math.h>

#include "appmath.h"

const double math_pi=M_PI;

double sine      (double x)
{
	return(sin(x));
}

double cosine    (double x)
{
	return(cos(x));
}

double invsine   (double x)
{
	return(asin(x));
}

double invcosine (double x)
{
	return(acos(x));
}

double power     (double x,double y)
{
	return(pow(x,y));
}

double square    (double x)
{
	return(x*x);
}

double squareroot(double x)
{
	return(sqrt(x));
}

double absolute  (double x)
{
	return(fabs(x));
}

double sign      (double x)
{
	if (x>=0)
	{
		return(1);
	}

	return(-1);
}

double modulus   (double x,double m)
{
	double s=sign(m);

	x*=s;
	m*=s;

	double r;

	if (x>=0)
	{
		r=((x/m)-(int)(x/m))*m;
	}
	else
	{
		r=m-(((-x/m)-(int)(-x/m))*m);
	}

	r*=s;

	return(r);
}

int    appround  (double x)
{
	if (x>=0)
	{
		return((int)(x+0.5));
	}

	return((int)(x-0.5));
}

bool   equalszero(double x)
{
	return(((x+math_epsilon)>0) && ((x-math_epsilon)<0));
}

double compact_gaussian     (double x,double interval,bool normalized)
{
	double dx=x-(interval/2);

	double radius=interval/4;

	double q=absolute(dx)/radius;

	double scale;

	if (normalized)
	{
		scale=2/(3*radius);
	}
	else
	{
		scale=1;
	}

	if (q>=0 && q<1)
	{
		return(scale*(1-(((double)3/2)*q*q)+(((double)3/4)*q*q*q)));
	}

	if (q>=1 && q<2)
	{
		double tq=2-q;

		return(scale*(((double)1/4)*tq*tq*tq));
	}

	return(0);
}

double compact_gaussian_diff(double x,double interval,bool normalized)
{
	double dx=x-(interval/2);

	double radius=interval/4;

	double q=absolute(dx)/radius;

	double scale;

	if (normalized)
	{
		scale=2/(3*radius*radius);
	}
	else
	{
		scale=1;
	}

	if (x>(interval/2))
	{
		scale*=-1;
	}

	if (q>=0 && q<1)
	{
		return(scale*((3*q)-(2.25*q*q)));
	}

	if (q>=1 && q<2)
	{
		double tq=2-q;

		return(scale*(0.75*tq*tq));
	}

	return(0);
}

double sph_kernel         (double x,double y,double px,double py,double radius)
{
	double dx=x-px;
	double dy=y-py;

	radius/=2;

	double q=squareroot(dx*dx + dy*dy)/radius;

	double scale=10/(7*math_pi*square(radius));

	if (q>=0 && q<1)
	{
		return(scale*(1-(((double)3/2)*q*q)+(((double)3/4)*q*q*q)));
	}

	if (q>=1 && q<2)
	{
		double tq=2-q;

		return(scale*(((double)1/4)*tq*tq*tq));
	}

	return(0);
}

double sph_kernel_grad    (double x,double y,double px,double py,double radius)
{
	double dx=x-px;
	double dy=y-py;

	radius/=2;

	double q=squareroot(dx*dx + dy*dy)/radius;

	double scale=10/(7*math_pi*radius*radius*radius);

	if (q>=0 && q<1)
	{
		return(scale*((3*q)-(2.25*q*q)));
	}

	if (q>=1 && q<2)
	{
		double tq=2-q;

		return(scale*(0.75*tq*tq));
	}

	return(0);
}

double sph_kernel_div_grad(double x,double y,double px,double py,double radius)
{
	double dx=x-px;
	double dy=y-py;

	radius/=2;

	double q=squareroot(dx*dx + dy*dy)/radius;

	double scale=10/(7*math_pi*radius*radius*radius*radius);

	double result;

	if (q>=0 && q<1)
	{
		result=scale*(-3+(4.5*q));
	}
	else if (q>=1 && q<2)
	{
		double tq=2-q;

		result=scale*(1.5*tq);
	}
	else
	{
		result=0;
	}

	if (result>0)
	{
		result=0;
	}

	return(result);
}
