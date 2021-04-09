//Code by Rogier Walraven. Do not distribute.

struct SOLID_OCTAGON_PROPERTIES
{
	double apothem;
	double mass;
	double skonst;
	double dkonst;
	double sfriction;
	double dfriction;
	double absorption;
	double refraction;
};

struct SOLID_OCTAGON
{
	SOLID_OCTAGON derive(double time) const;
	SOLID_OCTAGON operator+(const SOLID_OCTAGON &delta) const;
	SOLID_OCTAGON operator*(double timestep) const;

	SOLID_OCTAGON();

	void init_reset(const SOLID_OCTAGON_PROPERTIES &props);

	bool wasinit() const;
	bool isgone() const;

	void init_reset_kinematic_motions(int num,const KINEMATIC_MOTION *motions,double loop);

	double getapothem() const;

	double getbounding_radius() const;

	void setposition        (const VECTOR &pos);
	void setorientation     (double angle);
	void setlinear_velocity (const VECTOR &vel);
	void setangular_velocity(double angular_vel);

	VECTOR getposition() const;
	double getorientation() const;
	VECTOR getlinear_velocity() const;
	double getangular_velocity() const;

	VECTOR getlinear_momentum() const;
	double getangular_momentum() const;
	double getangular_momentum_origin() const;

	double getmass() const;
	double getangular_mass() const;

	void clearforces();
	void addforce (const VECTOR &frc);
	void addforce (const VECTOR &frc,const VECTOR &pos);
	void addtorque(double trq);

	void addheat(double ht);

	void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc);

	void change_origin(const VECTOR &new_origin);

	double gettotal_kinetic_energy() const;
	double getlinear_kinetic_energy() const;
	double getangular_kinetic_energy() const;
	double getinternal_kinetic_energy() const;

	double gettemperature() const;

	void getgeometry(TRIANGLE &triangle1,
                     TRIANGLE &triangle2,
                     TRIANGLE &triangle3,
                     TRIANGLE &triangle4,
                     QUAD &quad1,
                     QUAD &quad2,
                     QUAD &quad3,
                     QUAD &quad4,
                     QUAD &quad5) const;

	void fillclist(COLLISIONLIST &clist,const VECTOR &other_pos,double other_radius) const;

	void setclist(const COLLISIONLIST &clist);

	void integrate(double timestep);

	void draw(VIDEO::TEXTURE texture,double alpha) const;

  private:

	bool initialized;
	POINTMASS_SOLID pointmasses[16];
	const SOLID_OCTAGON_PROPERTIES *properties;
	double restl,restld1,restld2;
	double density;
	double cached_timestep;
	const COLLISIONLIST *clist;
	double heat_to_add;
	int num_kinematic_motions;
	const KINEMATIC_MOTION *kinematic_motions;
	double kinematic_motion_loop;
	double kinematic_motion_time;

	void addheat_pointmasses();

	void filltriangle(TRIANGLE &triangle,const VECTOR &pos,const VECTOR &vel,double angvelrad) const;
	void fillquad(QUAD &quad,const VECTOR &pos,const VECTOR &vel,double angvelrad) const;

	void draw_polygon(const POINTMASS_SOLID &a,const POINTMASS_SOLID &b,const POINTMASS_SOLID &c,
                      double tsx,double tex,double tsy,double tey,double alpha) const;
};
