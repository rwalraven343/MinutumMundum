//Code by Rogier Walraven. Do not distribute.

struct IMAGE
{
	IMAGE(int w,int h);
	IMAGE(const char *filename);
	~IMAGE();

	void save(const char *filename) const;

	int getwidth() const;
	int getheight() const;

	VECTOR<4> getcolour(int x,int y) const;
	void setcolour(int x,int y,const VECTOR<4> &c);

	VECTOR<4> getcolour_linear(double x,double y) const;
	void setcolour_linear(double x,double y,const VECTOR<4> &c);

	double getSHcoeff(int l,int m,int cmp) const;

    template<int SHn>
    VECTOR<SHn> getSHvector(int cmp) const;

    template<int SHn>
	void setSHvector(const VECTOR<SHn> &SHvector,int cmp);

  private:

	int width;
	int height;

	VECTOR<4> *colour;

	IMAGE(const IMAGE &other) {}
	IMAGE& operator=(const IMAGE &other) {}
};

template<int SHn>
VECTOR<SHn> BANANEN::IMAGE::getSHvector(int cmp) const
{
    VECTOR<SHn> SHvector;

    int l=0;
    int m=0;

    for (int i=0;i<SHn;i++)
    {
        SHvector.cmp[i] = getSHcoeff(l,m,cmp);

        if (l==m)
        {
            l++;
            m=-l;
        }
        else
        {
            m++;
        }
    }

    return(SHvector);
}

template<int SHn>
void BANANEN::IMAGE::setSHvector(const VECTOR<SHn> &SHvector,int cmp)
{
    double spsizex=(math_pi*2)/width;
    double spsizey=(math_pi)/height;

    double azimuth;
    double zenith;

    for (int y=0;y<height;y++)
    {
        for (int x=0;x<width;x++)
        {
		    azimuth=(x*spsizex)+(spsizex/2);
		    zenith =(y*spsizey)+(spsizey/2);

            colour[x + y*width].cmp[cmp] = SHvector(azimuth,zenith);
        }
    }
}
