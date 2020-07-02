//Code by Rogier Walraven. Do not distribute.

struct CONSTRAINT_LIQUID_PROPERTIES
{
	double rkonst;
	double akonst;
	double dkonst;
};

template<class O1,class O2>
struct CONSTRAINT_LIQUID
{
	CONSTRAINT_LIQUID();

	void init_reset(const CONSTRAINT_LIQUID_PROPERTIES &props,O1 *object1,O2 *object2,const VECTOR &pos1,const VECTOR &pos2,double minmul,double maxmul);

	bool wasinit() const;
	bool isgone() const;

	void enforce(double timestep);

	void change_origin(const VECTOR &new_origin);

  private:

	bool initialized;
	O1 *object1;
	O2 *object2;
	const CONSTRAINT_LIQUID_PROPERTIES *properties;
	VECTOR relpos1;
	VECTOR relpos2;
	double mindist,maxdist;

	CONSTRAINT_LIQUID(const CONSTRAINT_LIQUID<O1,O2> &other) {}
	CONSTRAINT_LIQUID<O1,O2>& operator=(const CONSTRAINT_LIQUID<O1,O2> &other) {}
};

template<class O1,class O2>
CONSTRAINT_LIQUID<O1,O2>::CONSTRAINT_LIQUID() : initialized(false) {}

template<class O1,class O2>
void CONSTRAINT_LIQUID<O1,O2>::init_reset(const CONSTRAINT_LIQUID_PROPERTIES &props,O1 *object1,O2 *object2,const VECTOR &pos1,const VECTOR &pos2,double minmul,double maxmul)
{
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

	double len=(pos2-pos1).getlength();

	mindist=len*minmul;
	maxdist=len*maxmul;

	initialized=true;
}

template<class O1,class O2>
bool CONSTRAINT_LIQUID<O1,O2>::wasinit() const
{
	return(initialized);
}

template<class O1,class O2>
bool CONSTRAINT_LIQUID<O1,O2>::isgone() const
{
	if (!object1 && !object2)
	{
		return(true);
	}

	if (!object1 &&  object2)
	{
		return(object2->isgone());
	}

	if ( object1 && !object2)
	{
		return(object1->isgone());
	}

	if ( object1 &&  object2)
	{
		return(object1->isgone() || object2->isgone());
	}

	return(true);
}

template<class O1,class O2>
void CONSTRAINT_LIQUID<O1,O2>::enforce(double timestep)
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

	if (object1)
	{
		VECTOR frc1=DVDT::vanderwaals_liquid(pos1,pos2,vel1,vel2,1,mindist,maxdist,properties->rkonst,properties->akonst,properties->dkonst);

		object1->addforce(frc1,pos1);
	}

	if (object2)
	{
		VECTOR frc2=DVDT::vanderwaals_liquid(pos2,pos1,vel2,vel1,1,mindist,maxdist,properties->rkonst,properties->akonst,properties->dkonst);

		object2->addforce(frc2,pos2);
	}
}

template<class O1,class O2>
void CONSTRAINT_LIQUID<O1,O2>::change_origin(const VECTOR &new_origin)
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
