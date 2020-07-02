//Code by Rogier Walraven. Do not distribute.

#include "GameLib\\GameLib.h"
#include "appmath.h"
#include "random.h"
#include "Geometry.h"
#include "physics.h"
#include "pointmass.h"
#include "solid.h"
#include "liquid.h"
#include "gas.h"
#include "solid_object.h"
#include "liquid_object.h"
#include "particle_emitter.h"
#include "beam.h"
#include "motor.h"
#include "constraint_solid.h"
#include "constraint_liquid.h"
#include "map.h"

MAP::MAP(const MAP_PROPERTIES &props)
{
	textures=VIDEO::loadtileset(props.tileset_filename,
                                props.bitmapwidth,props.bitmapheight,
                                props.tilewidth,props.tileheight,
                                props.transparentx,props.transparenty,
                                props.numtextures);

	primary_layer.init   (props,props.primary_layer   ,0             ,textures);
	secondary_layer.init (props,props.secondary_layer ,&primary_layer,textures);
	tertiary_layer.init  (props,props.tertiary_layer  ,&primary_layer,textures);
	foreground_layer.init(props,props.foreground_layer,&primary_layer,textures);
	background_layer.init(props,props.background_layer,&primary_layer,textures);

	skyplane=VIDEO::loadtexture(props.skyplane_filename);

	ambient_sounds=0;

	if (props.numambient_sounds)
	{
		ambient_sounds=new AMBIENT_SOUND[props.numambient_sounds];

		for (int i=0;i<props.numambient_sounds;i++)
		{
			ambient_sounds[i].source=new AUDIO::SOURCE(props.ambient_sounds[i].sample_filename,props.ambient_sounds[i].loudness);

			ambient_sounds[i].starttime  =RANDOM::global.compact_normal(0,props.ambient_sounds[i].silence_duration*2);
			ambient_sounds[i].currenttime=0;
		}
	}

	tilewidth =props.tilewidth *props.tilewidth_scale;
	tileheight=props.tileheight*props.tileheight_scale;

	properties=&props;
}

MAP::~MAP()
{
	VIDEO::freetileset(textures,properties->numtextures);
	VIDEO::freetexture(skyplane);

	delete[] ambient_sounds;
}

const MAP_PROPERTIES& MAP::getproperties() const
{
	return(*properties);
}

const MAP::LAYER& MAP::getprimary_layer() const
{
	return(primary_layer);
}

const MAP::LAYER& MAP::getsecondary_layer() const
{
	return(secondary_layer);
}

const MAP::LAYER& MAP::gettertiary_layer() const
{
	return(tertiary_layer);
}

void MAP::getfictitious_forces(VECTOR &axle_pos,double &angular_vel,VECTOR &linear_acc,double &angular_acc,double timestep) const
{
	double linear_motion_time =primary_layer.getlinear_motion_time();
	double angular_motion_time=primary_layer.getangular_motion_time();

	axle_pos=primary_layer.getcenter();

	angular_vel=primary_layer.getangular_velocity(angular_motion_time,timestep);

	VECTOR gravity;

	if (!properties->local_gravity)
	{
		gravity=properties->gravity;
	}
	else
	{
		gravity.setzero();
	}

	linear_acc=primary_layer.getlinear_acceleration(linear_motion_time,timestep) - gravity;

	if (properties->local_gravity)
	{
		gravity=properties->gravity;
	}
	else
	{
		gravity.setzero();
	}

	linear_acc=linear_acc.getrotated(-(primary_layer.getorientation()+(primary_layer.getangular_velocity(angular_motion_time,timestep/2)*(timestep/2)))) - gravity;

	angular_acc=primary_layer.getangular_acceleration(angular_motion_time,timestep);
}

void MAP::fillclist_primary  (COLLISIONLIST &clist,const VECTOR &pos,double radius,double timestep) const
{
	primary_layer.fillclist(clist,pos,radius,timestep);

	clist.friction  =properties->friction;
	clist.refraction=properties->refraction;
}

void MAP::fillclist_secondary(COLLISIONLIST &clist,const VECTOR &pos,double radius,double timestep) const
{
	secondary_layer.fillclist(clist,pos,radius,timestep);

	clist.friction  =properties->friction;
	clist.refraction=properties->refraction;
}

void MAP::fillclist_tertiary (COLLISIONLIST &clist,const VECTOR &pos,double radius,double timestep) const
{
	tertiary_layer.fillclist(clist,pos,radius,timestep);

	clist.friction  =properties->friction;
	clist.refraction=properties->refraction;
}

void MAP::integrate(double timestep)
{
	primary_layer.integrate(timestep);
	secondary_layer.integrate(timestep);
	tertiary_layer.integrate(timestep);

	for (int i=0;i<properties->numambient_sounds;i++)
	{
		if (ambient_sounds[i].source->initial())
		{
			ambient_sounds[i].currenttime+=timestep;
		}
	}
}

void MAP::light_primary   (const VECTOR &pos,double radius) const
{
	primary_layer.light(pos,radius);
}

void MAP::light_secondary (const VECTOR &pos,double radius) const
{
	secondary_layer.light(pos,radius);
}

void MAP::light_tertiary  (const VECTOR &pos,double radius) const
{
	tertiary_layer.light(pos,radius);
}

void MAP::light_foreground(const VECTOR &pos,double radius) const
{
	foreground_layer.light(pos,radius);
}

void MAP::light_background(const VECTOR &pos,double radius) const
{
	background_layer.light(pos,radius);
}

void MAP::draw_primary   (const VECTOR &pos,double radius,double alpha) const
{
	primary_layer.draw(pos,radius,alpha);
}

void MAP::draw_primary   (const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const
{
	primary_layer.draw(pos,radius,r,g,b,a,hud);
}

void MAP::draw_secondary (const VECTOR &pos,double radius,double alpha) const
{
	secondary_layer.draw(pos,radius,alpha);
}

void MAP::draw_secondary (const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const
{
	secondary_layer.draw(pos,radius,r,g,b,a,hud);
}

void MAP::draw_tertiary  (const VECTOR &pos,double radius,double alpha) const
{
	tertiary_layer.draw(pos,radius,alpha);
}

void MAP::draw_tertiary  (const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const
{
	tertiary_layer.draw(pos,radius,r,g,b,a,hud);
}

void MAP::draw_foreground(const VECTOR &pos,double radius,double alpha) const
{
	foreground_layer.draw(pos,radius,alpha);
}

void MAP::draw_background(const VECTOR &pos,double radius,double alpha) const
{
	background_layer.draw(pos,radius,alpha);
}

void MAP::draw_skyplane  (const VECTOR &pos,double radius) const
{
	VECTOR position_primary   =primary_layer.getposition();
	double orientation_primary=primary_layer.getorientation();

	VECTOR center=primary_layer.getcenter();

	VECTOR p=pos;

	p=p.gettransformed_to  (center,0);
	p=p.gettransformed_from(center+position_primary,orientation_primary);

	VECTOR q1=p+VECTOR(-radius,-radius);
	VECTOR q2=p+VECTOR( radius,-radius);
	VECTOR q3=p+VECTOR( radius, radius);
	VECTOR q4=p+VECTOR(-radius, radius);

	q1=q1.gettransformed_to(center+position_primary,orientation_primary);
	q2=q2.gettransformed_to(center+position_primary,orientation_primary);
	q3=q3.gettransformed_to(center+position_primary,orientation_primary);
	q4=q4.gettransformed_to(center+position_primary,orientation_primary);

	q1=q1.gettransformed_from(center,0);
	q2=q2.gettransformed_from(center,0);
	q3=q3.gettransformed_from(center,0);
	q4=q4.gettransformed_from(center,0);

	VIDEO::draw_quad_texture(q1.x,q1.y,q2.x,q2.y,q3.x,q3.y,q4.x,q4.y,skyplane,1);
}

void MAP::sound(const VECTOR &pos,double radius) const
{
	primary_layer.sound(pos,radius);
	secondary_layer.sound(pos,radius);
	tertiary_layer.sound(pos,radius);

	if (properties->numambient_sounds)
	{
		VECTOR position_primary   =primary_layer.getposition();
		double orientation_primary=primary_layer.getorientation();

		VECTOR linear_velocity_primary =primary_layer.getlinear_velocity();
		double angular_velocity_primary=primary_layer.getangular_velocity();

		VECTOR center=primary_layer.getcenter();

		VECTOR p=pos;

		p=p.gettransformed_to  (center,0);
		p=p.gettransformed_from(center+position_primary,orientation_primary);

		VECTOR spos;
		VECTOR svel;

		for (int i=0;i<properties->numambient_sounds;i++)
		{
			if ((properties->ambient_sounds[i].position-p).getlength_squared()<=square(properties->ambient_sounds[i].radius+radius))
			{
				if (ambient_sounds[i].source->initial())
				{
					if (ambient_sounds[i].currenttime>=ambient_sounds[i].starttime)
					{
						ambient_sounds[i].position=properties->ambient_sounds[i].position;

						ambient_sounds[i].position.x+=RANDOM::global.compact_normal(-properties->ambient_sounds[i].radius,properties->ambient_sounds[i].radius);
						ambient_sounds[i].position.y+=RANDOM::global.compact_normal(-properties->ambient_sounds[i].radius,properties->ambient_sounds[i].radius);

						ambient_sounds[i].pitch =RANDOM::global(1-properties->ambient_sounds[i].pitchvar,1+properties->ambient_sounds[i].pitchvar);
						ambient_sounds[i].volume=1-RANDOM::global(0,properties->ambient_sounds[i].volumevar);

						spos=ambient_sounds[i].position;

						spos=spos.gettransformed_to  (center+position_primary,orientation_primary);
						spos=spos.gettransformed_from(center,0);

						svel=-linear_velocity_primary;
						svel-=((ambient_sounds[i].position-(center+position_primary)).getperpend())*angular_velocity_primary*(math_pi/180);

						svel=svel.getrotated(-orientation_primary);

						ambient_sounds[i].source->play(spos.x,spos.y,svel.x,svel.y,ambient_sounds[i].pitch,ambient_sounds[i].volume);
					}
				}
				else if (ambient_sounds[i].source->playing())
				{
					spos=ambient_sounds[i].position;

					spos=spos.gettransformed_to  (center+position_primary,orientation_primary);
					spos=spos.gettransformed_from(center,0);

					svel=-linear_velocity_primary;
					svel-=((ambient_sounds[i].position-(center+position_primary)).getperpend())*angular_velocity_primary*(math_pi/180);

					svel=svel.getrotated(-orientation_primary);

					ambient_sounds[i].source->play(spos.x,spos.y,svel.x,svel.y,ambient_sounds[i].pitch,ambient_sounds[i].volume);
				}
				else if (ambient_sounds[i].source->stopped())
				{
					ambient_sounds[i].source->rewind();

					ambient_sounds[i].starttime  =RANDOM::global.compact_normal(0,properties->ambient_sounds[i].silence_duration*2);
					ambient_sounds[i].currenttime=0;
				}
			}
			else if (!ambient_sounds[i].source->initial())
			{
				ambient_sounds[i].source->rewind();

				ambient_sounds[i].starttime  =RANDOM::global.compact_normal(0,properties->ambient_sounds[i].silence_duration*2);
				ambient_sounds[i].currenttime=0;
			}
			else
			{
				ambient_sounds[i].currenttime=0;
			}
		}
	}
}

void MAP::LAYER::init(const MAP_PROPERTIES &props,const MAP_PROPERTIES::LAYER &props_layer,const LAYER *primary_layer,VIDEO::TEXTURE *textures)
{
	position   =props_layer.position;
	orientation=props_layer.orientation;

	linear_motion_time =0;
	angular_motion_time=0;

	if (props_layer.linear_motion_type==1 && props_layer.numlinear_motions)
	{
		linear_motion_once_end=props_layer.linear_motions[0].start + props_layer.linear_motions[0].duration;

		double cmp;

		for (int i=1;i<props_layer.numlinear_motions;i++)
		{
			cmp=props_layer.linear_motions[i].start + props_layer.linear_motions[i].duration;

			if (linear_motion_once_end<cmp)
			{
				linear_motion_once_end=cmp;
			}
		}
	}

	if (props_layer.angular_motion_type==1 && props_layer.numangular_motions)
	{
		angular_motion_once_end=props_layer.angular_motions[0].start + props_layer.angular_motions[0].duration;

		double cmp;

		for (int i=1;i<props_layer.numangular_motions;i++)
		{
			cmp=props_layer.angular_motions[i].start + props_layer.angular_motions[i].duration;

			if (angular_motion_once_end<cmp)
			{
				angular_motion_once_end=cmp;
			}
		}
	}

	int numtiles=props.mapwidth*props.mapheight;

	tiles=new TILE[numtiles];

	tilewidth =props.tilewidth *props.tilewidth_scale;
	tileheight=props.tileheight*props.tileheight_scale;

	for (int i=0;i<numtiles;i++)
	{
		if (props_layer.texture_index && props_layer.texture_index[i]!=-1)
		{
			tiles[i].texture=textures[props_layer.texture_index[i]];

			tiles[i].invisible=false;
		}
		else
		{
			tiles[i].invisible=true;
		}

		if (props_layer.shape_index && props_layer.shape_index[i]!=-1)
		{
			tiles[i].numtriangles=props.shapes[props_layer.shape_index[i]].numtriangles;
			tiles[i].numquads    =props.shapes[props_layer.shape_index[i]].numquads;

			if (tiles[i].numtriangles)
			{
				tiles[i].triangles=new TRIANGLE[tiles[i].numtriangles];

				for (int j=0;j<tiles[i].numtriangles;j++)
				{
					tiles[i].triangles[j]=props.shapes[props_layer.shape_index[i]].triangles[j];

					tiles[i].triangles[j].scale(tilewidth,tileheight);

					tiles[i].triangles[j]+=VECTOR((i%props.mapwidth)*tilewidth,(i/props.mapwidth)*tileheight);
				}
			}

			if (tiles[i].numquads)
			{
				tiles[i].quads=new QUAD[tiles[i].numquads];

				for (int j=0;j<tiles[i].numquads;j++)
				{
					tiles[i].quads[j]=props.shapes[props_layer.shape_index[i]].quads[j];

					tiles[i].quads[j].scale(tilewidth,tileheight);

					tiles[i].quads[j]+=VECTOR((i%props.mapwidth)*tilewidth,(i/props.mapwidth)*tileheight);
				}
			}
		}
		else
		{
			tiles[i].numtriangles=0;
			tiles[i].numquads    =0;
		}

		tiles[i].numpointlights=0;

		if (props_layer.numpointlights)
		{
			int x=i%props.mapwidth;
			int y=i/props.mapwidth;

			for (int j=0;j<props_layer.numpointlights;j++)
			{
				if (x>=props_layer.pointlights[j].x-props_layer.pointlights[j].radius && x<=props_layer.pointlights[j].x+props_layer.pointlights[j].radius &&
                    y>=props_layer.pointlights[j].y-props_layer.pointlights[j].radius && y<=props_layer.pointlights[j].y+props_layer.pointlights[j].radius)
				{
					tiles[i].numpointlights++;
				}
			}

			if (tiles[i].numpointlights)
			{
				tiles[i].pointlights=new TILE::POINTLIGHT[tiles[i].numpointlights];

				tiles[i].numpointlights=0;

				for (int j=0;j<props_layer.numpointlights;j++)
				{
					if (x>=props_layer.pointlights[j].x-props_layer.pointlights[j].radius && x<=props_layer.pointlights[j].x+props_layer.pointlights[j].radius &&
                        y>=props_layer.pointlights[j].y-props_layer.pointlights[j].radius && y<=props_layer.pointlights[j].y+props_layer.pointlights[j].radius)
					{
						tiles[i].pointlights[tiles[i].numpointlights].x=x-(props_layer.pointlights[j].x-props_layer.pointlights[j].radius);
						tiles[i].pointlights[tiles[i].numpointlights].y=y-(props_layer.pointlights[j].y-props_layer.pointlights[j].radius);

						tiles[i].pointlights[tiles[i].numpointlights].width =(props_layer.pointlights[j].radius*2)+1;
						tiles[i].pointlights[tiles[i].numpointlights].height=(props_layer.pointlights[j].radius*2)+1;

						tiles[i].pointlights[tiles[i].numpointlights].color[0]=props_layer.pointlights[j].color[0];
						tiles[i].pointlights[tiles[i].numpointlights].color[1]=props_layer.pointlights[j].color[1];
						tiles[i].pointlights[tiles[i].numpointlights].color[2]=props_layer.pointlights[j].color[2];
						tiles[i].pointlights[tiles[i].numpointlights].color[3]=props_layer.pointlights[j].color[3];

						tiles[i].numpointlights++;
					}
				}
			}
		}

		tiles[i].empty=tiles[i].invisible && tiles[i].numtriangles==0 && tiles[i].numquads==0 && tiles[i].numpointlights==0;
	}

	if (props_layer.linear_motion_type==1 || props_layer.linear_motion_type==2)
	{
		if (props_layer.numlinear_motions)
		{
			linear_motion_sounds=new MOTION_SOUND[props_layer.numlinear_motions];

			VECTOR vel,acc;

			for (int i=0;i<props_layer.numlinear_motions;i++)
			{
				if (props_layer.linear_motions[i].hassound)
				{
					linear_motion_sounds[i].velocity_source    =new AUDIO::SOURCE(props_layer.linear_motions[i].velocity_sample_filename    ,props_layer.linear_motions[i].velocity_sound_loudness);
					linear_motion_sounds[i].acceleration_source=new AUDIO::SOURCE(props_layer.linear_motions[i].acceleration_sample_filename,props_layer.linear_motions[i].acceleration_sound_loudness);

					vel.x=compact_gaussian(props_layer.linear_motions[i].duration/2,props_layer.linear_motions[i].duration);
					vel.y=compact_gaussian(props_layer.linear_motions[i].duration/2,props_layer.linear_motions[i].duration);

					acc.x=compact_gaussian_diff(props_layer.linear_motions[i].duration*((double)2/3),props_layer.linear_motions[i].duration);
					acc.y=compact_gaussian_diff(props_layer.linear_motions[i].duration*((double)2/3),props_layer.linear_motions[i].duration);

					linear_motion_sounds[i].max_velocity    =vel.getlength();
					linear_motion_sounds[i].max_acceleration=acc.getlength();
				}
			}
		}
	}

	if (props_layer.angular_motion_type==1 || props_layer.angular_motion_type==2)
	{
		if (props_layer.numangular_motions)
		{
			angular_motion_sounds=new MOTION_SOUND[props_layer.numangular_motions];

			double angvel,angacc;

			for (int i=0;i<props_layer.numangular_motions;i++)
			{
				if (props_layer.angular_motions[i].hassound)
				{
					angular_motion_sounds[i].velocity_source    =new AUDIO::SOURCE(props_layer.angular_motions[i].velocity_sample_filename    ,props_layer.angular_motions[i].velocity_sound_loudness);
					angular_motion_sounds[i].acceleration_source=new AUDIO::SOURCE(props_layer.angular_motions[i].acceleration_sample_filename,props_layer.angular_motions[i].acceleration_sound_loudness);

					angvel=compact_gaussian     (props_layer.angular_motions[i].duration/2            ,props_layer.angular_motions[i].duration);
					angacc=compact_gaussian_diff(props_layer.angular_motions[i].duration*((double)2/3),props_layer.angular_motions[i].duration);

					angular_motion_sounds[i].max_velocity    =absolute(angvel);
					angular_motion_sounds[i].max_acceleration=absolute(angacc);
				}
			}
		}
	}

	properties      =&props;
	properties_layer=&props_layer;

	this->primary_layer=primary_layer;

	calc_redundancy();
}

const MAP_PROPERTIES::LAYER& MAP::LAYER::getproperties() const
{
	return(*properties_layer);
}

VECTOR MAP::LAYER::getposition() const
{
	return(position);
}

double MAP::LAYER::getorientation() const
{
	return(orientation);
}

VECTOR MAP::LAYER::getcenter() const
{
	return(VECTOR((tilewidth*properties->mapwidth)/2,(tileheight*properties->mapheight)/2));
}

double MAP::LAYER::getlinear_motion_time() const
{
	return(linear_motion_time);
}

double MAP::LAYER::getangular_motion_time() const
{
	return(angular_motion_time);
}

VECTOR MAP::LAYER::getlinear_velocity() const
{
	return(getlinear_velocity(linear_motion_time));
}

double MAP::LAYER::getangular_velocity() const
{
	return(getangular_velocity(angular_motion_time));
}

VECTOR MAP::LAYER::getlinear_velocity (double time,double timestep) const
{
	VECTOR vel;

	if (properties_layer->linear_motion_type==1)
	{
		double time1=time;
		double time2=time+(timestep/2);
		double time3=time+timestep;

		vel=(getlinear_velocity(time1)+(getlinear_velocity(time2)*4)+getlinear_velocity(time3))/6;
	}
	else if (properties_layer->linear_motion_type==2)
	{
		double time1=time;
		double time2=time+(timestep/2);
		double time3=time+timestep;

		double loop_duration=properties_layer->linear_motion_loop_end-properties_layer->linear_motion_loop_start;

		if (time1>=properties_layer->linear_motion_loop_start)
		{
			time1=modulus(time1-properties_layer->linear_motion_loop_start,loop_duration)+properties_layer->linear_motion_loop_start;
		}

		if (time2>=properties_layer->linear_motion_loop_start)
		{
			time2=modulus(time2-properties_layer->linear_motion_loop_start,loop_duration)+properties_layer->linear_motion_loop_start;
		}

		if (time3>=properties_layer->linear_motion_loop_start)
		{
			time3=modulus(time3-properties_layer->linear_motion_loop_start,loop_duration)+properties_layer->linear_motion_loop_start;
		}

		vel=(getlinear_velocity(time1)+(getlinear_velocity(time2)*4)+getlinear_velocity(time3))/6;
	}

	return(vel);
}

double MAP::LAYER::getangular_velocity(double time,double timestep) const
{
	double angvel=0;

	if (properties_layer->angular_motion_type==1)
	{
		double time1=time;
		double time2=time+(timestep/2);
		double time3=time+timestep;

		angvel=(getangular_velocity(time1)+(getangular_velocity(time2)*4)+getangular_velocity(time3))/6;
	}
	else if (properties_layer->angular_motion_type==2)
	{
		double time1=time;
		double time2=time+(timestep/2);
		double time3=time+timestep;

		double loop_duration=properties_layer->angular_motion_loop_end-properties_layer->angular_motion_loop_start;

		if (time1>=properties_layer->angular_motion_loop_start)
		{
			time1=modulus(time1-properties_layer->angular_motion_loop_start,loop_duration)+properties_layer->angular_motion_loop_start;
		}

		if (time2>=properties_layer->angular_motion_loop_start)
		{
			time2=modulus(time2-properties_layer->angular_motion_loop_start,loop_duration)+properties_layer->angular_motion_loop_start;
		}

		if (time3>=properties_layer->angular_motion_loop_start)
		{
			time3=modulus(time3-properties_layer->angular_motion_loop_start,loop_duration)+properties_layer->angular_motion_loop_start;
		}

		angvel=(getangular_velocity(time1)+(getangular_velocity(time2)*4)+getangular_velocity(time3))/6;
	}

	return(angvel);
}

VECTOR MAP::LAYER::getlinear_acceleration (double time,double timestep) const
{
	VECTOR acc;

	if (properties_layer->linear_motion_type==1)
	{
		double time1=time;
		double time2=time+(timestep/2);
		double time3=time+timestep;

		acc=(getlinear_acceleration(time1)+(getlinear_acceleration(time2)*4)+getlinear_acceleration(time3))/6;
	}
	else if (properties_layer->linear_motion_type==2)
	{
		double time1=time;
		double time2=time+(timestep/2);
		double time3=time+timestep;

		double loop_duration=properties_layer->linear_motion_loop_end-properties_layer->linear_motion_loop_start;

		if (time1>=properties_layer->linear_motion_loop_start)
		{
			time1=modulus(time1-properties_layer->linear_motion_loop_start,loop_duration)+properties_layer->linear_motion_loop_start;
		}

		if (time2>=properties_layer->linear_motion_loop_start)
		{
			time2=modulus(time2-properties_layer->linear_motion_loop_start,loop_duration)+properties_layer->linear_motion_loop_start;
		}

		if (time3>=properties_layer->linear_motion_loop_start)
		{
			time3=modulus(time3-properties_layer->linear_motion_loop_start,loop_duration)+properties_layer->linear_motion_loop_start;
		}

		acc=(getlinear_acceleration(time1)+(getlinear_acceleration(time2)*4)+getlinear_acceleration(time3))/6;
	}

	return(acc);
}

double MAP::LAYER::getangular_acceleration(double time,double timestep) const
{
	double angacc=0;

	if (properties_layer->angular_motion_type==1)
	{
		double time1=time;
		double time2=time+(timestep/2);
		double time3=time+timestep;

		angacc=(getangular_acceleration(time1)+(getangular_acceleration(time2)*4)+getangular_acceleration(time3))/6;
	}
	else if (properties_layer->angular_motion_type==2)
	{
		double time1=time;
		double time2=time+(timestep/2);
		double time3=time+timestep;

		double loop_duration=properties_layer->angular_motion_loop_end-properties_layer->angular_motion_loop_start;

		if (time1>=properties_layer->angular_motion_loop_start)
		{
			time1=modulus(time1-properties_layer->angular_motion_loop_start,loop_duration)+properties_layer->angular_motion_loop_start;
		}

		if (time2>=properties_layer->angular_motion_loop_start)
		{
			time2=modulus(time2-properties_layer->angular_motion_loop_start,loop_duration)+properties_layer->angular_motion_loop_start;
		}

		if (time3>=properties_layer->angular_motion_loop_start)
		{
			time3=modulus(time3-properties_layer->angular_motion_loop_start,loop_duration)+properties_layer->angular_motion_loop_start;
		}

		angacc=(getangular_acceleration(time1)+(getangular_acceleration(time2)*4)+getangular_acceleration(time3))/6;
	}

	return(angacc);
}

void MAP::LAYER::fillclist(COLLISIONLIST &clist,const VECTOR &pos,double radius,double timestep) const
{
	VECTOR linear_velocity;
	double angular_velocity;

	VECTOR position_primary;
	double orientation_primary;

	VECTOR linear_velocity_primary;
	double angular_velocity_primary;

	VECTOR center;

	if (primary_layer)
	{
		linear_velocity =getlinear_velocity(linear_motion_time,timestep);
		angular_velocity=getangular_velocity(angular_motion_time,timestep);

		position_primary   =primary_layer->getposition();
		orientation_primary=primary_layer->getorientation();

		linear_velocity_primary =primary_layer->getlinear_velocity(primary_layer->getlinear_motion_time(),timestep);
		angular_velocity_primary=primary_layer->getangular_velocity(primary_layer->getangular_motion_time(),timestep);

		center=getcenter();
	}

	int x1,y1;
	int x2,y2;

	getmapcoords_rect(x1,y1,x2,y2,pos,radius*collision_bounding_radius_mul);

	double tile_bounding_radius=squareroot(square(tilewidth)+square(tileheight))/2;

	TRIANGLE triangle;
	QUAD     quad;

	int i;
	VECTOR offset_wrap;
	for (int y=y1;y<y2+1;y++)
	{
		if (properties_layer->wrap || (y>=0 && y<properties->mapheight))
		{
			for (int x=x1;x<x2+1;x++)
			{
				if (properties_layer->wrap || (x>=0 && x<properties->mapwidth))
				{
					if (!properties_layer->wrap)
					{
						i=x+y*properties->mapwidth;

						offset_wrap.setzero();
					}
					else
					{
						i=getmapindex_wrap(x,y);

						offset_wrap=getmapoffset_wrap(x,y);
					}

					if (!tiles[i].empty && !(tiles[i].numtriangles==0 && tiles[i].numquads==0))
					{
						if (properties_layer==&(properties->primary_layer))
						{
							clist.shapes[clist.numshapes].position.x=(x*tilewidth)+(tilewidth/2);
							clist.shapes[clist.numshapes].position.y=(y*tileheight)+(tileheight/2);

							clist.shapes[clist.numshapes].radius=tile_bounding_radius;

							clist.shapes[clist.numshapes].numtriangles=tiles[i].numtriangles;
							clist.shapes[clist.numshapes].numquads    =tiles[i].numquads;
							clist.shapes[clist.numshapes].numcircles  =0;

							clist.shapes[clist.numshapes].object=(void*)this;

							for (int j=0;j<tiles[i].numtriangles;j++)
							{
								clist.shapes[clist.numshapes].triangles[j]=tiles[i].triangles[j];

								clist.shapes[clist.numshapes].triangles[j]+=offset_wrap;
							}

							for (int j=0;j<tiles[i].numquads;j++)
							{
								clist.shapes[clist.numshapes].quads[j]=tiles[i].quads[j];

								clist.shapes[clist.numshapes].quads[j]+=offset_wrap;
							}

							clist.numshapes++;
						}
						else if (properties_layer==&(properties->secondary_layer) || properties_layer==&(properties->tertiary_layer))
						{
							clist.shapes[clist.numshapes].position.x=(x*tilewidth)+(tilewidth/2);
							clist.shapes[clist.numshapes].position.y=(y*tileheight)+(tileheight/2);

							clist.shapes[clist.numshapes].radius=tile_bounding_radius;

							clist.shapes[clist.numshapes].numtriangles=tiles[i].numtriangles;
							clist.shapes[clist.numshapes].numquads    =tiles[i].numquads;
							clist.shapes[clist.numshapes].numcircles  =0;

							clist.shapes[clist.numshapes].object=(void*)this;

							clist.shapes[clist.numshapes].position=clist.shapes[clist.numshapes].position.gettransformed_to  (center,0);
							clist.shapes[clist.numshapes].position=clist.shapes[clist.numshapes].position.gettransformed_from(center+position,orientation);
							clist.shapes[clist.numshapes].position=clist.shapes[clist.numshapes].position.gettransformed_to  (center+position_primary,orientation_primary);
							clist.shapes[clist.numshapes].position=clist.shapes[clist.numshapes].position.gettransformed_from(center,0);

							for (int j=0;j<tiles[i].numtriangles;j++)
							{
								triangle=tiles[i].triangles[j];

								triangle=triangle.gettransformed_to(center-offset_wrap,0,VECTOR(0,0),0);

								triangle=triangle.gettransformed_from(center+position,orientation,linear_velocity,angular_velocity);

								triangle=triangle.gettransformed_to(center+position_primary,orientation_primary,linear_velocity_primary,angular_velocity_primary);

								triangle=triangle.gettransformed_from(center,0,VECTOR(0,0),0);

								clist.shapes[clist.numshapes].triangles[j]=triangle;
							}

							for (int j=0;j<tiles[i].numquads;j++)
							{
								quad=tiles[i].quads[j];

								quad=quad.gettransformed_to(center-offset_wrap,0,VECTOR(0,0),0);

								quad=quad.gettransformed_from(center+position,orientation,linear_velocity,angular_velocity);

								quad=quad.gettransformed_to(center+position_primary,orientation_primary,linear_velocity_primary,angular_velocity_primary);

								quad=quad.gettransformed_from(center,0,VECTOR(0,0),0);

								clist.shapes[clist.numshapes].quads[j]=quad;
							}

							clist.numshapes++;
						}
					}
				}
			}
		}
	}
}

void MAP::LAYER::integrate(double timestep)
{
	if (properties_layer->linear_motion_type==1)
	{
		if (linear_motion_time<=linear_motion_once_end)
		{
			position+=getlinear_velocity(linear_motion_time,timestep)*timestep;

			linear_motion_time+=timestep;
		}
	}
	else if (properties_layer->linear_motion_type==2)
	{
		position+=getlinear_velocity(linear_motion_time,timestep)*timestep;

		linear_motion_time+=timestep;

		if (linear_motion_time>=properties_layer->linear_motion_loop_start)
		{
			double loop_duration=properties_layer->linear_motion_loop_end-properties_layer->linear_motion_loop_start;

			linear_motion_time=modulus(linear_motion_time-properties_layer->linear_motion_loop_start,loop_duration)+properties_layer->linear_motion_loop_start;
		}
	}

	if (properties_layer->angular_motion_type==1)
	{
		if (angular_motion_time<=angular_motion_once_end)
		{
			orientation+=getangular_velocity(angular_motion_time,timestep)*timestep;

			angular_motion_time+=timestep;
		}

		orientation=modulus(orientation,360);
	}
	else if (properties_layer->angular_motion_type==2)
	{
		orientation+=getangular_velocity(angular_motion_time,timestep)*timestep;

		angular_motion_time+=timestep;

		if (angular_motion_time>=properties_layer->angular_motion_loop_start)
		{
			double loop_duration=properties_layer->angular_motion_loop_end-properties_layer->angular_motion_loop_start;

			angular_motion_time=modulus(angular_motion_time-properties_layer->angular_motion_loop_start,loop_duration)+properties_layer->angular_motion_loop_start;
		}

		orientation=modulus(orientation,360);
	}
}

void MAP::LAYER::light(const VECTOR &pos,double radius) const
{
	VECTOR position_primary;
	double orientation_primary;

	VECTOR center;

	double mul;
	VECTOR add;

	if (primary_layer)
	{
		position_primary   =primary_layer->getposition();
		orientation_primary=primary_layer->getorientation();

		center=getcenter();

		if (properties_layer==&(properties->foreground_layer) || properties_layer==&(properties->background_layer))
		{
			VECTOR p=pos;

			p=p.gettransformed_to  (center,0);
			p=p.gettransformed_from(center+position_primary,orientation_primary);

			if (properties_layer==&(properties->foreground_layer))
			{
				mul=2;
			}
			else if (properties_layer==&(properties->background_layer))
			{
				mul=0.5;
			}

			add=p-(p*mul);
		}
	}

	int x1,y1;
	int x2,y2;

	getmapcoords_rect(x1,y1,x2,y2,pos,radius);

	VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_tile_start();

	int i;
	VECTOR q1,q2,q3,q4;
	for (int y=y1;y<y2+1;y++)
	{
		if (properties_layer->wrap || (y>=0 && y<properties->mapheight))
		{
			for (int x=x1;x<x2+1;x++)
			{
				if (properties_layer->wrap || (x>=0 && x<properties->mapwidth))
				{
					if (!properties_layer->wrap)
					{
						i=x+y*properties->mapwidth;
					}
					else
					{
						i=getmapindex_wrap(x,y);
					}

					if (!tiles[i].empty && tiles[i].numpointlights!=0)
					{
						q1.x=tilewidth *x;
						q1.y=tileheight*y;

						q2.x=q1.x+tilewidth;
						q2.y=q1.y;

						q3.x=q1.x+tilewidth;
						q3.y=q1.y+tileheight;

						q4.x=q1.x;
						q4.y=q1.y+tileheight;

						if (properties_layer==&(properties->primary_layer))
						{
							for (int j=0;j<tiles[i].numpointlights;j++)
							{
								VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_tile(q1.x,q1.y,q2.x,q2.y,q3.x,q3.y,q4.x,q4.y,tiles[i].pointlights[j].color[0],
                                                                                                                             tiles[i].pointlights[j].color[1],
                                                                                                                             tiles[i].pointlights[j].color[2],
                                                                                                                             tiles[i].pointlights[j].color[3],
                                                                                                                             tiles[i].pointlights[j].x,
                                                                                                                             tiles[i].pointlights[j].y,
                                                                                                                             tiles[i].pointlights[j].width,
                                                                                                                             tiles[i].pointlights[j].height);
							}
						}
						else if (properties_layer==&(properties->secondary_layer) || properties_layer==&(properties->tertiary_layer))
						{
							q1=q1.gettransformed_to(center,0);
							q2=q2.gettransformed_to(center,0);
							q3=q3.gettransformed_to(center,0);
							q4=q4.gettransformed_to(center,0);

							q1=q1.gettransformed_from(center+position,orientation);
							q2=q2.gettransformed_from(center+position,orientation);
							q3=q3.gettransformed_from(center+position,orientation);
							q4=q4.gettransformed_from(center+position,orientation);

							q1=q1.gettransformed_to(center+position_primary,orientation_primary);
							q2=q2.gettransformed_to(center+position_primary,orientation_primary);
							q3=q3.gettransformed_to(center+position_primary,orientation_primary);
							q4=q4.gettransformed_to(center+position_primary,orientation_primary);

							q1=q1.gettransformed_from(center,0);
							q2=q2.gettransformed_from(center,0);
							q3=q3.gettransformed_from(center,0);
							q4=q4.gettransformed_from(center,0);

							for (int j=0;j<tiles[i].numpointlights;j++)
							{
								VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_tile(q1.x,q1.y,q2.x,q2.y,q3.x,q3.y,q4.x,q4.y,tiles[i].pointlights[j].color[0],
                                                                                                                             tiles[i].pointlights[j].color[1],
                                                                                                                             tiles[i].pointlights[j].color[2],
                                                                                                                             tiles[i].pointlights[j].color[3],
                                                                                                                             tiles[i].pointlights[j].x,
                                                                                                                             tiles[i].pointlights[j].y,
                                                                                                                             tiles[i].pointlights[j].width,
                                                                                                                             tiles[i].pointlights[j].height);
							}
						}
						else if (properties_layer==&(properties->foreground_layer) || properties_layer==&(properties->background_layer))
						{
							q1*=mul;
							q2*=mul;
							q3*=mul;
							q4*=mul;

							q1+=add;
							q2+=add;
							q3+=add;
							q4+=add;

							q1=q1.gettransformed_to(center+position_primary,orientation_primary);
							q2=q2.gettransformed_to(center+position_primary,orientation_primary);
							q3=q3.gettransformed_to(center+position_primary,orientation_primary);
							q4=q4.gettransformed_to(center+position_primary,orientation_primary);

							q1=q1.gettransformed_from(center,0);
							q2=q2.gettransformed_from(center,0);
							q3=q3.gettransformed_from(center,0);
							q4=q4.gettransformed_from(center,0);

							for (int j=0;j<tiles[i].numpointlights;j++)
							{
								VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_tile(q1.x,q1.y,q2.x,q2.y,q3.x,q3.y,q4.x,q4.y,tiles[i].pointlights[j].color[0],
                                                                                                                             tiles[i].pointlights[j].color[1],
                                                                                                                             tiles[i].pointlights[j].color[2],
                                                                                                                             tiles[i].pointlights[j].color[3],
                                                                                                                             tiles[i].pointlights[j].x,
                                                                                                                             tiles[i].pointlights[j].y,
                                                                                                                             tiles[i].pointlights[j].width,
                                                                                                                             tiles[i].pointlights[j].height);
							}
						}
					}
				}
			}
		}
	}

	VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_tile_end();
}

void MAP::LAYER::draw(const VECTOR &pos,double radius,double alpha) const
{
	VECTOR position_primary;
	double orientation_primary;

	VECTOR center;

	double mul;
	VECTOR add;

	if (primary_layer)
	{
		position_primary   =primary_layer->getposition();
		orientation_primary=primary_layer->getorientation();

		center=getcenter();

		if (properties_layer==&(properties->foreground_layer) || properties_layer==&(properties->background_layer))
		{
			VECTOR p=pos;

			p=p.gettransformed_to  (center,0);
			p=p.gettransformed_from(center+position_primary,orientation_primary);

			if (properties_layer==&(properties->foreground_layer))
			{
				mul=2;
			}
			else if (properties_layer==&(properties->background_layer))
			{
				mul=0.5;
			}

			add=p-(p*mul);
		}
	}

	int x1,y1;
	int x2,y2;

	getmapcoords_rect(x1,y1,x2,y2,pos,radius);

	int i;
	VECTOR q1,q2,q3,q4;
	for (int y=y1;y<y2+1;y++)
	{
		if (properties_layer->wrap || (y>=0 && y<properties->mapheight))
		{
			for (int x=x1;x<x2+1;x++)
			{
				if (properties_layer->wrap || (x>=0 && x<properties->mapwidth))
				{
					if (!properties_layer->wrap)
					{
						i=x+y*properties->mapwidth;
					}
					else
					{
						i=getmapindex_wrap(x,y);
					}

					if (!(tiles[i].empty || tiles[i].invisible))
					{
						q1.x=tilewidth *x;
						q1.y=tileheight*y;

						q2.x=q1.x+tilewidth;
						q2.y=q1.y;

						q3.x=q1.x+tilewidth;
						q3.y=q1.y+tileheight;

						q4.x=q1.x;
						q4.y=q1.y+tileheight;

						if (properties_layer==&(properties->primary_layer))
						{
							VIDEO::draw_quad_texture(q1.x,q1.y,q2.x,q2.y,q3.x,q3.y,q4.x,q4.y,tiles[i].texture,alpha);
						}
						else if (properties_layer==&(properties->secondary_layer) || properties_layer==&(properties->tertiary_layer))
						{
							q1=q1.gettransformed_to(center,0);
							q2=q2.gettransformed_to(center,0);
							q3=q3.gettransformed_to(center,0);
							q4=q4.gettransformed_to(center,0);

							q1=q1.gettransformed_from(center+position,orientation);
							q2=q2.gettransformed_from(center+position,orientation);
							q3=q3.gettransformed_from(center+position,orientation);
							q4=q4.gettransformed_from(center+position,orientation);

							q1=q1.gettransformed_to(center+position_primary,orientation_primary);
							q2=q2.gettransformed_to(center+position_primary,orientation_primary);
							q3=q3.gettransformed_to(center+position_primary,orientation_primary);
							q4=q4.gettransformed_to(center+position_primary,orientation_primary);

							q1=q1.gettransformed_from(center,0);
							q2=q2.gettransformed_from(center,0);
							q3=q3.gettransformed_from(center,0);
							q4=q4.gettransformed_from(center,0);

							VIDEO::draw_quad_texture(q1.x,q1.y,q2.x,q2.y,q3.x,q3.y,q4.x,q4.y,tiles[i].texture,alpha);
						}
						else if (properties_layer==&(properties->foreground_layer) || properties_layer==&(properties->background_layer))
						{
							q1*=mul;
							q2*=mul;
							q3*=mul;
							q4*=mul;

							q1+=add;
							q2+=add;
							q3+=add;
							q4+=add;

							q1=q1.gettransformed_to(center+position_primary,orientation_primary);
							q2=q2.gettransformed_to(center+position_primary,orientation_primary);
							q3=q3.gettransformed_to(center+position_primary,orientation_primary);
							q4=q4.gettransformed_to(center+position_primary,orientation_primary);

							q1=q1.gettransformed_from(center,0);
							q2=q2.gettransformed_from(center,0);
							q3=q3.gettransformed_from(center,0);
							q4=q4.gettransformed_from(center,0);

							VIDEO::draw_quad_texture(q1.x,q1.y,q2.x,q2.y,q3.x,q3.y,q4.x,q4.y,tiles[i].texture,alpha);
						}
					}
				}
			}
		}
	}
}

void MAP::LAYER::draw(const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const
{
	VECTOR position_primary;
	double orientation_primary;

	VECTOR center;

	if (primary_layer)
	{
		position_primary   =primary_layer->getposition();
		orientation_primary=primary_layer->getorientation();

		center=getcenter();
	}

	int x1,y1;
	int x2,y2;

	getmapcoords_rect(x1,y1,x2,y2,pos,radius);

	TRIANGLE triangle;
	QUAD     quad;

	hud.batch_draw_line_start();

	int i;
	VECTOR offset_wrap;
	for (int y=y1;y<y2+1;y++)
	{
		if (properties_layer->wrap || (y>=0 && y<properties->mapheight))
		{
			for (int x=x1;x<x2+1;x++)
			{
				if (properties_layer->wrap || (x>=0 && x<properties->mapwidth))
				{
					if (!properties_layer->wrap)
					{
						i=x+y*properties->mapwidth;

						offset_wrap.setzero();
					}
					else
					{
						i=getmapindex_wrap(x,y);

						offset_wrap=getmapoffset_wrap(x,y);
					}

					if (!tiles[i].empty && !(tiles[i].numtriangles==0 && tiles[i].numquads==0))
					{
						if (properties_layer==&(properties->primary_layer))
						{
							for (int j=0;j<tiles[i].numtriangles;j++)
							{
								if (!tiles[i].triangles[j].l1.redundant)
								{
									hud.batch_draw_line(tiles[i].triangles[j].l1.start.x+offset_wrap.x,tiles[i].triangles[j].l1.start.y+offset_wrap.y,
                                                        tiles[i].triangles[j].l1.end.x  +offset_wrap.x,tiles[i].triangles[j].l1.end.y  +offset_wrap.y,r,g,b,a);
								}

								if (!tiles[i].triangles[j].l2.redundant)
								{
									hud.batch_draw_line(tiles[i].triangles[j].l2.start.x+offset_wrap.x,tiles[i].triangles[j].l2.start.y+offset_wrap.y,
                                                        tiles[i].triangles[j].l2.end.x  +offset_wrap.x,tiles[i].triangles[j].l2.end.y  +offset_wrap.y,r,g,b,a);
								}

								if (!tiles[i].triangles[j].l3.redundant)
								{
									hud.batch_draw_line(tiles[i].triangles[j].l3.start.x+offset_wrap.x,tiles[i].triangles[j].l3.start.y+offset_wrap.y,
                                                        tiles[i].triangles[j].l3.end.x  +offset_wrap.x,tiles[i].triangles[j].l3.end.y  +offset_wrap.y,r,g,b,a);
								}
							}

							for (int j=0;j<tiles[i].numquads;j++)
							{
								if (!tiles[i].quads[j].l1.redundant)
								{
									hud.batch_draw_line(tiles[i].quads[j].l1.start.x+offset_wrap.x,tiles[i].quads[j].l1.start.y+offset_wrap.y,
                                                        tiles[i].quads[j].l1.end.x  +offset_wrap.x,tiles[i].quads[j].l1.end.y  +offset_wrap.y,r,g,b,a);
								}

								if (!tiles[i].quads[j].l2.redundant)
								{
									hud.batch_draw_line(tiles[i].quads[j].l2.start.x+offset_wrap.x,tiles[i].quads[j].l2.start.y+offset_wrap.y,
                                                        tiles[i].quads[j].l2.end.x  +offset_wrap.x,tiles[i].quads[j].l2.end.y  +offset_wrap.y,r,g,b,a);
								}

								if (!tiles[i].quads[j].l3.redundant)
								{
									hud.batch_draw_line(tiles[i].quads[j].l3.start.x+offset_wrap.x,tiles[i].quads[j].l3.start.y+offset_wrap.y,
                                                        tiles[i].quads[j].l3.end.x  +offset_wrap.x,tiles[i].quads[j].l3.end.y  +offset_wrap.y,r,g,b,a);
								}

								if (!tiles[i].quads[j].l4.redundant)
								{
									hud.batch_draw_line(tiles[i].quads[j].l4.start.x+offset_wrap.x,tiles[i].quads[j].l4.start.y+offset_wrap.y,
                                                        tiles[i].quads[j].l4.end.x  +offset_wrap.x,tiles[i].quads[j].l4.end.y  +offset_wrap.y,r,g,b,a);
								}
							}
						}
						else if (properties_layer==&(properties->secondary_layer) || properties_layer==&(properties->tertiary_layer))
						{
							for (int j=0;j<tiles[i].numtriangles;j++)
							{
								triangle=tiles[i].triangles[j];

								triangle=triangle.gettransformed_to(center-offset_wrap,0,VECTOR(0,0),0);

								triangle=triangle.gettransformed_from(center+position,orientation,VECTOR(0,0),0);

								triangle=triangle.gettransformed_to(center+position_primary,orientation_primary,VECTOR(0,0),0);

								triangle=triangle.gettransformed_from(center,0,VECTOR(0,0),0);

								if (!triangle.l1.redundant)
								{
									hud.batch_draw_line(triangle.l1.start.x,triangle.l1.start.y,
                                                        triangle.l1.end.x  ,triangle.l1.end.y  ,r,g,b,a);
								}

								if (!triangle.l2.redundant)
								{
									hud.batch_draw_line(triangle.l2.start.x,triangle.l2.start.y,
                                                        triangle.l2.end.x  ,triangle.l2.end.y  ,r,g,b,a);
								}

								if (!triangle.l3.redundant)
								{
									hud.batch_draw_line(triangle.l3.start.x,triangle.l3.start.y,
                                                        triangle.l3.end.x  ,triangle.l3.end.y  ,r,g,b,a);
								}
							}

							for (int j=0;j<tiles[i].numquads;j++)
							{
								quad=tiles[i].quads[j];

								quad=quad.gettransformed_to(center-offset_wrap,0,VECTOR(0,0),0);

								quad=quad.gettransformed_from(center+position,orientation,VECTOR(0,0),0);

								quad=quad.gettransformed_to(center+position_primary,orientation_primary,VECTOR(0,0),0);

								quad=quad.gettransformed_from(center,0,VECTOR(0,0),0);

								if (!quad.l1.redundant)
								{
									hud.batch_draw_line(quad.l1.start.x,quad.l1.start.y,
                                                        quad.l1.end.x  ,quad.l1.end.y  ,r,g,b,a);
								}

								if (!quad.l2.redundant)
								{
									hud.batch_draw_line(quad.l2.start.x,quad.l2.start.y,
                                                        quad.l2.end.x  ,quad.l2.end.y  ,r,g,b,a);
								}

								if (!quad.l3.redundant)
								{
									hud.batch_draw_line(quad.l3.start.x,quad.l3.start.y,
                                                        quad.l3.end.x  ,quad.l3.end.y  ,r,g,b,a);
								}

								if (!quad.l4.redundant)
								{
									hud.batch_draw_line(quad.l4.start.x,quad.l4.start.y,
                                                        quad.l4.end.x  ,quad.l4.end.y  ,r,g,b,a);
								}
							}
						}
					}
				}
			}
		}
	}

	hud.batch_draw_line_end();
}

void MAP::LAYER::sound(const VECTOR &pos,double radius) const
{
	VECTOR linear_velocity;
	double angular_velocity;

	VECTOR position_primary;
	double orientation_primary;

	VECTOR linear_velocity_primary;
	double angular_velocity_primary;

	VECTOR center=getcenter();

	if (primary_layer)
	{
		linear_velocity =getlinear_velocity();
		angular_velocity=getangular_velocity();

		position_primary   =primary_layer->getposition();
		orientation_primary=primary_layer->getorientation();

		linear_velocity_primary =primary_layer->getlinear_velocity();
		angular_velocity_primary=primary_layer->getangular_velocity();
	}

	VECTOR offset_wrap;

	if (properties_layer->wrap)
	{
		VECTOR p=pos;

		if (primary_layer)
		{
			p=p.gettransformed_to  (center,0);
			p=p.gettransformed_from(center+position_primary,orientation_primary);

			p=p.gettransformed_to  (center+position,orientation);
			p=p.gettransformed_from(center,0);
		}

		int x,y;
		getmapcoords(x,y,p);

		offset_wrap=getmapoffset_wrap(x,y);
	}

	VECTOR spos,svel;

	if (properties_layer->linear_motion_type==1 || properties_layer->linear_motion_type==2)
	{
		if (properties_layer->numlinear_motions)
		{
			VECTOR vel,acc;
			double len;

			for (int i=0;i<properties_layer->numlinear_motions;i++)
			{
				if (properties_layer->linear_motions[i].hassound)
				{
					if (!equalszero(linear_motion_sounds[i].max_velocity))
					{
						vel.x=compact_gaussian(linear_motion_time-properties_layer->linear_motions[i].start,properties_layer->linear_motions[i].duration);
						vel.y=compact_gaussian(linear_motion_time-properties_layer->linear_motions[i].start,properties_layer->linear_motions[i].duration);

						len=vel.getlength();

						if (!equalszero(len))
						{
							if (properties_layer==&(properties->primary_layer))
							{
								spos=center+offset_wrap;

								if ((spos-pos).getlength_squared()<=square(radius))
								{
									linear_motion_sounds[i].velocity_source->play(spos.x,spos.y,0,0,properties_layer->linear_motions[i].velocity_sound_pitch,len/linear_motion_sounds[i].max_velocity);
								}
								else if (!linear_motion_sounds[i].velocity_source->initial())
								{
									linear_motion_sounds[i].velocity_source->rewind();
								}
							}
							else if (properties_layer==&(properties->secondary_layer) || properties_layer==&(properties->tertiary_layer))
							{
								spos=center+offset_wrap;

								spos=spos.gettransformed_to  (center,0);
								spos=spos.gettransformed_from(center+position,orientation);

								svel=linear_velocity+(((spos-(center+position)).getperpend())*angular_velocity*(math_pi/180));

								svel-=linear_velocity_primary;
								svel-=((spos-(center+position_primary)).getperpend())*angular_velocity_primary*(math_pi/180);

								svel=svel.getrotated(-orientation_primary);

								spos=spos.gettransformed_to  (center+position_primary,orientation_primary);
								spos=spos.gettransformed_from(center,0);

								if ((spos-pos).getlength_squared()<=square(radius))
								{
									linear_motion_sounds[i].velocity_source->play(spos.x,spos.y,svel.x,svel.y,properties_layer->linear_motions[i].velocity_sound_pitch,len/linear_motion_sounds[i].max_velocity);
								}
								else if (!linear_motion_sounds[i].velocity_source->initial())
								{
									linear_motion_sounds[i].velocity_source->rewind();
								}
							}
						}
						else if (!linear_motion_sounds[i].velocity_source->initial())
						{
							linear_motion_sounds[i].velocity_source->rewind();
						}
					}

					if (!equalszero(linear_motion_sounds[i].max_acceleration))
					{
						acc.x=compact_gaussian_diff(linear_motion_time-properties_layer->linear_motions[i].start,properties_layer->linear_motions[i].duration);
						acc.y=compact_gaussian_diff(linear_motion_time-properties_layer->linear_motions[i].start,properties_layer->linear_motions[i].duration);

						len=acc.getlength();

						if (!equalszero(len))
						{
							if (properties_layer==&(properties->primary_layer))
							{
								spos=center+offset_wrap;

								if ((spos-pos).getlength_squared()<=square(radius))
								{
									linear_motion_sounds[i].acceleration_source->play(spos.x,spos.y,0,0,properties_layer->linear_motions[i].acceleration_sound_pitch,len/linear_motion_sounds[i].max_acceleration);
								}
								else if (!linear_motion_sounds[i].acceleration_source->initial())
								{
									linear_motion_sounds[i].acceleration_source->rewind();
								}
							}
							else if (properties_layer==&(properties->secondary_layer) || properties_layer==&(properties->tertiary_layer))
							{
								spos=center+offset_wrap;

								spos=spos.gettransformed_to  (center,0);
								spos=spos.gettransformed_from(center+position,orientation);

								svel=linear_velocity+(((spos-(center+position)).getperpend())*angular_velocity*(math_pi/180));

								svel-=linear_velocity_primary;
								svel-=((spos-(center+position_primary)).getperpend())*angular_velocity_primary*(math_pi/180);

								svel=svel.getrotated(-orientation_primary);

								spos=spos.gettransformed_to  (center+position_primary,orientation_primary);
								spos=spos.gettransformed_from(center,0);

								if ((spos-pos).getlength_squared()<=square(radius))
								{
									linear_motion_sounds[i].acceleration_source->play(spos.x,spos.y,svel.x,svel.y,properties_layer->linear_motions[i].acceleration_sound_pitch,len/linear_motion_sounds[i].max_acceleration);
								}
								else if (!linear_motion_sounds[i].acceleration_source->initial())
								{
									linear_motion_sounds[i].acceleration_source->rewind();
								}
							}
						}
						else if (!linear_motion_sounds[i].acceleration_source->initial())
						{
							linear_motion_sounds[i].acceleration_source->rewind();
						}
					}
				}
			}
		}
	}

	if (properties_layer->angular_motion_type==1 || properties_layer->angular_motion_type==2)
	{
		if (properties_layer->numangular_motions)
		{
			double angvel,angacc;

			for (int i=0;i<properties_layer->numangular_motions;i++)
			{
				if (properties_layer->angular_motions[i].hassound)
				{
					if (!equalszero(angular_motion_sounds[i].max_velocity))
					{
						angvel=absolute(compact_gaussian(angular_motion_time-properties_layer->angular_motions[i].start,properties_layer->angular_motions[i].duration));

						if (!equalszero(angvel))
						{
							if (properties_layer==&(properties->primary_layer))
							{
								spos=center+offset_wrap;

								if ((spos-pos).getlength_squared()<=square(radius))
								{
									angular_motion_sounds[i].velocity_source->play(spos.x,spos.y,0,0,properties_layer->angular_motions[i].velocity_sound_pitch,angvel/angular_motion_sounds[i].max_velocity);
								}
								else if (!angular_motion_sounds[i].velocity_source->initial())
								{
									angular_motion_sounds[i].velocity_source->rewind();
								}
							}
							else if (properties_layer==&(properties->secondary_layer) || properties_layer==&(properties->tertiary_layer))
							{
								spos=center+offset_wrap;

								spos=spos.gettransformed_to  (center,0);
								spos=spos.gettransformed_from(center+position,orientation);

								svel=linear_velocity+(((spos-(center+position)).getperpend())*angular_velocity*(math_pi/180));

								svel-=linear_velocity_primary;
								svel-=((spos-(center+position_primary)).getperpend())*angular_velocity_primary*(math_pi/180);

								svel=svel.getrotated(-orientation_primary);

								spos=spos.gettransformed_to  (center+position_primary,orientation_primary);
								spos=spos.gettransformed_from(center,0);

								if ((spos-pos).getlength_squared()<=square(radius))
								{
									angular_motion_sounds[i].velocity_source->play(spos.x,spos.y,svel.x,svel.y,properties_layer->angular_motions[i].velocity_sound_pitch,angvel/angular_motion_sounds[i].max_velocity);
								}
								else if (!angular_motion_sounds[i].velocity_source->initial())
								{
									angular_motion_sounds[i].velocity_source->rewind();
								}
							}
						}
						else if (!angular_motion_sounds[i].velocity_source->initial())
						{
							angular_motion_sounds[i].velocity_source->rewind();
						}
					}

					if (!equalszero(angular_motion_sounds[i].max_acceleration))
					{
						angacc=absolute(compact_gaussian_diff(angular_motion_time-properties_layer->angular_motions[i].start,properties_layer->angular_motions[i].duration));

						if (!equalszero(angacc))
						{
							if (properties_layer==&(properties->primary_layer))
							{
								spos=center+offset_wrap;

								if ((spos-pos).getlength_squared()<=square(radius))
								{
									angular_motion_sounds[i].acceleration_source->play(spos.x,spos.y,0,0,properties_layer->angular_motions[i].acceleration_sound_pitch,angacc/angular_motion_sounds[i].max_acceleration);
								}
								else if (!angular_motion_sounds[i].acceleration_source->initial())
								{
									angular_motion_sounds[i].acceleration_source->rewind();
								}
							}
							else if (properties_layer==&(properties->secondary_layer) || properties_layer==&(properties->tertiary_layer))
							{
								spos=center+offset_wrap;

								spos=spos.gettransformed_to  (center,0);
								spos=spos.gettransformed_from(center+position,orientation);

								svel=linear_velocity+(((spos-(center+position)).getperpend())*angular_velocity*(math_pi/180));

								svel-=linear_velocity_primary;
								svel-=((spos-(center+position_primary)).getperpend())*angular_velocity_primary*(math_pi/180);

								svel=svel.getrotated(-orientation_primary);

								spos=spos.gettransformed_to  (center+position_primary,orientation_primary);
								spos=spos.gettransformed_from(center,0);

								if ((spos-pos).getlength_squared()<=square(radius))
								{
									angular_motion_sounds[i].acceleration_source->play(spos.x,spos.y,svel.x,svel.y,properties_layer->angular_motions[i].acceleration_sound_pitch,angacc/angular_motion_sounds[i].max_acceleration);
								}
								else if (!angular_motion_sounds[i].acceleration_source->initial())
								{
									angular_motion_sounds[i].acceleration_source->rewind();
								}
							}
						}
						else if (!angular_motion_sounds[i].acceleration_source->initial())
						{
							angular_motion_sounds[i].acceleration_source->rewind();
						}
					}
				}
			}
		}
	}
}

VECTOR MAP::LAYER::getlinear_velocity (double time) const
{
	VECTOR vel;

	for (int i=0;i<properties_layer->numlinear_motions;i++)
	{
		vel.x+=properties_layer->linear_motions[i].deltapos.x*compact_gaussian(time-properties_layer->linear_motions[i].start,properties_layer->linear_motions[i].duration);
		vel.y+=properties_layer->linear_motions[i].deltapos.y*compact_gaussian(time-properties_layer->linear_motions[i].start,properties_layer->linear_motions[i].duration);
	}

	return(vel);
}

double MAP::LAYER::getangular_velocity(double time) const
{
	double angvel=0;

	for (int i=0;i<properties_layer->numangular_motions;i++)
	{
		angvel+=properties_layer->angular_motions[i].deltaang*compact_gaussian(time-properties_layer->angular_motions[i].start,properties_layer->angular_motions[i].duration);
	}

	return(angvel);
}

VECTOR MAP::LAYER::getlinear_acceleration (double time) const
{
	VECTOR acc;

	for (int i=0;i<properties_layer->numlinear_motions;i++)
	{
		acc.x+=properties_layer->linear_motions[i].deltapos.x*compact_gaussian_diff(time-properties_layer->linear_motions[i].start,properties_layer->linear_motions[i].duration);
		acc.y+=properties_layer->linear_motions[i].deltapos.y*compact_gaussian_diff(time-properties_layer->linear_motions[i].start,properties_layer->linear_motions[i].duration);
	}

	return(acc);
}

double MAP::LAYER::getangular_acceleration(double time) const
{
	double angacc=0;

	for (int i=0;i<properties_layer->numangular_motions;i++)
	{
		angacc+=properties_layer->angular_motions[i].deltaang*compact_gaussian_diff(time-properties_layer->angular_motions[i].start,properties_layer->angular_motions[i].duration);
	}

	return(angacc);
}

void MAP::LAYER::getmapcoords     (int &x,int &y,const VECTOR &pos) const
{
	if (pos.x>=0)
	{
		x=(int)(pos.x/tilewidth);
	}
	else
	{
		x=(int)(pos.x/tilewidth)-1;
	}

	if (pos.y>=0)
	{
		y=(int)(pos.y/tileheight);
	}
	else
	{
		y=(int)(pos.y/tileheight)-1;
	}
}

void MAP::LAYER::getmapcoords_rect(int &x1,int &y1,int &x2,int &y2,const VECTOR &pos,double radius) const
{
	VECTOR p1,p2;

	if (properties_layer==&(properties->primary_layer))
	{
		p1.x=pos.x-radius;
		p1.y=pos.y-radius;

		p2.x=pos.x+radius;
		p2.y=pos.y+radius;
	}
	else if (properties_layer==&(properties->secondary_layer) || properties_layer==&(properties->tertiary_layer))
	{
		VECTOR position_primary   =primary_layer->getposition();
		double orientation_primary=primary_layer->getorientation();

		VECTOR center=getcenter();

		VECTOR p=pos;

		p=p.gettransformed_to  (center,0);
		p=p.gettransformed_from(center+position_primary,orientation_primary);

		p=p.gettransformed_to  (center+position,orientation);
		p=p.gettransformed_from(center,0);

		p1.x=p.x-radius;
		p1.y=p.y-radius;

		p2.x=p.x+radius;
		p2.y=p.y+radius;
	}
	else if (properties_layer==&(properties->foreground_layer) || properties_layer==&(properties->background_layer))
	{
		VECTOR position_primary   =primary_layer->getposition();
		double orientation_primary=primary_layer->getorientation();

		VECTOR center=getcenter();

		VECTOR p=pos;

		p=p.gettransformed_to  (center,0);
		p=p.gettransformed_from(center+position_primary,orientation_primary);

		double mul;

		if (properties_layer==&(properties->foreground_layer))
		{
			mul=2;
		}
		else if (properties_layer==&(properties->background_layer))
		{
			mul=0.5;
		}

		VECTOR add=p-(p*mul);

		p*=mul;
		p+=add;

		double mradius=radius/mul;

		p1.x=p.x-mradius;
		p1.y=p.y-mradius;

		p2.x=p.x+mradius;
		p2.y=p.y+mradius;
	}

	getmapcoords(x1,y1,p1);
	getmapcoords(x2,y2,p2);
}

int    MAP::LAYER::getmapindex_wrap (int x,int y) const
{
	int ix,iy;

	if (x>=0)
	{
		ix=x%properties->mapwidth;
	}
	else
	{
		ix=(properties->mapwidth-1)-(((x*-1)-1)%properties->mapwidth);
	}

	if (y>=0)
	{
		iy=y%properties->mapheight;
	}
	else
	{
		iy=(properties->mapheight-1)-(((y*-1)-1)%properties->mapheight);
	}

	return(ix+iy*properties->mapwidth);
}

VECTOR MAP::LAYER::getmapoffset_wrap(int x,int y) const
{
	VECTOR pos((x*tilewidth)+(tilewidth/2),(y*tileheight)+(tileheight/2));

	if (pos.x>=0)
	{
		x=(int)(pos.x/(tilewidth*properties->mapwidth));
	}
	else
	{
		x=(int)(pos.x/(tilewidth*properties->mapwidth))-1;
	}

	if (pos.y>=0)
	{
		y=(int)(pos.y/(tileheight*properties->mapheight));
	}
	else
	{
		y=(int)(pos.y/(tileheight*properties->mapheight))-1;
	}

	return(VECTOR(tilewidth*x*properties->mapwidth,tileheight*y*properties->mapheight));
}

void MAP::LAYER::calc_redundancy()
{
	MAP_PROPERTIES::SHAPE left;
	MAP_PROPERTIES::SHAPE right;
	MAP_PROPERTIES::SHAPE top;
	MAP_PROPERTIES::SHAPE bottom;

	VECTOR left_offset_wrap;
	VECTOR right_offset_wrap;
	VECTOR top_offset_wrap;
	VECTOR bottom_offset_wrap;

	int i;
	for (int y=0;y<properties->mapheight;y++)
	{
		for (int x=0;x<properties->mapwidth;x++)
		{
			i=x+y*properties->mapwidth;

			if (!tiles[i].empty && !(tiles[i].numtriangles==0 && tiles[i].numquads==0))
			{
				calc_redundancy(tiles[i].numtriangles,tiles[i].triangles,tiles[i].numtriangles,tiles[i].triangles,VECTOR(0,0));
				calc_redundancy(tiles[i].numtriangles,tiles[i].triangles,tiles[i].numquads    ,tiles[i].quads    ,VECTOR(0,0));
				calc_redundancy(tiles[i].numquads    ,tiles[i].quads    ,tiles[i].numtriangles,tiles[i].triangles,VECTOR(0,0));
				calc_redundancy(tiles[i].numquads    ,tiles[i].quads    ,tiles[i].numquads    ,tiles[i].quads    ,VECTOR(0,0));

				getneighbour_shapes(left,left_offset_wrap,right,right_offset_wrap,top,top_offset_wrap,bottom,bottom_offset_wrap,x,y);

				calc_redundancy(tiles[i],left  ,left_offset_wrap);
				calc_redundancy(tiles[i],right ,right_offset_wrap);
				calc_redundancy(tiles[i],top   ,top_offset_wrap);
				calc_redundancy(tiles[i],bottom,bottom_offset_wrap);
			}
		}
	}
}

void MAP::LAYER::calc_redundancy(TILE &tile,const MAP_PROPERTIES::SHAPE &shape,const VECTOR &offset_wrap)
{
	calc_redundancy(tile.numtriangles,tile.triangles,shape.numtriangles,shape.triangles,offset_wrap);
	calc_redundancy(tile.numtriangles,tile.triangles,shape.numquads    ,shape.quads    ,offset_wrap);
	calc_redundancy(tile.numquads    ,tile.quads    ,shape.numtriangles,shape.triangles,offset_wrap);
	calc_redundancy(tile.numquads    ,tile.quads    ,shape.numquads    ,shape.quads    ,offset_wrap);
}

void MAP::LAYER::calc_redundancy(int numtriangles,TRIANGLE *triangles,int cnumtriangles,const TRIANGLE *ctriangles,const VECTOR &offset_wrap)
{
	if (triangles!=ctriangles)
	{
		for (int i=0;i<numtriangles;i++)
		{
			for (int j=0;j<cnumtriangles;j++)
			{
				calc_redundancy(triangles[i],ctriangles[j],offset_wrap);
			}
		}

		return;
	}

	for (int i=0;i<numtriangles;i++)
	{
		for (int j=0;j<cnumtriangles;j++)
		{
			if (i!=j)
			{
				calc_redundancy(triangles[i],ctriangles[j],offset_wrap);
			}
		}
	}
}

void MAP::LAYER::calc_redundancy(int numtriangles,TRIANGLE *triangles,int cnumquads    ,const QUAD     *cquads    ,const VECTOR &offset_wrap)
{
	for (int i=0;i<numtriangles;i++)
	{
		for (int j=0;j<cnumquads;j++)
		{
			calc_redundancy(triangles[i],cquads[j],offset_wrap);
		}
	}
}

void MAP::LAYER::calc_redundancy(int numquads    ,QUAD     *quads    ,int cnumtriangles,const TRIANGLE *ctriangles,const VECTOR &offset_wrap)
{
	for (int i=0;i<numquads;i++)
	{
		for (int j=0;j<cnumtriangles;j++)
		{
			calc_redundancy(quads[i],ctriangles[j],offset_wrap);
		}
	}
}

void MAP::LAYER::calc_redundancy(int numquads    ,QUAD     *quads    ,int cnumquads    ,const QUAD     *cquads    ,const VECTOR &offset_wrap)
{
	if (quads!=cquads)
	{
		for (int i=0;i<numquads;i++)
		{
			for (int j=0;j<cnumquads;j++)
			{
				calc_redundancy(quads[i],cquads[j],offset_wrap);
			}
		}

		return;
	}

	for (int i=0;i<numquads;i++)
	{
		for (int j=0;j<cnumquads;j++)
		{
			if (i!=j)
			{
				calc_redundancy(quads[i],cquads[j],offset_wrap);
			}
		}
	}
}

void MAP::LAYER::calc_redundancy(TRIANGLE &triangle,const TRIANGLE &ctriangle,const VECTOR &offset_wrap)
{
	calc_redundancy(triangle.l1,ctriangle.l1,offset_wrap);
	calc_redundancy(triangle.l1,ctriangle.l2,offset_wrap);
	calc_redundancy(triangle.l1,ctriangle.l3,offset_wrap);

	calc_redundancy(triangle.l2,ctriangle.l1,offset_wrap);
	calc_redundancy(triangle.l2,ctriangle.l2,offset_wrap);
	calc_redundancy(triangle.l2,ctriangle.l3,offset_wrap);

	calc_redundancy(triangle.l3,ctriangle.l1,offset_wrap);
	calc_redundancy(triangle.l3,ctriangle.l2,offset_wrap);
	calc_redundancy(triangle.l3,ctriangle.l3,offset_wrap);
}

void MAP::LAYER::calc_redundancy(TRIANGLE &triangle,const QUAD     &cquad    ,const VECTOR &offset_wrap)
{
	calc_redundancy(triangle.l1,cquad.l1,offset_wrap);
	calc_redundancy(triangle.l1,cquad.l2,offset_wrap);
	calc_redundancy(triangle.l1,cquad.l3,offset_wrap);
	calc_redundancy(triangle.l1,cquad.l4,offset_wrap);

	calc_redundancy(triangle.l2,cquad.l1,offset_wrap);
	calc_redundancy(triangle.l2,cquad.l2,offset_wrap);
	calc_redundancy(triangle.l2,cquad.l3,offset_wrap);
	calc_redundancy(triangle.l2,cquad.l4,offset_wrap);

	calc_redundancy(triangle.l3,cquad.l1,offset_wrap);
	calc_redundancy(triangle.l3,cquad.l2,offset_wrap);
	calc_redundancy(triangle.l3,cquad.l3,offset_wrap);
	calc_redundancy(triangle.l3,cquad.l4,offset_wrap);
}

void MAP::LAYER::calc_redundancy(QUAD     &quad    ,const TRIANGLE &ctriangle,const VECTOR &offset_wrap)
{
	calc_redundancy(quad.l1,ctriangle.l1,offset_wrap);
	calc_redundancy(quad.l1,ctriangle.l2,offset_wrap);
	calc_redundancy(quad.l1,ctriangle.l3,offset_wrap);

	calc_redundancy(quad.l2,ctriangle.l1,offset_wrap);
	calc_redundancy(quad.l2,ctriangle.l2,offset_wrap);
	calc_redundancy(quad.l2,ctriangle.l3,offset_wrap);

	calc_redundancy(quad.l3,ctriangle.l1,offset_wrap);
	calc_redundancy(quad.l3,ctriangle.l2,offset_wrap);
	calc_redundancy(quad.l3,ctriangle.l3,offset_wrap);

	calc_redundancy(quad.l4,ctriangle.l1,offset_wrap);
	calc_redundancy(quad.l4,ctriangle.l2,offset_wrap);
	calc_redundancy(quad.l4,ctriangle.l3,offset_wrap);
}

void MAP::LAYER::calc_redundancy(QUAD     &quad    ,const QUAD     &cquad    ,const VECTOR &offset_wrap)
{
	calc_redundancy(quad.l1,cquad.l1,offset_wrap);
	calc_redundancy(quad.l1,cquad.l2,offset_wrap);
	calc_redundancy(quad.l1,cquad.l3,offset_wrap);
	calc_redundancy(quad.l1,cquad.l4,offset_wrap);

	calc_redundancy(quad.l2,cquad.l1,offset_wrap);
	calc_redundancy(quad.l2,cquad.l2,offset_wrap);
	calc_redundancy(quad.l2,cquad.l3,offset_wrap);
	calc_redundancy(quad.l2,cquad.l4,offset_wrap);

	calc_redundancy(quad.l3,cquad.l1,offset_wrap);
	calc_redundancy(quad.l3,cquad.l2,offset_wrap);
	calc_redundancy(quad.l3,cquad.l3,offset_wrap);
	calc_redundancy(quad.l3,cquad.l4,offset_wrap);

	calc_redundancy(quad.l4,cquad.l1,offset_wrap);
	calc_redundancy(quad.l4,cquad.l2,offset_wrap);
	calc_redundancy(quad.l4,cquad.l3,offset_wrap);
	calc_redundancy(quad.l4,cquad.l4,offset_wrap);
}

void MAP::LAYER::calc_redundancy(LINE &line,const LINE &cline,const VECTOR &offset_wrap)
{
	VECTOR dist1=cline.getdistance(line.start-offset_wrap);
	VECTOR dist2=cline.getdistance(line.end-offset_wrap);

	if (equalszero(dist1.getlength()) && equalszero(dist2.getlength()))
	{
		line.redundant=true;
	}
}

void MAP::LAYER::getneighbour_shapes(MAP_PROPERTIES::SHAPE &left  ,VECTOR &left_offset_wrap,
                                     MAP_PROPERTIES::SHAPE &right ,VECTOR &right_offset_wrap,
                                     MAP_PROPERTIES::SHAPE &top   ,VECTOR &top_offset_wrap,
                                     MAP_PROPERTIES::SHAPE &bottom,VECTOR &bottom_offset_wrap,
                                     int x,int y) const
{
	int nx,ny;

	nx=x-1;
	ny=y;

	getshape(left,left_offset_wrap,nx,ny);

	nx=x+1;
	ny=y;

	getshape(right,right_offset_wrap,nx,ny);

	nx=x;
	ny=y-1;

	getshape(top,top_offset_wrap,nx,ny);

	nx=x;
	ny=y+1;

	getshape(bottom,bottom_offset_wrap,nx,ny);
}

void MAP::LAYER::getshape(MAP_PROPERTIES::SHAPE &shape,VECTOR &offset_wrap,int x,int y) const
{
	int i;

	if (!properties_layer->wrap)
	{
		if (x>=0 && y>=0 && x<properties->mapwidth && y<properties->mapheight)
		{
			i=x+y*properties->mapwidth;
		}
		else
		{
			i=-1;
		}

		offset_wrap.setzero();
	}
	else
	{
		i=getmapindex_wrap(x,y);

		offset_wrap=getmapoffset_wrap(x,y);
	}

	if (i!=-1)
	{
		shape.numtriangles=tiles[i].numtriangles;
		shape.numquads    =tiles[i].numquads;

		shape.triangles=tiles[i].triangles;
		shape.quads    =tiles[i].quads;
	}
	else
	{
		shape.numtriangles=0;
		shape.numquads    =0;
	}
}
