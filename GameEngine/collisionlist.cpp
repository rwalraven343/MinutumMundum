//Code by Rogier Walraven. Do not distribute.

#include "GameLib\\GameLib.h"
#include "appmath.h"
#include "random.h"
#include "Geometry.h"

extern const int collision_max_triangles_per_shape;
extern const int collision_max_quads_per_shape;
extern const int collision_max_circles_per_shape;
extern const int collision_max_shapes_per_list;

COLLISIONLIST::COLLISIONLIST() : numshapes(0),friction(0),refraction(1)
{
	shapes=new SHAPE[collision_max_shapes_per_list];

	isect_exists  =new bool  [collision_max_shapes_per_list];
	isect         =new ISECT [collision_max_shapes_per_list];
	existing_isect=new ISECT [collision_max_shapes_per_list];
	isect_distance=new double[collision_max_shapes_per_list];
}

COLLISIONLIST::~COLLISIONLIST()
{
	delete[] shapes;
	delete[] isect_exists;
	delete[] isect;
	delete[] existing_isect;
	delete[] isect_distance;
}

COLLISIONLIST& COLLISIONLIST::operator=(const COLLISIONLIST &other)
{
	numshapes=other.numshapes;

	for (int i=0;i<numshapes;i++)
	{
		shapes[i]=other.shapes[i];
	}

	friction  =other.friction;
	refraction=other.refraction;

	return(*this);
}

void COLLISIONLIST::change_origin(const VECTOR &new_origin)
{
	for (int i=0;i<numshapes;i++)
	{
		shapes[i].change_origin(new_origin);
	}
}

bool COLLISIONLIST::getintersection_seg(VECTOR &i,VECTOR &n,double &absorp,void **obj,const LINE &l,double time) const
{
	if (l.redundant || numshapes==0)
	{
		return(false);
	}

	for (int f=0;f<numshapes;f++)
	{
		isect_exists[f]=shapes[f].getintersection_seg(isect[f].pos,isect[f].norm,isect[f].absorp,l,time);
		isect[f].obj   =shapes[f].object;
	}

	int numexisting=0;

	for (int f=0;f<numshapes;f++)
	{
		if (isect_exists[f])
		{
			existing_isect[numexisting]=isect[f];
			numexisting++;
		}
	}

	if (numexisting>0)
	{
		for (int f=0;f<numexisting;f++)
		{
			isect_distance[f]=(existing_isect[f].pos-l.start).getlength_squared();
		}

		int g=0;
		double closest=isect_distance[g];

		for (int f=1;f<numexisting;f++)
		{
			if (isect_distance[f]<closest)
			{
				closest=isect_distance[f];
				g=f;
			}
		}

		i     =existing_isect[g].pos;
		n     =existing_isect[g].norm;
		absorp=existing_isect[g].absorp;

		*obj=existing_isect[g].obj;

		return(true);
	}

	return(false);
}

bool COLLISIONLIST::iswithin(double &refract,const VECTOR &p,double time) const
{
	if (numshapes==0)
	{
		return(false);
	}

	double dens;

	double refract_add=0;
	double dens_add=0;

	for (int i=0;i<numshapes;i++)
	{
		if (shapes[i].iswithin(refract,dens,p,time))
		{
			refract_add+=refract;
			dens_add   +=dens;
		}
	}

	if (!equalszero(dens_add))
	{
		refract=refract_add/dens_add;

		return(true);
	}

	return(false);
}

bool COLLISIONLIST::iswithin_solid(VECTOR &snorm,VECTOR &svel,double &sdens,double &ssfric,double &sdfric,const VECTOR &p,double time) const
{
	if (numshapes==0)
	{
		return(false);
	}

	VECTOR snorm_add;
	VECTOR svel_add;
	double sdens_add=0;
	double ssfric_add=0;
	double sdfric_add=0;

	for (int i=0;i<numshapes;i++)
	{
		if (shapes[i].iswithin_solid(snorm,svel,sdens,ssfric,sdfric,p,time))
		{
			snorm_add +=snorm;
			svel_add  +=svel;
			sdens_add +=sdens;
			ssfric_add+=ssfric;
			sdfric_add+=sdfric;
		}
	}

	if (!equalszero(sdens_add))
	{
		snorm =snorm_add.getnormal();
		svel  =svel_add/sdens_add;
		sdens =sdens_add;
		ssfric=ssfric_add/sdens_add;
		sdfric=sdfric_add/sdens_add;

		return(true);
	}

	return(false);
}

bool COLLISIONLIST::iswithin_fluid(VECTOR &fvel,VECTOR &fpress_diff,double &fvisc,const VECTOR &p,double time) const
{
	if (numshapes==0)
	{
		return(false);
	}

	double fdens;

	VECTOR fvel_add;
	double fdens_add=0;
	VECTOR fpress_diff_add;
	double fvisc_add=0;

	for (int i=0;i<numshapes;i++)
	{
		if (shapes[i].iswithin_fluid(fvel,fdens,fpress_diff,fvisc,p,time))
		{
			fvel_add       +=fvel;
			fdens_add      +=fdens;
			fpress_diff_add+=fpress_diff;
			fvisc_add      +=fvisc;
		}
	}

	if (!equalszero(fdens_add))
	{
		fvel       =fvel_add/fdens_add;
		fpress_diff=fpress_diff_add;
		fvisc      =fvisc_add;

		return(true);
	}

	return(false);
}

COLLISIONLIST::SHAPE::SHAPE() : radius(0),numtriangles(0),numquads(0),numcircles(0),object(0)
{
	triangles=new TRIANGLE[collision_max_triangles_per_shape];
	quads    =new QUAD    [collision_max_quads_per_shape];
	circles  =new CIRCLE  [collision_max_circles_per_shape];

	isect_exists  =new bool  [collision_max_triangles_per_shape+collision_max_quads_per_shape+collision_max_circles_per_shape];
	isect         =new ISECT [collision_max_triangles_per_shape+collision_max_quads_per_shape+collision_max_circles_per_shape];
	existing_isect=new ISECT [collision_max_triangles_per_shape+collision_max_quads_per_shape+collision_max_circles_per_shape];
	isect_distance=new double[collision_max_triangles_per_shape+collision_max_quads_per_shape+collision_max_circles_per_shape];
}

COLLISIONLIST::SHAPE::~SHAPE()
{
	delete[] triangles;
	delete[] quads;
	delete[] circles;
	delete[] isect_exists;
	delete[] isect;
	delete[] existing_isect;
	delete[] isect_distance;
}

COLLISIONLIST::SHAPE& COLLISIONLIST::SHAPE::operator=(const SHAPE &other)
{
	position=other.position;
	radius  =other.radius;

	numtriangles=other.numtriangles;
	numquads    =other.numquads;
	numcircles  =other.numcircles;

	for (int i=0;i<numtriangles;i++)
	{
		triangles[i]=other.triangles[i];
	}

	for (int i=0;i<numquads;i++)
	{
		quads[i]=other.quads[i];
	}

	for (int i=0;i<numcircles;i++)
	{
		circles[i]=other.circles[i];
	}

	object=other.object;

	return(*this);
}

void COLLISIONLIST::SHAPE::change_origin(const VECTOR &new_origin)
{
	position-=new_origin;

	for (int i=0;i<numtriangles;i++)
	{
		triangles[i]-=new_origin;
	}

	for (int i=0;i<numquads;i++)
	{
		quads[i]-=new_origin;
	}

	for (int i=0;i<numcircles;i++)
	{
		circles[i]-=new_origin;
	}
}

bool COLLISIONLIST::SHAPE::getintersection_seg(VECTOR &i,VECTOR &n,double &absorp,const LINE &l,double time) const
{
	if (l.redundant || (numtriangles==0 && numquads==0 && numcircles==0))
	{
		return(false);
	}

	if ((l.start-position).getlength_squared()>square(radius*collision_bounding_radius_mul))
	{
		if (!CIRCLE(position,radius*collision_bounding_radius_mul).getintersection_seg(i,l,time))
		{
			return(false);
		}
	}

	int counter=0;

	for (int f=0;f<numtriangles;f++)
	{
		isect_exists[counter]=triangles[f].getintersection_seg(isect[counter].pos,isect[counter].norm,l,time);
		isect[counter].absorp=triangles[f].absorption;

		if (isect_exists[counter])
		{
			if ((isect[counter].pos-position).getlength_squared()>square(radius*collision_bounding_radius_mul))
			{
				isect_exists[counter]=false;
			}
		}

		counter++;
	}

	for (int f=0;f<numquads;f++)
	{
		isect_exists[counter]=quads[f].getintersection_seg(isect[counter].pos,isect[counter].norm,l,time);
		isect[counter].absorp=quads[f].absorption;

		if (isect_exists[counter])
		{
			if ((isect[counter].pos-position).getlength_squared()>square(radius*collision_bounding_radius_mul))
			{
				isect_exists[counter]=false;
			}
		}

		counter++;
	}

	for (int f=0;f<numcircles;f++)
	{
		isect_exists[counter]=circles[f].getintersection_seg(isect[counter].pos,isect[counter].norm,l,time);
		isect[counter].absorp=circles[f].absorption;

		if (isect_exists[counter])
		{
			if ((isect[counter].pos-position).getlength_squared()>square(radius*collision_bounding_radius_mul))
			{
				isect_exists[counter]=false;
			}
		}

		counter++;
	}

	int numexisting=0;

	for (int f=0;f<counter;f++)
	{
		if (isect_exists[f])
		{
			existing_isect[numexisting]=isect[f];
			numexisting++;
		}
	}

	if (numexisting>0)
	{
		for (int f=0;f<numexisting;f++)
		{
			isect_distance[f]=(existing_isect[f].pos-l.start).getlength_squared();
		}

		int g=0;
		double closest=isect_distance[g];

		for (int f=1;f<numexisting;f++)
		{
			if (isect_distance[f]<closest)
			{
				closest=isect_distance[f];
				g=f;
			}
		}

		i     =existing_isect[g].pos;
		n     =existing_isect[g].norm;
		absorp=existing_isect[g].absorp;

		return(true);
	}

	return(false);
}

bool COLLISIONLIST::SHAPE::iswithin(double &refract,double &dens,const VECTOR &p,double time) const
{
	if (numtriangles==0 && numquads==0 && numcircles==0)
	{
		return(false);
	}

	if ((p-position).getlength_squared()>square(radius*collision_bounding_radius_mul))
	{
		return(false);
	}

	double refract_add=0;
	double dens_add=0;

	int counter=0;

	for (int i=0;i<numtriangles;i++)
	{
		if (triangles[i].iswithin(p,time))
		{
			refract_add+=triangles[i].refraction*triangles[i].density;
			dens_add   +=triangles[i].density;

			counter++;
		}
	}

	for (int i=0;i<numquads;i++)
	{
		if (quads[i].iswithin(p,time))
		{
			refract_add+=quads[i].refraction*quads[i].density;
			dens_add   +=quads[i].density;

			counter++;
		}
	}

	for (int i=0;i<numcircles;i++)
	{
		if (circles[i].iswithin(p,time))
		{
			refract_add+=circles[i].refraction*circles[i].density;
			dens_add   +=circles[i].density;

			counter++;
		}
	}

	if (counter>0)
	{
		refract=refract_add;
		dens   =dens_add;

		return(true);
	}

	return(false);
}

bool COLLISIONLIST::SHAPE::iswithin_solid(VECTOR &snorm,VECTOR &svel,double &sdens,double &ssfric,double &sdfric,const VECTOR &p,double time) const
{
	if (numtriangles==0 && numquads==0)
	{
		return(false);
	}

	if ((p-position).getlength_squared()>square(radius*collision_bounding_radius_mul))
	{
		return(false);
	}

	VECTOR snorm_add;
	VECTOR svel_add;
	double sdens_add=0;
	double ssfric_add=0;
	double sdfric_add=0;

	int counter=0;

	for (int i=0;i<numtriangles;i++)
	{
		if (triangles[i].iswithin(snorm,svel,p,time))
		{
			snorm_add +=snorm*triangles[i].density;
			svel_add  +=svel*triangles[i].density;
			sdens_add +=triangles[i].density;
			ssfric_add+=triangles[i].sfriction*triangles[i].density;
			sdfric_add+=triangles[i].dfriction*triangles[i].density;

			counter++;
		}
	}

	for (int i=0;i<numquads;i++)
	{
		if (quads[i].iswithin(snorm,svel,p,time))
		{
			snorm_add +=snorm*quads[i].density;
			svel_add  +=svel*quads[i].density;
			sdens_add +=quads[i].density;
			ssfric_add+=quads[i].sfriction*quads[i].density;
			sdfric_add+=quads[i].dfriction*quads[i].density;

			counter++;
		}
	}

	if (counter>0)
	{
		snorm =snorm_add;
		svel  =svel_add;
		sdens =sdens_add;
		ssfric=ssfric_add;
		sdfric=sdfric_add;

		return(true);
	}

	return(false);
}

bool COLLISIONLIST::SHAPE::iswithin_fluid(VECTOR &fvel,double &fdens,VECTOR &fpress_diff,double &fvisc,const VECTOR &p,double time) const
{
	if (numcircles==0)
	{
		return(false);
	}

	if ((p-position).getlength_squared()>square(radius*collision_bounding_radius_mul))
	{
		return(false);
	}

	VECTOR fvel_add;
	double fdens_add=0;
	VECTOR fpress_diff_add;
	double fvisc_add=0;

	int counter=0;

	for (int i=0;i<numcircles;i++)
	{
		if (circles[i].iswithin(fdens,fpress_diff,fvisc,p,time))
		{
			fvel_add       +=circles[i].velocity*fdens;
			fdens_add      +=fdens;
			fpress_diff_add+=fpress_diff;
			fvisc_add      +=fvisc;

			counter++;
		}
	}

	if (counter>0)
	{
		fvel       =fvel_add;
		fdens      =fdens_add;
		fpress_diff=fpress_diff_add;
		fvisc      =fvisc_add;

		return(true);
	}

	return(false);
}
