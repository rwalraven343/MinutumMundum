//Code by Rogier Walraven. Do not distribute.

struct BEAM_PROPERTIES
{
	double length;
	double power;
	double gridsize;
};

template<class O>
struct BEAM
{
	BEAM(int num);
	~BEAM();

	void init_reset(const BEAM_PROPERTIES &props,const VECTOR &pos,double angle);
	void init_reset(const BEAM_PROPERTIES &props,const VECTOR &pos,double angle,const O &object);
	void init_reset(const BEAM_PROPERTIES &props,const VECTOR &pos,double angle,const O &object,double &fuel);

	bool wasinit() const;
	bool isgone() const;

	void activate();
	void deactivate();

	bool isactive() const;

	double getgridsize() const;

	LINE getline() const;

	const O* getobject() const;

	void setclist(const COLLISIONLIST &clist);

	BEAM<O>* getnextbeam(bool &hit,VECTOR &isect,bool &needsaddheat,double &heat,void **object,bool first_iter,double timestep);

	void change_origin(const VECTOR &new_origin);

	void light(double r,double g,double b,double a,double thickness) const;

	void draw(double r,double g,double b,double a,double thickness) const;

  private:

	bool initialized;
	bool active;
	const O *object;
	double *fuel;
	BEAM_PROPERTIES properties;
	VECTOR position;
	double orientation;
	double length_accum;
	double length_total;
	double power_total;
	const COLLISIONLIST *clist;
	BEAM<O> *next;

	void init_reset(const BEAM_PROPERTIES &props,const VECTOR &pos,double angle,double len_accum,double len_total,double pow_total);

	LINE getline(double timestep) const;

	double getnextangle(bool &reflect,double a1,double n1,double n2) const;

	void batch_light(double r,double g,double b,double a,double thickness) const;

	void batch_draw(double r,double g,double b,double a,double thickness) const;

	BEAM(const BEAM<O> &other) {}
	BEAM<O>& operator=(const BEAM<O> &other) {}
};

template<class O>
struct BEAM_OBJECT
{
	BEAM_OBJECT() {}

	virtual int getnumbeams() const=0;

	virtual BEAM<O>& getbeam(int i)=0;

  private:

	BEAM_OBJECT(const BEAM_OBJECT<O> &other) {}
	BEAM_OBJECT<O>& operator=(const BEAM_OBJECT<O> &other) {}
};

template<class O>
BEAM<O>::BEAM(int num) : initialized(false)
{
	if (num>0)
	{
		next=new BEAM<O>(num-1);

		return;
	}

	next=0;
}

template<class O>
BEAM<O>::~BEAM()
{
	delete next;
}

template<class O>
void BEAM<O>::init_reset(const BEAM_PROPERTIES &props,const VECTOR &pos,double angle)
{
	active=false;

	this->object=0;
	this->fuel  =0;

	properties=props;

	position   =pos;
	orientation=angle;

	length_accum=0;
	length_total=props.length;

	power_total=props.power;

	clist=0;

	next->init_reset(properties,position,orientation,length_accum,length_total,power_total);

	initialized=true;
}

template<class O>
void BEAM<O>::init_reset(const BEAM_PROPERTIES &props,const VECTOR &pos,double angle,const O &object)
{
	active=false;

	this->object=&object;
	this->fuel  =0;

	properties=props;

	position   =pos;
	orientation=angle;

	length_accum=0;
	length_total=props.length;

	power_total=props.power;

	clist=0;

	next->init_reset(properties,position,orientation,length_accum,length_total,power_total);

	initialized=true;
}

template<class O>
void BEAM<O>::init_reset(const BEAM_PROPERTIES &props,const VECTOR &pos,double angle,const O &object,double &fuel)
{
	active=false;

	this->object=&object;
	this->fuel  =&fuel;

	properties=props;

	position   =pos;
	orientation=angle;

	length_accum=0;
	length_total=props.length;

	power_total=props.power;

	clist=0;

	next->init_reset(properties,position,orientation,length_accum,length_total,power_total);

	initialized=true;
}

template<class O>
void BEAM<O>::init_reset(const BEAM_PROPERTIES &props,const VECTOR &pos,double angle,double len_accum,double len_total,double pow_total)
{
	active=false;

	this->object=0;
	this->fuel  =0;

	properties=props;

	position   =pos;
	orientation=angle;

	length_accum=len_accum;
	length_total=len_total;

	power_total=pow_total;

	clist=0;

	if (next)
	{
		next->init_reset(properties,position,orientation,length_accum,length_total,power_total);
	}

	initialized=true;
}

template<class O>
bool BEAM<O>::wasinit() const
{
	if (initialized)
	{
		if (object)
		{
			return(object->wasinit());
		}

		return(true);
	}

	return(false);
}

template<class O>
bool BEAM<O>::isgone() const
{
	if (object)
	{
		return(object->isgone() || object->hasvapourized());
	}

	return(false);
}

template<class O>
void BEAM<O>::activate()
{
	active=true;
}

template<class O>
void BEAM<O>::deactivate()
{
	active=false;

	if (next)
	{
		next->deactivate();
	}
}

template<class O>
bool BEAM<O>::isactive() const
{
	if (active)
	{
		if (fuel)
		{
			return(*fuel>0);
		}

		return(true);
	}

	return(false);
}

template<class O>
double BEAM<O>::getgridsize() const
{
	return(properties.gridsize);
}

template<class O>
LINE BEAM<O>::getline() const
{
	LINE line=LINE(position,position+(VECTOR(0,-1)*properties.length)).getrotatedroundstart(orientation);

	if (object)
	{
		VECTOR objpos=object->getposition();
		double objang=object->getorientation();

		line.start=line.start.gettransformed_from(objpos,objang);
		line.end=line.end.gettransformed_from(objpos,objang);
	}

	return(line);
}

template<class O>
LINE BEAM<O>::getline(double timestep) const
{
	LINE line=LINE(position,position+(VECTOR(0,-1)*properties.length)).getrotatedroundstart(orientation);

	if (object)
	{
		VECTOR objpos   =object->getposition();
		double objang   =object->getorientation();
		VECTOR objvel   =object->getlinear_velocity(timestep);
		double objangvel=object->getangular_velocity(timestep);

		objpos=objpos +    objvel*timestep;
		objang=objang + objangvel*timestep;

		line.start=line.start.gettransformed_from(objpos,objang);
		line.end=line.end.gettransformed_from(objpos,objang);
	}

	if (next && next->isactive())
	{
		line.end=next->position;
	}

	return(line);
}

template<class O>
const O* BEAM<O>::getobject() const
{
	return(object);
}

template<class O>
void BEAM<O>::setclist(const COLLISIONLIST &clist)
{
	this->clist=&clist;
}

template<class O>
BEAM<O>* BEAM<O>::getnextbeam(bool &hit,VECTOR &isect,bool &needsaddheat,double &heat,void **object,bool first_iter,double timestep)
{
	if (fuel && first_iter)
	{
		*fuel-=properties.power*timestep;

		if (*fuel<0)
		{
			*fuel=0;
		}
	}

	if (next)
	{
		next->deactivate();
	}

	hit=false;

	if (clist)
	{
		LINE line=getline(timestep);

		VECTOR surf_normal;
		double absorp;

		hit=clist->getintersection_seg(isect,surf_normal,absorp,object,line,timestep);

		if (hit)
		{
			VECTOR line_normal=(line.start-line.end).getnormal();
			double isected_line_len=(isect-line.start).getlength();

			bool entering=true;

			if (surf_normal.getdotproduct(line_normal)<0)
			{
				surf_normal=-surf_normal;

				entering=false;
			}

			VECTOR npos1=isect + surf_normal*math_epsilon;
			VECTOR npos2=isect - surf_normal*math_epsilon;

			double n1,n2;

			if (!clist->iswithin(n1,npos1,timestep))
			{
				n1=clist->refraction;
			}

			if (!clist->iswithin(n2,npos2,timestep))
			{
				n2=clist->refraction;
			}

			bool interfdiff=!equalszero(n1-n2);

			needsaddheat=entering && interfdiff;

			if (needsaddheat)
			{
				heat=properties.power*(1-((length_accum+isected_line_len)/length_total))*absorp*timestep;

				if (heat<0)
				{
					heat=0;
				}
			}

			if (next)
			{
				bool reflect;

				double a2=getnextangle(reflect,surf_normal.getanglewith(line_normal),n1,n2);

				VECTOR position_next;

				if (reflect)
				{
					position_next=npos1;
				}
				else
				{
					position_next=npos2;
				}

				double orientation_next=VECTOR(0,-1).getanglewith(surf_normal) + a2;

				BEAM_PROPERTIES props_next;

				props_next.length=properties.length - isected_line_len;

				if (props_next.length<0)
				{
					props_next.length=0;
				}

				props_next.power=properties.power;

				if (interfdiff)
				{
					props_next.power*=(1-absorp);
				}

				props_next.gridsize=properties.gridsize;

				next->init_reset(props_next,position_next,orientation_next,length_accum+isected_line_len,length_total,power_total);

				next->activate();

				return(next);
			}
		}
	}

	return(0);
}

template<class O>
double BEAM<O>::getnextangle(bool &reflect,double a1,double n1,double n2) const
{
	double oi=a1*(math_pi/180);

	double sinot=(sine(oi)*n1)/n2;

	if (sinot>=-1 && sinot<=1)
	{
		double ot=invsine(sinot);

		double fresd=n1*cosine(oi) + n2*cosine(ot);

		if (!equalszero(fresd))
		{
			double fresn=n1*cosine(oi) - n2*cosine(ot);

			if (square(fresn/fresd)<=0.5)
			{
				reflect=false;

				return((ot*(180/math_pi))+180);
			}
		}
	}

	reflect=true;

	return(-a1);
}

template<class O>
void BEAM<O>::change_origin(const VECTOR &new_origin)
{
	if (!object)
	{
		position-=new_origin;
	}

	if (next && next->isactive())
	{
		next->change_origin(new_origin);
	}
}

template<class O>
void BEAM<O>::light(double r,double g,double b,double a,double thickness) const
{
	LINE line=getline(0);

	double texstart=length_accum/length_total;
	double texend  =(length_accum+line.getlength())/length_total;

	if (texstart>1) {texstart=1;}
	if (texend  >1) {texend  =1;}

	VIDEO::DYNAMIC_LIGHTING::BATCH::draw_beamlight_start();

	VIDEO::DYNAMIC_LIGHTING::BATCH::draw_beamlight(line.start.x,line.start.y,line.end.x,line.end.y,r,g,b,a*(properties.power/power_total),texstart,texend,thickness);

	if (next->isactive())
	{
		next->batch_light(r,g,b,a,thickness);
	}

	VIDEO::DYNAMIC_LIGHTING::BATCH::draw_beamlight_end();
}

template<class O>
void BEAM<O>::batch_light(double r,double g,double b,double a,double thickness) const
{
	LINE line=getline(0);

	double texstart=length_accum/length_total;
	double texend  =(length_accum+line.getlength())/length_total;

	if (texstart>1) {texstart=1;}
	if (texend  >1) {texend  =1;}

	VIDEO::DYNAMIC_LIGHTING::BATCH::draw_beamlight(line.start.x,line.start.y,line.end.x,line.end.y,r,g,b,a*(properties.power/power_total),texstart,texend,thickness);

	if (next && next->isactive())
	{
		next->batch_light(r,g,b,a,thickness);
	}
}

template<class O>
void BEAM<O>::draw(double r,double g,double b,double a,double thickness) const
{
	LINE line=getline(0);

	double texstart=length_accum/length_total;
	double texend  =(length_accum+line.getlength())/length_total;

	if (texstart>1) {texstart=1;}
	if (texend  >1) {texend  =1;}

	VIDEO::BATCH::draw_beam_start();

	VIDEO::BATCH::draw_beam(line.start.x,line.start.y,line.end.x,line.end.y,r,g,b,a*(properties.power/power_total),texstart,texend,thickness);

	if (next->isactive())
	{
		next->batch_draw(r,g,b,a,thickness);
	}

	VIDEO::BATCH::draw_beam_end();
}

template<class O>
void BEAM<O>::batch_draw(double r,double g,double b,double a,double thickness) const
{
	LINE line=getline(0);

	double texstart=length_accum/length_total;
	double texend  =(length_accum+line.getlength())/length_total;

	if (texstart>1) {texstart=1;}
	if (texend  >1) {texend  =1;}

	VIDEO::BATCH::draw_beam(line.start.x,line.start.y,line.end.x,line.end.y,r,g,b,a*(properties.power/power_total),texstart,texend,thickness);

	if (next && next->isactive())
	{
		next->batch_draw(r,g,b,a,thickness);
	}
}
