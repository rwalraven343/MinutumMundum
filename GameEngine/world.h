//Code by Rogier Walraven. Do not distribute.

extern const int max_objects_per_world;

extern const double timestep_min;
extern const double timestep_max;
extern const double timestep_max_per_iteration;
extern const double timestep_max_error;
extern const double timestep_scale;

template<class O1,class O2,class O3,class O4>
struct WORLD
{
	WORLD(MAP &map);

	void change_map(MAP &new_map);
	void change_origin(const VECTOR &new_origin);

	const MAP_PROPERTIES& getproperties() const;

	MAP_LAYER_OBJECT& getprimary_layer_object();
	MAP_LAYER_OBJECT& getsecondary_layer_object();
	MAP_LAYER_OBJECT& gettertiary_layer_object();

	void putobject(O1 &object);
	void putobject(O2 &object);
	void putobject(O3 &object);
	void putobject(O4 &object);

	void putbeam_object(BEAM_OBJECT<O1> &beam_object);
	void putbeam_object(BEAM_OBJECT<O2> &beam_object);
	void putbeam_object(BEAM_OBJECT<O3> &beam_object);
	void putbeam_object(BEAM_OBJECT<O4> &beam_object);

	void removeobject(const O1 &object);
	void removeobject(const O2 &object);
	void removeobject(const O3 &object);
	void removeobject(const O4 &object);

	void removebeam_object(const BEAM_OBJECT<O1> &beam_object);
	void removebeam_object(const BEAM_OBJECT<O2> &beam_object);
	void removebeam_object(const BEAM_OBJECT<O3> &beam_object);
	void removebeam_object(const BEAM_OBJECT<O4> &beam_object);

	void begin_first_frame();
	void end_frame();

	double getreal_time_in_seconds() const;
	double getsimulation_time_in_seconds() const;
	double gettimestep_in_seconds() const;
	double gettimestep_error_in_seconds() const;

	void integrate();

	void light_objects() const;

	void light_primary   (const VECTOR &pos,double radius) const;
	void light_secondary (const VECTOR &pos,double radius) const;
	void light_tertiary  (const VECTOR &pos,double radius) const;
	void light_foreground(const VECTOR &pos,double radius) const;
	void light_background(const VECTOR &pos,double radius) const;

	void draw_objects() const;
	void draw_objects(const VIDEO::HUD &hud) const;

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

	template<class O>
	struct OBJECT_CONTAINER
	{
		int numobjects;
		int numbeam_objects;

		O              **objects;
		BEAM_OBJECT<O> **beam_objects;

		WORLD<O1,O2,O3,O4> *world;

		OBJECT_CONTAINER();
		~OBJECT_CONTAINER();

		void change_origin(const VECTOR &new_origin);

		void putobject(O &object);
		void putbeam_object(BEAM_OBJECT<O> &beam_object);

		void removeobject(const O &object);
		void removebeam_object(const BEAM_OBJECT<O> &beam_object);

		void clearforces();
		void control(double timestep);
		void addheat(double ht,const VECTOR &pos,void *object);
		void addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc);

		void fillclist(const VECTOR &pos,double radius,void *object);
		void setclist(double timestep);

		void update_beam(double timestep);

		void integrate(double timestep);

		void light() const;

		void draw() const;
		void draw(const VIDEO::HUD &hud) const;

		void sound() const;

      private:

		OBJECT_CONTAINER(const OBJECT_CONTAINER<O> &other) {}
		OBJECT_CONTAINER<O>& operator=(const OBJECT_CONTAINER<O> &other) {}
	};

	MAP *map;

	MAP_LAYER_OBJECT primary_layer_object;
	MAP_LAYER_OBJECT secondary_layer_object;
	MAP_LAYER_OBJECT tertiary_layer_object;

  protected:

	OBJECT_CONTAINER<O1> object_container1;
	OBJECT_CONTAINER<O2> object_container2;
	OBJECT_CONTAINER<O3> object_container3;
	OBJECT_CONTAINER<O4> object_container4;

  private:

	COLLISIONLIST clist;

	double begin_frame_time;
	double end_frame_time;

	double real_time;
	double real_time_offset;

	double simulation_time;

	double timestep;
	double timestep_error;

	void addheat(double ht,const VECTOR &pos,void *object);
	void fillclist(const VECTOR &pos,double radius,void *object,double timestep);

	void getgridcoords(int &x,int &y,const VECTOR &pos,double gridsize) const;

	template<class O>
	void update_beam(BEAM<O> &beam,double timestep);

	void integrate     (double timestep);
	void integrate_loop(double timestep);
	void integrate_min (double timestep);
	void integrate_call(double timestep);

	WORLD(const WORLD<O1,O2,O3,O4> &other) {}
	WORLD<O1,O2,O3,O4>& operator=(const WORLD<O1,O2,O3,O4> &other) {}
};

template<class O1,class O2,class O3,class O4>
WORLD<O1,O2,O3,O4>::WORLD(MAP &map) :
	primary_layer_object(map.getprimary_layer(),map.getprimary_layer()),
	secondary_layer_object(map.getsecondary_layer(),map.getprimary_layer()),
	tertiary_layer_object(map.gettertiary_layer(),map.getprimary_layer())
{
	this->map=&map;

	object_container1.world=this;
	object_container2.world=this;
	object_container3.world=this;
	object_container4.world=this;
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::change_map(MAP &new_map)
{
	map=&new_map;

	primary_layer_object.change_layer(new_map.getprimary_layer(),new_map.getprimary_layer());
	secondary_layer_object.change_layer(new_map.getsecondary_layer(),new_map.getprimary_layer());
	tertiary_layer_object.change_layer(new_map.gettertiary_layer(),new_map.getprimary_layer());
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::change_origin(const VECTOR &new_origin)
{
	if (primary_layer_object.hasparticle_emitter())
	{
		primary_layer_object.getparticle_emitter().change_origin(new_origin);
	}

	if (secondary_layer_object.hasparticle_emitter())
	{
		secondary_layer_object.getparticle_emitter().change_origin(new_origin);
	}

	if (tertiary_layer_object.hasparticle_emitter())
	{
		tertiary_layer_object.getparticle_emitter().change_origin(new_origin);
	}

	for (int i=0;i<primary_layer_object.getnumbeams();i++)
	{
		primary_layer_object.getbeam(i).change_origin(new_origin);
	}

	for (int i=0;i<secondary_layer_object.getnumbeams();i++)
	{
		secondary_layer_object.getbeam(i).change_origin(new_origin);
	}

	for (int i=0;i<tertiary_layer_object.getnumbeams();i++)
	{
		tertiary_layer_object.getbeam(i).change_origin(new_origin);
	}

	object_container1.change_origin(new_origin);
	object_container2.change_origin(new_origin);
	object_container3.change_origin(new_origin);
	object_container4.change_origin(new_origin);
}

template<class O1,class O2,class O3,class O4>
const MAP_PROPERTIES& WORLD<O1,O2,O3,O4>::getproperties() const
{
	return(map->getproperties());
}

template<class O1,class O2,class O3,class O4>
MAP_LAYER_OBJECT& WORLD<O1,O2,O3,O4>::getprimary_layer_object()
{
	return(primary_layer_object);
}

template<class O1,class O2,class O3,class O4>
MAP_LAYER_OBJECT& WORLD<O1,O2,O3,O4>::getsecondary_layer_object()
{
	return(secondary_layer_object);
}

template<class O1,class O2,class O3,class O4>
MAP_LAYER_OBJECT& WORLD<O1,O2,O3,O4>::gettertiary_layer_object()
{
	return(tertiary_layer_object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::putobject(O1 &object)
{
	object_container1.putobject(object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::putobject(O2 &object)
{
	object_container2.putobject(object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::putobject(O3 &object)
{
	object_container3.putobject(object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::putobject(O4 &object)
{
	object_container4.putobject(object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::putbeam_object(BEAM_OBJECT<O1> &beam_object)
{
	object_container1.putbeam_object(beam_object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::putbeam_object(BEAM_OBJECT<O2> &beam_object)
{
	object_container2.putbeam_object(beam_object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::putbeam_object(BEAM_OBJECT<O3> &beam_object)
{
	object_container3.putbeam_object(beam_object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::putbeam_object(BEAM_OBJECT<O4> &beam_object)
{
	object_container4.putbeam_object(beam_object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::removeobject(const O1 &object)
{
	object_container1.removeobject(object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::removeobject(const O2 &object)
{
	object_container2.removeobject(object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::removeobject(const O3 &object)
{
	object_container3.removeobject(object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::removeobject(const O4 &object)
{
	object_container4.removeobject(object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::removebeam_object(const BEAM_OBJECT<O1> &beam_object)
{
	object_container1.removebeam_object(beam_object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::removebeam_object(const BEAM_OBJECT<O2> &beam_object)
{
	object_container2.removebeam_object(beam_object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::removebeam_object(const BEAM_OBJECT<O3> &beam_object)
{
	object_container3.removebeam_object(beam_object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::removebeam_object(const BEAM_OBJECT<O4> &beam_object)
{
	object_container4.removebeam_object(beam_object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::begin_first_frame()
{
	begin_frame_time=SYSTEM::seconds_since_startup()*timestep_scale;

	real_time       =0;
	real_time_offset=begin_frame_time;

	simulation_time=0;

	timestep      =timestep_min;
	timestep_error=0;
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::end_frame()
{
	double timestep_new;

	end_frame_time=SYSTEM::seconds_since_startup()*timestep_scale;

	timestep_new=end_frame_time - begin_frame_time;

	begin_frame_time=end_frame_time;

	if (timestep_new<0)
	{
		begin_first_frame();

		return;
	}

	real_time=begin_frame_time - real_time_offset;

	timestep_error+=timestep_new - timestep;

	if (absolute(timestep_error)>timestep_max_error)
	{
		timestep_error=0;
	}

	timestep=timestep_new;
}

template<class O1,class O2,class O3,class O4>
double WORLD<O1,O2,O3,O4>::getreal_time_in_seconds() const
{
	return(real_time/timestep_scale);
}

template<class O1,class O2,class O3,class O4>
double WORLD<O1,O2,O3,O4>::getsimulation_time_in_seconds() const
{
	return(simulation_time/timestep_scale);
}

template<class O1,class O2,class O3,class O4>
double WORLD<O1,O2,O3,O4>::gettimestep_in_seconds() const
{
	return(timestep/timestep_scale);
}

template<class O1,class O2,class O3,class O4>
double WORLD<O1,O2,O3,O4>::gettimestep_error_in_seconds() const
{
	return(timestep_error/timestep_scale);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::integrate()
{
	double timestep_plus_error=timestep+timestep_error;

	if (timestep_plus_error>=0)
	{
		double cached_timestep_error=timestep_error;

		integrate(timestep_plus_error);

		timestep_error-=cached_timestep_error;

		return;
	}

	timestep_error+=timestep;
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::integrate(double timestep)
{
	if (timestep>timestep_max)
	{
		integrate_loop(timestep_max);

		timestep_error+=timestep-timestep_max;

		return;
	}

	integrate_loop(timestep);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::integrate_loop(double timestep)
{
	int    numiterations=(int)(timestep/timestep_max_per_iteration);
	double remainder    =timestep - numiterations*timestep_max_per_iteration;

	for (int i=0;i<numiterations;i++)
	{
		integrate_call(timestep_max_per_iteration);
	}

	if (remainder>0)
	{
		integrate_min(remainder);
	}
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::integrate_min(double timestep)
{
	if (timestep<timestep_min)
	{
		if (timestep<timestep_min/2)
		{
			timestep_error+=timestep;

			return;
		}

		integrate_call(timestep_min);

		timestep_error-=timestep_min-timestep;

		return;
	}

	integrate_call(timestep);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::integrate_call(double timestep)
{
	object_container1.setclist(timestep);
	object_container2.setclist(timestep);
	object_container3.setclist(timestep);
	object_container4.setclist(timestep);

	object_container1.clearforces();
	object_container2.clearforces();
	object_container3.clearforces();
	object_container4.clearforces();

	object_container1.control(timestep);
	object_container2.control(timestep);
	object_container3.control(timestep);
	object_container4.control(timestep);

	for (int i=0;i<primary_layer_object.getnumbeams();i++)
	{
		BEAM<MAP_LAYER_OBJECT>& beam=primary_layer_object.getbeam(i);

		if (beam.isactive())
		{
			update_beam(beam,timestep);
		}
	}

	for (int i=0;i<secondary_layer_object.getnumbeams();i++)
	{
		BEAM<MAP_LAYER_OBJECT>& beam=secondary_layer_object.getbeam(i);

		if (beam.isactive())
		{
			update_beam(beam,timestep);
		}
	}

	for (int i=0;i<tertiary_layer_object.getnumbeams();i++)
	{
		BEAM<MAP_LAYER_OBJECT>& beam=tertiary_layer_object.getbeam(i);

		if (beam.isactive())
		{
			update_beam(beam,timestep);
		}
	}

	object_container1.update_beam(timestep);
	object_container2.update_beam(timestep);
	object_container3.update_beam(timestep);
	object_container4.update_beam(timestep);

	VECTOR axle_pos;
	double angular_vel;
	VECTOR linear_acc;
	double angular_acc;

	map->getfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc,timestep);

	if (primary_layer_object.hasparticle_emitter())
	{
		primary_layer_object.getparticle_emitter().addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	}

	if (secondary_layer_object.hasparticle_emitter())
	{
		secondary_layer_object.getparticle_emitter().addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	}

	if (tertiary_layer_object.hasparticle_emitter())
	{
		tertiary_layer_object.getparticle_emitter().addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	}

	object_container1.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	object_container2.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	object_container3.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
	object_container4.addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);

	map->integrate(timestep);

	if (primary_layer_object.hasparticle_emitter())
	{
		primary_layer_object.getparticle_emitter().integrate(timestep);
	}

	if (secondary_layer_object.hasparticle_emitter())
	{
		secondary_layer_object.getparticle_emitter().integrate(timestep);
	}

	if (tertiary_layer_object.hasparticle_emitter())
	{
		tertiary_layer_object.getparticle_emitter().integrate(timestep);
	}

	object_container1.integrate(timestep);
	object_container2.integrate(timestep);
	object_container3.integrate(timestep);
	object_container4.integrate(timestep);

	simulation_time+=timestep;
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::light_objects() const
{
	object_container1.light();
	object_container2.light();
	object_container3.light();
	object_container4.light();
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::light_primary   (const VECTOR &pos,double radius) const
{
	map->light_primary(pos,radius);

	if (primary_layer_object.getnumbeams())
	{
		primary_layer_object.light_beams();
	}

	if (primary_layer_object.hasparticle_emitter())
	{
		primary_layer_object.light_particles();
	}
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::light_secondary (const VECTOR &pos,double radius) const
{
	map->light_secondary(pos,radius);

	if (secondary_layer_object.getnumbeams())
	{
		secondary_layer_object.light_beams();
	}

	if (secondary_layer_object.hasparticle_emitter())
	{
		secondary_layer_object.light_particles();
	}
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::light_tertiary  (const VECTOR &pos,double radius) const
{
	map->light_tertiary(pos,radius);

	if (tertiary_layer_object.getnumbeams())
	{
		tertiary_layer_object.light_beams();
	}

	if (tertiary_layer_object.hasparticle_emitter())
	{
		tertiary_layer_object.light_particles();
	}
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::light_foreground(const VECTOR &pos,double radius) const
{
	map->light_foreground(pos,radius);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::light_background(const VECTOR &pos,double radius) const
{
	map->light_background(pos,radius);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_objects() const
{
	object_container1.draw();
	object_container2.draw();
	object_container3.draw();
	object_container4.draw();
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_objects(const VIDEO::HUD &hud) const
{
	object_container1.draw(hud);
	object_container2.draw(hud);
	object_container3.draw(hud);
	object_container4.draw(hud);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_primary   (const VECTOR &pos,double radius,double alpha) const
{
	map->draw_primary(pos,radius,alpha);

	if (primary_layer_object.getnumbeams())
	{
		primary_layer_object.draw_beams();
	}

	if (primary_layer_object.hasparticle_emitter())
	{
		primary_layer_object.draw_particles();
	}
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_primary   (const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const
{
	map->draw_primary(pos,radius,r,g,b,a,hud);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_secondary (const VECTOR &pos,double radius,double alpha) const
{
	map->draw_secondary(pos,radius,alpha);

	if (secondary_layer_object.getnumbeams())
	{
		secondary_layer_object.draw_beams();
	}

	if (secondary_layer_object.hasparticle_emitter())
	{
		secondary_layer_object.draw_particles();
	}
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_secondary (const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const
{
	map->draw_secondary(pos,radius,r,g,b,a,hud);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_tertiary  (const VECTOR &pos,double radius,double alpha) const
{
	map->draw_tertiary(pos,radius,alpha);

	if (tertiary_layer_object.getnumbeams())
	{
		tertiary_layer_object.draw_beams();
	}

	if (tertiary_layer_object.hasparticle_emitter())
	{
		tertiary_layer_object.draw_particles();
	}
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_tertiary  (const VECTOR &pos,double radius,double r,double g,double b,double a,const VIDEO::HUD &hud) const
{
	map->draw_tertiary(pos,radius,r,g,b,a,hud);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_foreground(const VECTOR &pos,double radius,double alpha) const
{
	map->draw_foreground(pos,radius,alpha);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_background(const VECTOR &pos,double radius,double alpha) const
{
	map->draw_background(pos,radius,alpha);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::draw_skyplane  (const VECTOR &pos,double radius) const
{
	map->draw_skyplane(pos,radius);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::sound(const VECTOR &pos,double radius) const
{
	object_container1.sound();
	object_container2.sound();
	object_container3.sound();
	object_container4.sound();

	map->sound(pos,radius);
}

template<class O1,class O2,class O3,class O4>
template<class O>
WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::OBJECT_CONTAINER()
{
	numobjects     =0;
	numbeam_objects=0;

	objects     =new O*             [max_objects_per_world];
	beam_objects=new BEAM_OBJECT<O>*[max_objects_per_world];
}

template<class O1,class O2,class O3,class O4>
template<class O>
WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::~OBJECT_CONTAINER()
{
	delete[] objects;
	delete[] beam_objects;
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::change_origin(const VECTOR &new_origin)
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			objects[i]->change_origin(new_origin);
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::putobject(O &object)
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]==&object)
		{
			return;
		}
	}

	objects[numobjects]=&object;

	numobjects++;
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::putbeam_object(BEAM_OBJECT<O> &beam_object)
{
	for (int i=0;i<numbeam_objects;i++)
	{
		if (beam_objects[i]==&beam_object)
		{
			return;
		}
	}

	beam_objects[numbeam_objects]=&beam_object;

	numbeam_objects++;
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::removeobject(const O &object)
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]==&object)
		{
			for (int j=i;j<numobjects;j++)
			{
				if (j+1<max_objects_per_world)
				{
					objects[j]=objects[j+1];
				}
			}

			numobjects--;

			return;
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::removebeam_object(const BEAM_OBJECT<O> &beam_object)
{
	for (int i=0;i<numbeam_objects;i++)
	{
		if (beam_objects[i]==&beam_object)
		{
			for (int j=i;j<numbeam_objects;j++)
			{
				if (j+1<max_objects_per_world)
				{
					beam_objects[j]=beam_objects[j+1];
				}
			}

			numbeam_objects--;

			return;
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::clearforces()
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			objects[i]->clearforces();
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::control(double timestep)
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			objects[i]->control(timestep);
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::addheat(double ht,const VECTOR &pos,void *object)
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]==object)
		{
			if (objects[i]->wasinit() && !objects[i]->isgone())
			{
				objects[i]->addheat(ht,pos);
			}

			return;
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::addfictitious_forces(const VECTOR &axle_pos,double angular_vel,const VECTOR &linear_acc,double angular_acc)
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			objects[i]->addfictitious_forces(axle_pos,angular_vel,linear_acc,angular_acc);
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::fillclist(const VECTOR &pos,double radius,void *object)
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]!=object)
		{
			if (objects[i]->wasinit() && !objects[i]->isgone())
			{
				objects[i]->fillclist(world->clist,pos,radius);
			}
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::setclist(double timestep)
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			world->fillclist(objects[i]->getposition(),objects[i]->getbounding_radius(),(void*)objects[i],timestep);

			objects[i]->setclist(world->clist);
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::update_beam(double timestep)
{
	for (int i=0;i<numbeam_objects;i++)
	{
		for (int j=0;j<beam_objects[i]->getnumbeams();j++)
		{
			BEAM<O>& beam=beam_objects[i]->getbeam(j);

			if (beam.wasinit() && !beam.isgone() && beam.isactive())
			{
				world->update_beam(beam,timestep);
			}
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::integrate(double timestep)
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			objects[i]->integrate(timestep);
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::light() const
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			objects[i]->light();
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::draw() const
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			objects[i]->draw();
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::draw(const VIDEO::HUD &hud) const
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			objects[i]->draw(hud);
		}
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::OBJECT_CONTAINER<O>::sound() const
{
	for (int i=0;i<numobjects;i++)
	{
		if (objects[i]->wasinit() && !objects[i]->isgone())
		{
			objects[i]->sound();
		}
	}
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::addheat(double ht,const VECTOR &pos,void *object)
{
	if (&(map->getprimary_layer())==object)
	{
		if (primary_layer_object.hasparticle_emitter())
		{
			primary_layer_object.getparticle_emitter().addheat(ht,pos);
		}

		return;
	}

	if (&(map->getsecondary_layer())==object)
	{
		if (secondary_layer_object.hasparticle_emitter())
		{
			secondary_layer_object.getparticle_emitter().addheat(ht,pos);
		}

		return;
	}

	if (&(map->gettertiary_layer())==object)
	{
		if (tertiary_layer_object.hasparticle_emitter())
		{
			tertiary_layer_object.getparticle_emitter().addheat(ht,pos);
		}

		return;
	}

	object_container1.addheat(ht,pos,object);
	object_container2.addheat(ht,pos,object);
	object_container3.addheat(ht,pos,object);
	object_container4.addheat(ht,pos,object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::fillclist(const VECTOR &pos,double radius,void *object,double timestep)
{
	clist.numshapes=0;

	map->fillclist_primary  (clist,pos,radius,timestep);
	map->fillclist_secondary(clist,pos,radius,timestep);
	map->fillclist_tertiary (clist,pos,radius,timestep);

	object_container1.fillclist(pos,radius,object);
	object_container2.fillclist(pos,radius,object);
	object_container3.fillclist(pos,radius,object);
	object_container4.fillclist(pos,radius,object);
}

template<class O1,class O2,class O3,class O4>
void WORLD<O1,O2,O3,O4>::getgridcoords(int &x,int &y,const VECTOR &pos,double gridsize) const
{
	if (pos.x>=0)
	{
		x=(int)(pos.x/gridsize);
	}
	else
	{
		x=(int)(pos.x/gridsize)-1;
	}

	if (pos.y>=0)
	{
		y=(int)(pos.y/gridsize);
	}
	else
	{
		y=(int)(pos.y/gridsize)-1;
	}
}

template<class O1,class O2,class O3,class O4>
template<class O>
void WORLD<O1,O2,O3,O4>::update_beam(BEAM<O> &beam,double timestep)
{
	double gridsize=beam.getgridsize();

	VECTOR grid_pos;
	double grid_radius=squareroot(square(gridsize)*2)*1.5;

	bool   hit;
	VECTOR isect;
	bool   needsaddheat;
	double heat;
	void*  object;

	BEAM<O> *beam_next;

	LINE line=beam.getline();

	int x1,y1;
	getgridcoords(x1,y1,line.start,gridsize);

	int x2,y2;
	getgridcoords(x2,y2,line.end,gridsize);

	int deltax=x2-x1;
	int deltay=y2-y1;

	int x_inc;
	int y_inc;

	if (deltax>=0)
	{
		x_inc=1;
	}
	else
	{
		x_inc=-1;

		deltax=-deltax;
	}

	if (deltay>=0)
	{
		y_inc=1;
	}
	else
	{
		y_inc=-1;

		deltay=-deltay;
	}

	int deltax2=deltax*2;
	int deltay2=deltay*2;

	int error;

	if (deltax>=deltay)
	{
		error=deltax;

		for (int i=0;i<=deltax;i++)
		{
			grid_pos.x=(x1*gridsize)+(gridsize/2);
			grid_pos.y=(y1*gridsize)+(gridsize/2);

			fillclist(grid_pos,grid_radius/collision_bounding_radius_mul,(void*)beam.getobject(),timestep);

			beam.setclist(clist);

			beam_next=beam.getnextbeam(hit,isect,needsaddheat,heat,&object,i==0,timestep);

			if (hit)
			{
				if (!beam_next)
				{
					return;
				}

				if (isect.x>=(grid_pos.x-(gridsize*1.5)) && isect.x<=(grid_pos.x+(gridsize*1.5)) && isect.y>=(grid_pos.y-(gridsize*1.5)) && isect.y<=(grid_pos.y+(gridsize*1.5)))
				{
					if (needsaddheat)
					{
						addheat(heat,isect,object);
					}

					update_beam(*beam_next,timestep);

					return;
				}
			}

			x1+=x_inc;

			error+=deltay2;

			if (error>=deltax2)
			{
				y1+=y_inc;

				error-=deltax2;
			}
		}
	}
	else
	{
		error=deltay;

		for (int i=0;i<=deltay;i++)
		{
			grid_pos.x=(x1*gridsize)+(gridsize/2);
			grid_pos.y=(y1*gridsize)+(gridsize/2);

			fillclist(grid_pos,grid_radius/collision_bounding_radius_mul,(void*)beam.getobject(),timestep);

			beam.setclist(clist);

			beam_next=beam.getnextbeam(hit,isect,needsaddheat,heat,&object,i==0,timestep);

			if (hit)
			{
				if (!beam_next)
				{
					return;
				}

				if (isect.x>=(grid_pos.x-(gridsize*1.5)) && isect.x<=(grid_pos.x+(gridsize*1.5)) && isect.y>=(grid_pos.y-(gridsize*1.5)) && isect.y<=(grid_pos.y+(gridsize*1.5)))
				{
					if (needsaddheat)
					{
						addheat(heat,isect,object);
					}

					update_beam(*beam_next,timestep);

					return;
				}
			}

			y1+=y_inc;

			error+=deltax2;

			if (error>=deltay2)
			{
				x1+=x_inc;

				error-=deltay2;
			}
		}
	}
}
