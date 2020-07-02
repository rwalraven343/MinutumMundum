//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Bananen.h"

static const int dynarray_capacity=256;

BANANEN::SECTOR::SECTOR(int x,int y,int z,int d,double inner_radius,double outer_radius,
                        int numlodlevels,const TRIANGLE * const * triangles,const int *numtriangles,
                        double lodd,double lodm,
                        const DYNAMIC_MODEL * const * dynamic_models,const int *numdynamic_models) :
	isempty(false),
	lod_distance(lodd),
	lod_mul(lodm)
{
	this->dynamic_models=dynamic_models;
	this->numdynamic_models=numdynamic_models;

	VECTOR<3> X,Y,Z;

	X.setortho(0);
	Y.setortho(1);
	Z.setortho(2);

    double s=1/power(2,d);

    VECTOR<3> mX=X*((x*s)+(s/2));
    VECTOR<3> mY=Y*((y*s)+(s/2));
    VECTOR<3> mZ=Z*((z*s)+(s/2));

	VECTOR<3> xyz=mX+mY+mZ;

	MATRIX<3> m3;
	MATRIX<4> m4;
	m3.setone();
	m3*=s/2;
    m4=m3.gethomogeneous();
    m4.setcolumn(xyz.gethomogeneous(),3);

	VECTOR<3> point1,point2,point3,point4,point5,point6,point7,point8;

	point1.cmp[0]=-1;point1.cmp[1]=1 ;point1.cmp[2]=1 ;
	point2.cmp[0]=-1;point2.cmp[1]=1 ;point2.cmp[2]=-1;
	point3.cmp[0]=-1;point3.cmp[1]=-1;point3.cmp[2]=-1;
	point4.cmp[0]=-1;point4.cmp[1]=-1;point4.cmp[2]=1 ;
	point5.cmp[0]=1 ;point5.cmp[1]=1 ;point5.cmp[2]=-1;
	point6.cmp[0]=1 ;point6.cmp[1]=1 ;point6.cmp[2]=1 ;
	point7.cmp[0]=1 ;point7.cmp[1]=-1;point7.cmp[2]=1 ;
	point8.cmp[0]=1 ;point8.cmp[1]=-1;point8.cmp[2]=-1;

	Cmin=point3;
	Cmax=point6;

	point1=(m4 * point1.gethomogeneous()).getcartesian();
	point2=(m4 * point2.gethomogeneous()).getcartesian();
	point3=(m4 * point3.gethomogeneous()).getcartesian();
	point4=(m4 * point4.gethomogeneous()).getcartesian();
	point5=(m4 * point5.gethomogeneous()).getcartesian();
	point6=(m4 * point6.gethomogeneous()).getcartesian();
	point7=(m4 * point7.gethomogeneous()).getcartesian();
	point8=(m4 * point8.gethomogeneous()).getcartesian();

	if (point1.getlength()<=inner_radius &&
        point2.getlength()<=inner_radius &&
        point3.getlength()<=inner_radius &&
        point4.getlength()<=inner_radius &&
        point5.getlength()<=inner_radius &&
        point6.getlength()<=inner_radius &&
        point7.getlength()<=inner_radius &&
        point8.getlength()<=inner_radius)
	{
		isempty=true;

		return;
	}

	if (point1.getlength()>=outer_radius &&
        point2.getlength()>=outer_radius &&
        point3.getlength()>=outer_radius &&
        point4.getlength()>=outer_radius &&
        point5.getlength()>=outer_radius &&
        point6.getlength()>=outer_radius &&
        point7.getlength()>=outer_radius &&
        point8.getlength()>=outer_radius)
	{
		isempty=true;

		return;
	}

	VECTOR<3>   left_point1,  left_point2,  left_point3;
	VECTOR<3>  right_point1, right_point2, right_point3;
	VECTOR<3>  front_point1, front_point2, front_point3;
	VECTOR<3>   back_point1,  back_point2,  back_point3;
	VECTOR<3>    top_point1,   top_point2,   top_point3;
	VECTOR<3> bottom_point1,bottom_point2,bottom_point3;

      left_point1.cmp[0]=-1;  left_point2.cmp[0]=-1;  left_point3.cmp[0]=-1;
      left_point1.cmp[1]=1 ;  left_point2.cmp[1]=1 ;  left_point3.cmp[1]=-1;
      left_point1.cmp[2]=1 ;  left_point2.cmp[2]=-1;  left_point3.cmp[2]=-1;

     right_point1.cmp[0]=1 ; right_point2.cmp[0]=1 ; right_point3.cmp[0]=1 ;
     right_point1.cmp[1]=1 ; right_point2.cmp[1]=1 ; right_point3.cmp[1]=-1;
     right_point1.cmp[2]=-1; right_point2.cmp[2]=1 ; right_point3.cmp[2]=1 ;

     front_point1.cmp[0]=-1; front_point2.cmp[0]=1 ; front_point3.cmp[0]=1 ;
     front_point1.cmp[1]=1 ; front_point2.cmp[1]=1 ; front_point3.cmp[1]=-1;
     front_point1.cmp[2]=-1; front_point2.cmp[2]=-1; front_point3.cmp[2]=-1;

      back_point1.cmp[0]=1 ;  back_point2.cmp[0]=-1;  back_point3.cmp[0]=-1;
      back_point1.cmp[1]=1 ;  back_point2.cmp[1]=1 ;  back_point3.cmp[1]=-1;
      back_point1.cmp[2]=1 ;  back_point2.cmp[2]=1 ;  back_point3.cmp[2]=1 ;

       top_point1.cmp[0]=-1;   top_point2.cmp[0]=1 ;   top_point3.cmp[0]=1 ;
       top_point1.cmp[1]=1 ;   top_point2.cmp[1]=1 ;   top_point3.cmp[1]=1 ;
       top_point1.cmp[2]=1 ;   top_point2.cmp[2]=1 ;   top_point3.cmp[2]=-1;

    bottom_point1.cmp[0]=-1;bottom_point2.cmp[0]=1 ;bottom_point3.cmp[0]=1 ;
    bottom_point1.cmp[1]=-1;bottom_point2.cmp[1]=-1;bottom_point3.cmp[1]=-1;
    bottom_point1.cmp[2]=-1;bottom_point2.cmp[2]=-1;bottom_point3.cmp[2]=1 ;

	PLANE left  (left_point1  ,left_point3  ,left_point2  );
	PLANE right (right_point1 ,right_point3 ,right_point2 );
	PLANE front (front_point1 ,front_point3 ,front_point2 );
	PLANE back  (back_point1  ,back_point3  ,back_point2  );
	PLANE top   (top_point1   ,top_point3   ,top_point2   );
	PLANE bottom(bottom_point1,bottom_point3,bottom_point2);

	left  *=m4;
	right *=m4;
	front *=m4;
	back  *=m4;
	top   *=m4;
	bottom*=m4;

	m4.setinverse();

	this->numlodlevels=numlodlevels;

	this->triangles=new TRIANGLE*[numlodlevels];

	this->numtriangles=new int[numlodlevels];

	static int numclipped_left;
	static int numclipped_right;
	static int numclipped_front;
	static int numclipped_back;
	static int numclipped_top;
	static int numclipped_bottom;

	static ARRAY<TRIANGLE> toclip(dynarray_capacity);
	static ARRAY<TRIANGLE> clipped_left(dynarray_capacity);
	static ARRAY<TRIANGLE> clipped_right(dynarray_capacity);
	static ARRAY<TRIANGLE> clipped_front(dynarray_capacity);
	static ARRAY<TRIANGLE> clipped_back(dynarray_capacity);
	static ARRAY<TRIANGLE> clipped_top(dynarray_capacity);
	static ARRAY<TRIANGLE> clipped_bottom(dynarray_capacity);

	int center_add=0;

	for (int l=numlodlevels-1;l>=0;l--)
	{
		for (int i=0;i<numtriangles[l];i++)
		{
			toclip[i]=triangles[l][i];
		}

		doclip(left  ,toclip       ,numtriangles[l] ,clipped_left  ,numclipped_left);
		doclip(right ,clipped_left ,numclipped_left ,clipped_right ,numclipped_right);
		doclip(front ,clipped_right,numclipped_right,clipped_front ,numclipped_front);
		doclip(back  ,clipped_front,numclipped_front,clipped_back  ,numclipped_back);
		doclip(top   ,clipped_back ,numclipped_back ,clipped_top   ,numclipped_top);
		doclip(bottom,clipped_top  ,numclipped_top  ,clipped_bottom,numclipped_bottom);

		this->triangles[l]=new TRIANGLE[numclipped_bottom];

		this->numtriangles[l]=numclipped_bottom;

		for (int i=0;i<numclipped_bottom;i++)
		{
			this->triangles[l][i]=clipped_bottom[i] * m4;

			center+=triangles[l][i].vertex[0];
			center+=triangles[l][i].vertex[1];
			center+=triangles[l][i].vertex[2];

			center_add+=3;
		}
	}

	if (center_add!=0)
	{
		center/=center_add;
	}
}

BANANEN::SECTOR::~SECTOR()
{
	if (isempty)
	{
		return;
	}

	for (int l=numlodlevels-1;l>=0;l--)
	{
		delete[] triangles[l];
	}

	delete[] triangles;
	delete[] numtriangles;
}

BANANEN::SECTOR& BANANEN::SECTOR::operator*=(const MATRIX<4> &m)
{
	Cmin=(m * Cmin.gethomogeneous()).getcartesian();
	Cmax=(m * Cmax.gethomogeneous()).getcartesian();

	if (isempty)
	{
		return(*this);
	}

	for (int l=numlodlevels-1;l>=0;l--)
	{
		for (int i=0;i<numtriangles[l];i++)
		{
			triangles[l][i]*=m;
		}
	}

	center=(m * center.gethomogeneous()).getcartesian();

	return(*this);
}

void BANANEN::SECTOR::sort(const VECTOR<3> &p,
                           const LINE &l1,const LINE &l2,const LINE &l3,const LINE &l4,
                           ARRAY<TRIANGLE> &triangles,int &numtriangles) const
{
	VECTOR<3> dmodel_pos;
	double dmodel_radius;
	const TRIANGLE *dmodel_triangles;
	int dmodel_numtriangles;

	for (int i=0;i<*numdynamic_models;i++)
	{
		dmodel_pos=dynamic_models[i]->getposition();
		dmodel_radius=dynamic_models[i]->getradius();

		if (cube_sphere_intersect(dmodel_pos,dmodel_radius))
		{
			dmodel_triangles=dynamic_models[i]->gettriangles();
			dmodel_numtriangles=dynamic_models[i]->getnumtriangles();

			for (int j=0;j<dmodel_numtriangles;j++)
			{
				triangles[numtriangles]=dmodel_triangles[j];

				numtriangles++;
			}
		}
	}

	if (isempty)
	{
		return;
	}

	double distance=(center-p).getlength();

	int lod_level=0;

	while (distance>lod_distance*power(lod_mul,lod_level))
	{
			lod_level++;
	}

	if (lod_level>numlodlevels-1)
	{
		lod_level=numlodlevels-1;
	}

	for (int i=0;i<this->numtriangles[lod_level];i++)
	{
		triangles[numtriangles]=this->triangles[lod_level][i];

		numtriangles++;
	}
}

void BANANEN::SECTOR::doclip(const PLANE &plane,
                             ARRAY<TRIANGLE> &inarray,int innum,
                             ARRAY<TRIANGLE> &outarray,int &outnum) const
{
	int clipcase;

	TRIANGLE t1,t2;

	outnum=0;

	for (int i=0;i<innum;i++)
	{
		clipcase=inarray[i].clip(plane,t1,t2);

		switch (clipcase)
		{
		case 0:
			outarray[outnum]=inarray[i];

			outnum++;
		break;
		case 1:
		break;
		case 2:
			outarray[outnum]=t1;

			outnum++;

			outarray[outnum]=t2;

			outnum++;
		break;
		case 3:
			outarray[outnum]=t1;

			outnum++;
		break;
		}
	}
}

bool BANANEN::SECTOR::cube_sphere_intersect(const VECTOR<3> &pos,double radius) const
{
	double r2=square(radius);

	if      (pos.cmp[0]<Cmin.cmp[0]) {r2-=square(pos.cmp[0]-Cmin.cmp[0]);}
	else if (pos.cmp[0]>Cmax.cmp[0]) {r2-=square(pos.cmp[0]-Cmax.cmp[0]);}
	if      (pos.cmp[1]<Cmin.cmp[1]) {r2-=square(pos.cmp[1]-Cmin.cmp[1]);}
	else if (pos.cmp[1]>Cmax.cmp[1]) {r2-=square(pos.cmp[1]-Cmax.cmp[1]);}
	if      (pos.cmp[2]<Cmin.cmp[2]) {r2-=square(pos.cmp[2]-Cmin.cmp[2]);}
	else if (pos.cmp[2]>Cmax.cmp[2]) {r2-=square(pos.cmp[2]-Cmax.cmp[2]);}

	return(r2>=0);
}
