//Code by Rogier Walraven. Do not distribute.

#include "tinyxml.h"

int getmap_attribute_int(const char *name,TiXmlDocument &doc);

bool          getmap_property_bool  (bool &exists,const char *name,TiXmlDocument &doc);
int           getmap_property_int   (bool &exists,const char *name,TiXmlDocument &doc);
double        getmap_property_double(bool &exists,const char *name,TiXmlDocument &doc);
const double *getmap_property_csv   (bool &exists,const char *name,TiXmlDocument &doc);
const char   *getmap_property_string(bool &exists,const char *name,TiXmlDocument &doc);

int gettileset_attribute_int      (const char *name,const char *attr_name,TiXmlDocument &doc);
int gettileset_image_attribute_int(const char *name,const char *attr_name,TiXmlDocument &doc);

double        gettileset_tile_property_double(bool &exists,const char *name,int id,const char *prop_name,TiXmlDocument &doc);
const double *gettileset_tile_property_csv   (bool &exists,const char *name,int id,const char *prop_name,TiXmlDocument &doc);
const char   *gettileset_tile_property_string(bool &exists,const char *name,int id,const char *prop_name,TiXmlDocument &doc);

bool getlayer_exists(const char *name,TiXmlDocument &doc);

bool          getlayer_property_bool  (bool &exists,const char *name,const char *prop_name,TiXmlDocument &doc);
double        getlayer_property_double(bool &exists,const char *name,const char *prop_name,TiXmlDocument &doc);
const double *getlayer_property_csv   (bool &exists,const char *name,const char *prop_name,TiXmlDocument &doc);
const char   *getlayer_property_string(bool &exists,const char *name,const char *prop_name,TiXmlDocument &doc);

const double *getlayer_data_csv(const char *name,TiXmlDocument &doc);
