//Code by Rogier Walraven. Do not distribute.

template<class O>
struct MOTOR
{
	MOTOR();

	void init_reset(O &object,double &fuel);

	bool wasinit() const;
	bool isgone() const;

	void addforce (const VECTOR &frc,double timestep);
	void addforce (const VECTOR &frc,const VECTOR &pos,double timestep);
	void addtorque(double trq,double timestep);

  private:

	bool initialized;
	O      *object;
	double *fuel;

	MOTOR(const MOTOR<O> &other) {}
	MOTOR<O>& operator=(const MOTOR<O> &other) {}
};

template<class O>
MOTOR<O>::MOTOR() : initialized(false) {}

template<class O>
void MOTOR<O>::init_reset(O &object,double &fuel)
{
	this->object=&object;
	this->fuel  =&fuel;

	initialized=true;
}

template<class O>
bool MOTOR<O>::wasinit() const
{
	return(initialized && object->wasinit());
}

template<class O>
bool MOTOR<O>::isgone() const
{
	return(object->isgone());
}

template<class O>
void MOTOR<O>::addforce (const VECTOR &frc,double timestep)
{
	if (*fuel>0)
	{
		VECTOR dv=(frc*(timestep/2))/object->getmass();

		*fuel-=(frc.getdotproduct(dv)*timestep);

		object->addforce(frc);
	}

	if (*fuel<0)
	{
		*fuel=0;
	}
}

template<class O>
void MOTOR<O>::addforce (const VECTOR &frc,const VECTOR &pos,double timestep)
{
	addforce(frc,timestep);
	addtorque((pos-object->getposition()).getcrossproduct(frc)*(180/math_pi),timestep);
}

template<class O>
void MOTOR<O>::addtorque(double trq,double timestep)
{
	if (*fuel>0)
	{
		double trqrad=trq*(math_pi/180);

		double dv=(trqrad*(timestep/2))/object->getangular_mass();

		*fuel-=(trqrad*dv*timestep);

		object->addtorque(trq);
	}

	if (*fuel<0)
	{
		*fuel=0;
	}
}
