//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Bananen.h"

BANANEN::MODEL::MODEL(const double *vcoords,int numvcoords,const int *vindices,int numvindices) :
	nodraw(false),
	path(0)
{
	int numvertices=numvcoords/3;

	VECTOR<3> *vertices=new VECTOR<3>[numvertices];

	for (int i=0;i<numvcoords;i+=3)
	{
		vertices[i/3].cmp[0]=vcoords[i];
		vertices[i/3].cmp[1]=vcoords[i+1];
		vertices[i/3].cmp[2]=vcoords[i+2];
	}

	numtriangles=numvindices/3;

	triangles=new TRIANGLE[numtriangles];

	for (int i=0;i<numvindices;i+=3)
	{
		triangles[i/3].vertex[0]=vertices[vindices[i]-1];
		triangles[i/3].vertex[1]=vertices[vindices[i+1]-1];
		triangles[i/3].vertex[2]=vertices[vindices[i+2]-1];
	}

	delete[] vertices;

	striangles=new TRIANGLE[numtriangles];
	dtriangles=new TRIANGLE[numtriangles];

	for (int i=0;i<numtriangles;i++)
	{
		striangles[i]=triangles[i];
	}
}

BANANEN::MODEL::~MODEL()
{
	delete[] triangles;
	delete[] striangles;
	delete[] dtriangles;
}

BANANEN::MODEL& BANANEN::MODEL::operator*=(const MATRIX<4> &m)
{
	for (int i=0;i<numtriangles;i++)
	{
		striangles[i]*=m;
	}

	return(*this);
}

void BANANEN::MODEL::sort(const VECTOR<3> &p,
                          const LINE &l1,const LINE &l2,const LINE &l3,const LINE &l4,
                          ARRAY<TRIANGLE> &triangles,int &numtriangles) const
{
	for (int i=0;i<this->numtriangles;i++)
	{
		triangles[numtriangles]=this->striangles[i];

		numtriangles++;
	}
}

void BANANEN::MODEL::setnodraw(bool nd)
{
	nodraw=nd;
}

void BANANEN::MODEL::setworld(const MATRIX<4> &w)
{
	world=w;

	for (int i=0;i<numtriangles;i++)
	{
		dtriangles[i]=triangles[i]*w;
	}

	VECTOR<3> pos=w.getcolumn(3).getcartesian();

	radius=0;

	double len;

	for (int i=0;i<numtriangles;i++)
	{
		len=(dtriangles[i].vertex[0]-pos).getlength();
		if (len>radius) {radius=len;}
		len=(dtriangles[i].vertex[1]-pos).getlength();
		if (len>radius) {radius=len;}
		len=(dtriangles[i].vertex[2]-pos).getlength();
		if (len>radius) {radius=len;}
	}
}

BANANEN::MATRIX<4> BANANEN::MODEL::getworld() const
{
	return(world);
}

BANANEN::VECTOR<3> BANANEN::MODEL::getposition() const
{
	return(world.getcolumn(3).getcartesian());
}

double BANANEN::MODEL::getradius() const
{
	return(radius);
}

const BANANEN::TRIANGLE *BANANEN::MODEL::gettriangles() const
{
	if (nodraw)
	{
		return(0);
	}

	return(dtriangles);
}

int BANANEN::MODEL::getnumtriangles() const
{
	if (nodraw)
	{
		return(0);
	}

	return(numtriangles);
}

void BANANEN::MODEL::setpath(const PATH &path,double timescale,const VECTOR<3> &worigin,double scale)
{
	this->path=&path;
	this->timescale=timescale;
	this->worigin=worigin;
	this->scale=scale;

	time=0;

	pathtranslation.setzero();
	pathrotation=QUATERNION(1,0,0,0);
}

void BANANEN::MODEL::clearpath()
{
	path=0;
}

void BANANEN::MODEL::setpathtranslation(const VECTOR<3> &pt)
{
	pathtranslation=pt;
}

void BANANEN::MODEL::setpathrotation(const QUATERNION &pr)
{
	pathrotation=pr;
}

bool BANANEN::MODEL::haspath() const
{
	return((bool)path);
}

void BANANEN::MODEL::integrate(double timestep)
{
	VECTOR<3> pos,tan;

	(*path)(time,pos,tan);

	VECTOR<3> Zs=-tan;
	VECTOR<3> Ys=pos-worigin;
	VECTOR<3> Xs=Ys.getcrossproduct(Zs);
	Zs=Xs.getcrossproduct(Ys);

	Xs.setnormal();
	Ys.setnormal();
	Zs.setnormal();

	MATRIX<3> m3;

	m3.setcolumn(Xs,0);
	m3.setcolumn(Ys,1);
	m3.setcolumn(Zs,2);

	VECTOR<3> ptrans=m3 * pathtranslation;

	m3*=pathrotation.getrotationmatrix();

	if (!nodraw)
	{
		MATRIX<3> sm;
		sm.setone();
		sm*=scale;
		m3*=sm;
	}

	MATRIX<4> m4=m3.gethomogeneous();
	m4.setcolumn((pos+ptrans).gethomogeneous(),3);

	setworld(m4);

	time=modulus(time+timestep*timescale,1);
}
