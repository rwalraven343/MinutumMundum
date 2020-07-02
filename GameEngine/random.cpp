//Code by Rogier Walraven. Do not distribute.

#include "GameLib\\GameLib.h"
#include "appmath.h"
#include "random.h"

RANDOM::LCGENERATOR::LCGENERATOR(INT mod,INT mult,INT seed)
{
	modulus    = mod;
	multiplier = mult;
	quotient   = mod/mult;
	remainder  = mod%mult;

	result=seed;
}

RANDOM::INT RANDOM::LCGENERATOR::operator()() const
{
	INT k=result/quotient;

	result=multiplier*(result-k*quotient)-k*remainder;

	if (result<0)
	{
		result+=modulus;
	}

	return(result);
}

RANDOM::INT RANDOM::LCGENERATOR::getmodulus() const
{
	return(modulus);
}

RANDOM::GENERATOR::GENERATOR(INT seed1,INT seed2) : generator1(modulus1,multiplier1,seed1),
													generator2(modulus2,multiplier2,seed2)
{
	for (INT i=0;i<table_size;i++)
	{
		table[i]=0;
	}

	result=0;

	for (INT i=0;i<(table_size*4);i++)
	{
		operator()();
	}
}

RANDOM::INT RANDOM::GENERATOR::operator()() const
{
	INT i=result/(((generator1.getmodulus()-1)/table_size)+1);

	result=table[i]-generator2();
	table[i]=generator1();

	if (result<1)
	{
		result+=generator1.getmodulus()-1;
	}

	return(result);
}

RANDOM::INT RANDOM::GENERATOR::getmodulus() const
{
	return(generator1.getmodulus());
}

int RANDOM::GENERATOR::operator()(int max) const
{
	return(operator()()%max);
}

double RANDOM::GENERATOR::operator()(double min,double max) const
{
	return(min+(((double)operator()()/getmodulus())*(max-min)));
}

double RANDOM::GENERATOR::compact_normal(double min,double max) const
{
	return(min+((invcosine((invcosine(operator()(-1,1))/(math_pi*0.5))-1)/math_pi)*(max-min)));
}
