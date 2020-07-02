//Code by Rogier Walraven. Do not distribute.

namespace RANDOM
{
	typedef SYSTEM::S_DWORD INT;

	static const INT modulus1    = 2147483563;
	static const INT modulus2    = 2147483399;

	static const INT multiplier1 = 40014;
	static const INT multiplier2 = 40692;

	static const INT table_size  = 32;

	extern const struct GENERATOR global;

	struct LCGENERATOR
	{
		LCGENERATOR(INT mod,INT mult,INT seed);

		INT operator()() const;
		INT getmodulus() const;

	  private:

		INT modulus;
		INT multiplier;
		INT quotient;
		INT remainder;

		mutable INT result;
	};

	struct GENERATOR
	{
		GENERATOR(INT seed1,INT seed2);

		INT operator()() const;
		INT getmodulus() const;

		int operator()(int max) const;
		double operator()(double min,double max) const;

		double compact_normal(double min,double max) const;

	  private:

		LCGENERATOR generator1;
		LCGENERATOR generator2;

		mutable INT table[table_size];
		mutable INT result;
	};
}
