//Code by Rogier Walraven. Do not distribute.

#define _USE_MATH_DEFINES
#include <math.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include "video.h"

static void push_and_setup_matrices_for_screenspace_draw();
static void restore_matrices_after_screenspace_draw();

static void getquad_coords_fullscreen         (double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4);
static void getquad_coords_pos_radius         (double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,double x,double y,double radius);
static void getquad_coords_pos1_pos2_thickness(double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,double ix1,double iy1,double ix2,double iy2,double thickness);

static void set_parameters_for_bound_texture();

static void getpixel_locked_surface(Uint8 &r,Uint8 &g,Uint8 &b,Uint8 &a,int x,int y,SDL_Surface *surface);
static void setpixel_locked_surface(Uint8  r,Uint8  g,Uint8  b,Uint8  a,int x,int y,SDL_Surface *surface);

static GLuint sdl_surface_to_opengl_texture(Uint8 colkey_r,Uint8 colkey_g,Uint8 colkey_b,SDL_Surface *surface);

static double sph_kernel_alpha(double x,double y,double px,double py,double radius);

static GLuint generate_fluid_texture(int size);
static GLuint generate_beam_texture(int width,int height);
static GLuint generate_spotlight_texture(int size);

static SDL_Surface *screen;

static double camerax=0;
static double cameray=0;
static double cameraangle=0;
static double camerazoom=1;

static bool   cliprect_enabled=false;
static double cliprect_x;
static double cliprect_y;
static double cliprect_w;
static double cliprect_h;

extern const double math_epsilon;

extern const int max_unique_textures;
extern const int max_unique_tilesets;

struct REFCOUNTEDTEXTURE
{
	const char *filename;
	GLuint texture;
	int refcount;

	REFCOUNTEDTEXTURE() : filename(0) {}
};

struct REFCOUNTEDTILESET
{
	const char *filename;
	void **textures;
	int refcount;

	REFCOUNTEDTILESET() : filename(0) {}
};

static REFCOUNTEDTEXTURE *refcountedtextures;
static REFCOUNTEDTILESET *refcountedtilesets;

static GLuint fluidtexture;
static GLuint beamtexture;
static GLuint spotlighttexture;

static int numcharacters;
static VIDEO::TEXTURE *characters;

static const int max_numberstring=128;
static char numberstring[max_numberstring];

static PFNGLGENFRAMEBUFFERSEXTPROC      glGenFramebuffers;
static PFNGLBINDFRAMEBUFFEREXTPROC      glBindFramebuffer;
static PFNGLDELETEFRAMEBUFFERSEXTPROC   glDeleteFramebuffers;
static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D;

static GLuint framebufferobject;

static bool dynamic_lighting_available=false;
static int  dynamic_lighting_numlayers;
static int  dynamic_lighting_currentlayer;

struct DYNAMIC_LIGHTING_LAYER
{
	GLuint rendertexture_color;
	GLuint rendertexture_light;

	DYNAMIC_LIGHTING_LAYER();
	~DYNAMIC_LIGHTING_LAYER();
};

static DYNAMIC_LIGHTING_LAYER *dynamic_lighting_layers;

void VIDEO::init(int w,int h)
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,1);

    screen=SDL_SetVideoMode(w,h,32,SDL_OPENGL|SDL_FULLSCREEN);

	glViewport(0,0,screen->w,screen->h);

    glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

	refcountedtextures=new REFCOUNTEDTEXTURE[max_unique_textures];
	refcountedtilesets=new REFCOUNTEDTILESET[max_unique_tilesets];

	fluidtexture=generate_fluid_texture(128);
	beamtexture=generate_beam_texture(32,128);
	spotlighttexture=generate_spotlight_texture(128);
}

void VIDEO::quit()
{
	delete[] refcountedtextures;
	delete[] refcountedtilesets;

	glDeleteTextures(1,&fluidtexture);
	glDeleteTextures(1,&beamtexture);
	glDeleteTextures(1,&spotlighttexture);
}

double VIDEO::width()
{
	return((double)screen->w);
}

double VIDEO::height()
{
	return((double)screen->h);
}

void VIDEO::cls()
{
	disable_cliprect();

	if (dynamic_lighting_available)
	{
		if (dynamic_lighting_currentlayer==-1)
		{
			glBindFramebuffer(GL_FRAMEBUFFER_EXT,framebufferobject);
		}

		for (int i=0;i<dynamic_lighting_numlayers;i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[i].rendertexture_color,0);

			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT);

			glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[i].rendertexture_light,0);

			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		glBindFramebuffer(GL_FRAMEBUFFER_EXT,0);

		dynamic_lighting_currentlayer=-1;
	}

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camerax=0;
    cameray=0;
    cameraangle=0;
    camerazoom=1;
}

void VIDEO::show()
{
	if (dynamic_lighting_available)
	{
		disable_cliprect();

		camera(0,0,0,1);

		if (dynamic_lighting_currentlayer==-1)
		{
			glBindFramebuffer(GL_FRAMEBUFFER_EXT,framebufferobject);
		}

		double x1,y1,x2,y2,x3,y3,x4,y4;
		getquad_coords_fullscreen(x1,y1,x2,y2,x3,y3,x4,y4);

		glEnable(GL_TEXTURE_2D);

		glBlendFunc(GL_DST_COLOR,GL_ZERO);

		for (int i=0;i<dynamic_lighting_numlayers;i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[i].rendertexture_color,0);

			glBindTexture(GL_TEXTURE_2D,dynamic_lighting_layers[i].rendertexture_light);

			glBegin(GL_QUADS);
			glColor4d(1,1,1,1);
			glTexCoord2d(0,0); glVertex2d(x4,y4);
			glTexCoord2d(1,0); glVertex2d(x3,y3);
			glTexCoord2d(1,1); glVertex2d(x2,y2);
			glTexCoord2d(0,1); glVertex2d(x1,y1);
			glEnd();
		}

		glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

		glBindFramebuffer(GL_FRAMEBUFFER_EXT,0);

		dynamic_lighting_currentlayer=-1;

		for (int i=0;i<dynamic_lighting_numlayers;i++)
		{
			glBindTexture(GL_TEXTURE_2D,dynamic_lighting_layers[i].rendertexture_color);

			glBegin(GL_QUADS);
			glColor4d(1,1,1,1);
			glTexCoord2d(0,0); glVertex2d(x4,y4);
			glTexCoord2d(1,0); glVertex2d(x3,y3);
			glTexCoord2d(1,1); glVertex2d(x2,y2);
			glTexCoord2d(0,1); glVertex2d(x1,y1);
			glEnd();
		}

		glDisable(GL_TEXTURE_2D);
	}

    SDL_GL_SwapBuffers();
	glFinish();
}

void VIDEO::camera(double x,double y,double angle,double zoom)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,screen->w*zoom,screen->h*zoom,0,-1,1);
    glTranslated((screen->w*zoom)/2,(screen->h*zoom)/2,0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(-angle,0,0,1);
    glTranslated(-x,-y,0);

    camerax=x;
    cameray=y;
    cameraangle=angle;
    camerazoom=zoom;
}

void VIDEO::enable_cliprect(double x,double y,double w,double h)
{
	glScissor((GLint)(x+(((double)screen->w)/2)),(GLint)((-y)+(((double)screen->h)/2)-h),(GLsizei)w,(GLsizei)h);
	glEnable(GL_SCISSOR_TEST);

	cliprect_enabled=true;
	cliprect_x=x;
	cliprect_y=y;
	cliprect_w=w;
	cliprect_h=h;
}

void VIDEO::disable_cliprect()
{
	glDisable(GL_SCISSOR_TEST);

	cliprect_enabled=false;
}

void VIDEO::DYNAMIC_LIGHTING::init(int numlayers)
{
	glGenFramebuffers     =(PFNGLGENFRAMEBUFFERSEXTPROC)     SDL_GL_GetProcAddress("glGenFramebuffersEXT");
	glBindFramebuffer     =(PFNGLBINDFRAMEBUFFEREXTPROC)     SDL_GL_GetProcAddress("glBindFramebufferEXT");
	glDeleteFramebuffers  =(PFNGLDELETEFRAMEBUFFERSEXTPROC)  SDL_GL_GetProcAddress("glDeleteFramebuffersEXT");
	glFramebufferTexture2D=(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)SDL_GL_GetProcAddress("glFramebufferTexture2DEXT");

	if (glGenFramebuffers && glBindFramebuffer && glDeleteFramebuffers && glFramebufferTexture2D)
	{
		glGenFramebuffers(1,&framebufferobject);

		dynamic_lighting_numlayers   =numlayers;
		dynamic_lighting_currentlayer=-1;

		dynamic_lighting_layers=new DYNAMIC_LIGHTING_LAYER[numlayers];

		dynamic_lighting_available=true;

		return;
	}

	dynamic_lighting_available=false;
}

void VIDEO::DYNAMIC_LIGHTING::quit()
{
	if (dynamic_lighting_available)
	{
		delete[] dynamic_lighting_layers;

		glDeleteFramebuffers(1,&framebufferobject);
	}
}

inline DYNAMIC_LIGHTING_LAYER::DYNAMIC_LIGHTING_LAYER()
{
	glGenTextures(1,&rendertexture_color);
	glGenTextures(1,&rendertexture_light);

	glBindTexture(GL_TEXTURE_2D,rendertexture_color);

	set_parameters_for_bound_texture();

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,screen->w,screen->h,0,GL_RGBA,GL_UNSIGNED_BYTE,0);

	glBindTexture(GL_TEXTURE_2D,rendertexture_light);

	set_parameters_for_bound_texture();

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,screen->w,screen->h,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
}

inline DYNAMIC_LIGHTING_LAYER::~DYNAMIC_LIGHTING_LAYER()
{
	glDeleteTextures(1,&rendertexture_color);
	glDeleteTextures(1,&rendertexture_light);
}

bool VIDEO::DYNAMIC_LIGHTING::available()
{
	return(dynamic_lighting_available);
}

int VIDEO::DYNAMIC_LIGHTING::getnumlayers()
{
	return(dynamic_lighting_numlayers);
}

int VIDEO::DYNAMIC_LIGHTING::getcurrentlayer()
{
	return(dynamic_lighting_currentlayer);
}

void VIDEO::DYNAMIC_LIGHTING::setlayer(int layer)
{
	if (dynamic_lighting_currentlayer!=layer)
	{
		if (dynamic_lighting_currentlayer==-1)
		{
			glBindFramebuffer(GL_FRAMEBUFFER_EXT,framebufferobject);

			glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[layer].rendertexture_color,0);
		}
		else
		{
			if (layer==-1)
			{
				glBindFramebuffer(GL_FRAMEBUFFER_EXT,0);
			}
			else
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[layer].rendertexture_color,0);
			}
		}

		dynamic_lighting_currentlayer=layer;
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_ambientlight(double r,double g,double b,double a)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		push_and_setup_matrices_for_screenspace_draw();

		double x1,y1,x2,y2,x3,y3,x4,y4;
		getquad_coords_fullscreen(x1,y1,x2,y2,x3,y3,x4,y4);

		draw_quad(x1,y1,x2,y2,x3,y3,x4,y4,r,g,b,a);

		restore_matrices_after_screenspace_draw();

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_pointlight(double x,double y,double r,double g,double b,double a,double radius)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		draw_point_fluid(x,y,r,g,b,a,radius);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_spotlight (double x1,double y1,double x2,double y2,double r,double g,double b,double a,double thickness)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		double bx1,by1,bx2,by2,bx3,by3,bx4,by4;
		getquad_coords_pos1_pos2_thickness(bx1,by1,bx2,by2,bx3,by3,bx4,by4,x1,y1,x2,y2,thickness);

		glBindTexture(GL_TEXTURE_2D,spotlighttexture);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
		glColor4d(r*a,g*a,b*a,a);
		glTexCoord2d(0,0); glVertex2d(bx1,by1);
		glTexCoord2d(1,0); glVertex2d(bx2,by2);
		glTexCoord2d(1,1); glVertex2d(bx3,by3);
		glTexCoord2d(0,1); glVertex2d(bx4,by4);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_beamlight (double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		draw_beam(x1,y1,x2,y2,r,g,b,a,ts,te,thickness);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_sparklight(double x,double y,double r,double g,double b,double a,double s)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		draw_point_sized(x,y,r,g,b,a,s);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_pointlight_fixed(double x,double y,double r,double g,double b,double a,double radius)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		push_and_setup_matrices_for_screenspace_draw();

		draw_point_fluid(x,y,r,g,b,a,radius);

		restore_matrices_after_screenspace_draw();

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_spotlight_fixed (double x1,double y1,double x2,double y2,double r,double g,double b,double a,double thickness)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		push_and_setup_matrices_for_screenspace_draw();

		double bx1,by1,bx2,by2,bx3,by3,bx4,by4;
		getquad_coords_pos1_pos2_thickness(bx1,by1,bx2,by2,bx3,by3,bx4,by4,x1,y1,x2,y2,thickness);

		glBindTexture(GL_TEXTURE_2D,spotlighttexture);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
		glColor4d(r*a,g*a,b*a,a);
		glTexCoord2d(0,0); glVertex2d(bx1,by1);
		glTexCoord2d(1,0); glVertex2d(bx2,by2);
		glTexCoord2d(1,1); glVertex2d(bx3,by3);
		glTexCoord2d(0,1); glVertex2d(bx4,by4);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		restore_matrices_after_screenspace_draw();

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_beamlight_fixed (double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		push_and_setup_matrices_for_screenspace_draw();

		draw_beam(x1,y1,x2,y2,r,g,b,a,ts,te,thickness);

		restore_matrices_after_screenspace_draw();

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_sparklight_fixed(double x,double y,double r,double g,double b,double a,double s)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		push_and_setup_matrices_for_screenspace_draw();

		draw_point_sized(x,y,r,g,b,a,s);

		restore_matrices_after_screenspace_draw();

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::draw_pointlight_tile(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double r,double g,double b,double a,int x,int y,int width,int height)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		double tsx=(double)x/width;
		double tsy=(double)y/height;

		double tex=((double)x+1)/width;
		double tey=((double)y+1)/height;

		glBindTexture(GL_TEXTURE_2D,fluidtexture);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
		glColor4d(r*a,g*a,b*a,a);
		glTexCoord2d(tsx,tsy); glVertex2d(x1,y1);
		glTexCoord2d(tex,tsy); glVertex2d(x2,y2);
		glTexCoord2d(tex,tey); glVertex2d(x3,y3);
		glTexCoord2d(tsx,tey); glVertex2d(x4,y4);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_start()
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		glBindTexture(GL_TEXTURE_2D,fluidtexture);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_end()
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight(double x,double y,double r,double g,double b,double a,double radius)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		double x1,y1,x2,y2,x3,y3,x4,y4;
		getquad_coords_pos_radius(x1,y1,x2,y2,x3,y3,x4,y4,x,y,radius);

		glColor4d(r*a,g*a,b*a,a);
		glTexCoord2d(0,0); glVertex2d(x1,y1);
		glTexCoord2d(1,0); glVertex2d(x2,y2);
		glTexCoord2d(1,1); glVertex2d(x3,y3);
		glTexCoord2d(0,1); glVertex2d(x4,y4);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_spotlight_start()
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		glBindTexture(GL_TEXTURE_2D,spotlighttexture);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_spotlight_end()
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_spotlight(double x1,double y1,double x2,double y2,double r,double g,double b,double a,double thickness)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		double bx1,by1,bx2,by2,bx3,by3,bx4,by4;
		getquad_coords_pos1_pos2_thickness(bx1,by1,bx2,by2,bx3,by3,bx4,by4,x1,y1,x2,y2,thickness);

		glColor4d(r*a,g*a,b*a,a);
		glTexCoord2d(0,0); glVertex2d(bx1,by1);
		glTexCoord2d(1,0); glVertex2d(bx2,by2);
		glTexCoord2d(1,1); glVertex2d(bx3,by3);
		glTexCoord2d(0,1); glVertex2d(bx4,by4);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_beamlight_start()
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		glBindTexture(GL_TEXTURE_2D,beamtexture);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_beamlight_end()
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_beamlight(double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		double bx1,by1,bx2,by2,bx3,by3,bx4,by4;
		getquad_coords_pos1_pos2_thickness(bx1,by1,bx2,by2,bx3,by3,bx4,by4,x1,y1,x2,y2,thickness);

		glColor4d(r*a,g*a,b*a,a);
		glTexCoord2d(0,1-te); glVertex2d(bx1,by1);
		glTexCoord2d(1,1-te); glVertex2d(bx2,by2);
		glTexCoord2d(1,1-ts); glVertex2d(bx3,by3);
		glTexCoord2d(0,1-ts); glVertex2d(bx4,by4);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_sparklight_start(double s)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		if (s>=2)
		{
			glPointSize((GLfloat)s);
		}

		glBegin(GL_POINTS);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_sparklight_end()
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glEnd();

		glPointSize(1);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_sparklight(double x,double y,double r,double g,double b,double a)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glColor4d(r*a,g*a,b*a,a);
		glVertex2d(x,y);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_tile_start()
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_light,0);

		glBindTexture(GL_TEXTURE_2D,fluidtexture);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_tile_end()
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,dynamic_lighting_layers[dynamic_lighting_currentlayer].rendertexture_color,0);
	}
}

void VIDEO::DYNAMIC_LIGHTING::BATCH::draw_pointlight_tile(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double r,double g,double b,double a,int x,int y,int width,int height)
{
	if (dynamic_lighting_currentlayer!=-1)
	{
		double tsx=(double)x/width;
		double tsy=(double)y/height;

		double tex=((double)x+1)/width;
		double tey=((double)y+1)/height;

		glColor4d(r*a,g*a,b*a,a);
		glTexCoord2d(tsx,tsy); glVertex2d(x1,y1);
		glTexCoord2d(tex,tsy); glVertex2d(x2,y2);
		glTexCoord2d(tex,tey); glVertex2d(x3,y3);
		glTexCoord2d(tsx,tey); glVertex2d(x4,y4);
	}
}

void VIDEO::draw_point      (double x,double y,double r,double g,double b,double a)
{
	glBegin(GL_POINTS);
	glColor4d(r*a,g*a,b*a,a);
	glVertex2d(x,y);
	glEnd();
}

void VIDEO::draw_point_sized(double x,double y,double r,double g,double b,double a,double s)
{
	if (s>=2)
	{
		glPointSize((GLfloat)s);
	}

	draw_point(x,y,r,g,b,a);

	if (s>=2)
	{
		glPointSize(1);
	}
}

void VIDEO::draw_point_fluid(double x,double y,double r,double g,double b,double a,double radius)
{
	double x1,y1,x2,y2,x3,y3,x4,y4;
	getquad_coords_pos_radius(x1,y1,x2,y2,x3,y3,x4,y4,x,y,radius);

	glBindTexture(GL_TEXTURE_2D,fluidtexture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glColor4d(r*a,g*a,b*a,a);
	glTexCoord2d(0,0); glVertex2d(x1,y1);
	glTexCoord2d(1,0); glVertex2d(x2,y2);
	glTexCoord2d(1,1); glVertex2d(x3,y3);
	glTexCoord2d(0,1); glVertex2d(x4,y4);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void VIDEO::draw_line(double x1,double y1,double x2,double y2,double r,double g,double b,double a)
{
	glBegin(GL_LINES);
	glColor4d(r*a,g*a,b*a,a);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glEnd();
}

void VIDEO::draw_beam(double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness)
{
	double bx1,by1,bx2,by2,bx3,by3,bx4,by4;
	getquad_coords_pos1_pos2_thickness(bx1,by1,bx2,by2,bx3,by3,bx4,by4,x1,y1,x2,y2,thickness);

	glBindTexture(GL_TEXTURE_2D,beamtexture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glColor4d(r*a,g*a,b*a,a);
	glTexCoord2d(0,1-te); glVertex2d(bx1,by1);
	glTexCoord2d(1,1-te); glVertex2d(bx2,by2);
	glTexCoord2d(1,1-ts); glVertex2d(bx3,by3);
	glTexCoord2d(0,1-ts); glVertex2d(bx4,by4);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void VIDEO::draw_polygon        (double x1,double y1,double x2,double y2,double x3,double y3,double r,double g,double b,double a)
{
	glBegin(GL_TRIANGLES);
	glColor4d(r*a,g*a,b*a,a);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

void VIDEO::draw_polygon_texture(double x1,double y1,double x2,double y2,double x3,double y3,TEXTURE texture,double tsx,double tex,double tsy,double tey,double a)
{
	glBindTexture(GL_TEXTURE_2D,((REFCOUNTEDTEXTURE*)texture)->texture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);
	glColor4d(a,a,a,a);
    glTexCoord2d(tsx,tsy); glVertex2d(x1,y1);
    glTexCoord2d(tex,tsy); glVertex2d(x2,y2);
    glTexCoord2d(tex,tey); glVertex2d(x3,y3);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void VIDEO::draw_quad        (double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double r,double g,double b,double a)
{
	glBegin(GL_QUADS);
	glColor4d(r*a,g*a,b*a,a);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glVertex2d(x4,y4);
	glEnd();
}

void VIDEO::draw_quad_texture(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,TEXTURE texture,double a)
{
	glBindTexture(GL_TEXTURE_2D,((REFCOUNTEDTEXTURE*)texture)->texture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glColor4d(a,a,a,a);
	glTexCoord2d(0,0); glVertex2d(x1,y1);
	glTexCoord2d(1,0); glVertex2d(x2,y2);
	glTexCoord2d(1,1); glVertex2d(x3,y3);
	glTexCoord2d(0,1); glVertex2d(x4,y4);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void VIDEO::draw_quad_texture(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,TEXTURE texture,double r,double g,double b,double a)
{
	glBindTexture(GL_TEXTURE_2D,((REFCOUNTEDTEXTURE*)texture)->texture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glColor4d(r*a,g*a,b*a,a);
	glTexCoord2d(0,0); glVertex2d(x1,y1);
	glTexCoord2d(1,0); glVertex2d(x2,y2);
	glTexCoord2d(1,1); glVertex2d(x3,y3);
	glTexCoord2d(0,1); glVertex2d(x4,y4);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void VIDEO::draw_quad_texture_fixed(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,TEXTURE texture,double a)
{
	push_and_setup_matrices_for_screenspace_draw();

	draw_quad_texture(x1,y1,x2,y2,x3,y3,x4,y4,texture,a);

	restore_matrices_after_screenspace_draw();
}

void VIDEO::draw_quad_texture_fixed(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,TEXTURE texture,double r,double g,double b,double a)
{
	push_and_setup_matrices_for_screenspace_draw();

	draw_quad_texture(x1,y1,x2,y2,x3,y3,x4,y4,texture,r,g,b,a);

	restore_matrices_after_screenspace_draw();
}

void VIDEO::BATCH::draw_point_sized_start(double s)
{
	if (s>=2)
	{
		glPointSize((GLfloat)s);
	}

	glBegin(GL_POINTS);
}

void VIDEO::BATCH::draw_point_sized_end()
{
	glEnd();

	glPointSize(1);
}

void VIDEO::BATCH::draw_point_sized(double x,double y,double r,double g,double b,double a)
{
	glColor4d(r*a,g*a,b*a,a);
	glVertex2d(x,y);
}

void VIDEO::BATCH::draw_point_fluid_start()
{
	glBindTexture(GL_TEXTURE_2D,fluidtexture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
}

void VIDEO::BATCH::draw_point_fluid_end()
{
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void VIDEO::BATCH::draw_point_fluid(double x,double y,double r,double g,double b,double a,double radius)
{
	double x1,y1,x2,y2,x3,y3,x4,y4;
	getquad_coords_pos_radius(x1,y1,x2,y2,x3,y3,x4,y4,x,y,radius);

	glColor4d(r*a,g*a,b*a,a);
	glTexCoord2d(0,0); glVertex2d(x1,y1);
	glTexCoord2d(1,0); glVertex2d(x2,y2);
	glTexCoord2d(1,1); glVertex2d(x3,y3);
	glTexCoord2d(0,1); glVertex2d(x4,y4);
}

void VIDEO::BATCH::draw_line_start()
{
	glBegin(GL_LINES);
}

void VIDEO::BATCH::draw_line_end()
{
	glEnd();
}

void VIDEO::BATCH::draw_line(double x1,double y1,double x2,double y2,double r,double g,double b,double a)
{
	glColor4d(r*a,g*a,b*a,a);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
}

void VIDEO::BATCH::draw_beam_start()
{
	glBindTexture(GL_TEXTURE_2D,beamtexture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
}

void VIDEO::BATCH::draw_beam_end()
{
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void VIDEO::BATCH::draw_beam(double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness)
{
	double bx1,by1,bx2,by2,bx3,by3,bx4,by4;
	getquad_coords_pos1_pos2_thickness(bx1,by1,bx2,by2,bx3,by3,bx4,by4,x1,y1,x2,y2,thickness);

	glColor4d(r*a,g*a,b*a,a);
	glTexCoord2d(0,1-te); glVertex2d(bx1,by1);
	glTexCoord2d(1,1-te); glVertex2d(bx2,by2);
	glTexCoord2d(1,1-ts); glVertex2d(bx3,by3);
	glTexCoord2d(0,1-ts); glVertex2d(bx4,by4);
}

void VIDEO::BATCH::draw_polygon_start()
{
	glBegin(GL_TRIANGLES);
}

void VIDEO::BATCH::draw_polygon_end()
{
	glEnd();
}

void VIDEO::BATCH::draw_polygon(double x1,double y1,double x2,double y2,double x3,double y3,double r,double g,double b,double a)
{
	glColor4d(r*a,g*a,b*a,a);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
}

void VIDEO::BATCH::draw_polygon_texture_start(TEXTURE texture)
{
	glBindTexture(GL_TEXTURE_2D,((REFCOUNTEDTEXTURE*)texture)->texture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);
}

void VIDEO::BATCH::draw_polygon_texture_end()
{
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void VIDEO::BATCH::draw_polygon_texture(double x1,double y1,double x2,double y2,double x3,double y3,double tsx,double tex,double tsy,double tey,double a)
{
	glColor4d(a,a,a,a);
    glTexCoord2d(tsx,tsy); glVertex2d(x1,y1);
    glTexCoord2d(tex,tsy); glVertex2d(x2,y2);
    glTexCoord2d(tex,tey); glVertex2d(x3,y3);
}

VIDEO::TEXTURE VIDEO::loadtexture(const char *filename)
{
	for (int i=0;i<max_unique_textures;i++)
	{
		if (refcountedtextures[i].filename)
		{
			if (strcmp(refcountedtextures[i].filename,filename)==0)
			{
				refcountedtextures[i].refcount++;

				return((void*)(refcountedtextures+i));
			}
		}
	}

	for (int i=0;i<max_unique_textures;i++)
	{
		if (!refcountedtextures[i].filename)
		{
			refcountedtextures[i].filename=filename;
			refcountedtextures[i].refcount=1;

			SDL_Surface *surface=SDL_LoadBMP(filename);
			refcountedtextures[i].texture=sdl_surface_to_opengl_texture(0,0,0,surface);
			SDL_FreeSurface(surface);

			return((void*)(refcountedtextures+i));
		}
	}

	return(0);
}

void    VIDEO::freetexture(TEXTURE texture)
{
	for (int i=0;i<max_unique_textures;i++)
	{
		if (refcountedtextures[i].filename)
		{
			if ((void*)(refcountedtextures+i)==texture)
			{
				refcountedtextures[i].refcount--;

				if (refcountedtextures[i].refcount==0)
				{
					refcountedtextures[i].filename=0;

					glDeleteTextures(1,&(refcountedtextures[i].texture));
				}

				return;
			}
		}
	}
}

VIDEO::TEXTURE *VIDEO::loadtileset(const char *filename,int bitmapwidth,int bitmapheight,int tilewidth,int tileheight,int transparentx,int transparenty,int numtextures)
{
	for (int i=0;i<max_unique_tilesets;i++)
	{
		if (refcountedtilesets[i].filename)
		{
			if (strcmp(refcountedtilesets[i].filename,filename)==0)
			{
				refcountedtilesets[i].refcount++;

				return(refcountedtilesets[i].textures);
			}
		}
	}

	for (int i=0;i<max_unique_tilesets;i++)
	{
		if (!refcountedtilesets[i].filename)
		{
			refcountedtilesets[i].filename=filename;
			refcountedtilesets[i].refcount=1;

			SDL_Surface *surface=SDL_LoadBMP(filename);

			Uint8 colkey_r,colkey_g,colkey_b,dummy_a;
			SDL_LockSurface(surface);
			getpixel_locked_surface(colkey_r,colkey_g,colkey_b,dummy_a,transparentx,transparenty,surface);
			SDL_UnlockSurface(surface);

			refcountedtilesets[i].textures=new void*[numtextures];
			for (int j=0;j<numtextures;j++)
			{
				refcountedtilesets[i].textures[j]=(void*)new REFCOUNTEDTEXTURE;
			}

			SDL_Rect rect;
			SDL_Surface *tile;
			int texcount=0;

			for (int y=0;y<bitmapheight;y+=tileheight)
			{
				for (int x=0;x<bitmapwidth;x+=tilewidth)
				{
					if (texcount<numtextures)
					{
						rect.x=x;
						rect.y=y;
						rect.w=tilewidth;
						rect.h=tileheight;

						tile=SDL_CreateRGBSurface(SDL_SWSURFACE,tilewidth,tileheight,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
						SDL_BlitSurface(surface,&rect,tile,0);

						((REFCOUNTEDTEXTURE*)(refcountedtilesets[i].textures[texcount]))->texture=sdl_surface_to_opengl_texture(colkey_r,colkey_g,colkey_b,tile);
						texcount++;

						SDL_FreeSurface(tile);
					}
				}
			}

			SDL_FreeSurface(surface);

			return(refcountedtilesets[i].textures);
		}
	}

	return(0);
}

void     VIDEO::freetileset(TEXTURE *textures,int numtextures)
{
	for (int i=0;i<max_unique_tilesets;i++)
	{
		if (refcountedtilesets[i].filename)
		{
			if (refcountedtilesets[i].textures==textures)
			{
				refcountedtilesets[i].refcount--;

				if (refcountedtilesets[i].refcount==0)
				{
					refcountedtilesets[i].filename=0;

					for (int j=0;j<numtextures;j++)
					{
						glDeleteTextures(1,&(((REFCOUNTEDTEXTURE*)(refcountedtilesets[i].textures[j]))->texture));

						delete ((REFCOUNTEDTEXTURE*)(refcountedtilesets[i].textures[j]));
					}

					delete[] refcountedtilesets[i].textures;
				}

				return;
			}
		}
	}
}

void VIDEO::screenshot(const char *filename)
{
	SDL_Surface *surface        =SDL_CreateRGBSurface(SDL_SWSURFACE,screen->w,screen->h,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
	SDL_Surface *flipped_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,screen->w,screen->h,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);

	glReadBuffer(GL_FRONT);
	glReadPixels(0,0,surface->w,surface->h,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);
	glReadBuffer(GL_BACK);

	Uint8 r,g,b,a;

	SDL_LockSurface(surface);
	SDL_LockSurface(flipped_surface);
	for (int y=0;y<surface->h;y++)
	{
		for (int x=0;x<surface->w;x++)
		{
			getpixel_locked_surface(r,g,b,a,x,y,surface);
			setpixel_locked_surface(r,g,b,255,x,surface->h-y-1,flipped_surface);
		}
	}
	SDL_UnlockSurface(flipped_surface);
	SDL_UnlockSurface(surface);

	SDL_SaveBMP(flipped_surface,filename);

	SDL_FreeSurface(surface);
	SDL_FreeSurface(flipped_surface);
}

void VIDEO::TEXT::init(const char *filename)
{
	numcharacters=256;
	characters=loadtileset(filename,288,128,9,16,0,0,numcharacters);
}

void VIDEO::TEXT::quit()
{
	freetileset(characters,numcharacters);
}

void VIDEO::TEXT::getdimensions       (int &x,int &y,const char *string)
{
	x=0;
	y=1;

	int i=0;
	int xmax=0;

	while (string[i]!='\0')
	{
		if (string[i]=='\t')
		{
			x+=4;
		}
		else if (string[i]=='\n')
		{
			if (x>xmax)
			{
				xmax=x;
			}

			x=0;
			y++;
		}
		else
		{
			x++;
		}

		i++;
	}

	if (x<xmax)
	{
		x=xmax;
	}
}

void VIDEO::TEXT::getdimensions_number(int &x,int &y,int number)
{
	sprintf_s(numberstring,max_numberstring,"%i",number);

	getdimensions(x,y,numberstring);
}

void VIDEO::TEXT::getdimensions_number(int &x,int &y,double number)
{
	sprintf_s(numberstring,max_numberstring,"%f",number);

	getdimensions(x,y,numberstring);
}

void VIDEO::TEXT::print       (double x,double y,double gw,double gh,const char *string,double r,double g,double b,double a)
{
	int i=0;
	double xreset=x;

	while (string[i]!='\0')
	{
		if (string[i]=='\t')
		{
			x+=gw*4;
		}
		else if (string[i]=='\n')
		{
			x=xreset;
			y+=gh;
		}
		else
		{
			draw_quad_texture(x,y,x+gw,y,x+gw,y+gh,x,y+gh,characters[string[i]],r,g,b,a);
			x+=gw;
		}

		i++;
	}
}

void VIDEO::TEXT::print_number(double x,double y,double gw,double gh,int number        ,double r,double g,double b,double a)
{
	sprintf_s(numberstring,max_numberstring,"%i",number);

	print(x,y,gw,gh,numberstring,r,g,b,a);
}

void VIDEO::TEXT::print_number(double x,double y,double gw,double gh,double number     ,double r,double g,double b,double a)
{
	sprintf_s(numberstring,max_numberstring,"%f",number);

	print(x,y,gw,gh,numberstring,r,g,b,a);
}

void VIDEO::TEXT::print_fixed       (double x,double y,double gw,double gh,const char *string,double r,double g,double b,double a)
{
	push_and_setup_matrices_for_screenspace_draw();

	print(x,y,gw,gh,string,r,g,b,a);

	restore_matrices_after_screenspace_draw();
}

void VIDEO::TEXT::print_number_fixed(double x,double y,double gw,double gh,int number        ,double r,double g,double b,double a)
{
	push_and_setup_matrices_for_screenspace_draw();

	print_number(x,y,gw,gh,number,r,g,b,a);

	restore_matrices_after_screenspace_draw();
}

void VIDEO::TEXT::print_number_fixed(double x,double y,double gw,double gh,double number     ,double r,double g,double b,double a)
{
	push_and_setup_matrices_for_screenspace_draw();

	print_number(x,y,gw,gh,number,r,g,b,a);

	restore_matrices_after_screenspace_draw();
}

VIDEO::HUD::HUD(double x,double y,double w,double h,double s,const char *border,const char *display)
{
	xpos=x;
	ypos=y;
	width=w;
	height=h;
	scale=s;

	this->border=loadtexture(border);
	this->display=loadtexture(display);

	camerax=0;
	cameray=0;
	cameraangle=0;
	camerazoom=1;
}

VIDEO::HUD::~HUD()
{
	freetexture(border);
	freetexture(display);
}

void VIDEO::HUD::move   (double x,double y)
{
	xpos=x;
	ypos=y;
}

void VIDEO::HUD::resize (double w,double h)
{
	width=w;
	height=h;
}

void VIDEO::HUD::rescale(double s)
{
	scale=s;
}

double VIDEO::HUD::getwidth() const
{
	return(width);
}

double VIDEO::HUD::getheight() const
{
	return(height);
}

double VIDEO::HUD::getscale() const
{
	return(scale);
}

void VIDEO::HUD::camera(double x,double y,double angle,double zoom)
{
	camerax=x;
	cameray=y;
	cameraangle=angle;
	camerazoom=zoom;
}

void VIDEO::HUD::draw_border(double a) const
{
	draw_quad_texture_fixed(xpos,ypos,xpos+width,ypos,xpos+width,ypos+height,xpos,ypos+height,border,a);
}

void VIDEO::HUD::draw_display(double a) const
{
	draw_quad_texture_fixed(xpos,ypos,xpos+width,ypos,xpos+width,ypos+height,xpos,ypos+height,display,a);
}

void VIDEO::HUD::draw_point      (double x,double y,double r,double g,double b,double a) const
{
	transformcoord(x,y);

	push_and_setup_matrices_for_screenspace_draw();

	enable_cliprect();

	::VIDEO::draw_point(x,y,r,g,b,a);

	disable_cliprect();

	restore_matrices_after_screenspace_draw();
}

void VIDEO::HUD::draw_point_sized(double x,double y,double r,double g,double b,double a,double s) const
{
	transformcoord(x,y);

	push_and_setup_matrices_for_screenspace_draw();

	enable_cliprect();

	::VIDEO::draw_point_sized(x,y,r,g,b,a,s);

	disable_cliprect();

	restore_matrices_after_screenspace_draw();
}

void VIDEO::HUD::draw_line(double x1,double y1,double x2,double y2,double r,double g,double b,double a) const
{
	transformcoord(x1,y1);
	transformcoord(x2,y2);

	push_and_setup_matrices_for_screenspace_draw();

	enable_cliprect();

	::VIDEO::draw_line(x1,y1,x2,y2,r,g,b,a);

	disable_cliprect();

	restore_matrices_after_screenspace_draw();
}

void VIDEO::HUD::batch_draw_line_start() const
{
	push_and_setup_matrices_for_screenspace_draw();

	enable_cliprect();

	::VIDEO::BATCH::draw_line_start();
}

void VIDEO::HUD::batch_draw_line_end() const
{
	::VIDEO::BATCH::draw_line_end();

	disable_cliprect();

	restore_matrices_after_screenspace_draw();
}

void VIDEO::HUD::batch_draw_line(double x1,double y1,double x2,double y2,double r,double g,double b,double a) const
{
	transformcoord(x1,y1);
	transformcoord(x2,y2);

	::VIDEO::BATCH::draw_line(x1,y1,x2,y2,r,g,b,a);
}

void VIDEO::HUD::enable_cliprect() const
{
	if (!cliprect_enabled)
	{
		glScissor((GLint)(xpos+(((double)screen->w)/2)),(GLint)((-ypos)+(((double)screen->h)/2)-height),(GLsizei)width,(GLsizei)height);
		glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		double x,y,w,h;

		if (commoncliprect(x,y,w,h))
		{
			glScissor((GLint)(x+(((double)screen->w)/2)),(GLint)((-y)+(((double)screen->h)/2)-h),(GLsizei)w,(GLsizei)h);
		}
		else
		{
			glScissor(0,0,0,0);
		}
	}
}

void VIDEO::HUD::disable_cliprect() const
{
	if (!cliprect_enabled)
	{
		glDisable(GL_SCISSOR_TEST);
	}
	else
	{
		::VIDEO::enable_cliprect(cliprect_x,cliprect_y,cliprect_w,cliprect_h);
	}
}

void VIDEO::HUD::transformcoord(double &x,double &y) const
{
	double tx=(x-camerax)*(1/camerazoom)*(1/scale);
	double ty=(y-cameray)*(1/camerazoom)*(1/scale);

	double rad=((M_PI)/180)*(-cameraangle);

	double sinrad=sin(rad);
	double cosrad=cos(rad);

	x=tx*cosrad - ty*sinrad;
	y=ty*cosrad + tx*sinrad;

	x+=xpos+(width/2);
	y+=ypos+(height/2);
}

bool VIDEO::HUD::commoncliprect(double &x,double &y,double &w,double &h) const
{
	double start_x1=cliprect_x;
	double   end_x1=cliprect_x+cliprect_w;
	double start_y1=cliprect_y;
	double   end_y1=cliprect_y+cliprect_h;

	double start_x2=xpos;
	double   end_x2=xpos+width;
	double start_y2=ypos;
	double   end_y2=ypos+height;

	if (start_x2>=end_x1 || end_x2<=start_x1)
	{
		return(false);
	}

	if (start_y2>=end_y1 || end_y2<=start_y1)
	{
		return(false);
	}

	double start_x3;
	double   end_x3;
	double start_y3;
	double   end_y3;

	if (start_x2>=start_x1)
	{
		start_x3=start_x2;
	}
	else
	{
		start_x3=start_x1;
	}

	if (end_x2<=end_x1)
	{
		end_x3=end_x2;
	}
	else
	{
		end_x3=end_x1;
	}

	if (start_y2>=start_y1)
	{
		start_y3=start_y2;
	}
	else
	{
		start_y3=start_y1;
	}

	if (end_y2<=end_y1)
	{
		end_y3=end_y2;
	}
	else
	{
		end_y3=end_y1;
	}

	x=start_x3;
	y=start_y3;
	w=end_x3-start_x3;
	h=end_y3-start_y3;

	return(true);
}

static void push_and_setup_matrices_for_screenspace_draw()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,screen->w,screen->h,0,-1,1);
	glTranslated(((double)screen->w)/2,((double)screen->h)/2,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void restore_matrices_after_screenspace_draw()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

static void getquad_coords_fullscreen         (double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4)
{
	double width =VIDEO::width();
	double height=VIDEO::height();

	x1=-width/2;
	y1=-height/2;

	x2= width/2;
	y2=-height/2;

	x3= width/2;
	y3= height/2;

	x4=-width/2;
	y4= height/2;
}

static void getquad_coords_pos_radius         (double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,double x,double y,double radius)
{
	x1=x-radius;
	y1=y-radius;

	x2=x+radius;
	y2=y-radius;

	x3=x+radius;
	y3=y+radius;

	x4=x-radius;
	y4=y+radius;
}

static void getquad_coords_pos1_pos2_thickness(double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,double ix1,double iy1,double ix2,double iy2,double thickness)
{
	double dx=ix2-ix1;
	double dy=iy2-iy1;

	double len=sqrt(dx*dx + dy*dy);

	if (len<math_epsilon)
	{
		len=math_epsilon;
	}

	double hwx=(-dy/len)*(thickness/2);
	double hwy=( dx/len)*(thickness/2);

	x1=ix2-hwx;
	y1=iy2-hwy;

	x2=ix2+hwx;
	y2=iy2+hwy;

	x3=ix1+hwx;
	y3=iy1+hwy;

	x4=ix1-hwx;
	y4=iy1-hwy;
}

static void set_parameters_for_bound_texture()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
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

static GLuint sdl_surface_to_opengl_texture(Uint8 colkey_r,Uint8 colkey_g,Uint8 colkey_b,SDL_Surface *surface)
{
	SDL_Surface *colkey_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,surface->w,surface->h,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);

	Uint8  r, g, b, a;
	Uint8 nr,ng,nb,na;
	int nx,ny,ncounter;
	double alpha;

	SDL_LockSurface(surface);
	SDL_LockSurface(colkey_surface);
	for (int y=0;y<surface->h;y++)
	{
		for (int x=0;x<surface->w;x++)
		{
			getpixel_locked_surface(r,g,b,a,x,y,surface);

			if (r==colkey_r && g==colkey_g && b==colkey_b)
			{
				r=0;
				g=0;
				b=0;
				a=0;

				alpha=0;
			}
			else
			{
				ncounter=0;

				nx=x-1;
				ny=y-1;

				if (nx>=0 && nx<surface->w && ny>=0 && ny<surface->h)
				{
					getpixel_locked_surface(nr,ng,nb,na,nx,ny,surface);

					if (nr==colkey_r && ng==colkey_g && nb==colkey_b)
					{
						ncounter++;
					}
				}

				nx=x;
				ny=y-1;

				if (nx>=0 && nx<surface->w && ny>=0 && ny<surface->h)
				{
					getpixel_locked_surface(nr,ng,nb,na,nx,ny,surface);

					if (nr==colkey_r && ng==colkey_g && nb==colkey_b)
					{
						ncounter++;
					}
				}

				nx=x+1;
				ny=y-1;

				if (nx>=0 && nx<surface->w && ny>=0 && ny<surface->h)
				{
					getpixel_locked_surface(nr,ng,nb,na,nx,ny,surface);

					if (nr==colkey_r && ng==colkey_g && nb==colkey_b)
					{
						ncounter++;
					}
				}

				nx=x-1;
				ny=y;

				if (nx>=0 && nx<surface->w && ny>=0 && ny<surface->h)
				{
					getpixel_locked_surface(nr,ng,nb,na,nx,ny,surface);

					if (nr==colkey_r && ng==colkey_g && nb==colkey_b)
					{
						ncounter++;
					}
				}

				nx=x+1;
				ny=y;

				if (nx>=0 && nx<surface->w && ny>=0 && ny<surface->h)
				{
					getpixel_locked_surface(nr,ng,nb,na,nx,ny,surface);

					if (nr==colkey_r && ng==colkey_g && nb==colkey_b)
					{
						ncounter++;
					}
				}

				nx=x-1;
				ny=y+1;

				if (nx>=0 && nx<surface->w && ny>=0 && ny<surface->h)
				{
					getpixel_locked_surface(nr,ng,nb,na,nx,ny,surface);

					if (nr==colkey_r && ng==colkey_g && nb==colkey_b)
					{
						ncounter++;
					}
				}

				nx=x;
				ny=y+1;

				if (nx>=0 && nx<surface->w && ny>=0 && ny<surface->h)
				{
					getpixel_locked_surface(nr,ng,nb,na,nx,ny,surface);

					if (nr==colkey_r && ng==colkey_g && nb==colkey_b)
					{
						ncounter++;
					}
				}

				nx=x+1;
				ny=y+1;

				if (nx>=0 && nx<surface->w && ny>=0 && ny<surface->h)
				{
					getpixel_locked_surface(nr,ng,nb,na,nx,ny,surface);

					if (nr==colkey_r && ng==colkey_g && nb==colkey_b)
					{
						ncounter++;
					}
				}

				a=255-(int)(ncounter*((double)255/9));

				alpha=1-(ncounter*((double)1/9));
			}

			setpixel_locked_surface((Uint8)(r*alpha),(Uint8)(g*alpha),(Uint8)(b*alpha),a,x,y,colkey_surface);
		}
	}
	SDL_UnlockSurface(colkey_surface);
	SDL_UnlockSurface(surface);

    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

	set_parameters_for_bound_texture();

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,colkey_surface->w,colkey_surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,colkey_surface->pixels);

	SDL_FreeSurface(colkey_surface);

	return(texture);
}

static double sph_kernel_alpha(double x,double y,double px,double py,double radius)
{
	double dx=x-px;
	double dy=y-py;

	double q=(sqrt(dx*dx + dy*dy)/radius)*2;

	if (q>=0 && q<1)
	{
		return(1-(((double)3/2)*q*q)+(((double)3/4)*q*q*q));
	}

	if (q>=1 && q<2)
	{
		double tq=2-q;

		return(((double)1/4)*tq*tq*tq);
	}

	return(0);
}

static GLuint generate_fluid_texture(int size)
{
	SDL_Surface *fluid_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,size,size,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);

	double radius=(double)size/2;

	double alpha;
	Uint8  a;

	SDL_LockSurface(fluid_surface);
	for (int y=0;y<size;y++)
	{
		for (int x=0;x<size;x++)
		{
			alpha=sph_kernel_alpha(x+0.5,y+0.5,radius,radius,radius);

			a=(int)(255*alpha);

			setpixel_locked_surface(a,a,a,a,x,y,fluid_surface);
		}
	}
	SDL_UnlockSurface(fluid_surface);

    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

	set_parameters_for_bound_texture();

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,size,size,0,GL_RGBA,GL_UNSIGNED_BYTE,fluid_surface->pixels);

	SDL_FreeSurface(fluid_surface);

	return(texture);
}

static GLuint generate_beam_texture(int width,int height)
{
	SDL_Surface *beam_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);

	double radius=(double)width/2;

	double alpha;
	Uint8  a;

	SDL_LockSurface(beam_surface);
	for (int y=0;y<height;y++)
	{
		for (int x=0;x<width;x++)
		{
			alpha=sph_kernel_alpha(x+0.5,0,radius,0,radius);

			alpha*=(double)y/(height-1);

			a=(int)(255*alpha);

			setpixel_locked_surface(a,a,a,a,x,y,beam_surface);
		}
	}
	SDL_UnlockSurface(beam_surface);

    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

	set_parameters_for_bound_texture();

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,beam_surface->pixels);

	SDL_FreeSurface(beam_surface);

	return(texture);
}

static GLuint generate_spotlight_texture(int size)
{
	SDL_Surface *spotlight_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,size,size,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);

	double radius=(double)size/2;

	double mul;

	double alpha;
	Uint8  a;

	SDL_LockSurface(spotlight_surface);
	for (int y=0;y<size;y++)
	{
		for (int x=0;x<size;x++)
		{
			if ((y+0.5)<=radius)
			{
				alpha=sph_kernel_alpha(x+0.5,y+0.5,radius,radius,radius);
			}
			else
			{
				mul=1+((y+0.5-radius)/radius);

				alpha=sph_kernel_alpha(((x+0.5-radius)*mul)+radius,0,radius,0,radius);
			}

			a=(int)(255*alpha);

			setpixel_locked_surface(a,a,a,a,x,y,spotlight_surface);
		}
	}
	SDL_UnlockSurface(spotlight_surface);

    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

	set_parameters_for_bound_texture();

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,size,size,0,GL_RGBA,GL_UNSIGNED_BYTE,spotlight_surface->pixels);

	SDL_FreeSurface(spotlight_surface);

	return(texture);
}
