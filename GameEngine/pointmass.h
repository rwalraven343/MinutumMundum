//Code by Rogier Walraven. Do not distribute.

struct POINTMASS_SOLID
{
	VECTOR position;
	VECTOR velocity;

	double mass;

	VECTOR force;

	POINTMASS_SOLID operator+(const POINTMASS_SOLID &delta) const
	{
		POINTMASS_SOLID temp;

		temp.position=position+delta.position;
		temp.velocity=velocity+delta.velocity;

		temp.mass =mass;
		temp.force=force;

		return(temp);
	}

	POINTMASS_SOLID operator*(double timestep) const
	{
		POINTMASS_SOLID temp;

		temp.position=position*timestep;
		temp.velocity=velocity*timestep;

		return(temp);
	}
};

struct POINTMASS_LIQUID
{
	VECTOR position;
	VECTOR velocity;

	double mass;

	VECTOR force;

	double sph_radius;

	POINTMASS_LIQUID operator+(const POINTMASS_LIQUID &delta) const
	{
		POINTMASS_LIQUID temp;

		temp.position=position+delta.position;
		temp.velocity=velocity+delta.velocity;

		temp.mass      =mass;
		temp.force     =force;
		temp.sph_radius=sph_radius;

		return(temp);
	}

	POINTMASS_LIQUID operator*(double timestep) const
	{
		POINTMASS_LIQUID temp;

		temp.position=position*timestep;
		temp.velocity=velocity*timestep;

		return(temp);
	}
};

struct POINTMASS_GAS
{
	VECTOR position;
	VECTOR velocity;

	double mass;
	double charge;

	VECTOR force;

	double sph_radius;

	double lifetime;
	double lifeleft;

	POINTMASS_GAS operator+(const POINTMASS_GAS &delta) const
	{
		POINTMASS_GAS temp;

		temp.position=position+delta.position;
		temp.velocity=velocity+delta.velocity;

		temp.mass      =mass;
		temp.charge    =charge;
		temp.force     =force;
		temp.sph_radius=sph_radius;
		temp.lifetime  =lifetime;
		temp.lifeleft  =lifeleft;

		return(temp);
	}

	POINTMASS_GAS operator*(double timestep) const
	{
		POINTMASS_GAS temp;

		temp.position=position*timestep;
		temp.velocity=velocity*timestep;

		return(temp);
	}
};
