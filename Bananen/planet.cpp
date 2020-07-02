//Code by Rogier Walraven. Do not distribute.

#include "..\\GameEngine\\GameEngine.h"
#include "Bananen.h"

BANANEN::PLANET::PLANET(int depth,const VECTOR<3> &origin,double scale,int numstars,
                        double pleft,double pright,
                        double pbottom,double ptop,
                        double pnear,double pfar,
                        int numlodlevels,int numstepsa,int numstepsb,
		                const char *colourmap_filename,const char *heightmap_filename,
		                const QUATERNION &orientation,
                        double inner_radius,double outer_radius,
                        double lod_distance,double lod_mul) :
	RENDERER(depth,origin,scale,numstars,pleft,pright,pbottom,ptop,pnear,pfar)
{
	dynamic_models=new DYNAMIC_MODEL*[max_dynamic_models_per_planet];

	numdynamic_models=0;

	TRIANGLE **triangles;
	int *numtriangles;

	triangles=new TRIANGLE*[numlodlevels];

	numtriangles=new int[numlodlevels];

	IMAGE colourmap(colourmap_filename);
	IMAGE heightmap(heightmap_filename);

	MATRIX<3> m3=orientation.getrotationmatrix();
	MATRIX<4> m4;

	for (int l=numlodlevels-1;l>=0;l--)
	{
		VECTOR<3> *points =new VECTOR<3>[numstepsa*numstepsb];
		VECTOR<4> *colours=new VECTOR<4>[numstepsa*numstepsb];

		double A,B;

		double height;

		int i=0;

		for (int b=0;b<numstepsb;b++)
		{
			B=b*(math_pi/numstepsb);

			for (int a=0;a<numstepsa;a++)
			{
				A=a*((2*math_pi)/numstepsa);

				points[i].cmp[0]=cosine(A)*sine(B);
				points[i].cmp[1]=sine(A)*sine(B);
				points[i].cmp[2]=cosine(B);

				colours[i]=colourmap.getcolour_linear((A/(2*math_pi))*colourmap.getwidth(),
                                                      (B/(math_pi))*colourmap.getheight());

				height=heightmap.getcolour_linear((A/(2*math_pi))*heightmap.getwidth(),
                                                  (B/(math_pi))*heightmap.getheight()).cmp[0];

				points[i]=points[i]*(inner_radius+(outer_radius-inner_radius)*height);

				i++;
			}
		}

		for (int i=0;i<numstepsa*numstepsb;i++)
		{
			points[i]=m3 * points[i];
		}

		numtriangles[l]=(numstepsa*numstepsb*2) - (numstepsa*2);

		triangles[l]=new TRIANGLE[numtriangles[l]];

		i=0;

		for (int b=1;b<numstepsb+1;b++)
		{
			for (int a=0;a<numstepsa;a++)
			{
				if (b!=numstepsb)
				{
					triangles[l][i].vertex[0].cmp[0]=points[((a  )%numstepsa)+((b  )%numstepsb)*numstepsa].cmp[0];
					triangles[l][i].vertex[0].cmp[1]=points[((a  )%numstepsa)+((b  )%numstepsb)*numstepsa].cmp[1];
					triangles[l][i].vertex[0].cmp[2]=points[((a  )%numstepsa)+((b  )%numstepsb)*numstepsa].cmp[2];

					triangles[l][i].vertex[1].cmp[0]=points[((a+1)%numstepsa)+((b  )%numstepsb)*numstepsa].cmp[0];
					triangles[l][i].vertex[1].cmp[1]=points[((a+1)%numstepsa)+((b  )%numstepsb)*numstepsa].cmp[1];
					triangles[l][i].vertex[1].cmp[2]=points[((a+1)%numstepsa)+((b  )%numstepsb)*numstepsa].cmp[2];

					triangles[l][i].vertex[2].cmp[0]=points[((a  )%numstepsa)+((b-1)%numstepsb)*numstepsa].cmp[0];
					triangles[l][i].vertex[2].cmp[1]=points[((a  )%numstepsa)+((b-1)%numstepsb)*numstepsa].cmp[1];
					triangles[l][i].vertex[2].cmp[2]=points[((a  )%numstepsa)+((b-1)%numstepsb)*numstepsa].cmp[2];

					triangles[l][i].colour=
                    (colours[((a  )%numstepsa)+((b  )%numstepsb)*numstepsa] +
                     colours[((a+1)%numstepsa)+((b  )%numstepsb)*numstepsa] +
                     colours[((a  )%numstepsa)+((b-1)%numstepsb)*numstepsa])/3;
                    triangles[l][i].hascolour=true;

					i++;
				}

				if (b!=1)
				{
					triangles[l][i].vertex[0].cmp[0]=points[((a+1)%numstepsa)+((b  )%numstepsb)*numstepsa].cmp[0];
					triangles[l][i].vertex[0].cmp[1]=points[((a+1)%numstepsa)+((b  )%numstepsb)*numstepsa].cmp[1];
					triangles[l][i].vertex[0].cmp[2]=points[((a+1)%numstepsa)+((b  )%numstepsb)*numstepsa].cmp[2];

					if (b==numstepsb)
					{
						triangles[l][i].vertex[0]=-triangles[l][i].vertex[0];
					}

					triangles[l][i].vertex[1].cmp[0]=points[((a+1)%numstepsa)+((b-1)%numstepsb)*numstepsa].cmp[0];
					triangles[l][i].vertex[1].cmp[1]=points[((a+1)%numstepsa)+((b-1)%numstepsb)*numstepsa].cmp[1];
					triangles[l][i].vertex[1].cmp[2]=points[((a+1)%numstepsa)+((b-1)%numstepsb)*numstepsa].cmp[2];

					triangles[l][i].vertex[2].cmp[0]=points[((a  )%numstepsa)+((b-1)%numstepsb)*numstepsa].cmp[0];
					triangles[l][i].vertex[2].cmp[1]=points[((a  )%numstepsa)+((b-1)%numstepsb)*numstepsa].cmp[1];
					triangles[l][i].vertex[2].cmp[2]=points[((a  )%numstepsa)+((b-1)%numstepsb)*numstepsa].cmp[2];

					triangles[l][i].colour=
                    (colours[((a+1)%numstepsa)+((b  )%numstepsb)*numstepsa] +
                     colours[((a+1)%numstepsa)+((b-1)%numstepsb)*numstepsa] +
                     colours[((a  )%numstepsa)+((b-1)%numstepsb)*numstepsa])/3;
                    triangles[l][i].hascolour=true;

					i++;
				}
			}
		}

		delete[] points;
		delete[] colours;

		numstepsa*=2;
		numstepsb*=2;
	}

	int indexsize=RENDERER::getindexsize(depth);

	for (int z=-indexsize;z<indexsize;z++)
	{
		for (int y=-indexsize;y<indexsize;y++)
		{
			for (int x=-indexsize;x<indexsize;x++)
			{
				SECTOR *sector=new SECTOR(x,y,z,depth,inner_radius,outer_radius,
                                          numlodlevels,triangles,numtriangles,
                                          lod_distance,lod_mul,
                                          dynamic_models,&numdynamic_models);

				if (RENDERER::setnode(x,y,z,depth,m4,sector))
				{
					(*sector)*=m4;
				}
				else
				{
					delete sector;
				}
			}
		}
	}

	for (int l=numlodlevels-1;l>=0;l--)
	{
		delete[] triangles[l];
	}

	delete[] triangles;
	delete[] numtriangles;
}

BANANEN::PLANET::~PLANET()
{
	delete[] dynamic_models;
}

void BANANEN::PLANET::add_dynamic_model(DYNAMIC_MODEL &dmodel)
{
	for (int i=0;i<numdynamic_models;i++)
	{
		if (dynamic_models[i]==&dmodel)
		{
			return;
		}
	}

	dynamic_models[numdynamic_models]=&dmodel;

	numdynamic_models++;
}

void BANANEN::PLANET::remove_dynamic_model(const DYNAMIC_MODEL &dmodel)
{
	for (int i=0;i<numdynamic_models;i++)
	{
		if (dynamic_models[i]==&dmodel)
		{
			for (int j=i;j<numdynamic_models;j++)
			{
				if (j+1<max_dynamic_models_per_planet)
				{
					dynamic_models[j]=dynamic_models[j+1];
				}
			}

			numdynamic_models--;

			return;
		}
	}
}

void BANANEN::PLANET::begin_first_frame()
{
	begin_frame_time=SYSTEM::seconds_since_startup()*timestep_scale;

	real_time       =0;
	real_time_offset=begin_frame_time;

	simulation_time=0;

	timestep      =timestep_min;
	timestep_error=0;
}

void BANANEN::PLANET::end_frame()
{
	double timestep_new;

	end_frame_time=SYSTEM::seconds_since_startup()*timestep_scale;

	timestep_new=end_frame_time - begin_frame_time;

	begin_frame_time=end_frame_time;

	if (timestep_new<0)
	{
		begin_first_frame();

		return;
	}

	real_time=begin_frame_time - real_time_offset;

	timestep_error+=timestep_new - timestep;

	if (absolute(timestep_error)>timestep_max_error)
	{
		timestep_error=0;
	}

	timestep=timestep_new;
}

void BANANEN::PLANET::integrate()
{
	double timestep_plus_error=timestep+timestep_error;

	if (timestep_plus_error>=0)
	{
		integrate(timestep_plus_error);

		timestep_error=0;

		return;
	}

	timestep_error+=timestep;
}

void BANANEN::PLANET::integrate(double timestep)
{
	for (int i=0;i<numdynamic_models;i++)
	{
		dynamic_models[i]->control(timestep);
	}

	for (int i=0;i<numdynamic_models;i++)
	{
		if (dynamic_models[i]->haspath())
		{
			dynamic_models[i]->integrate(timestep);
		}
	}
}
