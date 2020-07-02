//Code by Rogier Walraven. Do not distribute.

struct PLANE
{
	VECTOR<3> normal;
	double distance;

	PLANE();
	PLANE(const VECTOR<3> &p1,const VECTOR<3> &p2,const VECTOR<3> &p3);
	PLANE(const VECTOR<3> &p,const LINE &l);

	bool isdegenerate() const;

	PLANE operator*(const MATRIX<4> &m) const;
	PLANE& operator*=(const MATRIX<4> &m);

	bool meet(const LINE &l,VECTOR<3> &p) const;
    bool meet(const PLANE &p,LINE &l) const;

	bool meet_coplanar(const LINE &l1,const LINE &l2,VECTOR<3> &p) const;

    VECTOR<3> getdistance(const VECTOR<3> &p) const;

	bool halfspace(const VECTOR<3> &p) const;
};

inline PLANE::PLANE() : distance(0) {}

inline PLANE::PLANE(const VECTOR<3> &p1,const VECTOR<3> &p2,const VECTOR<3> &p3) :
	normal(((p2-p1).getcrossproduct(p3-p1)).getnormal()),
    distance(p1.getdotproduct(normal)) {}

inline PLANE::PLANE(const VECTOR<3> &p,const LINE &l)
{
	VECTOR<3> lpos=l.getposition();

	*this=PLANE(p,lpos,lpos+l.getdirection());
}

inline bool PLANE::isdegenerate() const
{
	return(!normal.isnormal());
}

inline PLANE PLANE::operator*(const MATRIX<4> &m) const
{
	PLANE tplane;

	tplane.normal=m.getminor(3,3).getinverse().gettranspose() * normal;
	tplane.distance=distance + tplane.normal.getdotproduct(m.getcolumn(3).getcartesian());
	
	double l=tplane.normal.getlength();

	tplane.normal/=l;
	tplane.distance/=l;

	return(tplane);
}

inline PLANE& PLANE::operator*=(const MATRIX<4> &m)
{
	*this=*this*m;

	return(*this);
}

inline bool PLANE::meet(const LINE &l,VECTOR<3> &p) const
{
	double div=l.direction.getdotproduct(normal);

	if (!equalszero(div))
	{
		p=(normal.getcrossproduct(l.moment) + l.direction*distance)/div;

		return(true);
	}

	return(false);
}

inline bool PLANE::meet(const PLANE &p,LINE &l) const
{
    l.direction=normal.getcrossproduct(p.normal);

    if (!l.isdegenerate())
    {
        l.moment=normal*p.distance - p.normal*distance;

        return(true);
    }

    return(false);
}

inline bool PLANE::meet_coplanar(const LINE &l1,const LINE &l2,VECTOR<3> &p) const
{
	if (isdegenerate() || l1.isdegenerate() || l2.isdegenerate())
	{
		return(false);
	}

	VECTOR<3> origin;

	VECTOR<3> l1dist=l1.getdistance(origin);
	VECTOR<3> l1dir=l1.getdirection();

	VECTOR<3> l2dist=l2.getdistance(origin);
	VECTOR<3> l2dir=l2.getdirection();

	VECTOR<3> xaxis=l1dir;
	VECTOR<3> yaxis=l1dir.getcrossproduct(normal);

	VECTOR<3> l1distl2dist=l2dist - l1dist;

	::VECTOR l2start(l1distl2dist.getdotproduct(xaxis),l1distl2dist.getdotproduct(yaxis));
	::VECTOR l2end(l2start.x + l2dir.getdotproduct(xaxis),l2start.y + l2dir.getdotproduct(yaxis));

	::LINE l1f(::VECTOR(0,0),::VECTOR(1,0));
	::LINE l2f(l2start,l2end);

	::VECTOR pf;

	if (l1f.getintersection_inf_inf(pf,l2f))
	{
		p=l1dist + xaxis*pf.x + yaxis*pf.y;

		return(true);
	}

	return(false);
}

inline VECTOR<3> PLANE::getdistance(const VECTOR<3> &p) const
{
	return(normal * (distance + normal.getdotproduct(-p)));
}

inline bool PLANE::halfspace(const VECTOR<3> &p) const
{
	return(normal.getdotproduct(getdistance(p))<=0);
}
