//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Bananen.h"

BANANEN::PATH::PATH(const VECTOR<3> *points,int numpoints)
{
	if (numpoints<4)
	{
		curves=0;
		numcurves=0;

		return;
	}

	numcurves=(numpoints-4)+1;

	curves=new CURVE[numcurves];

	int j=0;

	for (int i=0;i<numcurves;i++)
	{
		curves[i].P0=points[j];
		curves[i].P1=points[j+1];
		curves[i].P2=points[j+2];
		curves[i].P3=points[j+3];

		curves[i].alpha=1;

		j++;
	}
}

BANANEN::PATH::~PATH()
{
	delete[] curves;
}

void BANANEN::PATH::operator()(double t,VECTOR<3> &C,VECTOR<3> &Cs) const
{
	curves[(int)(t*numcurves)](modulus(t,(double)1/numcurves)*numcurves,C,Cs);
}
