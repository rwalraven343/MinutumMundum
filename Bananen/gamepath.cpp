//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Bananen.h"

BANANEN::VECTOR<3> *BANANEN::generate_ellipse_points(ELLIPSE_AXIS axis,const QUATERNION &rot,
                                                     double radiusx,double radiusy,int numsteps)
{
	VECTOR<3> *points=new VECTOR<3>[numsteps+3];

	MATRIX<3> rotm=rot.getrotationmatrix();

	double A;

	for (int i=0;i<numsteps;i++)
	{
		A=i*((2*math_pi)/numsteps);

		switch (axis)
		{
		case EA_X:
			points[i].cmp[2]=-sine(A)*radiusx;
			points[i].cmp[1]=cosine(A)*radiusy;
			points[i].cmp[0]=0;
		break;
		case EA_Y:
			points[i].cmp[0]=sine(A)*radiusx;
			points[i].cmp[2]=-cosine(A)*radiusy;
			points[i].cmp[1]=0;
		break;
		case EA_Z:
			points[i].cmp[0]=sine(A)*radiusx;
			points[i].cmp[1]=cosine(A)*radiusy;
			points[i].cmp[2]=0;
		break;
		}

		points[i]=rotm * points[i];
	}

	points[numsteps]  =points[0];
	points[numsteps+1]=points[1];
	points[numsteps+2]=points[2];

	return(points);
}

BANANEN::VECTOR<3> *BANANEN::generate_ellipses_points(int numapp,int numorb,
                                                      ELLIPSE_AXIS axis,const QUATERNION &rot,
                                                      double radiusx,double radiusy,int numsteps)
{
	int numpoints=(numapp*2 + numorb)*numsteps + 3;

	VECTOR<3> *points=new VECTOR<3>[numpoints];

	VECTOR<3> *ellipse_points;

	int k=0;

	for (int i=0;i<numapp;i++)
	{
		ellipse_points=generate_ellipse_points(axis,rot,radiusx*(numapp-i),radiusy*(numapp-i),numsteps);

		for (int j=0;j<numsteps;j++)
		{
			points[k]=ellipse_points[j];

			k++;
		}

		delete[] ellipse_points;
	}

	for (int i=0;i<numorb;i++)
	{
		ellipse_points=generate_ellipse_points(axis,rot,radiusx,radiusx,numsteps);

		for (int j=0;j<numsteps;j++)
		{
			points[k]=ellipse_points[j];

			k++;
		}

		delete[] ellipse_points;
	}

	for (int i=0;i<numapp;i++)
	{
		ellipse_points=generate_ellipse_points(axis,rot,radiusx*(i+1),radiusy*(i+1),numsteps);

		for (int j=0;j<numsteps;j++)
		{
			points[k]=ellipse_points[j];

			k++;
		}

		delete[] ellipse_points;
	}

	points[numpoints-3]=points[0];
	points[numpoints-2]=points[1];
	points[numpoints-1]=points[2];

	return(points);
}
