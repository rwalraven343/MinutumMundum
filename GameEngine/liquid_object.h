//Code by Rogier Walraven. Do not distribute.

struct LIQUID_OBJECT_PROPERTIES
{
	LIQUID_PROPERTIES liquid;
	GAS_PROPERTIES    gas;
	int maxhealth;
};

template<int N>
struct LIQUID_OBJECT
{
	LIQUID_OBJECT();
	virtual ~LIQUID_OBJECT();

	void init_reset(const LIQUID_OBJECT_PROPERTIES &props);

	bool wasinit() const;
	bool isgone() const;

	int gethealth() const;
	int getmaxhealth() const;

	void sethealth(int h);

	double getbounding_radius() const;

	void setposition        (const VECTOR &pos);
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

	void draw(double r1,double g1,double b1,double r2,double g2,double b2,double a) const;

	bool hasvapourized() const;

	void light(double r1,double g1,double b1,double r2,double g2,double b2,double a,double radius_mul) const;

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
	LIQUID<N> *liquid;
	GAS<N>    *gas;
	const LIQUID_OBJECT_PROPERTIES *properties;

	LIQUID_OBJECT(const LIQUID_OBJECT<N> &other) {}
	LIQUID_OBJECT<N>& operator=(const LIQUID_OBJECT<N> &other) {}
};

template<int N>
inline LIQUID_OBJECT<N>::LIQUID_OBJECT() : initialized(false),liquid(new LIQUID<N>),gas(new GAS<N>) {}

template<int N>
inline LIQUID_OBJECT<N>::~LIQUID_OBJECT()
{
	delete liquid;
	delete gas;
}

template<int N>
inline void LIQUID_OBJECT<N>::init_reset(const LIQUID_OBJECT_PROPERTIES &props)
{
	vapourized=false;

	health=props.maxhealth;

	liquid->init_reset(props.liquid);

	properties=&props;

	initialized=true;
}

template<int N>
inline bool LIQUID_OBJECT<N>::wasinit() const
{
	return(initialized);
}

template<int N>
inline bool LIQUID_OBJECT<N>::isgone() const
{
	if (vapourized)
	{
		return(gas->isgone());
	}

	return(false);
}

template<int N>
inline int LIQUID_OBJECT<N>::gethealth() const
{
	return(health);
}

template<int N>
inline int LIQUID_OBJECT<N>::getmaxhealth() const
{
	return(properties->maxhealth);
}

template<int N>
inline void LIQUID_OBJECT<N>::sethealth(int h)
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

template<int N>
inline double LIQUID_OBJECT<N>::getbounding_radius() const
{
	if (!vapourized)
	{
		return(liquid->getbounding_radius());
	}

	return(gas->getbounding_radius());
}

template<int N>
inline void LIQUID_OBJECT<N>::setposition        (const VECTOR &pos)
{
	if (!vapourized)
	{
		liquid->setposition(pos);

		return;
	}

	gas->setposition(pos);
}

template<int N>
inline void LIQUID_OBJECT<N>::setlinear_velocity (const VECTOR &vel)
{
	if (!vapourized)
	{
		liquid->setlinear_velocity(vel);

		return;
	}

	gas->setlinear_velocity(vel);
}

template<int N>
inline void LIQUID_OBJECT<N>::setangular_velocity(double angular_vel)
{
	if (!vapourized)
	{
		liquid->setangular_velocity(angular_vel);

		return;
	}

	gas->setangular_velocity(angular_vel);
}

template<int N>
inline VECTOR LIQUID_OBJECT<N>::getposition() const
{
	if (!vapourized)
	{
		return(liquid->getposition());
	}

	return(gas->getposition());
}

template<int N>
inline double LIQUID_OBJECT<N>::getorientation() const
{
	return(0);
}

template<int N>
inline VECTOR LIQUID_OBJECT<N>::getlinear_velocity() const
{
	if (!vapourized)
	{
		return(liquid->getlinear_velocity());
	}

	return(gas->getlinear_velocity());
}

template<int N>
inline double LIQUID_OBJECT<N>::getangular_velocity() const
{
	if (!vapourized)
	{
		return(liquid->getangular_velocity());
	}

	return(gas->getangular_velocity());
}

template<int N>
inline VECTOR LIQUID_OBJECT<N>::getlinear_velocity (double) const
{
	return(getlinear_velocity());
}

template<int N>
inline double LIQUID_OBJECT<N>::getangular_velocity(double) const
{
	return(getangular_velocity());
}

template<int N>
inline VECTOR LIQUID_OBJECT<N>::getlinear_momentum() const
{
	if (!vapourized)
	{
		return(liquid->getlinear_momentum());
	}

	return(gas->getlinear_momentum());
}

template<int N>
inline double LIQUID_OBJECT<N>::getangular_momentum() const
{
	if (!vapourized)
	{
		return(liquid->getangular_momentum());
	}

	return(gas->getangular_momentum());
}

template<int N>
inline double LIQUID_OBJECT<N>::getangular_momentum_origin() const
{
	if (!vapourized)
	{
		return(liquid->getangular_momentum_origin());
	}

	return(gas->getangular_momentum_origin());
}

template<int N>
inline double LIQUID_OBJECT<N>::getmass() const
{
	if (!vapourized)
	{
		return(liquid->getmass());
	}

	return(gas->getmass());
}

template<int N>
inline double LIQUID_OBJECT<N>::getangular_mass() const
{
	if (!vapourized)
	{
		return(liquid->getangular_mass());
	}

	return(gas->getangular_mass());
}

template<int N>
inline void LIQUID_OBJECT<N>::clearforces()
{
	if (!vapourized)
	{
		liquid->clearforces();

		return;
	}

	gas->clearforces();
}

template<int N>
inline void LIQUID_OBJECT<N>::addforce (const VECTOR &frc)
{
	if (!vapourized)
	{
		liquid->addforce(frc);

		return;
	}

	gas->addforce(frc);
}

template<int N>
inline void LIQUID_OBJECT<N>::addforce (const VECTOR &frc,const VECTOR &pos)
{
	if (!vapourized)
	{
		liquid->addforce(frc,pos);

		return;
	}

	gas->addforce(frc,pos);
}

template<int N>
inline void LIQUID_OBJECT<N>::addtorque(double trq)
{
	if (!vapourized)
	{
		liquid->addtorque(trq);

		return;
	}

	gas->addtorque(trq);
}

template<int N>
inline void LIQUID_OBJECT<N>::addheat(double ht,const VECTOR &)
{
	if (!vapourized)
	{
		liquid->addheat(ht);

		return;
	}

	gas->addheat(ht);
}

template<int N>
inline void LIQUID_OBJECT<N>::addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
{
	if (!vapourized)
	{
		liquid->addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);

		return;
	}

	gas->addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
}

template<int N>
inline void LIQUID_OBJECT<N>::change_origin(const VECTOR &new_origin)
{
	clist.change_origin(new_origin);

	if (!vapourized)
	{
		liquid->change_origin(new_origin);

		return;
	}

	gas->change_origin(new_origin);
}

template<int N>
inline double LIQUID_OBJECT<N>::gettotal_kinetic_energy() const
{
	if (!vapourized)
	{
		return(liquid->gettotal_kinetic_energy());
	}

	return(gas->gettotal_kinetic_energy());
}

template<int N>
inline double LIQUID_OBJECT<N>::getlinear_kinetic_energy() const
{
	if (!vapourized)
	{
		return(liquid->getlinear_kinetic_energy());
	}

	return(gas->getlinear_kinetic_energy());
}

template<int N>
inline double LIQUID_OBJECT<N>::getangular_kinetic_energy() const
{
	if (!vapourized)
	{
		return(liquid->getangular_kinetic_energy());
	}

	return(gas->getangular_kinetic_energy());
}

template<int N>
inline double LIQUID_OBJECT<N>::getinternal_kinetic_energy() const
{
	if (!vapourized)
	{
		return(liquid->getinternal_kinetic_energy());
	}

	return(gas->getinternal_kinetic_energy());
}

template<int N>
inline double LIQUID_OBJECT<N>::gettemperature() const
{
	if (!vapourized)
	{
		return(liquid->gettemperature());
	}

	return(gas->gettemperature());
}

template<int N>
inline void LIQUID_OBJECT<N>::fillclist(COLLISIONLIST &clist,const VECTOR &other_pos,double other_radius) const
{
	clist.shapes[clist.numshapes].object=(void*)this;

	if (!vapourized)
	{
		liquid->fillclist(clist,other_pos,other_radius);

		return;
	}

	gas->fillclist(clist,other_pos,other_radius);
}

template<int N>
inline void LIQUID_OBJECT<N>::setclist(const COLLISIONLIST &clist)
{
	this->clist=clist;

	if (!vapourized)
	{
		liquid->setclist(this->clist);

		return;
	}

	gas->setclist(this->clist);
}

template<int N>
inline void LIQUID_OBJECT<N>::integrate(double timestep)
{
	if (!vapourized)
	{
		VECTOR pos;
		VECTOR linvel;
		double angvel;

		if (health>0)
		{
			liquid->integrate(timestep);

			if (liquid->isgone())
			{
				health--;

				if (health>0)
				{
					liquid->reset_offending_particles();

					return;
				}

				pos   =liquid->getposition();
				linvel=liquid->getlinear_velocity();
				angvel=liquid->getangular_velocity();

				gas->init_reset(properties->gas);

				gas->setposition        (pos);
				gas->setlinear_velocity (linvel);
				gas->setangular_velocity(angvel);

				vapourized=true;
			}

			return;
		}

		pos   =liquid->getposition();
		linvel=liquid->getlinear_velocity();
		angvel=liquid->getangular_velocity();

		gas->init_reset(properties->gas);

		gas->setposition        (pos);
		gas->setlinear_velocity (linvel);
		gas->setangular_velocity(angvel);

		vapourized=true;

		return;
	}

	gas->integrate(timestep);
}

template<int N>
inline void LIQUID_OBJECT<N>::draw(double r1,double g1,double b1,double r2,double g2,double b2,double a) const
{
	if (!vapourized)
	{
		liquid->draw(r1,g1,b1,r2,g2,b2,a);

		return;
	}

	gas->draw(r1,g1,b1,r2,g2,b2,a);
}

template<int N>
inline bool LIQUID_OBJECT<N>::hasvapourized() const
{
	return(vapourized);
}

template<int N>
inline void LIQUID_OBJECT<N>::light(double r1,double g1,double b1,double r2,double g2,double b2,double a,double radius_mul) const
{
	gas->light(r1,g1,b1,r2,g2,b2,a,radius_mul);
}
