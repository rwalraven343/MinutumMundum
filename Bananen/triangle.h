//Code by Rogier Walraven. Do not distribute.

struct TRIANGLE
{
	VECTOR<3> vertex[3];

	bool hascolour;
	VECTOR<4> colour;

	TRIANGLE();
	TRIANGLE(const VECTOR<3> &p1,const VECTOR<3> &p2,const VECTOR<3> &p3);

	bool isdegenerate() const;

	TRIANGLE operator*(const MATRIX<4> &m) const;
	TRIANGLE& operator*=(const MATRIX<4> &m);

	bool meet(const LINE &l) const;

	int clip(const PLANE &p,TRIANGLE &t1,TRIANGLE &t2) const;
};

inline TRIANGLE::TRIANGLE() : hascolour(false) {}

inline TRIANGLE::TRIANGLE(const VECTOR<3> &p1,const VECTOR<3> &p2,const VECTOR<3> &p3) : hascolour(false)
{
	vertex[0]=p1;
	vertex[1]=p2;
	vertex[2]=p3;
}

inline bool TRIANGLE::isdegenerate() const
{
	return(PLANE(vertex[0],vertex[1],vertex[2]).isdegenerate());
}

inline TRIANGLE TRIANGLE::operator*(const MATRIX<4> &m) const
{
	TRIANGLE ttri;

	ttri.vertex[0]=(m * vertex[0].gethomogeneous()).getcartesian();
	ttri.vertex[1]=(m * vertex[1].gethomogeneous()).getcartesian();
	ttri.vertex[2]=(m * vertex[2].gethomogeneous()).getcartesian();

	ttri.hascolour=hascolour;
	ttri.colour=colour;

	return(ttri);
}

inline TRIANGLE& TRIANGLE::operator*=(const MATRIX<4> &m)
{
	vertex[0]=(m * vertex[0].gethomogeneous()).getcartesian();
	vertex[1]=(m * vertex[1].gethomogeneous()).getcartesian();
	vertex[2]=(m * vertex[2].gethomogeneous()).getcartesian();

	return(*this);
}

inline bool TRIANGLE::meet(const LINE &l) const
{
	double s[3];
	double w;

	LINE line[3];

	line[0]=LINE(vertex[0],vertex[1]);
	line[1]=LINE(vertex[1],vertex[2]);
	line[2]=LINE(vertex[2],vertex[0]);

	int num=0;

	for (int i=0;i<3;i++)
	{
		w=line[i].getpermuteddp(l);

		if (!equalszero(w))
		{
			s[num]=w;

			num++;
		}
	}

	switch (num)
	{
	case 0:
		return(false);
	break;
	case 1:
		return(true);
	break;
	case 2:
		if ((s[0]<0 && s[1]<0) || (s[0]>0 && s[1]>0))
		{
			return(true);
		}

		return(false);
	break;
	case 3:
		if ((s[0]<0 && s[1]<0 && s[2]<0) || (s[0]>0 && s[1]>0 && s[2]>0))
		{
			return(true);
		}

		return(false);
	break;
	}

	return(false);
}

inline int TRIANGLE::clip(const PLANE &p,TRIANGLE &t1,TRIANGLE &t2) const
{
	if (isdegenerate())
	{
		return(1);
	}

	if (p.isdegenerate())
	{
		return(0);
	}

	bool halfspace_v0=p.halfspace(vertex[0]);
	bool halfspace_v1=p.halfspace(vertex[1]);
	bool halfspace_v2=p.halfspace(vertex[2]);

	if (halfspace_v0 && halfspace_v1 && halfspace_v2)
	{
		return(0);
	}

	if (!halfspace_v0 && !halfspace_v1 && !halfspace_v2)
	{
		return(1);
	}

	LINE line0(vertex[0],vertex[1]);
	LINE line1(vertex[1],vertex[2]);
	LINE line2(vertex[2],vertex[0]);

	PLANE trplane(vertex[0],vertex[1],vertex[2]);

	LINE iline;

	if (!p.meet(trplane,iline))
	{
		return(1);
	}

	VECTOR<3> i0,i1,i2;

	bool i0_exists=false;
	bool i1_exists=false;
	bool i2_exists=false;

	if (trplane.meet_coplanar(line0,iline,i0))
	{
		i0_exists=true;
	}
	if (trplane.meet_coplanar(line1,iline,i1))
	{
		i1_exists=true;
	}
	if (trplane.meet_coplanar(line2,iline,i2))
	{
		i2_exists=true;
	}

	if (!halfspace_v0 && halfspace_v1 && halfspace_v2)
	{
		if (!i0_exists || !i2_exists)
		{
			return(1);
		}

		t1.vertex[0]=vertex[1];
		t1.vertex[1]=i2;
		t1.vertex[2]=i0;

		t1.hascolour=hascolour;
		t1.colour=colour;

		t2.vertex[0]=vertex[1];
		t2.vertex[1]=vertex[2];
		t2.vertex[2]=i2;

		t2.hascolour=hascolour;
		t2.colour=colour;

		return(2);
	}
	if (halfspace_v0 && !halfspace_v1 && halfspace_v2)
	{
		if (!i0_exists || !i1_exists)
		{
			return(1);
		}

		t1.vertex[0]=vertex[2];
		t1.vertex[1]=i0;
		t1.vertex[2]=i1;

		t1.hascolour=hascolour;
		t1.colour=colour;

		t2.vertex[0]=vertex[2];
		t2.vertex[1]=vertex[0];
		t2.vertex[2]=i0;

		t2.hascolour=hascolour;
		t2.colour=colour;

		return(2);
	}
	if (halfspace_v0 && halfspace_v1 && !halfspace_v2)
	{
		if (!i1_exists || !i2_exists)
		{
			return(1);
		}

		t1.vertex[0]=vertex[0];
		t1.vertex[1]=i1;
		t1.vertex[2]=i2;

		t1.hascolour=hascolour;
		t1.colour=colour;

		t2.vertex[0]=vertex[0];
		t2.vertex[1]=vertex[1];
		t2.vertex[2]=i1;

		t2.hascolour=hascolour;
		t2.colour=colour;

		return(2);
	}

	if (halfspace_v0 && !halfspace_v1 && !halfspace_v2)
	{
		if (!i0_exists || !i2_exists)
		{
			return(1);
		}

		t1.vertex[0]=vertex[0];
		t1.vertex[1]=i0;
		t1.vertex[2]=i2;

		t1.hascolour=hascolour;
		t1.colour=colour;

		return(3);
	}
	if (!halfspace_v0 && halfspace_v1 && !halfspace_v2)
	{
		if (!i0_exists || !i1_exists)
		{
			return(1);
		}

		t1.vertex[0]=vertex[1];
		t1.vertex[1]=i1;
		t1.vertex[2]=i0;

		t1.hascolour=hascolour;
		t1.colour=colour;

		return(3);
	}
	if (!halfspace_v0 && !halfspace_v1 && halfspace_v2)
	{
		if (!i1_exists || !i2_exists)
		{
			return(1);
		}

		t1.vertex[0]=vertex[2];
		t1.vertex[1]=i2;
		t1.vertex[2]=i1;

		t1.hascolour=hascolour;
		t1.colour=colour;

		return(3);
	}

	return(1);
}
