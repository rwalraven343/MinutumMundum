//Code by Rogier Walraven. Do not distribute.

#include <SDL.h>

#include "..\\GameEngine\\GameEngine.h"
#include "Bananen.h"

static void getpixel_locked_surface(Uint8 &r,Uint8 &g,Uint8 &b,Uint8 &a,int x,int y,SDL_Surface *surface);
static void setpixel_locked_surface(Uint8  r,Uint8  g,Uint8  b,Uint8  a,int x,int y,SDL_Surface *surface);

BANANEN::IMAGE::IMAGE(int w,int h) :
	width(w),
	height(h),
	colour(new VECTOR<4>[w*h]) {}

BANANEN::IMAGE::IMAGE(const char *filename)
{
	SDL_Surface *surface=SDL_LoadBMP(filename);

	width=surface->w;
	height=surface->h;

	colour=new VECTOR<4>[width*height];

	Uint8 r,g,b,a;

	SDL_LockSurface(surface);
	for (int y=0;y<surface->h;y++)
	{
		for (int x=0;x<surface->w;x++)
		{
			getpixel_locked_surface(r,g,b,a,x,y,surface);

			colour[x + y*width].cmp[0]=(double)r/255;
			colour[x + y*width].cmp[1]=(double)g/255;
			colour[x + y*width].cmp[2]=(double)b/255;
			colour[x + y*width].cmp[3]=(double)a/255;
		}
	}
	SDL_UnlockSurface(surface);

	SDL_FreeSurface(surface);
}

BANANEN::IMAGE::~IMAGE()
{
	delete[] colour;
}

void BANANEN::IMAGE::save(const char *filename) const
{
	SDL_Surface *surface=SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);

	Uint8 r,g,b,a;
	double cmp0,cmp1,cmp2,cmp3;

	SDL_LockSurface(surface);
	for (int y=0;y<height;y++)
	{
		for (int x=0;x<width;x++)
		{
			cmp0=colour[x + y*width].cmp[0]; if (cmp0<0) {cmp0=0;} if (cmp0>1) {cmp0=1;}
			cmp1=colour[x + y*width].cmp[1]; if (cmp1<0) {cmp1=0;} if (cmp1>1) {cmp1=1;}
			cmp2=colour[x + y*width].cmp[2]; if (cmp2<0) {cmp2=0;} if (cmp2>1) {cmp2=1;}
			cmp3=colour[x + y*width].cmp[3]; if (cmp3<0) {cmp3=0;} if (cmp3>1) {cmp3=1;}

			r=(Uint8)(cmp0*255);
			g=(Uint8)(cmp1*255);
			b=(Uint8)(cmp2*255);
			a=(Uint8)(cmp3*255);

			setpixel_locked_surface(r,g,b,a,x,y,surface);
		}
	}
	SDL_UnlockSurface(surface);

	SDL_SaveBMP(surface,filename);

	SDL_FreeSurface(surface);
}

int BANANEN::IMAGE::getwidth() const
{
	return(width);
}

int BANANEN::IMAGE::getheight() const
{
	return(height);
}

BANANEN::VECTOR<4> BANANEN::IMAGE::getcolour(int x,int y) const
{
	return(colour[x + y*width]);
}

void BANANEN::IMAGE::setcolour(int x,int y,const VECTOR<4> &c)
{
	colour[x + y*width]=c;
}

BANANEN::VECTOR<4> BANANEN::IMAGE::getcolour_linear(double x,double y) const
{
    if (x<0)
    {
        x*=-1;
    }

    if (y<0)
    {
        y*=-1;
    }

    int xi=(int)x;
    int yi=(int)y;

    double xr=x-xi;
    double yr=y-yi;

    VECTOR<4> a=colour[((xi)  %width) + ((yi)  %height)*width] * (1-xr) * (1-yr);
    VECTOR<4> b=colour[((xi+1)%width) + ((yi)  %height)*width] *    xr  * (1-yr);
    VECTOR<4> c=colour[((xi)  %width) + ((yi+1)%height)*width] * (1-xr) *    yr;
    VECTOR<4> d=colour[((xi+1)%width) + ((yi+1)%height)*width] *    xr  *    yr;

    return(a+b+c+d);
}

void BANANEN::IMAGE::setcolour_linear(double x,double y,const VECTOR<4> &c)
{
    if (x>=0 && y>=0)
    {
        int xi=(int)x;
        int yi=(int)y;

        int ci1=((xi)  %width) + ((yi)  %height)*width;
        int ci2=((xi+1)%width) + ((yi)  %height)*width;
        int ci3=((xi)  %width) + ((yi+1)%height)*width;
        int ci4=((xi+1)%width) + ((yi+1)%height)*width;

        double xr=x-xi;
        double yr=y-yi;

        double w1=(1-xr) * (1-yr);
        double w2=   xr  * (1-yr);
        double w3=(1-xr) *    yr;
        double w4=   xr  *    yr;

        VECTOR<4> ca=(c*w1) + (colour[ci1]*(1-w1));
        VECTOR<4> cb=(c*w2) + (colour[ci2]*(1-w2));
        VECTOR<4> cc=(c*w3) + (colour[ci3]*(1-w3));
        VECTOR<4> cd=(c*w4) + (colour[ci4]*(1-w4));

        colour[ci1]=ca;
        colour[ci2]=cb;
        colour[ci3]=cc;
        colour[ci4]=cd;
    }
}

double BANANEN::IMAGE::getSHcoeff(int l,int m,int cmp) const
{
    double result=0;

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

            result += SH(l,m,azimuth,zenith) * colour[x + y*width].cmp[cmp] * sine(zenith) * math_pi * 4;
        }
    }

    return(result/(width*(height*(2/math_pi))));
}

static void getpixel_locked_surface(Uint8 &r,Uint8 &g,Uint8 &b,Uint8 &a,int x,int y,SDL_Surface *surface)
{
	Uint32 pixel=*((Uint32*)((Uint8*)surface->pixels+((x*surface->format->BytesPerPixel)+(y*surface->pitch))));

	SDL_GetRGBA(pixel,surface->format,&r,&g,&b,&a);
}

static void setpixel_locked_surface(Uint8  r,Uint8  g,Uint8  b,Uint8  a,int x,int y,SDL_Surface *surface)
{
	Uint32 pixel=SDL_MapRGBA(surface->format,r,g,b,a);

	*((Uint32*)((Uint8*)surface->pixels+((x*surface->format->BytesPerPixel)+(y*surface->pitch))))=pixel;
}
