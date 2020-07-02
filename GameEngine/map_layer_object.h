//Code by Rogier Walraven. Do not distribute.

struct MAP_LAYER_OBJECT
{
	MAP_LAYER_OBJECT(const MAP::LAYER &layer,const MAP::LAYER &primary_layer);
	~MAP_LAYER_OBJECT();

	void change_layer(const MAP::LAYER &new_layer,const MAP::LAYER &new_primary_layer);

	bool wasinit() const;
	bool isgone() const;
	bool hasvapourized() const;

	VECTOR getposition() const;
	double getorientation() const;

	VECTOR getlinear_velocity() const;
	double getangular_velocity() const;

	VECTOR getlinear_velocity (double timestep) const;
	double getangular_velocity(double timestep) const;

	void addforce(const VECTOR &frc,const VECTOR &pos) const {}

	bool hasparticle_emitter() const;

	PARTICLE_EMITTER<MAP_LAYER_OBJECT>& getparticle_emitter();

	int getnumbeams() const;

	BEAM<MAP_LAYER_OBJECT>& getbeam(int i);

	void light_particles() const;
	void light_beams() const;

	void draw_particles() const;
	void draw_beams() const;

  private:

	const MAP::LAYER *layer;
	const MAP::LAYER *primary_layer;

	PARTICLE_EMITTER<MAP_LAYER_OBJECT> *particle_emitter;

	int numbeams;

	BEAM<MAP_LAYER_OBJECT> **beams;

	MAP_LAYER_OBJECT(const MAP_LAYER_OBJECT &other) {}
	MAP_LAYER_OBJECT& operator=(const MAP_LAYER_OBJECT &other) {}
};

inline MAP_LAYER_OBJECT::MAP_LAYER_OBJECT(const MAP::LAYER &layer,const MAP::LAYER &primary_layer)
{
	this->layer        =&layer;
	this->primary_layer=&primary_layer;

	if (layer.getproperties().hasparticle_emitter)
	{
		particle_emitter=new PARTICLE_EMITTER<MAP_LAYER_OBJECT>(layer.getproperties().particle_emitter.max_particles);

		if (this->layer!=this->primary_layer)
		{
			particle_emitter->init_reset(layer.getproperties().particle_emitter.properties,*this);
		}
		else
		{
			particle_emitter->init_reset(layer.getproperties().particle_emitter.properties);
		}
	}
	else
	{
		particle_emitter=0;
	}

	numbeams=layer.getproperties().numbeams;

	if (numbeams)
	{
		beams=new BEAM<MAP_LAYER_OBJECT>*[numbeams];

		for (int i=0;i<numbeams;i++)
		{
			beams[i]=new BEAM<MAP_LAYER_OBJECT>(layer.getproperties().beams[i].max_divisions);

			if (this->layer!=this->primary_layer)
			{
				beams[i]->init_reset(layer.getproperties().beams[i].properties,layer.getproperties().beams[i].position,layer.getproperties().beams[i].orientation,*this);
			}
			else
			{
				beams[i]->init_reset(layer.getproperties().beams[i].properties,layer.getproperties().beams[i].position,layer.getproperties().beams[i].orientation);
			}
		}
	}
	else
	{
		beams=0;
	}
}

inline MAP_LAYER_OBJECT::~MAP_LAYER_OBJECT()
{
	delete particle_emitter;

	for (int i=0;i<numbeams;i++)
	{
		delete beams[i];
	}

	delete[] beams;
}

inline void MAP_LAYER_OBJECT::change_layer(const MAP::LAYER &new_layer,const MAP::LAYER &new_primary_layer)
{
	layer        =&new_layer;
	primary_layer=&new_primary_layer;
}

inline bool MAP_LAYER_OBJECT::wasinit() const
{
	return(true);
}

inline bool MAP_LAYER_OBJECT::isgone() const
{
	return(false);
}

inline bool MAP_LAYER_OBJECT::hasvapourized() const
{
	return(false);
}

inline VECTOR MAP_LAYER_OBJECT::getposition() const
{
	if (layer!=primary_layer)
	{
		VECTOR center  =layer->getcenter();
		VECTOR position=layer->getposition() + center;

		position=position.gettransformed_to  (center+primary_layer->getposition(),primary_layer->getorientation());
		position=position.gettransformed_from(center,0);

		return(position);
	}

	return(VECTOR(0,0));
}

inline double MAP_LAYER_OBJECT::getorientation() const
{
	if (layer!=primary_layer)
	{
		return(layer->getorientation() - primary_layer->getorientation());
	}

	return(0);
}

inline VECTOR MAP_LAYER_OBJECT::getlinear_velocity() const
{
	if (layer!=primary_layer)
	{
		return(layer->getlinear_velocity() - primary_layer->getlinear_velocity());
	}

	return(VECTOR(0,0));
}

inline double MAP_LAYER_OBJECT::getangular_velocity() const
{
	if (layer!=primary_layer)
	{
		return(layer->getangular_velocity() - primary_layer->getangular_velocity());
	}

	return(0);
}

inline VECTOR MAP_LAYER_OBJECT::getlinear_velocity (double timestep) const
{
	if (layer!=primary_layer)
	{
		return(layer->getlinear_velocity(layer->getlinear_motion_time(),timestep) - primary_layer->getlinear_velocity(primary_layer->getlinear_motion_time(),timestep));
	}

	return(VECTOR(0,0));
}

inline double MAP_LAYER_OBJECT::getangular_velocity(double timestep) const
{
	if (layer!=primary_layer)
	{
		return(layer->getangular_velocity(layer->getangular_motion_time(),timestep) - primary_layer->getangular_velocity(primary_layer->getangular_motion_time(),timestep));
	}

	return(0);
}

inline bool MAP_LAYER_OBJECT::hasparticle_emitter() const
{
	return(particle_emitter!=0);
}

inline PARTICLE_EMITTER<MAP_LAYER_OBJECT>& MAP_LAYER_OBJECT::getparticle_emitter()
{
	return(*particle_emitter);
}

inline int MAP_LAYER_OBJECT::getnumbeams() const
{
	return(numbeams);
}

inline BEAM<MAP_LAYER_OBJECT>& MAP_LAYER_OBJECT::getbeam(int i)
{
	return(*(beams[i]));
}

inline void MAP_LAYER_OBJECT::light_particles() const
{
	if (layer->getproperties().particle_emitter.lit)
	{
		particle_emitter->light(layer->getproperties().particle_emitter.color1_light[0],
                                layer->getproperties().particle_emitter.color1_light[1],
                                layer->getproperties().particle_emitter.color1_light[2],
                                layer->getproperties().particle_emitter.color2_light[0],
                                layer->getproperties().particle_emitter.color2_light[1],
                                layer->getproperties().particle_emitter.color2_light[2],
                                layer->getproperties().particle_emitter.alpha_light,
                                layer->getproperties().particle_emitter.size_light);
	}
}

inline void MAP_LAYER_OBJECT::light_beams() const
{
	for (int i=0;i<numbeams;i++)
	{
		if (beams[i]->isactive())
		{
			beams[i]->light(layer->getproperties().beams[i].color_light[0],
                            layer->getproperties().beams[i].color_light[1],
                            layer->getproperties().beams[i].color_light[2],
                            layer->getproperties().beams[i].color_light[3],
                            layer->getproperties().beams[i].thickness_light);
		}
	}
}

inline void MAP_LAYER_OBJECT::draw_particles() const
{
	particle_emitter->draw(layer->getproperties().particle_emitter.color1[0],
                           layer->getproperties().particle_emitter.color1[1],
                           layer->getproperties().particle_emitter.color1[2],
                           layer->getproperties().particle_emitter.color2[0],
                           layer->getproperties().particle_emitter.color2[1],
                           layer->getproperties().particle_emitter.color2[2],
                           layer->getproperties().particle_emitter.alpha,
                           layer->getproperties().particle_emitter.size);
}

inline void MAP_LAYER_OBJECT::draw_beams() const
{
	for (int i=0;i<numbeams;i++)
	{
		if (beams[i]->isactive())
		{
			beams[i]->draw(layer->getproperties().beams[i].color[0],
                           layer->getproperties().beams[i].color[1],
                           layer->getproperties().beams[i].color[2],
                           layer->getproperties().beams[i].color[3],
                           layer->getproperties().beams[i].thickness);
		}
	}
}
