//Code by Rogier Walraven. Do not distribute.

struct SECTOR : public BSPTREE
{
	const DYNAMIC_MODEL * const * dynamic_models;
	const int *numdynamic_models;

	VECTOR<3> Cmin,Cmax;

	bool isempty;

	int numlodlevels;
	TRIANGLE **triangles;
	int *numtriangles;

	VECTOR<3> center;

	double lod_distance;
	double lod_mul;

	SECTOR(int x,int y,int z,int d,double inner_radius,double outer_radius,
           int numlodlevels,const TRIANGLE * const * triangles,const int *numtriangles,
           double lodd,double lodm,
           const DYNAMIC_MODEL * const * dynamic_models,const int *numdynamic_models);
	~SECTOR();

	SECTOR& operator*=(const MATRIX<4> &m);

	void sort(const VECTOR<3> &p,
              const LINE &l1,const LINE &l2,const LINE &l3,const LINE &l4,
              ARRAY<TRIANGLE> &triangles,int &numtriangles) const;

  private:

	void doclip(const PLANE &plane,
                ARRAY<TRIANGLE> &inarray,int innum,
                ARRAY<TRIANGLE> &outarray,int &outnum) const;

	bool cube_sphere_intersect(const VECTOR<3> &pos,double radius) const;
};
