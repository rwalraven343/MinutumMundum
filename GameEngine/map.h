//Code by Rogier Walraven. Do not distribute.

struct MAP_PROPERTIES
{
	double meter_scale;
	double second_scale;
	double kilogram_scale;
	double joules_per_kelvin;
	double temperature;

	double audio_spatial_scale;

	const char *tileset_filename;
	const char *skyplane_filename;

	int bitmapwidth;
	int bitmapheight;

	int tilewidth;
	int tileheight;

	int transparentx;
	int transparenty;

	int numtextures;

	struct SHAPE
	{
		int numtriangles;
		int numquads;

		const TRIANGLE *triangles;
		const QUAD     *quads;
	};

	int numshapes;

	const SHAPE *shapes;

	struct AMBIENT_SOUND
	{
		const char *sample_filename;

		VECTOR position;
		double radius;

		double silence_duration;

		double loudness;

		double pitchvar;
		double volumevar;
	};

	int numambient_sounds;

	const AMBIENT_SOUND *ambient_sounds;

	double tilewidth_scale;
	double tileheight_scale;

	int mapwidth;
	int mapheight;

	bool local_gravity;

	VECTOR gravity;

	double friction;
	double refraction;

	struct LINEAR_MOTION
	{
		VECTOR deltapos;

		double start;
		double duration;

		bool hassound;

		const char *velocity_sample_filename;
		const char *acceleration_sample_filename;

		double velocity_sound_loudness;
		double velocity_sound_pitch;

		double acceleration_sound_loudness;
		double acceleration_sound_pitch;
	};

	struct ANGULAR_MOTION
	{
		double deltaang;

		double start;
		double duration;

		bool hassound;

		const char *velocity_sample_filename;
		const char *acceleration_sample_filename;

		double velocity_sound_loudness;
		double velocity_sound_pitch;

		double acceleration_sound_loudness;
		double acceleration_sound_pitch;
	};

	struct PARTICLE_EMITTER
	{
		int max_particles;

		PARTICLE_EMITTER_PROPERTIES properties;

		double color1[3];
		double color2[3];

		double alpha;
		double size;

		bool lit;

		double color1_light[3];
		double color2_light[3];

		double alpha_light;
		double size_light;
	};

	struct BEAM
	{
		int max_divisions;

		BEAM_PROPERTIES properties;

		VECTOR position;
		double orientation;

		double color[4];
		double thickness;

		double color_light[4];
		double thickness_light;
	};

	struct POINTLIGHT
	{
		int x,y;
		int radius;

		double color[4];
	};

	struct LAYER
	{
		const int *texture_index;
		const int *shape_index;

		bool wrap;

		VECTOR position;
		double orientation;

		int    linear_motion_type;
		double linear_motion_loop_start;
		double linear_motion_loop_end;

		int    angular_motion_type;
		double angular_motion_loop_start;
		double angular_motion_loop_end;

		int numlinear_motions;
		int numangular_motions;

		const LINEAR_MOTION  *linear_motions;
		const ANGULAR_MOTION *angular_motions;

		bool hasparticle_emitter;

		PARTICLE_EMITTER particle_emitter;

		int numbeams;

		const BEAM *beams;

		int numpointlights;

		const POINTLIGHT *pointlights;
	};

	LAYER primary_layer;
	LAYER secondary_layer;
	LAYER tertiary_layer;
	LAYER foreground_layer;
	LAYER background_layer;
};

struct MAP
{
	struct LAYER;

	MAP(const MAP_PROPERTIES &props);
	~MAP();

	const MAP_PROPERTIES& getproperties() const;

	const LAYER& getprimary_layer() const;
	const LAYER& getsecondary_layer() const;
	const LAYER& gettertiary_layer() const;

	void getfictitious_forces(VECTOR &axle_pos,double &angular_vel,VECTOR &linear_acc,double &angular_acc,double timestep) const;

	void fillclist_primary  (COLLISIONLIST &clist,const VECTOR &pos,double radius,double timestep) const;
	void fillclist_secondary(COLLISIONLIST &clist,const VECTOR &pos,double radius,double timestep) const;
	void fillclist_tertiary (COLLISIONLIST &clist,const VECTOR &pos,double radius,double timestep) const;

	void integrate(double timestep);

	void light_primary   (const VECTOR &pos,double radius) const;
	void light_secondary (const VECTOR &pos,double radius) const;
	void light_tertiary  (const VECTOR &pos,double radius) const;
	void light_foreground(const VECTOR &pos,double radius) const;
	void light_background(const VECTOR &pos,double radius) const;

	void draw_primary   (const VECTOR &pos,double radius,double alpha) const;
	void draw_primary   (const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const;

	void draw_secondary (const VECTOR &pos,double radius,double alpha) const;
	void draw_secondary (const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const;

	void draw_tertiary  (const VECTOR &pos,double radius,double alpha) const;
	void draw_tertiary  (const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const;

	void draw_foreground(const VECTOR &pos,double radius,double alpha) const;
	void draw_background(const VECTOR &pos,double radius,double alpha) const;

	void draw_skyplane  (const VECTOR &pos,double radius) const;

	void sound(const VECTOR &pos,double radius) const;

  private:

	struct TILE
	{
		bool empty;
		bool invisible;

		VIDEO::TEXTURE texture;

		int numtriangles;
		int numquads;

		TRIANGLE *triangles;
		QUAD     *quads;

		struct POINTLIGHT
		{
			int x,y;
			int width,height;

			double color[4];
		};

		int numpointlights;

		POINTLIGHT *pointlights;

		TILE() : triangles(0),quads(0),pointlights(0) {}
		~TILE() {delete[] triangles;delete[] quads;delete[] pointlights;}

      private:

		TILE(const TILE &other) {}
		TILE& operator=(const TILE &other) {}
	};

	struct MOTION_SOUND
	{
		const AUDIO::SOURCE *velocity_source;
		const AUDIO::SOURCE *acceleration_source;

		double max_velocity;
		double max_acceleration;

		MOTION_SOUND() : velocity_source(0),acceleration_source(0) {}
		~MOTION_SOUND() {delete velocity_source;delete acceleration_source;}

      private:

		MOTION_SOUND(const MOTION_SOUND &other) {}
		MOTION_SOUND& operator=(const MOTION_SOUND &other) {}
	};

  public:

	struct LAYER
	{
		LAYER() : tiles(0),linear_motion_sounds(0),angular_motion_sounds(0) {}
		~LAYER() {delete[] tiles;delete[] linear_motion_sounds;delete[] angular_motion_sounds;}

		void init(const MAP_PROPERTIES &props,const MAP_PROPERTIES::LAYER &props_layer,const LAYER *primary_layer,VIDEO::TEXTURE *textures);

		const MAP_PROPERTIES::LAYER& getproperties() const;

		VECTOR getposition() const;
		double getorientation() const;

		VECTOR getcenter() const;

		double getlinear_motion_time() const;
		double getangular_motion_time() const;

		VECTOR getlinear_velocity() const;
		double getangular_velocity() const;

		VECTOR getlinear_velocity (double time,double timestep) const;
		double getangular_velocity(double time,double timestep) const;

		VECTOR getlinear_acceleration (double time,double timestep) const;
		double getangular_acceleration(double time,double timestep) const;

		void fillclist(COLLISIONLIST &clist,const VECTOR &pos,double radius,double timestep) const;

		void integrate(double timestep);

		void light(const VECTOR &pos,double radius) const;

		void draw(const VECTOR &pos,double radius,double alpha) const;
		void draw(const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const;

		void sound(const VECTOR &pos,double radius) const;

      private:

		VECTOR position;
		double orientation;

		double linear_motion_time;
		double angular_motion_time;

		double linear_motion_once_end;
		double angular_motion_once_end;

		TILE *tiles;

		MOTION_SOUND *linear_motion_sounds;
		MOTION_SOUND *angular_motion_sounds;

		double tilewidth;
		double tileheight;

		const MAP_PROPERTIES        *properties;
		const MAP_PROPERTIES::LAYER *properties_layer;

		const LAYER *primary_layer;

		VECTOR getlinear_velocity (double time) const;
		double getangular_velocity(double time) const;

		VECTOR getlinear_acceleration (double time) const;
		double getangular_acceleration(double time) const;

		void getmapcoords     (int &x,int &y,const VECTOR &pos) const;
		void getmapcoords_rect(int &x1,int &y1,int &x2,int &y2,const VECTOR &pos,double radius) const;

		int    getmapindex_wrap (int x,int y) const;
		VECTOR getmapoffset_wrap(int x,int y) const;

		void calc_redundancy();

		void calc_redundancy(TILE &tile,const MAP_PROPERTIES::SHAPE &shape,const VECTOR &offset_wrap);

		void calc_redundancy(int numtriangles,TRIANGLE *triangles,int cnumtriangles,const TRIANGLE *ctriangles,const VECTOR &offset_wrap);
		void calc_redundancy(int numtriangles,TRIANGLE *triangles,int cnumquads    ,const QUAD     *cquads    ,const VECTOR &offset_wrap);
		void calc_redundancy(int numquads    ,QUAD     *quads    ,int cnumtriangles,const TRIANGLE *ctriangles,const VECTOR &offset_wrap);
		void calc_redundancy(int numquads    ,QUAD     *quads    ,int cnumquads    ,const QUAD     *cquads    ,const VECTOR &offset_wrap);

		void calc_redundancy(TRIANGLE &triangle,const TRIANGLE &ctriangle,const VECTOR &offset_wrap);
		void calc_redundancy(TRIANGLE &triangle,const QUAD     &cquad    ,const VECTOR &offset_wrap);
		void calc_redundancy(QUAD     &quad    ,const TRIANGLE &ctriangle,const VECTOR &offset_wrap);
		void calc_redundancy(QUAD     &quad    ,const QUAD     &cquad    ,const VECTOR &offset_wrap);

		void calc_redundancy(LINE &line,const LINE &cline,const VECTOR &offset_wrap);

		void getneighbour_shapes(MAP_PROPERTIES::SHAPE &left  ,VECTOR &left_offset_wrap,
                                 MAP_PROPERTIES::SHAPE &right ,VECTOR &right_offset_wrap,
                                 MAP_PROPERTIES::SHAPE &top   ,VECTOR &top_offset_wrap,
                                 MAP_PROPERTIES::SHAPE &bottom,VECTOR &bottom_offset_wrap,
                                 int x,int y) const;

		void getshape(MAP_PROPERTIES::SHAPE &shape,VECTOR &offset_wrap,int x,int y) const;

		LAYER(const LAYER &other) {}
		LAYER& operator=(const LAYER &other) {}
	};

  private:

	struct AMBIENT_SOUND
	{
		const AUDIO::SOURCE *source;

		VECTOR position;

		double starttime;
		double currenttime;

		double pitch;
		double volume;

		AMBIENT_SOUND() : source(0) {}
		~AMBIENT_SOUND() {delete source;}

      private:

		AMBIENT_SOUND(const AMBIENT_SOUND &other) {}
		AMBIENT_SOUND& operator=(const AMBIENT_SOUND &other) {}
	};

	VIDEO::TEXTURE *textures;

	LAYER primary_layer;
	LAYER secondary_layer;
	LAYER tertiary_layer;
	LAYER foreground_layer;
	LAYER background_layer;

	VIDEO::TEXTURE skyplane;

	AMBIENT_SOUND *ambient_sounds;

	double tilewidth;
	double tileheight;

	const MAP_PROPERTIES *properties;

	MAP(const MAP &other) {}
	MAP& operator=(const MAP &other) {}
};
