//Code by Rogier Walraven. Do not distribute.

#include "GameLib\\GameLib.h"
#include "appmath.h"
#include "random.h"
#include "Geometry.h"
#include "physics.h"
#include "pointmass.h"
#include "kinematic_motion.h"
#include "solid_octagon.h"

SOLID_OCTAGON SOLID_OCTAGON::derive(double time) const
{
	SOLID_OCTAGON delta;

	int i1,i2;
	VECTOR dvdt_a,dvdt_b;

	for (int y=0;y<4-1;y++)
	{
		for (int x=0;x<4-1;x++)
		{
			i1=(x  )+(y  )*4;
			i2=(x+1)+(y  )*4;

			if (!(i1==0 || i1==3 || i1==12 || i1==15 || i2==0 || i2==3 || i2==12 || i2==15))
			{
				DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                           pointmasses[i1].velocity,pointmasses[i2].velocity,
                                           pointmasses[i1].mass,    pointmasses[i2].mass,
                                           restl,properties->skonst,properties->dkonst);
				delta.pointmasses[i1].velocity+=dvdt_a;
				delta.pointmasses[i2].velocity+=dvdt_b;
			}

			i1=(x  )+(y  )*4;
			i2=(x  )+(y+1)*4;

			if (!(i1==0 || i1==3 || i1==12 || i1==15 || i2==0 || i2==3 || i2==12 || i2==15))
			{
				DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                           pointmasses[i1].velocity,pointmasses[i2].velocity,
                                           pointmasses[i1].mass,    pointmasses[i2].mass,
                                           restl,properties->skonst,properties->dkonst);
				delta.pointmasses[i1].velocity+=dvdt_a;
				delta.pointmasses[i2].velocity+=dvdt_b;
			}

			i1=(x  )+(y  )*4;
			i2=(x+1)+(y+1)*4;

			if (!(i1==0 || i1==3 || i1==12 || i1==15 || i2==0 || i2==3 || i2==12 || i2==15))
			{
				DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                           pointmasses[i1].velocity,pointmasses[i2].velocity,
                                           pointmasses[i1].mass,    pointmasses[i2].mass,
                                           restld1,properties->skonst,properties->dkonst);
				delta.pointmasses[i1].velocity+=dvdt_a;
				delta.pointmasses[i2].velocity+=dvdt_b;
			}

			i1=(x+1)+(y  )*4;
			i2=(x  )+(y+1)*4;

			if (!(i1==0 || i1==3 || i1==12 || i1==15 || i2==0 || i2==3 || i2==12 || i2==15))
			{
				DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                           pointmasses[i1].velocity,pointmasses[i2].velocity,
                                           pointmasses[i1].mass,    pointmasses[i2].mass,
                                           restld1,properties->skonst,properties->dkonst);
				delta.pointmasses[i1].velocity+=dvdt_a;
				delta.pointmasses[i2].velocity+=dvdt_b;
			}
		}
	}

	for (int i=0;i<4-1;i++)
	{
		i1=(i  )+(4-1)*4;
		i2=(i+1)+(4-1)*4;

		if (!(i1==0 || i1==3 || i1==12 || i1==15 || i2==0 || i2==3 || i2==12 || i2==15))
		{
			DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                       pointmasses[i1].velocity,pointmasses[i2].velocity,
                                       pointmasses[i1].mass,    pointmasses[i2].mass,
                                       restl,properties->skonst,properties->dkonst);
			delta.pointmasses[i1].velocity+=dvdt_a;
			delta.pointmasses[i2].velocity+=dvdt_b;
		}

		i1=(4-1)+(i  )*4;
		i2=(4-1)+(i+1)*4;

		if (!(i1==0 || i1==3 || i1==12 || i1==15 || i2==0 || i2==3 || i2==12 || i2==15))
		{
			DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                                       pointmasses[i1].velocity,pointmasses[i2].velocity,
                                       pointmasses[i1].mass,    pointmasses[i2].mass,
                                       restl,properties->skonst,properties->dkonst);
			delta.pointmasses[i1].velocity+=dvdt_a;
			delta.pointmasses[i2].velocity+=dvdt_b;
		}
	}

	i1=4;
	i2=0;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=0;
	i2=1;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=2;
	i2=3;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=3;
	i2=7;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=11;
	i2=15;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=15;
	i2=14;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=13;
	i2=12;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=12;
	i2=8;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=5;
	i2=0;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=6;
	i2=3;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=10;
	i2=15;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	i1=9;
	i2=12;

	DVDT::spring(dvdt_a,dvdt_b,pointmasses[i1].position,pointmasses[i2].position,
                               pointmasses[i1].velocity,pointmasses[i2].velocity,
                               pointmasses[i1].mass,    pointmasses[i2].mass,
                               restld2,properties->skonst,properties->dkonst);
	delta.pointmasses[i1].velocity+=dvdt_a;
	delta.pointmasses[i2].velocity+=dvdt_b;

	for (int i=0;i<16;i++)
	{
		delta.pointmasses[i].position=pointmasses[i].velocity;

		delta.pointmasses[i].velocity+=DVDT::force(pointmasses[i].force,pointmasses[i].mass);

		if (clist)
		{
			delta.pointmasses[i].velocity+=DVDT::friction(pointmasses[i].velocity,pointmasses[i].mass,clist->friction);

			if (!(num_kinematic_motions>0 && !kinematic_motion_hascol))
			{
				delta.pointmasses[i].velocity+=DVDT::collision(pointmasses[i].position,*clist,
                                                               pointmasses[i].velocity,delta.pointmasses[i].velocity,
                                                               pointmasses[i].mass,density,
                                                               true,properties->sfriction,properties->dfriction,
                                                               time,cached_timestep);
			}
		}
	}

	if (num_kinematic_motions>0)
	{
		VECTOR acc;

		for (int i=0;i<num_kinematic_motions;i++)
		{
			acc.x+=kinematic_motions[i].deltapos.x*compact_gaussian_diff(time+kinematic_motion_time-kinematic_motions[i].start,kinematic_motions[i].duration);
			acc.y+=kinematic_motions[i].deltapos.y*compact_gaussian_diff(time+kinematic_motion_time-kinematic_motions[i].start,kinematic_motions[i].duration);
		}

		delta.pointmasses[ 0].velocity=acc;
		delta.pointmasses[ 3].velocity=acc;
		delta.pointmasses[12].velocity=acc;
		delta.pointmasses[15].velocity=acc;
	}

	return(delta);
}

SOLID_OCTAGON SOLID_OCTAGON::operator+(const SOLID_OCTAGON &delta) const
{
	SOLID_OCTAGON temp;

	for (int i=0;i<16;i++)
	{
		temp.pointmasses[i]=pointmasses[i]+delta.pointmasses[i];
	}

	temp.initialized    =initialized;
	temp.properties     =properties;
	temp.restl          =restl;
	temp.restld1        =restld1;
	temp.restld2        =restld2;
	temp.density        =density;
	temp.cached_timestep=cached_timestep;
	temp.clist          =clist;
	temp.heat_to_add    =heat_to_add;
	temp.num_kinematic_motions=num_kinematic_motions;
	temp.kinematic_motions    =kinematic_motions;
	temp.kinematic_motion_loop=kinematic_motion_loop;
	temp.kinematic_motion_time=kinematic_motion_time;
	temp.kinematic_motion_hascol =kinematic_motion_hascol;
	temp.kinematic_motion_hasheat=kinematic_motion_hasheat;

	return(temp);
}

SOLID_OCTAGON SOLID_OCTAGON::operator*(double timestep) const
{
	SOLID_OCTAGON temp;

	for (int i=0;i<16;i++)
	{
		temp.pointmasses[i]=pointmasses[i]*timestep;
	}

	return(temp);
}

SOLID_OCTAGON::SOLID_OCTAGON() : initialized(false) {}

void SOLID_OCTAGON::init_reset(const SOLID_OCTAGON_PROPERTIES &props)
{
	restl  =props.apothem*((double)2/3);
	restld1=squareroot(square(restl)*2);
	restld2=restld1/2;

	int i;
	for (int y=0;y<4;y++)
	{
		for (int x=0;x<4;x++)
		{
			i=x+y*4;

			pointmasses[i].position.x=x*restl;
			pointmasses[i].position.y=y*restl;
			pointmasses[i].velocity.setzero();
			pointmasses[i].mass=props.mass/16;
			pointmasses[i].force.setzero();
		}
	}

	pointmasses[0].position.x+=restl/2;
	pointmasses[0].position.y+=restl/2;

	pointmasses[3].position.x-=restl/2;
	pointmasses[3].position.y+=restl/2;

	pointmasses[12].position.x+=restl/2;
	pointmasses[12].position.y-=restl/2;

	pointmasses[15].position.x-=restl/2;
	pointmasses[15].position.y-=restl/2;

	properties=&props;

	density=props.mass/(square(restl)*7);

	cached_timestep=0;

	clist=0;

	heat_to_add=0;

	num_kinematic_motions=0;
	kinematic_motions=0;
	kinematic_motion_loop=0;
	kinematic_motion_time=0;
	kinematic_motion_hascol=true;
	kinematic_motion_hasheat=true;

	initialized=true;
}

bool SOLID_OCTAGON::wasinit() const
{
	return(initialized);
}

bool SOLID_OCTAGON::isgone() const
{
	VECTOR pos=getposition();
	double ang=getorientation();

	pos+=VECTOR(-properties->apothem,-properties->apothem).getrotated(ang);

	VECTOR xaxis=VECTOR(1,0).getrotated(ang);
	VECTOR yaxis=VECTOR(0,1).getrotated(ang);

	VECTOR refpos;

	double div;

	int i;
	for (int y=0;y<4;y++)
	{
		for (int x=0;x<4;x++)
		{
			i=x+y*4;

			refpos=pos+(xaxis*(x*restl))+(yaxis*(y*restl));

			div=1;

			switch (i)
			{
			case 0:
				refpos+=( xaxis*(restl/2))+( yaxis*(restl/2));
				div=2;
			break;
			case 3:
				refpos+=(-xaxis*(restl/2))+( yaxis*(restl/2));
				div=2;
			break;
			case 12:
				refpos+=( xaxis*(restl/2))+(-yaxis*(restl/2));
				div=2;
			break;
			case 15:
				refpos+=(-xaxis*(restl/2))+(-yaxis*(restl/2));
				div=2;
			break;
			}

			if (absolute((pointmasses[i].position-refpos).getdotproduct(xaxis))>=(restl/div) ||
				absolute((pointmasses[i].position-refpos).getdotproduct(yaxis))>=(restl/div))
			{
				return(true);
			}
		}
	}

	return(false);
}

void SOLID_OCTAGON::init_reset_kinematic_motions(int num,const KINEMATIC_MOTION *motions,double loop,bool hascol,bool hasheat)
{
	num_kinematic_motions=num;
	kinematic_motions=motions;
	kinematic_motion_loop=loop;
	kinematic_motion_time=0;
	kinematic_motion_hascol=hascol;
	kinematic_motion_hasheat=hasheat;
}

double SOLID_OCTAGON::getapothem() const
{
	return(properties->apothem);
}

double SOLID_OCTAGON::getbounding_radius() const
{
	return(squareroot(square(restl/2)+square(restl*1.5)));
}

void SOLID_OCTAGON::setposition        (const VECTOR &pos)
{
	VECTOR opos=getposition();

	for (int i=0;i<16;i++)
	{
		pointmasses[i].position+=pos-opos;
		pointmasses[i].velocity.setzero();
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

void SOLID_OCTAGON::setorientation     (double angle)
{
	VECTOR pos=getposition();

	VECTOR start(pos.x-properties->apothem,pos.y-properties->apothem);

	double shiftx;
	double shifty;

	int i;
	for (int y=0;y<4;y++)
	{
		for (int x=0;x<4;x++)
		{
			i=x+y*4;

			shiftx=0;
			shifty=0;

			switch (i)
			{
			case 0:
				shiftx= restl/2;
				shifty= restl/2;
			break;
			case 3:
				shiftx=-restl/2;
				shifty= restl/2;
			break;
			case 12:
				shiftx= restl/2;
				shifty=-restl/2;
			break;
			case 15:
				shiftx=-restl/2;
				shifty=-restl/2;
			break;
			}

			pointmasses[i].position.x=start.x+(x*restl)+shiftx;
			pointmasses[i].position.y=start.y+(y*restl)+shifty;
			pointmasses[i].position=LINE(pos,pointmasses[i].position).getrotatedroundstart(angle).end;
			pointmasses[i].velocity.setzero();
			pointmasses[i].force.setzero();
		}
	}

	heat_to_add=0;
}

void SOLID_OCTAGON::setlinear_velocity (const VECTOR &vel)
{
	for (int i=0;i<16;i++)
	{
		pointmasses[i].velocity=vel;
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

void SOLID_OCTAGON::setangular_velocity(double angular_vel)
{
	double angvelrad=angular_vel*(math_pi/180);

	VECTOR pos=getposition();

	for (int i=0;i<16;i++)
	{
		pointmasses[i].velocity+=((pointmasses[i].position-pos).getperpend())*angvelrad;
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

VECTOR SOLID_OCTAGON::getposition() const
{
	VECTOR pos;

	for (int i=0;i<16;i++)
	{
		pos+=pointmasses[i].position;
	}

	pos/=16;

	return(pos);
}

double SOLID_OCTAGON::getorientation() const
{
	VECTOR start,end;

	for (int y=0;y<4-1;y++)
	{
		for (int x=0;x<4;x++)
		{
			start+=pointmasses[x+(y+1)*4].position;
			end  +=pointmasses[x+(y  )*4].position;
		}
	}

	LINE line;

	for (int y=0;y<4;y++)
	{
		for (int x=0;x<4-1;x++)
		{
			line.start+=pointmasses[(x+1)+y*4].position;
			line.end  +=pointmasses[(x  )+y*4].position;
		}
	}

	line=line.getrotatedroundstart(90);

	return(VECTOR(0,-1).getanglewith((end+line.end)-(start+line.start)));
}

VECTOR SOLID_OCTAGON::getlinear_velocity() const
{
	VECTOR vel;

	for (int i=0;i<16;i++)
	{
		vel+=pointmasses[i].velocity;
	}

	vel/=16;

	return(vel);
}

double SOLID_OCTAGON::getangular_velocity() const
{
	double angvel=0;

	VECTOR pos=getposition();

	VECTOR r;
	double rlen2;

	int num=0;
	for (int i=0;i<16;i++)
	{
		r=pointmasses[i].position-pos;
		rlen2=r.getlength_squared();

		if (!equalszero(rlen2))
		{
			angvel+=(r.getcrossproduct(pointmasses[i].velocity))/rlen2;
			num++;
		}
	}

	if (num>0)
	{
		angvel/=num;

		return(angvel*(180/math_pi));
	}

	return(0);
}

VECTOR SOLID_OCTAGON::getlinear_momentum() const
{
	VECTOR result;

	for (int i=0;i<16;i++)
	{
		result+=pointmasses[i].velocity*pointmasses[i].mass;
	}

	return(result);
}

double SOLID_OCTAGON::getangular_momentum() const
{
	double result=0;

	VECTOR pos=getposition();

	for (int i=0;i<16;i++)
	{
		result+=(pointmasses[i].position-pos).getcrossproduct(pointmasses[i].velocity*pointmasses[i].mass);
	}

	return(result*(180/math_pi));
}

double SOLID_OCTAGON::getangular_momentum_origin() const
{
	double result=0;

	for (int i=0;i<16;i++)
	{
		result+=(pointmasses[i].position).getcrossproduct(pointmasses[i].velocity*pointmasses[i].mass);
	}

	return(result*(180/math_pi));
}

double SOLID_OCTAGON::getmass() const
{
	return(properties->mass);
}

double SOLID_OCTAGON::getangular_mass() const
{
	double result=0;

	VECTOR pos=getposition();

	for (int i=0;i<16;i++)
	{
		result+=((pointmasses[i].position-pos).getlength_squared())*pointmasses[i].mass;
	}

	return(result);
}

void SOLID_OCTAGON::clearforces()
{
	for (int i=0;i<16;i++)
	{
		pointmasses[i].force.setzero();
	}

	heat_to_add=0;
}

void SOLID_OCTAGON::addforce (const VECTOR &frc)
{
	VECTOR pmfrc=frc/16;

	for (int i=0;i<16;i++)
	{
		pointmasses[i].force+=pmfrc;
	}
}

void SOLID_OCTAGON::addforce (const VECTOR &frc,const VECTOR &pos)
{
	addforce(frc);
	addtorque((pos-getposition()).getcrossproduct(frc)*(180/math_pi));
}

void SOLID_OCTAGON::addtorque(double trq)
{
	double trqrad=trq*(math_pi/180);

	VECTOR pos=getposition();

	VECTOR r;
	double rlen;

	for (int i=0;i<16;i++)
	{
		r=pointmasses[i].position-pos;
		rlen=r.getlength();

		if (!equalszero(rlen))
		{
			pointmasses[i].force+=((r.getperpend().getnormal())*(trqrad/rlen))/16;
		}
	}
}

void SOLID_OCTAGON::addheat(double ht)
{
	heat_to_add+=ht;
}

void SOLID_OCTAGON::addheat_pointmasses()
{
	if (!(num_kinematic_motions>0 && !kinematic_motion_hasheat))
	{
		double pmht=                 heat_to_add/16;
		double pmke=getinternal_kinetic_energy()/16;

		VECTOR pos=getposition();

		double deltav;

		for (int i=0;i<16;i++)
		{
			deltav=squareroot(((pmht*2)/pointmasses[i].mass)+((pmke*2)/pointmasses[i].mass)) - squareroot((pmke*2)/pointmasses[i].mass);

			pointmasses[i].velocity+=(pointmasses[i].position-pos).getnormal()*deltav;
		}
	}
}

void SOLID_OCTAGON::addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
{
	for (int i=0;i<16;i++)
	{
		pointmasses[i].force+=-(linear_acc*pointmasses[i].mass);

		pointmasses[i].force+=-((pointmasses[i].velocity.getperpend())*angular_vel*(math_pi/180)*pointmasses[i].mass*2);

		pointmasses[i].force+=(pointmasses[i].position-axle_pos)*square(angular_vel*(math_pi/180))*pointmasses[i].mass;

		pointmasses[i].force+=-(((pointmasses[i].position-axle_pos).getperpend())*angular_acc*(math_pi/180)*pointmasses[i].mass);
	}
}

void SOLID_OCTAGON::change_origin(const VECTOR &new_origin)
{
	for (int i=0;i<16;i++)
	{
		pointmasses[i].position-=new_origin;
	}
}

double SOLID_OCTAGON::gettotal_kinetic_energy() const
{
	double result=0;

	for (int i=0;i<16;i++)
	{
		result+=(pointmasses[i].velocity.getlength_squared())*pointmasses[i].mass*0.5;
	}

	return(result);
}

double SOLID_OCTAGON::getlinear_kinetic_energy() const
{
	return((getlinear_velocity().getlength_squared())*getmass()*0.5);
}

double SOLID_OCTAGON::getangular_kinetic_energy() const
{
	return(square(getangular_velocity()*(math_pi/180))*getangular_mass()*0.5);
}

double SOLID_OCTAGON::getinternal_kinetic_energy() const
{
	double result=gettotal_kinetic_energy()-getlinear_kinetic_energy()-getangular_kinetic_energy();

	if (result<0)
	{
		result=0;
	}

	return(result);
}

double SOLID_OCTAGON::gettemperature() const
{
	return(getinternal_kinetic_energy()/16);
}

void SOLID_OCTAGON::getgeometry(TRIANGLE &triangle1,
                                TRIANGLE &triangle2,
                                TRIANGLE &triangle3,
                                TRIANGLE &triangle4,
                                QUAD &quad1,
                                QUAD &quad2,
                                QUAD &quad3,
                                QUAD &quad4,
                                QUAD &quad5) const
{
	VECTOR pos      =getposition();
	double ang      =getorientation();
	VECTOR vel      =getlinear_velocity();
	double angvelrad=getangular_velocity()*(math_pi/180);

	VECTOR points[16];

	double shiftx;
	double shifty;

	int i;
	for (int y=0;y<4;y++)
	{
		for (int x=0;x<4;x++)
		{
			i=x+y*4;

			shiftx=0;
			shifty=0;

			switch (i)
			{
			case 0:
				shiftx= restl/2;
				shifty= restl/2;
			break;
			case 3:
				shiftx=-restl/2;
				shifty= restl/2;
			break;
			case 12:
				shiftx= restl/2;
				shifty=-restl/2;
			break;
			case 15:
				shiftx=-restl/2;
				shifty=-restl/2;
			break;
			}

			points[i].x=(x*restl)+shiftx-properties->apothem;
			points[i].y=(y*restl)+shifty-properties->apothem;
		}
	}

	for (i=0;i<16;i++)
	{
		points[i]=points[i].gettransformed_from(pos,ang);
	}

	triangle1=TRIANGLE(points[4],points[1],points[5],true);
	triangle2=TRIANGLE(points[2],points[7],points[6],true);
	triangle3=TRIANGLE(points[10],points[11],points[14],true);
	triangle4=TRIANGLE(points[8],points[9],points[13],true);

	quad1=QUAD(points[4],points[5],points[9],points[8],true);
	quad2=QUAD(points[5],points[6],points[10],points[9],true);
	quad3=QUAD(points[6],points[7],points[11],points[10],true);
	quad4=QUAD(points[1],points[2],points[6],points[5],true);
	quad5=QUAD(points[9],points[10],points[14],points[13],true);

	filltriangle(triangle1,pos,vel,angvelrad);
	filltriangle(triangle2,pos,vel,angvelrad);
	filltriangle(triangle3,pos,vel,angvelrad);
	filltriangle(triangle4,pos,vel,angvelrad);

	fillquad(quad1,pos,vel,angvelrad);
	fillquad(quad2,pos,vel,angvelrad);
	fillquad(quad3,pos,vel,angvelrad);
	fillquad(quad4,pos,vel,angvelrad);
	fillquad(quad5,pos,vel,angvelrad);

	triangle1.l1.redundant=false;
	triangle2.l1.redundant=false;
	triangle3.l2.redundant=false;
	triangle4.l3.redundant=false;

	quad1.l4.redundant=false;
	quad3.l2.redundant=false;
	quad4.l1.redundant=false;
	quad5.l3.redundant=false;
}

void SOLID_OCTAGON::filltriangle(TRIANGLE &triangle,const VECTOR &pos,const VECTOR &vel,double angvelrad) const
{
	triangle.vel1=vel;
	triangle.vel2=vel;
	triangle.vel3=vel;

	triangle.vel1+=((triangle.l1.start-pos).getperpend())*angvelrad;
	triangle.vel2+=((triangle.l2.start-pos).getperpend())*angvelrad;
	triangle.vel3+=((triangle.l3.start-pos).getperpend())*angvelrad;

	triangle.density=density;

	triangle.sfriction=properties->sfriction;
	triangle.dfriction=properties->dfriction;

	triangle.absorption=properties->absorption;
	triangle.refraction=properties->refraction;
}

void SOLID_OCTAGON::fillquad(QUAD &quad,const VECTOR &pos,const VECTOR &vel,double angvelrad) const
{
	quad.vel1=vel;
	quad.vel2=vel;
	quad.vel3=vel;
	quad.vel4=vel;

	quad.vel1+=((quad.l1.start-pos).getperpend())*angvelrad;
	quad.vel2+=((quad.l2.start-pos).getperpend())*angvelrad;
	quad.vel3+=((quad.l3.start-pos).getperpend())*angvelrad;
	quad.vel4+=((quad.l4.start-pos).getperpend())*angvelrad;

	quad.density=density;

	quad.sfriction=properties->sfriction;
	quad.dfriction=properties->dfriction;

	quad.absorption=properties->absorption;
	quad.refraction=properties->refraction;
}

void SOLID_OCTAGON::fillclist(COLLISIONLIST &clist,const VECTOR &other_pos,double other_radius) const
{
	VECTOR pos   =getposition();
	double radius=getbounding_radius();

	if ((other_pos-pos).getlength_squared()<=square((radius*collision_bounding_radius_mul)+(other_radius*collision_bounding_radius_mul)))
	{
		clist.shapes[clist.numshapes].position=pos;
		clist.shapes[clist.numshapes].radius  =radius;

		clist.shapes[clist.numshapes].numtriangles=4;
		clist.shapes[clist.numshapes].numquads    =5;
		clist.shapes[clist.numshapes].numcircles  =0;

		getgeometry(clist.shapes[clist.numshapes].triangles[0],
                    clist.shapes[clist.numshapes].triangles[1],
                    clist.shapes[clist.numshapes].triangles[2],
                    clist.shapes[clist.numshapes].triangles[3],
                    clist.shapes[clist.numshapes].quads[0],
                    clist.shapes[clist.numshapes].quads[1],
                    clist.shapes[clist.numshapes].quads[2],
                    clist.shapes[clist.numshapes].quads[3],
                    clist.shapes[clist.numshapes].quads[4]);

		clist.numshapes++;
	}
}

void SOLID_OCTAGON::setclist(const COLLISIONLIST &clist)
{
	this->clist=&clist;
}

void SOLID_OCTAGON::integrate(double timestep)
{
	cached_timestep=timestep;

	addheat_pointmasses();

	*this=integrate_RK4(*this,timestep);

	if (num_kinematic_motions>0)
	{
		kinematic_motion_time=modulus(kinematic_motion_time+timestep,kinematic_motion_loop);
	}
}

void SOLID_OCTAGON::draw(VIDEO::TEXTURE texture,double alpha) const
{
	VIDEO::BATCH::draw_polygon_texture_start(texture);

	double ang=getorientation();

	POINTMASS_SOLID pointmass1,pointmass2,pointmass3;

	double shiftx,shifty;

	int i1,i2,i3;
	for (int y=0;y<4-1;y++)
	{
		for (int x=0;x<4-1;x++)
		{
			i1=(x  )+((y  )*4);
			i2=(x+1)+((y  )*4);
			i3=(x+1)+((y+1)*4);

			shiftx=0;
			shifty=0;

			switch (i1)
			{
			case 0:
				shiftx= restl/2;
				shifty= restl/2;
			break;
			case 3:
				shiftx=-restl/2;
				shifty= restl/2;
			break;
			case 12:
				shiftx= restl/2;
				shifty=-restl/2;
			break;
			case 15:
				shiftx=-restl/2;
				shifty=-restl/2;
			break;
			}

			pointmass1.position=pointmasses[i1].position+VECTOR(-shiftx,-shifty).getrotated(ang);

			shiftx=0;
			shifty=0;

			switch (i2)
			{
			case 0:
				shiftx= restl/2;
				shifty= restl/2;
			break;
			case 3:
				shiftx=-restl/2;
				shifty= restl/2;
			break;
			case 12:
				shiftx= restl/2;
				shifty=-restl/2;
			break;
			case 15:
				shiftx=-restl/2;
				shifty=-restl/2;
			break;
			}

			pointmass2.position=pointmasses[i2].position+VECTOR(-shiftx,-shifty).getrotated(ang);

			shiftx=0;
			shifty=0;

			switch (i3)
			{
			case 0:
				shiftx= restl/2;
				shifty= restl/2;
			break;
			case 3:
				shiftx=-restl/2;
				shifty= restl/2;
			break;
			case 12:
				shiftx= restl/2;
				shifty=-restl/2;
			break;
			case 15:
				shiftx=-restl/2;
				shifty=-restl/2;
			break;
			}

			pointmass3.position=pointmasses[i3].position+VECTOR(-shiftx,-shifty).getrotated(ang);

			draw_polygon(pointmass1,
                         pointmass2,
                         pointmass3,
                         ((double)x  )/(4-1),
                         ((double)x+1)/(4-1),
                         ((double)y  )/(4-1),
                         ((double)y+1)/(4-1),alpha);
		}
	}

	for (int y=0;y<4-1;y++)
	{
		for (int x=0;x<4-1;x++)
		{
			i1=(x+1)+((y+1)*4);
			i2=(x  )+((y+1)*4);
			i3=(x  )+((y  )*4);

			shiftx=0;
			shifty=0;

			switch (i1)
			{
			case 0:
				shiftx= restl/2;
				shifty= restl/2;
			break;
			case 3:
				shiftx=-restl/2;
				shifty= restl/2;
			break;
			case 12:
				shiftx= restl/2;
				shifty=-restl/2;
			break;
			case 15:
				shiftx=-restl/2;
				shifty=-restl/2;
			break;
			}

			pointmass1.position=pointmasses[i1].position+VECTOR(-shiftx,-shifty).getrotated(ang);

			shiftx=0;
			shifty=0;

			switch (i2)
			{
			case 0:
				shiftx= restl/2;
				shifty= restl/2;
			break;
			case 3:
				shiftx=-restl/2;
				shifty= restl/2;
			break;
			case 12:
				shiftx= restl/2;
				shifty=-restl/2;
			break;
			case 15:
				shiftx=-restl/2;
				shifty=-restl/2;
			break;
			}

			pointmass2.position=pointmasses[i2].position+VECTOR(-shiftx,-shifty).getrotated(ang);

			shiftx=0;
			shifty=0;

			switch (i3)
			{
			case 0:
				shiftx= restl/2;
				shifty= restl/2;
			break;
			case 3:
				shiftx=-restl/2;
				shifty= restl/2;
			break;
			case 12:
				shiftx= restl/2;
				shifty=-restl/2;
			break;
			case 15:
				shiftx=-restl/2;
				shifty=-restl/2;
			break;
			}

			pointmass3.position=pointmasses[i3].position+VECTOR(-shiftx,-shifty).getrotated(ang);

			draw_polygon(pointmass1,
                         pointmass2,
                         pointmass3,
                         ((double)x+1)/(4-1),
                         ((double)x  )/(4-1),
                         ((double)y+1)/(4-1),
                         ((double)y  )/(4-1),alpha);
		}
	}

	VIDEO::BATCH::draw_polygon_texture_end();
}

void SOLID_OCTAGON::draw_polygon(const POINTMASS_SOLID &a,const POINTMASS_SOLID &b,const POINTMASS_SOLID &c,
                                 double tsx,double tex,double tsy,double tey,double alpha) const
{
	VIDEO::BATCH::draw_polygon_texture(a.position.x,a.position.y,b.position.x,b.position.y,c.position.x,c.position.y,
                                       tsx,tex,tsy,tey,alpha);
}
