//Code by Rogier Walraven. Do not distribute.

template<class S>
S integrate_RK4(const S &state,double timestep)
{
    S k1=state.derive(0);
    S k2=(state+(k1*(timestep/2))).derive(timestep/2);
    S k3=(state+(k2*(timestep/2))).derive(timestep/2);
    S k4=(state+(k3*timestep)).derive(timestep);

    return(state+((k1+k2+k2+k3+k3+k4)*(timestep/6)));
}

namespace DVDT
{
	VECTOR force(const VECTOR &frc,double mass);

	void spring(VECTOR &dvdt_a,VECTOR &dvdt_b,
                const VECTOR &posa,const VECTOR &posb,const VECTOR &vela,const VECTOR &velb,double massa,double massb,
                double restl,double skonst,double dkonst);

	VECTOR vanderwaals_liquid(const VECTOR &posa,const VECTOR &posb,const VECTOR &vela,const VECTOR &velb,double massa,
                              double mindist,double maxdist,double rkonst,double akonst,double dkonst);

	VECTOR vanderwaals_gas(const VECTOR &posa,const VECTOR &posb,double chargea,double chargeb,double massa,double maxdist,double konst);

	VECTOR friction(const VECTOR &vel,double mass,double konst);

	VECTOR collision(const VECTOR &pos,const COLLISIONLIST &clist,
                     const VECTOR &vel,const VECTOR &acc,
                     double mass,double density,
                     bool hasfriction,double sfriction,double dfriction,
                     double time,double timestep);
}

inline VECTOR DVDT::force(const VECTOR &frc,double mass)
{
    return(frc/mass);
}

inline void DVDT::spring(VECTOR &dvdt_a,VECTOR &dvdt_b,
                         const VECTOR &posa,const VECTOR &posb,const VECTOR &vela,const VECTOR &velb,double massa,double massb,
                         double restl,double skonst,double dkonst)
{
	VECTOR ab =posb-posa;
	VECTOR abn=ab.getnormal();

	double u=(ab.getlength()-restl)*skonst;

	VECTOR avab=vela.getprojectedon(ab);
	VECTOR bvab=velb.getprojectedon(ab);

	double d=(avab-bvab).getlength()*dkonst;

	dvdt_a=(  abn *(u/massa))+((-avab.getnormal())*(d/massa));
	dvdt_b=((-abn)*(u/massb))+((-bvab.getnormal())*(d/massb));
}

inline VECTOR DVDT::vanderwaals_liquid(const VECTOR &posa,const VECTOR &posb,const VECTOR &vela,const VECTOR &velb,double massa,
                                       double mindist,double maxdist,double rkonst,double akonst,double dkonst)
{
	VECTOR ab=posb-posa;

	double r=ab.getlength();

	if (r<mindist)
	{
		VECTOR dvdt=(-ab.getnormal())*((compact_gaussian(r+mindist,mindist*2,false)*rkonst)/massa);

		VECTOR avab=vela.getprojectedon(ab);
		VECTOR bvab=velb.getprojectedon(ab);

		double d=(avab-bvab).getlength()*dkonst;

		dvdt+=(-avab.getnormal())*(d/massa);

		return(dvdt);
	}

	if (r<(mindist+((maxdist-mindist)*2)))
	{
		VECTOR dvdt=( ab.getnormal())*((compact_gaussian(r-mindist,(maxdist-mindist)*2,false)*akonst)/massa);

		VECTOR avab=vela.getprojectedon(ab);
		VECTOR bvab=velb.getprojectedon(ab);

		double d=(avab-bvab).getlength()*dkonst;

		dvdt+=(-avab.getnormal())*(d/massa);

		return(dvdt);
	}

	return(VECTOR(0,0));
}

inline VECTOR DVDT::vanderwaals_gas(const VECTOR &posa,const VECTOR &posb,double chargea,double chargeb,double massa,double maxdist,double konst)
{
	VECTOR ab=posb-posa;

	double r=ab.getlength();

	if (r<maxdist)
	{
		return((-ab.getnormal())*((compact_gaussian(r+maxdist,maxdist*2,false)*chargea*chargeb*konst)/massa));
	}

	return(VECTOR(0,0));
}

inline VECTOR DVDT::friction(const VECTOR &vel,double mass,double konst)
{
	return((-vel.getnormal())*((konst/mass)*vel.getlength()));
}

inline VECTOR DVDT::collision(const VECTOR &pos,const COLLISIONLIST &clist,
                              const VECTOR &vel,const VECTOR &acc,
                              double mass,double density,
                              bool hasfriction,double sfriction,double dfriction,
                              double time,double timestep)
{
	VECTOR dvdt;

	VECTOR fvel,fpress_diff;
	double fvisc;

	if (clist.iswithin_fluid(fvel,fpress_diff,fvisc,pos,time))
	{
		dvdt+=fpress_diff/density;

		dvdt+=(vel-fvel)*(fvisc/density);
	}

	VECTOR tacc=acc+dvdt;

	VECTOR snorm,svel;
	double sdens,ssfric,sdfric;

	if (clist.iswithin_solid(snorm,svel,sdens,ssfric,sdfric,pos,time))
	{
		VECTOR rvel=vel-svel;
		VECTOR mvel=vel-((rvel*((density-sdens)/(density+sdens)))+svel);

		if (mvel.getdotproduct(snorm)<0)
		{
			VECTOR coll_impulse=(-mvel.getprojectedon(snorm))             *compact_gaussian(time,timestep);
			VECTOR fric_impulse=(-mvel.getprojectedon(snorm.getperpend()))*compact_gaussian(time,timestep);

			dvdt+=coll_impulse;

			if (hasfriction)
			{
				ssfric=(ssfric+sfriction)/2;
				sdfric=(sdfric+dfriction)/2;
			}

			if (fric_impulse.getlength()<=(coll_impulse.getlength()*ssfric))
			{
				dvdt+=fric_impulse;
			}
			else
			{
				VECTOR gvel=rvel.getprojectedon(snorm.getperpend());

				dvdt+=(-gvel.getnormal())*((sdfric/mass)*gvel.getlength());
			}
		}

		if (tacc.getdotproduct(snorm)<0)
		{
			dvdt+=(-tacc.getprojectedon(snorm));
		}
	}

	return(dvdt);
}
