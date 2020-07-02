//Code by Rogier Walraven. Do not distribute.

extern const double collision_bounding_radius_mul;

struct COLLISIONLIST
{
	private:
	struct SHAPE;
	public:

	int numshapes;

	SHAPE *shapes;

	double friction;
	double refraction;

	COLLISIONLIST();
	~COLLISIONLIST();

	COLLISIONLIST& operator=(const COLLISIONLIST &other);

	void change_origin(const VECTOR &new_origin);

	bool getintersection_seg(VECTOR &i,VECTOR &n,double &absorp,void **obj,const LINE &l,double time) const;

	bool iswithin                                                             (double &refract,const VECTOR &p,double time) const;
	bool iswithin_solid(VECTOR &snorm,VECTOR &svel,double &sdens,double &ssfric,double &sdfric,const VECTOR &p,double time) const;
	bool iswithin_fluid                        (VECTOR &fvel,VECTOR &fpress_diff,double &fvisc,const VECTOR &p,double time) const;

  private:

	struct SHAPE
	{
		VECTOR position;
		double radius;

		int numtriangles;
		int numquads;
		int numcircles;

		TRIANGLE *triangles;
		QUAD     *quads;
		CIRCLE   *circles;

		void *object;

      private:

		SHAPE();
		~SHAPE();

		SHAPE& operator=(const SHAPE &other);

		void change_origin(const VECTOR &new_origin);

		bool getintersection_seg(VECTOR &i,VECTOR &n,double &absorp,const LINE &l,double time) const;

		bool iswithin                                                (double &refract,double &dens,const VECTOR &p,double time) const;
		bool iswithin_solid(VECTOR &snorm,VECTOR &svel,double &sdens,double &ssfric,double &sdfric,const VECTOR &p,double time) const;
		bool iswithin_fluid          (VECTOR &fvel,double &fdens,VECTOR &fpress_diff,double &fvisc,const VECTOR &p,double time) const;

		struct ISECT
		{
			VECTOR pos;
			VECTOR norm;
			double absorp;
		};

		bool   *isect_exists;
		ISECT  *isect;
		ISECT  *existing_isect;
		double *isect_distance;

		friend struct COLLISIONLIST;

		SHAPE(const SHAPE &other) {}
	};

	struct ISECT
	{
		VECTOR pos;
		VECTOR norm;
		double absorp;
		void*  obj;
	};

	bool   *isect_exists;
	ISECT  *isect;
	ISECT  *existing_isect;
	double *isect_distance;

	COLLISIONLIST(const COLLISIONLIST &other) {}
};
