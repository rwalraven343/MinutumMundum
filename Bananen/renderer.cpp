//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Bananen.h"

static const int dynarray_capacity=256;

BANANEN::RENDERER::RENDERER(int depth,const VECTOR<3> &origin,double scale,int numstars,
                            double pleft,double pright,
                            double pbottom,double ptop,
                            double pnear,double pfar) :
	octtree(depth,origin,scale),
	view(new MATRIX<4>),
	view_inverse(new MATRIX<4>),
	projection(new MATRIX<4>),
	worldspace_triangles(dynarray_capacity),
	viewspace_triangles(dynarray_capacity),
	clipspace_triangles(dynarray_capacity),
	clipped_left(dynarray_capacity),
	clipped_right(dynarray_capacity),
	clipped_front(dynarray_capacity),
	clipped_back(dynarray_capacity),
	clipped_top(dynarray_capacity),
	clipped_bottom(dynarray_capacity),
	visible_triangles(dynarray_capacity),
	zdotn(dynarray_capacity)
{
	*projection=MATRIX<4>::projection(pleft,pright,pbottom,ptop,pnear,pfar);

	VECTOR<3> zero;
	VECTOR<3> l1_point,l2_point,l3_point,l4_point;

	l1_point.cmp[0]=pleft;
	l1_point.cmp[1]=ptop;
	l1_point.cmp[2]=-pnear;

	l2_point.cmp[0]=pright;
	l2_point.cmp[1]=ptop;
	l2_point.cmp[2]=-pnear;

	l3_point.cmp[0]=pright;
	l3_point.cmp[1]=pbottom;
	l3_point.cmp[2]=-pnear;

	l4_point.cmp[0]=pleft;
	l4_point.cmp[1]=pbottom;
	l4_point.cmp[2]=-pnear;

	l1=LINE(zero,l1_point);
	l2=LINE(zero,l2_point);
	l3=LINE(zero,l3_point);
	l4=LINE(zero,l4_point);

	VECTOR<3>  zclip_point1, zclip_point2, zclip_point3;
	VECTOR<3>   left_point1,  left_point2,  left_point3;
	VECTOR<3>  right_point1, right_point2, right_point3;
	VECTOR<3>  front_point1, front_point2, front_point3;
	VECTOR<3>   back_point1,  back_point2,  back_point3;
	VECTOR<3>    top_point1,   top_point2,   top_point3;
	VECTOR<3> bottom_point1,bottom_point2,bottom_point3;

	zclip_point1.cmp[0]=0            ;zclip_point2.cmp[0]=0            ;zclip_point3.cmp[0]=1            ;
	zclip_point1.cmp[1]=0            ;zclip_point2.cmp[1]=1            ;zclip_point3.cmp[1]=0            ;
	zclip_point1.cmp[2]=-math_epsilon;zclip_point2.cmp[2]=-math_epsilon;zclip_point3.cmp[2]=-math_epsilon;

      left_point1.cmp[0]=-1;  left_point2.cmp[0]=-1;  left_point3.cmp[0]=-1;
      left_point1.cmp[1]=1 ;  left_point2.cmp[1]=1 ;  left_point3.cmp[1]=-1;
      left_point1.cmp[2]=-1;  left_point2.cmp[2]=1 ;  left_point3.cmp[2]=1 ;

     right_point1.cmp[0]=1 ; right_point2.cmp[0]=1 ; right_point3.cmp[0]=1 ;
     right_point1.cmp[1]=1 ; right_point2.cmp[1]=1 ; right_point3.cmp[1]=-1;
     right_point1.cmp[2]=1 ; right_point2.cmp[2]=-1; right_point3.cmp[2]=-1;

     front_point1.cmp[0]=-1; front_point2.cmp[0]=1 ; front_point3.cmp[0]=1 ;
     front_point1.cmp[1]=1 ; front_point2.cmp[1]=1 ; front_point3.cmp[1]=-1;
     front_point1.cmp[2]=1 ; front_point2.cmp[2]=1 ; front_point3.cmp[2]=1 ;

      back_point1.cmp[0]=1 ;  back_point2.cmp[0]=-1;  back_point3.cmp[0]=-1;
      back_point1.cmp[1]=1 ;  back_point2.cmp[1]=1 ;  back_point3.cmp[1]=-1;
      back_point1.cmp[2]=-1;  back_point2.cmp[2]=-1;  back_point3.cmp[2]=-1;

       top_point1.cmp[0]=-1;   top_point2.cmp[0]=1 ;   top_point3.cmp[0]=1 ;
       top_point1.cmp[1]=1 ;   top_point2.cmp[1]=1 ;   top_point3.cmp[1]=1 ;
       top_point1.cmp[2]=-1;   top_point2.cmp[2]=-1;   top_point3.cmp[2]=1 ;

    bottom_point1.cmp[0]=-1;bottom_point2.cmp[0]=1 ;bottom_point3.cmp[0]=1 ;
    bottom_point1.cmp[1]=-1;bottom_point2.cmp[1]=-1;bottom_point3.cmp[1]=-1;
    bottom_point1.cmp[2]=1 ;bottom_point2.cmp[2]=1 ;bottom_point3.cmp[2]=-1;

    zclip =PLANE(zclip_point1 ,zclip_point2 ,zclip_point3 );
	left  =PLANE(left_point1  ,left_point2  ,left_point3  );
	right =PLANE(right_point1 ,right_point2 ,right_point3 );
	front =PLANE(front_point1 ,front_point2 ,front_point3 );
	back  =PLANE(back_point1  ,back_point2  ,back_point3  );
	top   =PLANE(top_point1   ,top_point2   ,top_point3   );
	bottom=PLANE(bottom_point1,bottom_point2,bottom_point3);

	this->numstars=numstars;

	stars=new VECTOR<3>[numstars];

	for (int i=0;i<numstars;i++)
	{
		stars[i].cmp[0]=RANDOM::global(-1,1);
		stars[i].cmp[1]=RANDOM::global(-1,1);
		stars[i].cmp[2]=RANDOM::global(-1,1);
	}
}

BANANEN::RENDERER::~RENDERER()
{
	delete view;
	delete view_inverse;
	delete projection;
	delete[] stars;
}

int BANANEN::RENDERER::getdepth() const
{
	return(octtree.getdepth());
}

int BANANEN::RENDERER::getindexsize(int depth) const
{
	return(octtree.getindexsize(depth));
}

bool BANANEN::RENDERER::getnode(int x,int y,int z,int depth,MATRIX<4> &m,BSPTREE **node) const
{
	return(octtree.getnode(x,y,z,depth,m,node));
}

bool BANANEN::RENDERER::setnode(int x,int y,int z,int depth,MATRIX<4> &m,BSPTREE *node)
{
	return(octtree.setnode(x,y,z,depth,m,node));
}

void BANANEN::RENDERER::setview(const MATRIX<4> &v)
{
	*view=v.getinverse();
	*view_inverse=v;
}

void BANANEN::RENDERER::setambient(const VECTOR<4> &a)
{
	ambient=a;
}

void BANANEN::RENDERER::draw(double x,double y,double w,double h,bool wireframe) const
{
	draw_stars(x,y,w,h);

	VECTOR<3> origin;

	int numworldspace_triangles=0;

	octtree.sort(((*view_inverse) * origin.gethomogeneous()).getcartesian(),
                 l1 * (*view_inverse),
                 l2 * (*view_inverse),
                 l3 * (*view_inverse),
                 l4 * (*view_inverse),
                 worldspace_triangles,numworldspace_triangles);

	for (int i=numworldspace_triangles-1,j=0;i>=0;i--,j++)
	{
		viewspace_triangles[j]=worldspace_triangles[i] * (*view);
	}

	int numclipped_z;

	doclip(zclip,viewspace_triangles,numworldspace_triangles,clipspace_triangles,numclipped_z);

	for (int i=0;i<numclipped_z;i++)
	{
		clipspace_triangles[i]=clipspace_triangles[i] * (*projection);
	}

	int numclipped_left;
	int numclipped_right;
	int numclipped_front;
	int numclipped_back;
	int numclipped_top;
	int numclipped_bottom;

	doclip(left  ,clipspace_triangles,numclipped_z    ,clipped_left  ,numclipped_left);
	doclip(right ,clipped_left       ,numclipped_left ,clipped_right ,numclipped_right);
	doclip(front ,clipped_right      ,numclipped_right,clipped_front ,numclipped_front);
	doclip(back  ,clipped_front      ,numclipped_front,clipped_back  ,numclipped_back);
	doclip(top   ,clipped_back       ,numclipped_back ,clipped_top   ,numclipped_top);
	doclip(bottom,clipped_top        ,numclipped_top  ,clipped_bottom,numclipped_bottom);

	VECTOR<3> zaxis;
	VECTOR<3> normal;

	zaxis.setortho(2);

	VECTOR<3> vertex0,vertex1,vertex2;

	double zdotn;

	int numvisible=0;

	for (int i=0;i<numclipped_bottom;i++)
	{
		vertex0=clipped_bottom[i].vertex[0];
		vertex1=clipped_bottom[i].vertex[1];
		vertex2=clipped_bottom[i].vertex[2];

		normal=((vertex1-vertex0).getcrossproduct(vertex2-vertex0)).getnormal();

		zdotn=zaxis.getdotproduct(normal);

		if (zdotn>=0)
		{
			visible_triangles[numvisible]=clipped_bottom[i];

			this->zdotn[numvisible]=zdotn;

			numvisible++;
		}
	}

	::VECTOR scoords0,scoords1,scoords2;

	double shade_depth0,shade_depth1,shade_depth2;

	double shade;

	double red,green,blue,alpha;

	if (!wireframe)
	{
		VIDEO::BATCH::draw_polygon_start();
	}

	for (int i=0;i<numvisible;i++)
	{
		vertex0=visible_triangles[i].vertex[0];
		vertex1=visible_triangles[i].vertex[1];
		vertex2=visible_triangles[i].vertex[2];

		scoords0.x=x + (w/2) + vertex0.cmp[0]*(w/2);
		scoords0.y=y + (h/2) + vertex0.cmp[1]*(-h/2);

		scoords1.x=x + (w/2) + vertex1.cmp[0]*(w/2);
		scoords1.y=y + (h/2) + vertex1.cmp[1]*(-h/2);

		scoords2.x=x + (w/2) + vertex2.cmp[0]*(w/2);
		scoords2.y=y + (h/2) + vertex2.cmp[1]*(-h/2);

		shade_depth0=(1-((vertex0.cmp[2]+1)/2))+ambient.cmp[3]; if (shade_depth0>1) {shade_depth0=1;}
		shade_depth1=(1-((vertex1.cmp[2]+1)/2))+ambient.cmp[3]; if (shade_depth1>1) {shade_depth1=1;}
		shade_depth2=(1-((vertex2.cmp[2]+1)/2))+ambient.cmp[3]; if (shade_depth2>1) {shade_depth2=1;}

		shade=this->zdotn[i]*((shade_depth0+shade_depth1+shade_depth2)/3);

		if (visible_triangles[i].hascolour)
		{
			red  =visible_triangles[i].colour.cmp[0];
			green=visible_triangles[i].colour.cmp[1];
			blue =visible_triangles[i].colour.cmp[2];
			alpha=visible_triangles[i].colour.cmp[3];
		}
		else
		{
			red  =1;
			green=1;
			blue =1;
			alpha=1;
		}

		red  =red  *shade + ambient.cmp[0]; if (red  >1) {red  =1;}
		green=green*shade + ambient.cmp[1]; if (green>1) {green=1;}
		blue =blue *shade + ambient.cmp[2]; if (blue >1) {blue =1;}

		if (!wireframe)
		{
			VIDEO::BATCH::draw_polygon(scoords0.x,scoords0.y,scoords1.x,scoords1.y,scoords2.x,scoords2.y,
                                       red,green,blue,alpha);
		}
		else
		{
			VIDEO::draw_polygon(scoords0.x,scoords0.y,scoords1.x,scoords1.y,scoords2.x,scoords2.y,
                                red,green,blue,alpha);

			VIDEO::draw_line(scoords0.x,scoords0.y,scoords1.x,scoords1.y,1,1,1,1);
			VIDEO::draw_line(scoords1.x,scoords1.y,scoords2.x,scoords2.y,1,1,1,1);
			VIDEO::draw_line(scoords2.x,scoords2.y,scoords0.x,scoords0.y,1,1,1,1);
		}
	}

	if (!wireframe)
	{
		VIDEO::BATCH::draw_polygon_end();
	}
}

void BANANEN::RENDERER::draw_stars(double x,double y,double w,double h) const
{
	VECTOR<3> tstar;

	MATRIX<3> view_stars=view->getminor(3,3);

	::VECTOR scoords;

	VIDEO::BATCH::draw_point_sized_start(4);

	for (int i=0;i<numstars;i++)
	{
		tstar=view_stars * stars[i];

		if (tstar.cmp[2]<=-math_epsilon)
		{
			tstar=((*projection) * tstar.gethomogeneous()).getcartesian();

			if (tstar.cmp[0]>=-1 && tstar.cmp[0]<=1)
			{
				if (tstar.cmp[1]>=-1 && tstar.cmp[1]<=1)
				{
					scoords.x=x + (w/2) + tstar.cmp[0]*(w/2);
					scoords.y=y + (h/2) + tstar.cmp[1]*(-h/2);

					VIDEO::BATCH::draw_point_sized(scoords.x,scoords.y,1,1,1,1);
				}
			}
		}
	}

	VIDEO::BATCH::draw_point_sized_end();
}

void BANANEN::RENDERER::doclip(const PLANE &plane,
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
