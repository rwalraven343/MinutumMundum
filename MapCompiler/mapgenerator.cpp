//Code by Rogier Walraven. Do not distribute.

#include "mapgenerator.h"

static const int precision=12;

static const int max_triangles_per_shape=64;
static const int max_quads_per_shape    =64;

static const int max_ambient_sounds=64;

static const int max_linear_motions_per_layer =64;
static const int max_angular_motions_per_layer=64;

static const int max_beams_per_layer=64;

static const int max_object_layers=64;

static const int appendnum_len=4;
static char appendnum[appendnum_len];

static void generate_motions_layer    (const char *name,FILE *outfile,TiXmlDocument &doc);
static void generate_beams_layer      (const char *name,FILE *outfile,TiXmlDocument &doc);
static void generate_pointlights_layer(const char *name,FILE *outfile,TiXmlDocument &doc);
static void generate_indices_layer    (const char *name,const char *ts_name,FILE *outfile,TiXmlDocument &doc);
static void generate_struct_layer     (const char *name,FILE *outfile,TiXmlDocument &doc);
static void generate_objects_layer    (const char *name,FILE *outfile,TiXmlDocument &doc);

static void generate_struct_layer_motioninfo(const char *name,FILE *outfile,TiXmlDocument &doc);

static int getnumtiles(const char *name,TiXmlDocument &doc);

void generate_shapes (FILE *outfile,TiXmlDocument &doc)
{
	fprintf(outfile,"	namespace SHAPES\n");
	fprintf(outfile,"	{\n");

	int numshapes=getnumtiles("shapes",doc);

	if (numshapes<1)
	{
		exit(1);
	}

	for (int i=0;i<numshapes;i++)
	{
		fprintf(outfile,"		namespace SHAPE%i\n",i+1);
		fprintf(outfile,"		{\n");

		const char *format_triangle="			static const TRIANGLE triangle%i(VECTOR(%.*f,%.*f),VECTOR(%.*f,%.*f),VECTOR(%.*f,%.*f),%.*f,%.*f,%.*f,%.*f);\n";
		const char *format_quad    ="			static const QUAD     quad%i    (VECTOR(%.*f,%.*f),VECTOR(%.*f,%.*f),VECTOR(%.*f,%.*f),VECTOR(%.*f,%.*f),%.*f,%.*f,%.*f,%.*f);\n";

		bool exists;

		double sfriction =gettileset_tile_property_double(exists,"shapes",i,"sfriction" ,doc); if (!exists) {exit(1);}
		double dfriction =gettileset_tile_property_double(exists,"shapes",i,"dfriction" ,doc); if (!exists) {exit(1);}
		double absorption=gettileset_tile_property_double(exists,"shapes",i,"absorption",doc); if (!exists) {exit(1);}
		double refraction=gettileset_tile_property_double(exists,"shapes",i,"refraction",doc); if (!exists) {exit(1);}

		int numtriangles=0;
		int numquads    =0;

		for (int j=0;j<max_triangles_per_shape;j++)
		{
			sprintf_s(appendnum,appendnum_len,"%i",j+1);

			const double *csv=gettileset_tile_property_csv(exists,"shapes",i,(TiXmlString("triangle")+appendnum).c_str(),doc);

			if (exists)
			{
				fprintf(outfile,format_triangle,j+1,precision,csv[0],
                                                    precision,csv[1],
                                                    precision,csv[2],
                                                    precision,csv[3],
                                                    precision,csv[4],
                                                    precision,csv[5],
                                                    precision,sfriction,
                                                    precision,dfriction,
                                                    precision,absorption,
                                                    precision,refraction);

				numtriangles++;

				delete[] csv;
			}
		}

		for (int j=0;j<max_quads_per_shape;j++)
		{
			sprintf_s(appendnum,appendnum_len,"%i",j+1);

			const double *csv=gettileset_tile_property_csv(exists,"shapes",i,(TiXmlString("quad")+appendnum).c_str(),doc);

			if (exists)
			{
				fprintf(outfile,format_quad,j+1,precision,csv[0],
                                                precision,csv[1],
                                                precision,csv[2],
                                                precision,csv[3],
                                                precision,csv[4],
                                                precision,csv[5],
                                                precision,csv[6],
                                                precision,csv[7],
                                                precision,sfriction,
                                                precision,dfriction,
                                                precision,absorption,
                                                precision,refraction);

				numquads++;

				delete[] csv;
			}
		}

		if (numtriangles==0 && numquads==0)
		{
			exit(1);
		}

		fprintf(outfile,"\n");

		if (numtriangles!=0)
		{
			fprintf(outfile,"			static const TRIANGLE triangles[%i]={",numtriangles);

			for (int j=0;j<numtriangles;j++)
			{
				fprintf(outfile,"triangle%i",j+1);

				if (j!=numtriangles-1)
				{
					fprintf(outfile,",");
				}
			}

			fprintf(outfile,"};\n");
		}

		if (numquads!=0)
		{
			fprintf(outfile,"			static const QUAD     quads    [%i]={",numquads);

			for (int j=0;j<numquads;j++)
			{
				fprintf(outfile,"quad%i",j+1);

				if (j!=numquads-1)
				{
					fprintf(outfile,",");
				}
			}

			fprintf(outfile,"};\n");
		}

		fprintf(outfile,"		}\n");
		fprintf(outfile,"\n");

		fprintf(outfile,"		static const MAP_PROPERTIES::SHAPE shape%i={%i,%i,",i+1,numtriangles,numquads);

		if (numtriangles!=0)
		{
			fprintf(outfile,"SHAPE%i::triangles,",i+1);
		}
		else
		{
			fprintf(outfile,"0,");
		}

		if (numquads!=0)
		{
			fprintf(outfile,"SHAPE%i::quads};\n",i+1);
		}
		else
		{
			fprintf(outfile,"0};\n");
		}

		if (i!=numshapes-1)
		{
			fprintf(outfile,"\n");
		}
	}

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	static const int numshapes=%i;\n",numshapes);
	fprintf(outfile,"\n");

	fprintf(outfile,"	static const MAP_PROPERTIES::SHAPE shapes[%i]={",numshapes);

	for (int i=0;i<numshapes;i++)
	{
		fprintf(outfile,"SHAPES::shape%i",i+1);

		if (i!=numshapes-1)
		{
			fprintf(outfile,",");
		}
	}

	fprintf(outfile,"};\n");
}

void generate_sounds (FILE *outfile,TiXmlDocument &doc)
{
	fprintf(outfile,"	namespace AMBIENT_SOUNDS\n");
	fprintf(outfile,"	{\n");

	int numambient_sounds=0;

	for (int i=0;i<max_ambient_sounds;i++)
	{
		bool exists;

		sprintf_s(appendnum,appendnum_len,"%i",i+1);

		const double *csv=getmap_property_csv(exists,(TiXmlString("ambient_sound")+appendnum).c_str(),doc);

		if (exists)
		{
			const char *string=getmap_property_string(exists,(TiXmlString("ambient_sound")+appendnum+TiXmlString("_filename")).c_str(),doc);

			if (!exists)
			{
				exit(1);
			}

			fprintf(outfile,"		static const MAP_PROPERTIES::AMBIENT_SOUND ambient_sound%i={\"%s\",VECTOR(%.*f,%.*f),%.*f,%.*f,%.*f,%.*f,%.*f};\n",i+1,string,
                                                                                                                                                       precision,csv[0],
                                                                                                                                                       precision,csv[1],
                                                                                                                                                       precision,csv[2],
                                                                                                                                                       precision,csv[3],
                                                                                                                                                       precision,csv[4],
                                                                                                                                                       precision,csv[5],
                                                                                                                                                       precision,csv[6]);

			numambient_sounds++;

			delete[] csv;
		}
	}

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	static const int numambient_sounds=%i;\n",numambient_sounds);
	fprintf(outfile,"\n");

	if (numambient_sounds!=0)
	{
		fprintf(outfile,"	static const MAP_PROPERTIES::AMBIENT_SOUND ambient_sounds[%i]={",numambient_sounds);

		for (int i=0;i<numambient_sounds;i++)
		{
			fprintf(outfile,"AMBIENT_SOUNDS::ambient_sound%i",i+1);

			if (i!=numambient_sounds-1)
			{
				fprintf(outfile,",");
			}
		}

		fprintf(outfile,"};\n");
	}
	else
	{
		fprintf(outfile,"	static const MAP_PROPERTIES::AMBIENT_SOUND *ambient_sounds=0;\n");
	}
}

void generate_motions(FILE *outfile,TiXmlDocument &doc)
{
	fprintf(outfile,"	namespace PRIMARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_motions_layer("primary",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace SECONDARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_motions_layer("secondary",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace TERTIARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_motions_layer("tertiary",outfile,doc);

	fprintf(outfile,"	}\n");
}

void generate_beams  (FILE *outfile,TiXmlDocument &doc)
{
	fprintf(outfile,"	namespace PRIMARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_beams_layer("primary",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace SECONDARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_beams_layer("secondary",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace TERTIARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_beams_layer("tertiary",outfile,doc);

	fprintf(outfile,"	}\n");
}

void generate_pointlights(FILE *outfile,TiXmlDocument &doc)
{
	fprintf(outfile,"	namespace PRIMARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_pointlights_layer("primary",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace SECONDARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_pointlights_layer("secondary",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace TERTIARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_pointlights_layer("tertiary",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace FOREGROUND_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_pointlights_layer("foreground",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace BACKGROUND_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_pointlights_layer("background",outfile,doc);

	fprintf(outfile,"	}\n");
}

void generate_indices(FILE *outfile,TiXmlDocument &doc)
{
	fprintf(outfile,"	namespace PRIMARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_indices_layer("primary","textures",outfile,doc);
	fprintf(outfile,"\n");
	generate_indices_layer("primary","shapes",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace SECONDARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_indices_layer("secondary","textures",outfile,doc);
	fprintf(outfile,"\n");
	generate_indices_layer("secondary","shapes",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace TERTIARY_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_indices_layer("tertiary","textures",outfile,doc);
	fprintf(outfile,"\n");
	generate_indices_layer("tertiary","shapes",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace FOREGROUND_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_indices_layer("foreground","textures",outfile,doc);

	fprintf(outfile,"	}\n");
	fprintf(outfile,"\n");

	fprintf(outfile,"	namespace BACKGROUND_LAYER\n");
	fprintf(outfile,"	{\n");

	generate_indices_layer("background","textures",outfile,doc);

	fprintf(outfile,"	}\n");
}

void generate_struct (FILE *outfile,TiXmlDocument &doc)
{
	bool exists;

	bool          bvalue;
	int           ivalue;
	double        dvalue;
	const double *csv;
	const char   *string;

	const char *identifier=getmap_property_string(exists,"identifier",doc); if (!exists) {exit(1);}

	fprintf(outfile,"static const MAP_PROPERTIES %s_map_properties=\n",identifier);
	fprintf(outfile,"{\n");

	dvalue=getmap_property_double(exists,"meter_scale"        ,doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);
	dvalue=getmap_property_double(exists,"second_scale"       ,doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);
	dvalue=getmap_property_double(exists,"kilogram_scale"     ,doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);
	dvalue=getmap_property_double(exists,"joules_per_kelvin"  ,doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);
	dvalue=getmap_property_double(exists,"temperature"        ,doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);
	dvalue=getmap_property_double(exists,"audio_spatial_scale",doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);

	string=getmap_property_string(exists,"tileset_filename" ,doc); if (!exists) {exit(1);}
	fprintf(outfile,"	\"%s\",\n",string);
	string=getmap_property_string(exists,"skyplane_filename",doc); if (!exists) {exit(1);}
	fprintf(outfile,"	\"%s\",\n",string);

	fprintf(outfile,"	%i,\n",gettileset_image_attribute_int("textures","width" ,doc));
	fprintf(outfile,"	%i,\n",gettileset_image_attribute_int("textures","height",doc));

	fprintf(outfile,"	%i,\n",getmap_attribute_int("tilewidth" ,doc));
	fprintf(outfile,"	%i,\n",getmap_attribute_int("tileheight",doc));

	ivalue=getmap_property_int(exists,"transparentx",doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%i,\n",ivalue);
	ivalue=getmap_property_int(exists,"transparenty",doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%i,\n",ivalue);

	fprintf(outfile,"	%i,\n",getnumtiles("textures",doc));

	fprintf(outfile,"	%s_MAP::numshapes,\n",TiXmlString(identifier).to_uppercase().c_str());
	fprintf(outfile,"	%s_MAP::shapes,\n"   ,TiXmlString(identifier).to_uppercase().c_str());

	fprintf(outfile,"	%s_MAP::numambient_sounds,\n",TiXmlString(identifier).to_uppercase().c_str());
	fprintf(outfile,"	%s_MAP::ambient_sounds,\n"   ,TiXmlString(identifier).to_uppercase().c_str());

	dvalue=getmap_property_double(exists,"tilewidth_scale" ,doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);
	dvalue=getmap_property_double(exists,"tileheight_scale",doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);

	fprintf(outfile,"	%i,\n",getmap_attribute_int("width" ,doc));
	fprintf(outfile,"	%i,\n",getmap_attribute_int("height",doc));

	bvalue=getmap_property_bool(exists,"local_gravity",doc); if (!exists) {exit(1);}

	if (bvalue)
	{
		fprintf(outfile,"	true,\n");
	}
	else
	{
		fprintf(outfile,"	false,\n");
	}

	csv=getmap_property_csv(exists,"gravity",doc); if (!exists) {exit(1);}
	fprintf(outfile,"	VECTOR(%.*f,%.*f),\n",precision,csv[0],precision,csv[1]);
	delete[] csv;

	dvalue=getmap_property_double(exists,"friction"  ,doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);
	dvalue=getmap_property_double(exists,"refraction",doc); if (!exists) {exit(1);}
	fprintf(outfile,"	%.*f,\n",precision,dvalue);

	fprintf(outfile,"	{\n");
	generate_struct_layer("primary",outfile,doc);
	fprintf(outfile,"	},\n");

	fprintf(outfile,"	{\n");
	generate_struct_layer("secondary",outfile,doc);
	fprintf(outfile,"	},\n");

	fprintf(outfile,"	{\n");
	generate_struct_layer("tertiary",outfile,doc);
	fprintf(outfile,"	},\n");

	fprintf(outfile,"	{\n");
	generate_struct_layer("foreground",outfile,doc);
	fprintf(outfile,"	},\n");

	fprintf(outfile,"	{\n");
	generate_struct_layer("background",outfile,doc);
	fprintf(outfile,"	}\n");

	fprintf(outfile,"};\n");
}

void generate_objects    (FILE *outfile,TiXmlDocument &doc)
{
	bool exists;

	const char *identifier=getmap_property_string(exists,"identifier",doc); if (!exists) {exit(1);}

	for (int i=0;i<max_object_layers;i++)
	{
		sprintf_s(appendnum,appendnum_len,"%i",i+1);

		if (getlayer_exists((TiXmlString("objects")+appendnum).c_str(),doc))
		{
			fprintf(outfile,"\n");
			fprintf(outfile,"namespace %s_MAP\n",TiXmlString(identifier).to_uppercase().c_str());
			fprintf(outfile,"{\n");
			fprintf(outfile,"	namespace %s\n",(TiXmlString("objects")+appendnum).to_uppercase().c_str());
			fprintf(outfile,"	{\n");
			generate_objects_layer((TiXmlString("objects")+appendnum).c_str(),outfile,doc);
			fprintf(outfile,"	}\n");
			fprintf(outfile,"}\n");
		}
	}
}

static void generate_motions_layer(const char *name,FILE *outfile,TiXmlDocument &doc)
{
	const char *format_linear_motion         ="		static const MAP_PROPERTIES::LINEAR_MOTION linear_motion%i={VECTOR(%.*f,%.*f),%.*f,%.*f,true,\"%s\",\"%s\",%.*f,%.*f,%.*f,%.*f};\n";
	const char *format_linear_motion_nosound ="		static const MAP_PROPERTIES::LINEAR_MOTION linear_motion%i={VECTOR(%.*f,%.*f),%.*f,%.*f,false,0,0,1,1,1,1};\n";

	const char *format_angular_motion        ="		static const MAP_PROPERTIES::ANGULAR_MOTION angular_motion%i={%.*f,%.*f,%.*f,true,\"%s\",\"%s\",%.*f,%.*f,%.*f,%.*f};\n";
	const char *format_angular_motion_nosound="		static const MAP_PROPERTIES::ANGULAR_MOTION angular_motion%i={%.*f,%.*f,%.*f,false,0,0,1,1,1,1};\n";

	int numlinear_motions=0;

	for (int i=0;i<max_linear_motions_per_layer;i++)
	{
		bool exists;

		sprintf_s(appendnum,appendnum_len,"%i",i+1);

		const double *csv=getlayer_property_csv(exists,name,(TiXmlString("linear_motion")+appendnum).c_str(),doc);

		if (exists)
		{
			bool exists_vel;
			bool exists_acc;

			const char *string_vel=getlayer_property_string(exists_vel,name,(TiXmlString("linear_motion")+appendnum+TiXmlString("_filename_vel")).c_str(),doc);
			const char *string_acc=getlayer_property_string(exists_acc,name,(TiXmlString("linear_motion")+appendnum+TiXmlString("_filename_acc")).c_str(),doc);

			if (exists_vel && exists_acc)
			{
				fprintf(outfile,format_linear_motion,i+1,precision,csv[0],
                                                         precision,csv[1],
                                                         precision,csv[2],
                                                         precision,csv[3],
                                                         string_vel,
                                                         string_acc,
                                                         precision,csv[4],
                                                         precision,csv[5],
                                                         precision,csv[6],
                                                         precision,csv[7]);
			}
			else
			{
				fprintf(outfile,format_linear_motion_nosound,i+1,precision,csv[0],
                                                                 precision,csv[1],
                                                                 precision,csv[2],
                                                                 precision,csv[3]);
			}

			numlinear_motions++;

			delete[] csv;
		}
	}

	int numangular_motions=0;

	for (int i=0;i<max_angular_motions_per_layer;i++)
	{
		bool exists;

		sprintf_s(appendnum,appendnum_len,"%i",i+1);

		const double *csv=getlayer_property_csv(exists,name,(TiXmlString("angular_motion")+appendnum).c_str(),doc);

		if (exists)
		{
			bool exists_vel;
			bool exists_acc;

			const char *string_vel=getlayer_property_string(exists_vel,name,(TiXmlString("angular_motion")+appendnum+TiXmlString("_filename_vel")).c_str(),doc);
			const char *string_acc=getlayer_property_string(exists_acc,name,(TiXmlString("angular_motion")+appendnum+TiXmlString("_filename_acc")).c_str(),doc);

			if (exists_vel && exists_acc)
			{
				fprintf(outfile,format_angular_motion,i+1,precision,csv[0],
                                                          precision,csv[1],
                                                          precision,csv[2],
                                                          string_vel,
                                                          string_acc,
                                                          precision,csv[3],
                                                          precision,csv[4],
                                                          precision,csv[5],
                                                          precision,csv[6]);
			}
			else
			{
				fprintf(outfile,format_angular_motion_nosound,i+1,precision,csv[0],
                                                                  precision,csv[1],
                                                                  precision,csv[2]);
			}

			numangular_motions++;

			delete[] csv;
		}
	}

	if (numlinear_motions!=0 || numangular_motions!=0)
	{
		fprintf(outfile,"\n");
	}

	fprintf(outfile,"		static const int numlinear_motions =%i;\n",numlinear_motions);
	fprintf(outfile,"		static const int numangular_motions=%i;\n",numangular_motions);

	fprintf(outfile,"\n");

	if (numlinear_motions!=0)
	{
		fprintf(outfile,"		static const MAP_PROPERTIES::LINEAR_MOTION linear_motions[%i]={",numlinear_motions);

		for (int i=0;i<numlinear_motions;i++)
		{
			fprintf(outfile,"linear_motion%i",i+1);

			if (i!=numlinear_motions-1)
			{
				fprintf(outfile,",");
			}
		}

		fprintf(outfile,"};\n");
	}
	else
	{
		fprintf(outfile,"		static const MAP_PROPERTIES::LINEAR_MOTION *linear_motions=0;\n");
	}

	if (numangular_motions!=0)
	{
		fprintf(outfile,"		static const MAP_PROPERTIES::ANGULAR_MOTION angular_motions[%i]={",numangular_motions);

		for (int i=0;i<numangular_motions;i++)
		{
			fprintf(outfile,"angular_motion%i",i+1);

			if (i!=numangular_motions-1)
			{
				fprintf(outfile,",");
			}
		}

		fprintf(outfile,"};\n");
	}
	else
	{
		fprintf(outfile,"		static const MAP_PROPERTIES::ANGULAR_MOTION *angular_motions=0;\n");
	}
}

static void generate_beams_layer  (const char *name,FILE *outfile,TiXmlDocument &doc)
{
	int numbeams=0;

	for (int i=0;i<max_beams_per_layer;i++)
	{
		bool exists;

		sprintf_s(appendnum,appendnum_len,"%i",i+1);

		const double *csv=getlayer_property_csv(exists,name,(TiXmlString("beam")+appendnum).c_str(),doc);

		if (exists)
		{
			fprintf(outfile,"		static const MAP_PROPERTIES::BEAM beam%i={%i,{%.*f,%.*f,%.*f},VECTOR(%.*f,%.*f),%.*f,{%.*f,%.*f,%.*f,%.*f},%.*f,{%.*f,%.*f,%.*f,%.*f},%.*f};\n",i+1,(int)csv[0],
                                                                                                                                                                                        precision,csv[1],
                                                                                                                                                                                        precision,csv[2],
                                                                                                                                                                                        precision,csv[3],
                                                                                                                                                                                        precision,csv[4],
                                                                                                                                                                                        precision,csv[5],
                                                                                                                                                                                        precision,csv[6],
                                                                                                                                                                                        precision,csv[7],
                                                                                                                                                                                        precision,csv[8],
                                                                                                                                                                                        precision,csv[9],
                                                                                                                                                                                        precision,csv[10],
                                                                                                                                                                                        precision,csv[11],
                                                                                                                                                                                        precision,csv[12],
                                                                                                                                                                                        precision,csv[13],
                                                                                                                                                                                        precision,csv[14],
                                                                                                                                                                                        precision,csv[15],
                                                                                                                                                                                        precision,csv[16]);

			numbeams++;

			delete[] csv;
		}
	}

	if (numbeams!=0)
	{
		fprintf(outfile,"\n");
	}

	fprintf(outfile,"		static const int numbeams=%i;\n",numbeams);

	fprintf(outfile,"\n");

	if (numbeams!=0)
	{
		fprintf(outfile,"		static const MAP_PROPERTIES::BEAM beams[%i]={",numbeams);

		for (int i=0;i<numbeams;i++)
		{
			fprintf(outfile,"beam%i",i+1);

			if (i!=numbeams-1)
			{
				fprintf(outfile,",");
			}
		}

		fprintf(outfile,"};\n");
	}
	else
	{
		fprintf(outfile,"		static const MAP_PROPERTIES::BEAM *beams=0;\n");
	}
}

static void generate_pointlights_layer(const char *name,FILE *outfile,TiXmlDocument &doc)
{
	bool exists;

	int mapwidth =getmap_attribute_int("width",doc);
	int mapheight=getmap_attribute_int("height",doc);

	int numtiles=mapwidth*mapheight;
	int firstgid=gettileset_attribute_int("pointlights","firstgid",doc);

	int numpointlights=0;

	const double *csv=getlayer_data_csv((TiXmlString(name)+"_pointlights").c_str(),doc);

	for (int i=0;i<numtiles;i++)
	{
		if ((int)csv[i]>0)
		{
			int index=(int)csv[i]-firstgid;

			int radius=(int)gettileset_tile_property_double(exists,"pointlights",index,"radius",doc); if (!exists) {exit(1);}

			const double *color=gettileset_tile_property_csv(exists,"pointlights",index,"color",doc); if (!exists) {exit(1);}

			fprintf(outfile,"		static const MAP_PROPERTIES::POINTLIGHT pointlight%i={%i,%i,%i,{%.*f,%.*f,%.*f,%.*f}};\n",numpointlights+1,i%mapwidth,i/mapwidth,radius,
                                                                                                                              precision,color[0],
                                                                                                                              precision,color[1],
                                                                                                                              precision,color[2],
                                                                                                                              precision,color[3]);

			numpointlights++;

			delete[] color;
		}
	}

	delete[] csv;

	if (numpointlights!=0)
	{
		fprintf(outfile,"\n");
	}

	fprintf(outfile,"		static const int numpointlights=%i;\n",numpointlights);

	fprintf(outfile,"\n");

	if (numpointlights!=0)
	{
		fprintf(outfile,"		static const MAP_PROPERTIES::POINTLIGHT pointlights[%i]={",numpointlights);

		for (int i=0;i<numpointlights;i++)
		{
			fprintf(outfile,"pointlight%i",i+1);

			if (i!=numpointlights-1)
			{
				fprintf(outfile,",");
			}
		}

		fprintf(outfile,"};\n");
	}
	else
	{
		fprintf(outfile,"		static const MAP_PROPERTIES::POINTLIGHT *pointlights=0;\n");
	}
}

static void generate_indices_layer(const char *name,const char *ts_name,FILE *outfile,TiXmlDocument &doc)
{
	int mapwidth =getmap_attribute_int("width",doc);
	int mapheight=getmap_attribute_int("height",doc);

	int numtiles=mapwidth*mapheight;
	int firstgid=gettileset_attribute_int(ts_name,"firstgid",doc);

	const double *csv;

	if (TiXmlString(ts_name)=="textures")
	{
		fprintf(outfile,"		static const int texture_index[%i]=\n",numtiles);
		fprintf(outfile,"		{");

		csv=getlayer_data_csv(name,doc);
	}
	else if (TiXmlString(ts_name)=="shapes")
	{
		fprintf(outfile,"		static const int shape_index[%i]=\n",numtiles);
		fprintf(outfile,"		{");

		csv=getlayer_data_csv((TiXmlString(name)+"_shapes").c_str(),doc);
	}
	else
	{
		exit(1);
	}

	int index;

	for (int i=0;i<numtiles;i++)
	{
		if ((int)csv[i]>0)
		{
			index=(int)csv[i]-firstgid;
		}
		else
		{
			index=-1;
		}

		if ((i%mapwidth)==0)
		{
			fprintf(outfile,"\n			%i",index);
		}
		else
		{
			fprintf(outfile,"%i",index);
		}

		if (i!=numtiles-1)
		{
			fprintf(outfile,",");
		}
	}

	fprintf(outfile,"\n		};\n");

	delete[] csv;
}

static void generate_struct_layer (const char *name,FILE *outfile,TiXmlDocument &doc)
{
	bool exists;

	bool          bvalue;
	double        dvalue;
	const double *csv;

	const char *identifier=getmap_property_string(exists,"identifier",doc); if (!exists) {exit(1);}

	if (TiXmlString(name)!="foreground" && TiXmlString(name)!="background")
	{
		fprintf(outfile,"		%s_MAP::%s_LAYER::texture_index,\n",TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
		fprintf(outfile,"		%s_MAP::%s_LAYER::shape_index,\n"  ,TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());

		bvalue=getlayer_property_bool(exists,name,"wrap",doc); if (!exists) {exit(1);}

		if (bvalue)
		{
			fprintf(outfile,"		true,\n");
		}
		else
		{
			fprintf(outfile,"		false,\n");
		}

		csv=getlayer_property_csv(exists,name,"position",doc); if (!exists) {exit(1);}
		fprintf(outfile,"		VECTOR(%.*f,%.*f),\n",precision,csv[0],precision,csv[1]);
		delete[] csv;

		dvalue=getlayer_property_double(exists,name,"orientation",doc); if (!exists) {exit(1);}
		fprintf(outfile,"		%.*f,\n",precision,dvalue);

		generate_struct_layer_motioninfo(name,outfile,doc);

		fprintf(outfile,"		%s_MAP::%s_LAYER::numlinear_motions,\n" ,TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
		fprintf(outfile,"		%s_MAP::%s_LAYER::numangular_motions,\n",TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
		fprintf(outfile,"		%s_MAP::%s_LAYER::linear_motions,\n"    ,TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
		fprintf(outfile,"		%s_MAP::%s_LAYER::angular_motions,\n"   ,TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());

		csv=getlayer_property_csv(exists,name,"particle_emitter",doc);

		if (exists)
		{
			fprintf(outfile,"		true,\n");

			fprintf(outfile,"		{%i,{%.*f,%.*f,%.*f,%.*f,%i,%i,%.*f,%.*f,%.*f},{%.*f,%.*f,%.*f},{%.*f,%.*f,%.*f},%.*f,%.*f,%s,{%.*f,%.*f,%.*f},{%.*f,%.*f,%.*f},%.*f,%.*f},\n",(int)csv[0],
                                                                                                                                                                                   precision,csv[1],
                                                                                                                                                                                   precision,csv[2],
                                                                                                                                                                                   precision,csv[3],
                                                                                                                                                                                   precision,csv[4],
                                                                                                                                                                                   (int)csv[5],
                                                                                                                                                                                   (int)csv[6],
                                                                                                                                                                                   precision,csv[7],
                                                                                                                                                                                   precision,csv[8],
                                                                                                                                                                                   precision,csv[9],
                                                                                                                                                                                   precision,csv[10],
                                                                                                                                                                                   precision,csv[11],
                                                                                                                                                                                   precision,csv[12],
                                                                                                                                                                                   precision,csv[13],
                                                                                                                                                                                   precision,csv[14],
                                                                                                                                                                                   precision,csv[15],
                                                                                                                                                                                   precision,csv[16],
                                                                                                                                                                                   precision,csv[17],
                                                                                                                                                                                   ((int)csv[18]==1) ? ("true") : ("false"),
                                                                                                                                                                                   precision,csv[19],
                                                                                                                                                                                   precision,csv[20],
                                                                                                                                                                                   precision,csv[21],
                                                                                                                                                                                   precision,csv[22],
                                                                                                                                                                                   precision,csv[23],
                                                                                                                                                                                   precision,csv[24],
                                                                                                                                                                                   precision,csv[25],
                                                                                                                                                                                   precision,csv[26]);

			delete[] csv;
		}
		else
		{
			fprintf(outfile,"		false,\n");

			fprintf(outfile,"		{0,{0,0,0,0,0,0,0,0,0},{0,0,0},{0,0,0},0,0},\n");
		}

		fprintf(outfile,"		%s_MAP::%s_LAYER::numbeams,\n",TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
		fprintf(outfile,"		%s_MAP::%s_LAYER::beams,\n"   ,TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());

		fprintf(outfile,"		%s_MAP::%s_LAYER::numpointlights,\n",TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
		fprintf(outfile,"		%s_MAP::%s_LAYER::pointlights\n"   ,TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
	}
	else if (TiXmlString(name)=="foreground" || TiXmlString(name)=="background")
	{
		fprintf(outfile,"		%s_MAP::%s_LAYER::texture_index,\n",TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
		fprintf(outfile,"		0,\n");

		bvalue=getlayer_property_bool(exists,name,"wrap",doc); if (!exists) {exit(1);}

		if (bvalue)
		{
			fprintf(outfile,"		true,\n");
		}
		else
		{
			fprintf(outfile,"		false,\n");
		}

		fprintf(outfile,"		VECTOR(0,0),\n");
		fprintf(outfile,"		0,\n");

		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");

		fprintf(outfile,"		false,\n");
		fprintf(outfile,"		{0,{0,0,0,0,0,0,0,0,0},{0,0,0},{0,0,0},0,0},\n");

		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");

		fprintf(outfile,"		%s_MAP::%s_LAYER::numpointlights,\n",TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
		fprintf(outfile,"		%s_MAP::%s_LAYER::pointlights\n"   ,TiXmlString(identifier).to_uppercase().c_str(),TiXmlString(name).to_uppercase().c_str());
	}
	else
	{
		exit(1);
	}
}

static void generate_objects_layer    (const char *name,FILE *outfile,TiXmlDocument &doc)
{
	bool exists;

	int mapwidth =getmap_attribute_int("width",doc);
	int mapheight=getmap_attribute_int("height",doc);

	int tilewidth =getmap_attribute_int("tilewidth" ,doc);
	int tileheight=getmap_attribute_int("tileheight",doc);

	double tws=getmap_property_double(exists,"tilewidth_scale" ,doc); if (!exists) {exit(1);}
	double ths=getmap_property_double(exists,"tileheight_scale",doc); if (!exists) {exit(1);}

	int numtiles=mapwidth*mapheight;
	int firstgid=gettileset_attribute_int("objects","firstgid",doc);

	const double *csv=getlayer_data_csv(name,doc);

	const char *fmt=
"		inline %s *create_%s_%i_%i(WORLD &world)\n"
"		{\n"
"			%s *object=new %s;\n"
"			object->main_init(VECTOR(%.*f,%.*f),0,VECTOR(0,0),0,world);\n"
"			return(object);\n"
"		}\n";

	for (int i=0;i<numtiles;i++)
	{
		if ((int)csv[i]>0)
		{
			int index=(int)csv[i]-firstgid;

			const char *identifier=gettileset_tile_property_string(exists,"objects",index,"identifier",doc); if (!exists) {exit(1);}

			TiXmlString object_typename=(TiXmlString(identifier).to_uppercase())+"_OBJECT";

			int gx=i%mapwidth;
			int gy=i/mapwidth;

			double x=(gx*(tilewidth*tws))+((tilewidth*tws)/2);
			double y=(gy*(tileheight*ths))+((tileheight*ths)/2);

			fprintf(outfile,fmt,object_typename.c_str(),identifier,gx,gy,object_typename.c_str(),object_typename.c_str(),precision,x,precision,y);
		}
	}

	delete[] csv;
}

static void generate_struct_layer_motioninfo(const char *name,FILE *outfile,TiXmlDocument &doc)
{
	bool exists;

	double dvalue;

	const char *linear_motion_type_string =getlayer_property_string(exists,name,"linear_motion_type" ,doc); if (!exists) {exit(1);}
	const char *angular_motion_type_string=getlayer_property_string(exists,name,"angular_motion_type",doc); if (!exists) {exit(1);}

	if (TiXmlString(linear_motion_type_string)=="none")
	{
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
	}
	else if (TiXmlString(linear_motion_type_string)=="once")
	{
		fprintf(outfile,"		1,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
	}
	else if (TiXmlString(linear_motion_type_string)=="loop")
	{
		fprintf(outfile,"		2,\n");

		dvalue=getlayer_property_double(exists,name,"linear_motion_loop_start",doc); if (!exists) {exit(1);}
		fprintf(outfile,"		%.*f,\n",precision,dvalue);
		dvalue=getlayer_property_double(exists,name,"linear_motion_loop_end"  ,doc); if (!exists) {exit(1);}
		fprintf(outfile,"		%.*f,\n",precision,dvalue);
	}
	else
	{
		exit(1);
	}

	if (TiXmlString(angular_motion_type_string)=="none")
	{
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
	}
	else if (TiXmlString(angular_motion_type_string)=="once")
	{
		fprintf(outfile,"		1,\n");
		fprintf(outfile,"		0,\n");
		fprintf(outfile,"		0,\n");
	}
	else if (TiXmlString(angular_motion_type_string)=="loop")
	{
		fprintf(outfile,"		2,\n");

		dvalue=getlayer_property_double(exists,name,"angular_motion_loop_start",doc); if (!exists) {exit(1);}
		fprintf(outfile,"		%.*f,\n",precision,dvalue);
		dvalue=getlayer_property_double(exists,name,"angular_motion_loop_end"  ,doc); if (!exists) {exit(1);}
		fprintf(outfile,"		%.*f,\n",precision,dvalue);
	}
	else
	{
		exit(1);
	}
}

static int getnumtiles(const char *name,TiXmlDocument &doc)
{
	int tilewidth =getmap_attribute_int("tilewidth",doc);
	int tileheight=getmap_attribute_int("tileheight",doc);

	int bitmapwidth =gettileset_image_attribute_int(name,"width",doc);
	int bitmapheight=gettileset_image_attribute_int(name,"height",doc);

	return((bitmapwidth/tilewidth)*(bitmapheight/tileheight));
}
