//Code by Rogier Walraven. Do not distribute.

struct LINE
{
	VECTOR<3> direction;
	VECTOR<3> moment;

	LINE();
	LINE(const VECTOR<3> &p1,const VECTOR<3> &p2);

	bool isdegenerate() const;

	VECTOR<3> getposition() const;
	VECTOR<3> getdirection() const;

	LINE operator*(const MATRIX<4> &m) const;
	LINE& operator*=(const MATRIX<4> &m);

	double getpermuteddp(const LINE &l) const;

    VECTOR<3> getdistance(const VECTOR<3> &p) const;
};

inline LINE::LINE() {}

inline LINE::LINE(const VECTOR<3> &p1,const VECTOR<3> &p2) :
	direction(p2-p1),
	moment(p1.getcrossproduct(p2)) {}

inline bool LINE::isdegenerate() const
{
	return(direction.iszero());
}

inline VECTOR<3> LINE::getposition() const
{
	return(direction.getcrossproduct(moment)/direction.getdotproduct(direction));
}

inline VECTOR<3> LINE::getdirection() const
{
	return(direction.getnormal());
}

inline LINE LINE::operator*(const MATRIX<4> &m) const
{
	VECTOR<3> tpos=(m * getposition().gethomogeneous()).getcartesian();
	VECTOR<3> tdir=m.getminor(3,3) * getdirection();

	return(LINE(tpos,tpos+tdir));
}

inline LINE& LINE::operator*=(const MATRIX<4> &m)
{
	*this=*this*m;

	return(*this);
}

inline double LINE::getpermuteddp(const LINE &l) const
{
	return(direction.getdotproduct(l.moment) + moment.getdotproduct(l.direction));
}

inline VECTOR<3> LINE::getdistance(const VECTOR<3> &p) const
{
    VECTOR<3> tpos=getposition()-p;

    return(LINE(tpos,tpos+getdirection()).getposition());
}
