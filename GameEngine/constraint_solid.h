//Code by Rogier Walraven. Do not distribute.

struct CONSTRAINT_SOLID_PROPERTIES
{
	double skonst;
	double dkonst;
	double lobp;
	double upbp;
};

template<class O1,class O2>
struct CONSTRAINT_SOLID
{
	CONSTRAINT_SOLID();

	void init_reset(const CONSTRAINT_SOLID_PROPERTIES &props,O1 *object1,O2 *object2,const VECTOR &pos1,const VECTOR &pos2);

	bool wasinit() const;
	bool isgone() const;

	void rotate_worldspace_coords_around_point(const VECTOR &pos,double ang);

	void enforce(double timestep);

	void change_origin(const VECTOR &new_origin);

	void draw(VIDEO::TEXTURE texture,double alpha,double thickness,double extend) const;

  private:

	bool initialized;
	bool broken;
	O1 *object1;
	O2 *object2;
	const CONSTRAINT_SOLID_PROPERTIES *properties;
	VECTOR relpos1;
	VECTOR relpos2;
	double restl;

	CONSTRAINT_SOLID(const CONSTRAINT_SOLID<O1,O2> &other) {}
	CONSTRAINT_SOLID<O1,O2>& operator=(const CONSTRAINT_SOLID<O1,O2> &other) {}
};

template<class O1,class O2>
CONSTRAINT_SOLID<O1,O2>::CONSTRAINT_SOLID() : initialized(false) {}

template<class O1,class O2>
void CONSTRAINT_SOLID<O1,O2>::init_reset(const CONSTRAINT_SOLID_PROPERTIES &props,O1 *object1,O2 *object2,const VECTOR &pos1,const VECTOR &pos2)
{
	broken=false;

	this->object1=0;
	this->object2=0;

	if (object1 && object1->wasinit() && !object1->isgone())
	{
		this->object1=object1;
	}

	if (object2 && object2->wasinit() && !object2->isgone())
	{
		this->object2=object2;
	}

	properties=&props;

	relpos1=pos1;
	relpos2=pos2;

	if (this->object1)
	{
		relpos1=relpos1.gettransformed_to(object1->getposition(),object1->getorientation());
	}

	if (this->object2)
	{
		relpos2=relpos2.gettransformed_to(object2->getposition(),object2->getorientation());
	}

	restl=(pos2-pos1).getlength();

	initialized=true;
}

template<class O1,class O2>
bool CONSTRAINT_SOLID<O1,O2>::wasinit() const
{
	return(initialized);
}

template<class O1,class O2>
bool CONSTRAINT_SOLID<O1,O2>::isgone() const
{
	if (!object1 && !object2)
	{
		return(true);
	}

	if (!object1 &&  object2)
	{
		return(broken || object2->isgone());
	}

	if ( object1 && !object2)
	{
		return(broken || object1->isgone());
	}

	if ( object1 &&  object2)
	{
		return(broken || object1->isgone() || object2->isgone());
	}

	return(true);
}

template<class O1,class O2>
void CONSTRAINT_SOLID<O1,O2>::rotate_worldspace_coords_around_point(const VECTOR &pos,double ang)
{
	if (!object1)
	{
		relpos1=LINE(pos,relpos1).getrotatedroundstart(ang).end;
	}

	if (!object2)
	{
		relpos2=LINE(pos,relpos2).getrotatedroundstart(ang).end;
	}
}

template<class O1,class O2>
void CONSTRAINT_SOLID<O1,O2>::enforce(double timestep)
{
	VECTOR pos1=relpos1;
	VECTOR pos2=relpos2;

	VECTOR vel1;
	VECTOR vel2;

	if (object1)
	{
		VECTOR objpos   =object1->getposition();
		double objang   =object1->getorientation();
		VECTOR objvel   =object1->getlinear_velocity(timestep/2);
		double objangvel=object1->getangular_velocity(timestep/2);

		objpos=objpos +    objvel*(timestep/2);
		objang=objang + objangvel*(timestep/2);

		pos1=relpos1.gettransformed_from(objpos,objang);
		vel1=objvel+(((pos1-objpos).getperpend())*objangvel*(math_pi/180));
	}

	if (object2)
	{
		VECTOR objpos   =object2->getposition();
		double objang   =object2->getorientation();
		VECTOR objvel   =object2->getlinear_velocity(timestep/2);
		double objangvel=object2->getangular_velocity(timestep/2);

		objpos=objpos +    objvel*(timestep/2);
		objang=objang + objangvel*(timestep/2);

		pos2=relpos2.gettransformed_from(objpos,objang);
		vel2=objvel+(((pos2-objpos).getperpend())*objangvel*(math_pi/180));
	}

	double lenratio=(pos2-pos1).getlength()/restl;

	if (lenratio<properties->lobp || lenratio>properties->upbp)
	{
		broken=true;

		return;
	}

	VECTOR frc1;
	VECTOR frc2;

	DVDT::spring(frc1,frc2,pos1,pos2,vel1,vel2,1,1,restl,properties->skonst,properties->dkonst);

	if (object1)
	{
		object1->addforce(frc1,pos1);
	}

	if (object2)
	{
		object2->addforce(frc2,pos2);
	}
}

template<class O1,class O2>
void CONSTRAINT_SOLID<O1,O2>::change_origin(const VECTOR &new_origin)
{
	if (!object1)
	{
		relpos1-=new_origin;
	}

	if (!object2)
	{
		relpos2-=new_origin;
	}
}

template<class O1,class O2>
void CONSTRAINT_SOLID<O1,O2>::draw(VIDEO::TEXTURE texture,double alpha,double thickness,double extend) const
{
	VECTOR pos1=relpos1;
	VECTOR pos2=relpos2;

	if (object1)
	{
		VECTOR objpos=object1->getposition();
		double objang=object1->getorientation();

		pos1=relpos1.gettransformed_from(objpos,objang);
	}

	if (object2)
	{
		VECTOR objpos=object2->getposition();
		double objang=object2->getorientation();

		pos2=relpos2.gettransformed_from(objpos,objang);
	}

	VECTOR vec12=pos2-pos1;

	VECTOR xaxis= vec12.getperpend().getnormal();
	VECTOR yaxis=-vec12.getnormal();

	VECTOR xshift=xaxis*(thickness/2);
	VECTOR yshift=yaxis*extend;

	VECTOR c1=pos2 - xshift - yshift;
	VECTOR c2=pos2 + xshift - yshift;
	VECTOR c3=pos1 + xshift + yshift;
	VECTOR c4=pos1 - xshift + yshift;

	VIDEO::draw_quad_texture(c1.x,c1.y,c2.x,c2.y,c3.x,c3.y,c4.x,c4.y,texture,alpha);
}
