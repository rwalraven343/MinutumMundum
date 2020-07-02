//Code by Rogier Walraven. Do not distribute.

static const int max_dynamic_models_per_planet=64;

struct PLANET : public RENDERER
{
	PLANET(int depth,const VECTOR<3> &origin,double scale,int numstars,
           double pleft,double pright,
           double pbottom,double ptop,
           double pnear,double pfar,
           int numlodlevels,int numstepsa,int numstepsb,
		   const char *colourmap_filename,const char *heightmap_filename,
		   const QUATERNION &orientation,
           double inner_radius,double outer_radius,
           double lod_distance,double lod_mul);
	~PLANET();

	void add_dynamic_model(DYNAMIC_MODEL &dmodel);
	void remove_dynamic_model(const DYNAMIC_MODEL &dmodel);

	void begin_first_frame();
	void end_frame();

	void integrate();

  private:

	DYNAMIC_MODEL **dynamic_models;
	int numdynamic_models;

	double begin_frame_time;
	double end_frame_time;

	double real_time;
	double real_time_offset;

	double simulation_time;

	double timestep;
	double timestep_error;

	void integrate(double timestep);
};
