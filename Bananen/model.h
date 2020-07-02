//Code by Rogier Walraven. Do not distribute.

struct MODEL : public BSPTREE, public DYNAMIC_MODEL
{
	MODEL(const double *vcoords,int numvcoords,const int *vindices,int numvindices);
	~MODEL();

	MODEL& operator*=(const MATRIX<4> &m);

	void sort(const VECTOR<3> &p,
              const LINE &l1,const LINE &l2,const LINE &l3,const LINE &l4,
              ARRAY<TRIANGLE> &triangles,int &numtriangles) const;

	void setnodraw(bool nd);

	void setworld(const MATRIX<4> &w);
	MATRIX<4> getworld() const;

	VECTOR<3> getposition() const;
	double getradius() const;
	const TRIANGLE *gettriangles() const;
	int getnumtriangles() const;

	void setpath(const PATH &path,double timescale,const VECTOR<3> &worigin,double scale);
	void clearpath();

	void setpathtranslation(const VECTOR<3> &pt);
	void setpathrotation(const QUATERNION &pr);

	bool haspath() const;
	void integrate(double timestep);

  private:

	TRIANGLE *triangles;
	int numtriangles;

	TRIANGLE *striangles;
	TRIANGLE *dtriangles;

	bool nodraw;

	MATRIX<4> world;
	double radius;

	const PATH *path;
	double timescale;
	VECTOR<3> worigin;
	double scale;
	double time;

	VECTOR<3> pathtranslation;
	QUATERNION pathrotation;
};
