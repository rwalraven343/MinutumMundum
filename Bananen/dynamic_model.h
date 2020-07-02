//Code by Rogier Walraven. Do not distribute.

struct DYNAMIC_MODEL
{
	DYNAMIC_MODEL() {}

	virtual void control(double timestep)=0;

	virtual VECTOR<3> getposition() const=0;
	virtual double getradius() const=0;
	virtual const TRIANGLE *gettriangles() const=0;
	virtual int getnumtriangles() const=0;

	virtual bool haspath() const=0;
	virtual void integrate(double timestep)=0;

  private:

	DYNAMIC_MODEL(const DYNAMIC_MODEL &other) {}
	DYNAMIC_MODEL& operator=(const DYNAMIC_MODEL &other) {}
};
