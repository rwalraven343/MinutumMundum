//Code by Rogier Walraven. Do not distribute.

struct BSPTREE
{
	PLANE plane;

	BSPTREE *node1;
	BSPTREE *node2;

	BSPTREE();
	BSPTREE(int depth);
	virtual ~BSPTREE();

	bool getnode(const VECTOR<3> &xyz,int depth,BSPTREE **node) const;
    bool setnode(const VECTOR<3> &xyz,int depth,BSPTREE *node);

	virtual void sort(const VECTOR<3> &p,
                      const LINE &l1,const LINE &l2,const LINE &l3,const LINE &l4,
                      ARRAY<TRIANGLE> &triangles,int &numtriangles) const;

  private:

	BSPTREE(const BSPTREE &other) {}
	BSPTREE& operator=(const BSPTREE &other) {}
};

inline BSPTREE::BSPTREE() : node1(0),node2(0) {}

inline BSPTREE::BSPTREE(int depth) : node1(0),node2(0)
{
	if (depth>0)
	{
		node1=new BSPTREE(depth-1);
		node2=new BSPTREE(depth-1);
	}
}

inline BSPTREE::~BSPTREE()
{
	delete node1;
	delete node2;
}

inline bool BSPTREE::getnode(const VECTOR<3> &xyz,int depth,BSPTREE **node) const
{
	if (!plane.isdegenerate())
	{
		if (plane.halfspace(xyz))
		{
            if (depth>0)
            {
                if (node1)
                {
                    return(node1->getnode(xyz,depth-1,node));
                }
            }
            else
            {
				*node=node1;

                return(true);
            }
		}
		else
		{
            if (depth>0)
            {
                if (node2)
                {
                    return(node2->getnode(xyz,depth-1,node));
                }
            }
            else
            {
				*node=node2;

                return(true);
            }
		}
	}

	return(false);
}

inline bool BSPTREE::setnode(const VECTOR<3> &xyz,int depth,BSPTREE *node)
{
	if (!plane.isdegenerate())
	{
		if (plane.halfspace(xyz))
		{
            if (depth>0)
            {
                if (node1)
                {
                    return(node1->setnode(xyz,depth-1,node));
                }
            }
            else
            {
                if (node1)
                {
                    delete node1;
                }

                node1=node;

                return(true);
            }
		}
		else
		{
            if (depth>0)
            {
                if (node2)
                {
                    return(node2->setnode(xyz,depth-1,node));
                }
            }
            else
            {
                if (node2)
                {
                    delete node2;
                }

                node2=node;

                return(true);
            }
		}
	}

	return(false);
}

inline void BSPTREE::sort(const VECTOR<3> &p,
                          const LINE &l1,const LINE &l2,const LINE &l3,const LINE &l4,
                          ARRAY<TRIANGLE> &triangles,int &numtriangles) const
{
	if (!plane.isdegenerate())
	{
		if (plane.halfspace(p))
		{
			if (node1)
			{
				node1->sort(p,l1,l2,l3,l4,triangles,numtriangles);
			}

			if (node2)
			{
                if (l1.getdirection().getdotproduct(plane.normal) < 0 ||
					l2.getdirection().getdotproduct(plane.normal) < 0 ||
					l3.getdirection().getdotproduct(plane.normal) < 0 ||
					l4.getdirection().getdotproduct(plane.normal) < 0)
                {
					node2->sort(p,l1,l2,l3,l4,triangles,numtriangles);
                }
			}
		}
		else
		{
			if (node2)
			{
				node2->sort(p,l1,l2,l3,l4,triangles,numtriangles);
			}

			if (node1)
			{
                if (l1.getdirection().getdotproduct(plane.normal) > 0 ||
					l2.getdirection().getdotproduct(plane.normal) > 0 ||
					l3.getdirection().getdotproduct(plane.normal) > 0 ||
					l4.getdirection().getdotproduct(plane.normal) > 0)
                {
					node1->sort(p,l1,l2,l3,l4,triangles,numtriangles);
                }
            }
		}
	}
}
