//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Bananen.h"

BANANEN::OCTTREE::OCTTREE(int depth,const VECTOR<3> &origin,double scale) :
	BSPTREE(2)
{
	VECTOR<3> X,Y,Z;

	X.setortho(0);
	Y.setortho(1);
	Z.setortho(2);

	PLANE XY,
		  YZ,
		  ZX;

	XY.normal=X.getcrossproduct(Y);
	YZ.normal=Y.getcrossproduct(Z);
	ZX.normal=Z.getcrossproduct(X);

	MATRIX<4> translate;

	translate.setone();
	translate.setcolumn(origin.gethomogeneous(),3);

	XY*=translate;
	YZ*=translate;
	ZX*=translate;

	BSPTREE::plane=XY;

	BSPTREE::node1->plane=YZ;
	BSPTREE::node2->plane=YZ;

	BSPTREE::node1->node1->plane=ZX;
	BSPTREE::node1->node2->plane=ZX;
	BSPTREE::node2->node1->plane=ZX;
	BSPTREE::node2->node2->plane=ZX;

	this->depth=depth;
	this->origin=origin;
	this->scale=scale;

	if (depth>0)
	{
		VECTOR<3> o1=((( X*scale) + ( Y*scale) + ( Z*scale))/2);
		VECTOR<3> o2=(((-X*scale) + ( Y*scale) + ( Z*scale))/2);
		VECTOR<3> o3=(((-X*scale) + ( Y*scale) + (-Z*scale))/2);
		VECTOR<3> o4=((( X*scale) + ( Y*scale) + (-Z*scale))/2);
		VECTOR<3> o5=((( X*scale) + (-Y*scale) + ( Z*scale))/2);
		VECTOR<3> o6=(((-X*scale) + (-Y*scale) + ( Z*scale))/2);
		VECTOR<3> o7=(((-X*scale) + (-Y*scale) + (-Z*scale))/2);
		VECTOR<3> o8=((( X*scale) + (-Y*scale) + (-Z*scale))/2);

		BSPTREE::node1->node1->node1=new OCTTREE(depth-1,origin+o1,scale/2);
		BSPTREE::node1->node1->node2=new OCTTREE(depth-1,origin+o5,scale/2);
		BSPTREE::node1->node2->node1=new OCTTREE(depth-1,origin+o2,scale/2);
		BSPTREE::node1->node2->node2=new OCTTREE(depth-1,origin+o6,scale/2);
		BSPTREE::node2->node1->node1=new OCTTREE(depth-1,origin+o4,scale/2);
		BSPTREE::node2->node1->node2=new OCTTREE(depth-1,origin+o8,scale/2);
		BSPTREE::node2->node2->node1=new OCTTREE(depth-1,origin+o3,scale/2);
		BSPTREE::node2->node2->node2=new OCTTREE(depth-1,origin+o7,scale/2);
	}
}

int BANANEN::OCTTREE::getdepth() const
{
	return(depth);
}

int BANANEN::OCTTREE::getindexsize(int depth) const
{
	if (depth>=0)
	{
		return((int)power(2,depth));
	}

	return(0);
}

bool BANANEN::OCTTREE::getnode(int x,int y,int z,int depth,MATRIX<4> &m,BSPTREE **node) const
{
	VECTOR<3> X,Y,Z;

	X.setortho(0);
	Y.setortho(1);
	Z.setortho(2);

    double s=scale/power(2,depth);

    VECTOR<3> mX=X*((x*s)+(s/2));
    VECTOR<3> mY=Y*((y*s)+(s/2));
    VECTOR<3> mZ=Z*((z*s)+(s/2));

    if (absolute(mX.cmp[0])>scale ||
        absolute(mY.cmp[1])>scale ||
        absolute(mZ.cmp[2])>scale)
    {
        return(false);
    }

	VECTOR<3> xyz=origin+(mX+mY+mZ);

    if (BSPTREE::getnode(xyz,2+(depth*3),node))
    {
		MATRIX<3> m3;
		m3.setone();
		m3*=s/2;
        m=m3.gethomogeneous();
        m.setcolumn(xyz.gethomogeneous(),3);

        return(true);
    }

    return(false);
}

bool BANANEN::OCTTREE::setnode(int x,int y,int z,int depth,MATRIX<4> &m,BSPTREE *node)
{
	VECTOR<3> X,Y,Z;

	X.setortho(0);
	Y.setortho(1);
	Z.setortho(2);

    double s=scale/power(2,depth);

    VECTOR<3> mX=X*((x*s)+(s/2));
    VECTOR<3> mY=Y*((y*s)+(s/2));
    VECTOR<3> mZ=Z*((z*s)+(s/2));

    if (absolute(mX.cmp[0])>scale ||
        absolute(mY.cmp[1])>scale ||
        absolute(mZ.cmp[2])>scale)
    {
        return(false);
    }

	VECTOR<3> xyz=origin+(mX+mY+mZ);

    if (BSPTREE::setnode(xyz,2+(depth*3),node))
    {
		MATRIX<3> m3;
		m3.setone();
		m3*=s/2;
        m=m3.gethomogeneous();
        m.setcolumn(xyz.gethomogeneous(),3);

        return(true);
    }

    return(false);
}
