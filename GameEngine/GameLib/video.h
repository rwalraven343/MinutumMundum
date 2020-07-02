//Code by Rogier Walraven. Do not distribute.

namespace VIDEO
{
    typedef void* TEXTURE;

    void init(int w,int h);
    void quit();

    double width();
    double height();

    void cls();
    void show();

    void camera(double x,double y,double angle,double zoom);

	void enable_cliprect(double x,double y,double w,double h);
	void disable_cliprect();

	namespace DYNAMIC_LIGHTING
	{
		void init(int numlayers);
		void quit();

		bool available();

		int getnumlayers();
		int getcurrentlayer();

		void setlayer(int layer);

		void draw_ambientlight(double r,double g,double b,double a);

		void draw_pointlight(double x,double y,double r,double g,double b,double a,double radius);
		void draw_spotlight (double x1,double y1,double x2,double y2,double r,double g,double b,double a,double thickness);
		void draw_beamlight (double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness);
		void draw_sparklight(double x,double y,double r,double g,double b,double a,double s);

		void draw_pointlight_fixed(double x,double y,double r,double g,double b,double a,double radius);
		void draw_spotlight_fixed (double x1,double y1,double x2,double y2,double r,double g,double b,double a,double thickness);
		void draw_beamlight_fixed (double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness);
		void draw_sparklight_fixed(double x,double y,double r,double g,double b,double a,double s);

		void draw_pointlight_tile(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double r,double g,double b,double a,int x,int y,int width,int height);

		namespace BATCH
		{
			void draw_pointlight_start();
			void draw_pointlight_end();

			void draw_pointlight(double x,double y,double r,double g,double b,double a,double radius);

			void draw_spotlight_start();
			void draw_spotlight_end();

			void draw_spotlight(double x1,double y1,double x2,double y2,double r,double g,double b,double a,double thickness);

			void draw_beamlight_start();
			void draw_beamlight_end();

			void draw_beamlight(double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness);

			void draw_sparklight_start(double s);
			void draw_sparklight_end();

			void draw_sparklight(double x,double y,double r,double g,double b,double a);

			void draw_pointlight_tile_start();
			void draw_pointlight_tile_end();

			void draw_pointlight_tile(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double r,double g,double b,double a,int x,int y,int width,int height);
		}
	}

    void draw_point      (double x,double y,double r,double g,double b,double a);
    void draw_point_sized(double x,double y,double r,double g,double b,double a,double s);
	void draw_point_fluid(double x,double y,double r,double g,double b,double a,double radius);

    void draw_line(double x1,double y1,double x2,double y2,double r,double g,double b,double a);
    void draw_beam(double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness);

    void draw_polygon        (double x1,double y1,double x2,double y2,double x3,double y3,double r,double g,double b,double a);
    void draw_polygon_texture(double x1,double y1,double x2,double y2,double x3,double y3,TEXTURE texture,double tsx,double tex,double tsy,double tey,double a);

	void draw_quad        (double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double r,double g,double b,double a);
	void draw_quad_texture(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,TEXTURE texture,double a);
	void draw_quad_texture(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,TEXTURE texture,double r,double g,double b,double a);

	void draw_quad_texture_fixed(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,TEXTURE texture,double a);
	void draw_quad_texture_fixed(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,TEXTURE texture,double r,double g,double b,double a);

	namespace BATCH
	{
		void draw_point_sized_start(double s);
		void draw_point_sized_end();

		void draw_point_sized(double x,double y,double r,double g,double b,double a);

		void draw_point_fluid_start();
		void draw_point_fluid_end();

		void draw_point_fluid(double x,double y,double r,double g,double b,double a,double radius);

		void draw_line_start();
		void draw_line_end();

		void draw_line(double x1,double y1,double x2,double y2,double r,double g,double b,double a);

		void draw_beam_start();
		void draw_beam_end();

		void draw_beam(double x1,double y1,double x2,double y2,double r,double g,double b,double a,double ts,double te,double thickness);

		void draw_polygon_start();
		void draw_polygon_end();

		void draw_polygon(double x1,double y1,double x2,double y2,double x3,double y3,double r,double g,double b,double a);

		void draw_polygon_texture_start(TEXTURE texture);
		void draw_polygon_texture_end();

		void draw_polygon_texture(double x1,double y1,double x2,double y2,double x3,double y3,double tsx,double tex,double tsy,double tey,double a);
	}

    TEXTURE loadtexture(const char *filename);
    void    freetexture(TEXTURE texture);

    TEXTURE *loadtileset(const char *filename,int bitmapwidth,int bitmapheight,int tilewidth,int tileheight,int transparentx,int transparenty,int numtextures);
    void     freetileset(TEXTURE *textures,int numtextures);

    void screenshot(const char *filename);

    namespace TEXT
    {
        void init(const char *filename);
        void quit();

		void getdimensions       (int &x,int &y,const char *string);
		void getdimensions_number(int &x,int &y,int number);
		void getdimensions_number(int &x,int &y,double number);

        void print       (double x,double y,double gw,double gh,const char *string,double r,double g,double b,double a);
        void print_number(double x,double y,double gw,double gh,int number        ,double r,double g,double b,double a);
        void print_number(double x,double y,double gw,double gh,double number     ,double r,double g,double b,double a);

        void print_fixed       (double x,double y,double gw,double gh,const char *string,double r,double g,double b,double a);
        void print_number_fixed(double x,double y,double gw,double gh,int number        ,double r,double g,double b,double a);
        void print_number_fixed(double x,double y,double gw,double gh,double number     ,double r,double g,double b,double a);
    }

    struct HUD
    {
        HUD(double x,double y,double w,double h,double s,const char *border,const char *display);
        ~HUD();

		void move   (double x,double y);
		void resize (double w,double h);
		void rescale(double s);

		double getwidth() const;
		double getheight() const;
		double getscale() const;

        void camera(double x,double y,double angle,double zoom);

        void draw_border(double a) const;
        void draw_display(double a) const;

        void draw_point      (double x,double y,double r,double g,double b,double a) const;
        void draw_point_sized(double x,double y,double r,double g,double b,double a,double s) const;

		void draw_line(double x1,double y1,double x2,double y2,double r,double g,double b,double a) const;

		void batch_draw_line_start() const;
		void batch_draw_line_end() const;

		void batch_draw_line(double x1,double y1,double x2,double y2,double r,double g,double b,double a) const;

      private:

        double xpos;
        double ypos;
        double width;
        double height;
        double scale;

        TEXTURE border;
        TEXTURE display;

        double camerax;
        double cameray;
        double cameraangle;
        double camerazoom;

		void enable_cliprect() const;
		void disable_cliprect() const;

		void transformcoord(double &x,double &y) const;
		bool commoncliprect(double &x,double &y,double &w,double &h) const;

        HUD(const HUD &other) {}
        HUD& operator=(const HUD &other) {}
    };
}
