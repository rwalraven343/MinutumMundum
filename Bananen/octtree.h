//Code by Rogier Walraven. Do not distribute.

struct OCTTREE : public BSPTREE
{
	OCTTREE(int depth,const VECTOR<3> &origin,double scale);

	int getdepth() const;
	int getindexsize(int depth) const;

    bool getnode(int x,int y,int z,int depth,MATRIX<4> &m,BSPTREE **node) const;
    bool setnode(int x,int y,int z,int depth,MATRIX<4> &m,BSPTREE *node);

  private:

	int depth;
	VECTOR<3> origin;
	double scale;
};
