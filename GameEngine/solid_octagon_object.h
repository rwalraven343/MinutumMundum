//Code by Rogier Walraven. Do not distribute.

struct SOLID_OCTAGON_OBJECT_PROPERTIES
{
	SOLID_OCTAGON_PROPERTIES solid_octagon;
	GAS_PROPERTIES gas;
	int maxhealth;
};

struct SOLID_OCTAGON_OBJECT
{
	SOLID_OCTAGON_OBJECT();
	virtual ~SOLID_OCTAGON_OBJECT();

	void init_reset(const SOLID_OCTAGON_OBJECT_PROPERTIES &props);

	bool wasinit() const;
	bool isgone() const;

	void init_reset_kinematic_motions(int num,const KINEMATIC_MOTION *motions,double loop,bool hascol,bool hasheat);

	int gethealth() const;
	int getmaxhealth() const;

	void sethealth(int h);

	double getbounding_radius() const;

	void setposition        (const VECTOR &pos);
	void setorientation     (double angle);
	void setlinear_velocity (const VECTOR &vel);
	void setangular_velocity(double angular_vel);

	VECTOR getposition() const;
	double getorientation() const;
	VECTOR getlinear_velocity() const;
	double getangular_velocity() const;

	VECTOR getlinear_velocity (double timestep) const;
	double getangular_velocity(double timestep) const;

	VECTOR getlinear_momentum() const;
	double getangular_momentum() const;
	double getangular_momentum_origin() const;

	double getmass() const;
	double getangular_mass() const;

	void clearforces();
	void addforce (const VECTOR &frc);
	void addforce (const VECTOR &frc,const VECTOR &pos);
	void addtorque(double trq);

	virtual void addheat(double ht,const VECTOR &pos);

	virtual void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc);

	virtual void change_origin(const VECTOR &new_origin);

	double gettotal_kinetic_energy() const;
	double getlinear_kinetic_energy() const;
	double getangular_kinetic_energy() const;
	double getinternal_kinetic_energy() const;

	double gettemperature() const;

	void fillclist(COLLISIONLIST &clist,const VECTOR &other_pos,double other_radius) const;

	void setclist(const COLLISIONLIST &clist);

	virtual void integrate(double timestep);

	bool hasvapourized() const;

	double getapothem() const;

	void light(double r1,double g1,double b1,double r2,double g2,double b2,double a,double radius_mul) const;

	void draw(VIDEO::TEXTURE texture,double alpha) const;
	void draw(double r1,double g1,double b1,double r2,double g2,double b2,double a) const;

	virtual void reset()=0;

	virtual void control(double timestep)=0;

	virtual void light() const=0;

	virtual void draw() const=0;
	virtual void draw(const VIDEO::HUD &hud) const=0;

	virtual void sound() const=0;

  protected:

	COLLISIONLIST clist;

  private:

	bool initialized;
	bool vapourized;
	int health;
	SOLID_OCTAGON *solid_octagon;
	GAS<4> *gas;
	const SOLID_OCTAGON_OBJECT_PROPERTIES *properties;

	SOLID_OCTAGON_OBJECT(const SOLID_OCTAGON_OBJECT &other) {}
	SOLID_OCTAGON_OBJECT& operator=(const SOLID_OCTAGON_OBJECT &other) {}
};

inline SOLID_OCTAGON_OBJECT::SOLID_OCTAGON_OBJECT() : initialized(false),solid_octagon(new SOLID_OCTAGON),gas(new GAS<4>) {}

inline SOLID_OCTAGON_OBJECT::~SOLID_OCTAGON_OBJECT()
{
	delete solid_octagon;
	delete gas;
}

inline void SOLID_OCTAGON_OBJECT::init_reset(const SOLID_OCTAGON_OBJECT_PROPERTIES &props)
{
	vapourized=false;

	health=props.maxhealth;

	solid_octagon->init_reset(props.solid_octagon);

	properties=&props;

	initialized=true;
}

inline bool SOLID_OCTAGON_OBJECT::wasinit() const
{
	return(initialized);
}

inline bool SOLID_OCTAGON_OBJECT::isgone() const
{
	if (vapourized)
	{
		return(gas->isgone());
	}

	return(false);
}

inline void SOLID_OCTAGON_OBJECT::init_reset_kinematic_motions(int num,const KINEMATIC_MOTION *motions,double loop,bool hascol,bool hasheat)
{
	if (!vapourized)
	{
		solid_octagon->init_reset_kinematic_motions(num,motions,loop,hascol,hasheat);
	}
}

inline int SOLID_OCTAGON_OBJECT::gethealth() const
{
	return(health);
}

inline int SOLID_OCTAGON_OBJECT::getmaxhealth() const
{
	return(properties->maxhealth);
}

inline void SOLID_OCTAGON_OBJECT::sethealth(int h)
{
	if (!vapourized)
	{
		if (health<=properties->maxhealth)
		{
			health=h;

			return;
		}

		health=properties->maxhealth;
	}
}

inline double SOLID_OCTAGON_OBJECT::getbounding_radius() const
{
	if (!vapourized)
	{
		return(solid_octagon->getbounding_radius());
	}

	return(gas->getbounding_radius());
}

inline void SOLID_OCTAGON_OBJECT::setposition        (const VECTOR &pos)
{
	if (!vapourized)
	{
		solid_octagon->setposition(pos);

		return;
	}

	gas->setposition(pos);
}

inline void SOLID_OCTAGON_OBJECT::setorientation     (double angle)
{
	if (!vapourized)
	{
		solid_octagon->setorientation(angle);
	}
}

inline void SOLID_OCTAGON_OBJECT::setlinear_velocity (const VECTOR &vel)
{
	if (!vapourized)
	{
		solid_octagon->setlinear_velocity(vel);

		return;
	}

	gas->setlinear_velocity(vel);
}

inline void SOLID_OCTAGON_OBJECT::setangular_velocity(double angular_vel)
{
	if (!vapourized)
	{
		solid_octagon->setangular_velocity(angular_vel);

		return;
	}

	gas->setangular_velocity(angular_vel);
}

inline VECTOR SOLID_OCTAGON_OBJECT::getposition() const
{
	if (!vapourized)
	{
		return(solid_octagon->getposition());
	}

	return(gas->getposition());
}

inline double SOLID_OCTAGON_OBJECT::getorientation() const
{
	return(solid_octagon->getorientation());
}

inline VECTOR SOLID_OCTAGON_OBJECT::getlinear_velocity() const
{
	if (!vapourized)
	{
		return(solid_octagon->getlinear_velocity());
	}

	return(gas->getlinear_velocity());
}

inline double SOLID_OCTAGON_OBJECT::getangular_velocity() const
{
	if (!vapourized)
	{
		return(solid_octagon->getangular_velocity());
	}

	return(gas->getangular_velocity());
}

inline VECTOR SOLID_OCTAGON_OBJECT::getlinear_velocity (double) const
{
	return(getlinear_velocity());
}

inline double SOLID_OCTAGON_OBJECT::getangular_velocity(double) const
{
	return(getangular_velocity());
}

inline VECTOR SOLID_OCTAGON_OBJECT::getlinear_momentum() const
{
	if (!vapourized)
	{
		return(solid_octagon->getlinear_momentum());
	}

	return(gas->getlinear_momentum());
}

inline double SOLID_OCTAGON_OBJECT::getangular_momentum() const
{
	if (!vapourized)
	{
		return(solid_octagon->getangular_momentum());
	}

	return(gas->getangular_momentum());
}

inline double SOLID_OCTAGON_OBJECT::getangular_momentum_origin() const
{
	if (!vapourized)
	{
		return(solid_octagon->getangular_momentum_origin());
	}

	return(gas->getangular_momentum_origin());
}

inline double SOLID_OCTAGON_OBJECT::getmass() const
{
	if (!vapourized)
	{
		return(solid_octagon->getmass());
	}

	return(gas->getmass());
}

inline double SOLID_OCTAGON_OBJECT::getangular_mass() const
{
	if (!vapourized)
	{
		return(solid_octagon->getangular_mass());
	}

	return(gas->getangular_mass());
}

inline void SOLID_OCTAGON_OBJECT::clearforces()
{
	if (!vapourized)
	{
		solid_octagon->clearforces();

		return;
	}

	gas->clearforces();
}

inline void SOLID_OCTAGON_OBJECT::addforce (const VECTOR &frc)
{
	if (!vapourized)
	{
		solid_octagon->addforce(frc);

		return;
	}

	gas->addforce(frc);
}

inline void SOLID_OCTAGON_OBJECT::addforce (const VECTOR &frc,const VECTOR &pos)
{
	if (!vapourized)
	{
		solid_octagon->addforce(frc,pos);

		return;
	}

	gas->addforce(frc,pos);
}

inline void SOLID_OCTAGON_OBJECT::addtorque(double trq)
{
	if (!vapourized)
	{
		solid_octagon->addtorque(trq);

		return;
	}

	gas->addtorque(trq);
}

inline void SOLID_OCTAGON_OBJECT::addheat(double ht,const VECTOR &pos)
{
	if (!vapourized)
	{
		solid_octagon->addheat(ht);

		return;
	}

	gas->addheat(ht);
}

inline void SOLID_OCTAGON_OBJECT::addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
{
	if (!vapourized)
	{
		solid_octagon->addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);

		return;
	}

	gas->addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
}

inline void SOLID_OCTAGON_OBJECT::change_origin(const VECTOR &new_origin)
{
	clist.change_origin(new_origin);

	if (!vapourized)
	{
		solid_octagon->change_origin(new_origin);

		return;
	}

	gas->change_origin(new_origin);
}

inline double SOLID_OCTAGON_OBJECT::gettotal_kinetic_energy() const
{
	if (!vapourized)
	{
		return(solid_octagon->gettotal_kinetic_energy());
	}

	return(gas->gettotal_kinetic_energy());
}

inline double SOLID_OCTAGON_OBJECT::getlinear_kinetic_energy() const
{
	if (!vapourized)
	{
		return(solid_octagon->getlinear_kinetic_energy());
	}

	return(gas->getlinear_kinetic_energy());
}

inline double SOLID_OCTAGON_OBJECT::getangular_kinetic_energy() const
{
	if (!vapourized)
	{
		return(solid_octagon->getangular_kinetic_energy());
	}

	return(gas->getangular_kinetic_energy());
}

inline double SOLID_OCTAGON_OBJECT::getinternal_kinetic_energy() const
{
	if (!vapourized)
	{
		return(solid_octagon->getinternal_kinetic_energy());
	}

	return(gas->getinternal_kinetic_energy());
}

inline double SOLID_OCTAGON_OBJECT::gettemperature() const
{
	if (!vapourized)
	{
		return(solid_octagon->gettemperature());
	}

	return(gas->gettemperature());
}

inline void SOLID_OCTAGON_OBJECT::fillclist(COLLISIONLIST &clist,const VECTOR &other_pos,double other_radius) const
{
	clist.shapes[clist.numshapes].object=(void*)this;

	if (!vapourized)
	{
		solid_octagon->fillclist(clist,other_pos,other_radius);

		return;
	}

	gas->fillclist(clist,other_pos,other_radius);
}

inline void SOLID_OCTAGON_OBJECT::setclist(const COLLISIONLIST &clist)
{
	this->clist=clist;

	if (!vapourized)
	{
		solid_octagon->setclist(this->clist);

		return;
	}

	gas->setclist(this->clist);
}

inline void SOLID_OCTAGON_OBJECT::integrate(double timestep)
{
	if (!vapourized)
	{
		VECTOR pos;
		double ang;
		VECTOR linvel;
		double angvel;

		if (health>0)
		{
			solid_octagon->integrate(timestep);

			if (solid_octagon->isgone())
			{
				health--;

				if (health>0)
				{
					pos   =solid_octagon->getposition();
					ang   =solid_octagon->getorientation();
					linvel=solid_octagon->getlinear_velocity();
					angvel=solid_octagon->getangular_velocity();

					solid_octagon->init_reset(properties->solid_octagon);

					solid_octagon->setposition        (pos);
					solid_octagon->setorientation     (ang);
					solid_octagon->setlinear_velocity (linvel);
					solid_octagon->setangular_velocity(angvel);

					return;
				}

				pos   =solid_octagon->getposition();
				linvel=solid_octagon->getlinear_velocity();
				angvel=solid_octagon->getangular_velocity();

				gas->init_reset(properties->gas);

				gas->setposition        (pos);
				gas->setlinear_velocity (linvel);
				gas->setangular_velocity(angvel);

				vapourized=true;
			}

			return;
		}

		pos   =solid_octagon->getposition();
		linvel=solid_octagon->getlinear_velocity();
		angvel=solid_octagon->getangular_velocity();

		gas->init_reset(properties->gas);

		gas->setposition        (pos);
		gas->setlinear_velocity (linvel);
		gas->setangular_velocity(angvel);

		vapourized=true;

		return;
	}

	gas->integrate(timestep);
}

inline bool SOLID_OCTAGON_OBJECT::hasvapourized() const
{
	return(vapourized);
}

inline double SOLID_OCTAGON_OBJECT::getapothem() const
{
	return(solid_octagon->getapothem());
}

inline void SOLID_OCTAGON_OBJECT::light(double r1,double g1,double b1,double r2,double g2,double b2,double a,double radius_mul) const
{
	gas->light(r1,g1,b1,r2,g2,b2,a,radius_mul);
}

inline void SOLID_OCTAGON_OBJECT::draw(VIDEO::TEXTURE texture,double alpha) const
{
	solid_octagon->draw(texture,alpha);
}

inline void SOLID_OCTAGON_OBJECT::draw(double r1,double g1,double b1,double r2,double g2,double b2,double a) const
{
	gas->draw(r1,g1,b1,r2,g2,b2,a);
}
