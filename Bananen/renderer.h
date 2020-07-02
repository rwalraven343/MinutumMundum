//Code by Rogier Walraven. Do not distribute.

struct RENDERER
{
	RENDERER(int depth,const VECTOR<3> &origin,double scale,int numstars,
             double pleft,double pright,
             double pbottom,double ptop,
             double pnear,double pfar);
	~RENDERER();

	int getdepth() const;
	int getindexsize(int depth) const;

    bool getnode(int x,int y,int z,int depth,MATRIX<4> &m,BSPTREE **node) const;
    bool setnode(int x,int y,int z,int depth,MATRIX<4> &m,BSPTREE *node);

	void setview(const MATRIX<4> &v);
	void setambient(const VECTOR<4> &a);

	void draw(double x,double y,double w,double h,bool wireframe=false) const;

  private:

	OCTTREE octtree;

	MATRIX<4> *view;
	MATRIX<4> *view_inverse;
	MATRIX<4> *projection;

	LINE l1,l2,l3,l4;

	PLANE zclip;
	PLANE left,right;
	PLANE front,back;
	PLANE top,bottom;

	mutable ARRAY<TRIANGLE> worldspace_triangles;
	mutable ARRAY<TRIANGLE> viewspace_triangles;
	mutable ARRAY<TRIANGLE> clipspace_triangles;
	mutable ARRAY<TRIANGLE> clipped_left;
	mutable ARRAY<TRIANGLE> clipped_right;
	mutable ARRAY<TRIANGLE> clipped_front;
	mutable ARRAY<TRIANGLE> clipped_back;
	mutable ARRAY<TRIANGLE> clipped_top;
	mutable ARRAY<TRIANGLE> clipped_bottom;
	mutable ARRAY<TRIANGLE> visible_triangles;
	mutable ARRAY<double>   zdotn;

	VECTOR<4> ambient;

	VECTOR<3> *stars;
	int numstars;

	void draw_stars(double x,double y,double w,double h) const;

	void doclip(const PLANE &plane,
                ARRAY<TRIANGLE> &inarray,int innum,
                ARRAY<TRIANGLE> &outarray,int &outnum) const;
};
