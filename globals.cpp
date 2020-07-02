//Code by Rogier Walraven. Do not distribute.

#include "GameEngine\\GameEngine.h"

const int max_unique_textures=128;
const int max_unique_tilesets=128;

const int max_unique_samples=64;
const int max_audio_channels=64;

const double math_epsilon=(double)1/16777216;

namespace RANDOM
{
	const GENERATOR global(211,1977);
}

const double collision_bounding_radius_mul=1.5;

const int collision_max_triangles_per_shape=64;
const int collision_max_quads_per_shape    =64;
const int collision_max_circles_per_shape  =64;
const int collision_max_shapes_per_list    =128;

const int max_objects_per_world=64;

const double timestep_min              =(double)1/256;
const double timestep_max              =(double)1/2;
const double timestep_max_per_iteration=(double)1/16;
const double timestep_max_error        =128;
const double timestep_scale            =8;
