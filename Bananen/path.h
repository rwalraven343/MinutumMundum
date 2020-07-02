//Code by Rogier Walraven. Do not distribute.

struct PATH
{
	PATH(const VECTOR<3> *points,int numpoints);
	~PATH();

	void operator()(double t,VECTOR<3> &C,VECTOR<3> &Cs) const;

  private:

	CURVE *curves;
	int numcurves;

	PATH(const PATH &other) {}
	PATH& operator=(const PATH &other) {}
};
