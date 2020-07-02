//Code by Rogier Walraven. Do not distribute.

struct CURVE
{
	VECTOR<3> P0,P1,P2,P3;
	double alpha;

	CURVE();

	void operator()(double t,VECTOR<3> &C,VECTOR<3> &Cs) const;

  private:

	double getT(double ti,const VECTOR<3> &Pi,const VECTOR<3> &Pj) const;
};

inline CURVE::CURVE() : alpha(0) {}

inline void CURVE::operator()(double t,VECTOR<3> &C,VECTOR<3> &Cs) const
{
	double t0=0;
	double t1=getT(t0,P0,P1);
	double t2=getT(t1,P1,P2);
	double t3=getT(t2,P2,P3);

	t=t1 + (t2-t1)*t;

	VECTOR<3> A1=P0*((t1-t)/(t1-t0)) + P1*((t-t0)/(t1-t0));
	VECTOR<3> A2=P1*((t2-t)/(t2-t1)) + P2*((t-t1)/(t2-t1));
	VECTOR<3> A3=P2*((t3-t)/(t3-t2)) + P3*((t-t2)/(t3-t2));

	VECTOR<3> B1=A1*((t2-t)/(t2-t0)) + A2*((t-t0)/(t2-t0));
	VECTOR<3> B2=A2*((t3-t)/(t3-t1)) + A3*((t-t1)/(t3-t1));

	C=B1*((t2-t)/(t2-t1)) + B2*((t-t1)/(t2-t1));

	VECTOR<3> As1=(P1-P0)*(1/(t1-t0));
	VECTOR<3> As2=(P2-P1)*(1/(t2-t1));
	VECTOR<3> As3=(P3-P2)*(1/(t3-t2));

	VECTOR<3> Bs1=(A2-A1)*(1/(t2-t0)) + As1*((t2-t)/(t2-t0)) + As2*((t-t0)/(t2-t0));
	VECTOR<3> Bs2=(A3-A2)*(1/(t3-t1)) + As2*((t3-t)/(t3-t1)) + As3*((t-t1)/(t3-t1));

	Cs=(B2-B1)*(1/(t2-t1)) + Bs1*((t2-t)/(t2-t1)) + Bs2*((t-t1)/(t2-t1));
}

inline double CURVE::getT(double ti,const VECTOR<3> &Pi,const VECTOR<3> &Pj) const
{
	return
	(
		power
		(
			square(Pj.cmp[0]-Pi.cmp[0]) +
			square(Pj.cmp[1]-Pi.cmp[1]) +
			square(Pj.cmp[2]-Pi.cmp[2]) ,
			alpha/2
		) + ti
	);
}
