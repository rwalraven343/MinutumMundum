//Code by Rogier Walraven. Do not distribute.

#include "mapparser.h"

static const double *string_to_csv(const char *string);

int getmap_attribute_int(const char *name,TiXmlDocument &doc)
{
	int value;

	if (doc.RootElement()->QueryIntAttribute(name,&value)!=TIXML_SUCCESS)
	{
		exit(1);
	}

	return(value);
}

bool          getmap_property_bool  (bool &exists,const char *name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("properties").FirstChild("property").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("property"))
	{
		if (!element->Attribute("name") || !element->Attribute("value"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			if (TiXmlString("true")==element->Attribute("value"))
			{
				exists=true;

				return(true);
			}
			else if (TiXmlString("false")==element->Attribute("value"))
			{
				exists=true;

				return(false);
			}
			else
			{
				exit(1);
			}
		}
	}

	exists=false;

	return(false);
}

int           getmap_property_int   (bool &exists,const char *name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("properties").FirstChild("property").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("property"))
	{
		if (!element->Attribute("name") || !element->Attribute("value"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			int value;

			if (element->QueryIntAttribute("value",&value)!=TIXML_SUCCESS)
			{
				exit(1);
			}

			exists=true;

			return(value);
		}
	}

	exists=false;

	return(0);
}

double        getmap_property_double(bool &exists,const char *name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("properties").FirstChild("property").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("property"))
	{
		if (!element->Attribute("name") || !element->Attribute("value"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			double value;

			if (element->QueryDoubleAttribute("value",&value)!=TIXML_SUCCESS)
			{
				exit(1);
			}

			exists=true;

			return(value);
		}
	}

	exists=false;

	return(0);
}

const double *getmap_property_csv   (bool &exists,const char *name,TiXmlDocument &doc)
{
	const char *string=getmap_property_string(exists,name,doc);

	if (exists)
	{
		return(string_to_csv(string));
	}

	return(0);
}

const char   *getmap_property_string(bool &exists,const char *name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("properties").FirstChild("property").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("property"))
	{
		if (!element->Attribute("name") || !element->Attribute("value"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			exists=true;

			return(element->Attribute("value"));
		}
	}

	exists=false;

	return(0);
}

int gettileset_attribute_int      (const char *name,const char *attr_name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("tileset").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("tileset"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			int value;

			if (element->QueryIntAttribute(attr_name,&value)!=TIXML_SUCCESS)
			{
				exit(1);
			}

			return(value);
		}
	}

	exit(1);

	return(0);
}

int gettileset_image_attribute_int(const char *name,const char *attr_name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("tileset").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("tileset"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			TiXmlElement *image_element=TiXmlHandle(element).FirstChild("image").ToElement();

			if (!image_element)
			{
				exit(1);
			}

			int value;

			if (image_element->QueryIntAttribute(attr_name,&value)!=TIXML_SUCCESS)
			{
				exit(1);
			}

			return(value);
		}
	}

	exit(1);

	return(0);
}

double        gettileset_tile_property_double(bool &exists,const char *name,int id,const char *prop_name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("tileset").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("tileset"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			TiXmlElement *tile_element=TiXmlHandle(element).FirstChild("tile").ToElement();

			if (!tile_element)
			{
				exit(1);
			}

			for (;tile_element;tile_element=tile_element->NextSiblingElement("tile"))
			{
				if (!tile_element->Attribute("id"))
				{
					exit(1);
				}

				int tile_id;

				if (tile_element->QueryIntAttribute("id",&tile_id)!=TIXML_SUCCESS)
				{
					exit(1);
				}

				if (tile_id==id)
				{
					TiXmlElement *property_element=TiXmlHandle(tile_element).FirstChild("properties").FirstChild("property").ToElement();

					if (!property_element)
					{
						exit(1);
					}

					for (;property_element;property_element=property_element->NextSiblingElement("property"))
					{
						if (!property_element->Attribute("name") || !property_element->Attribute("value"))
						{
							exit(1);
						}

						if (TiXmlString(prop_name)==property_element->Attribute("name"))
						{
							double value;

							if (property_element->QueryDoubleAttribute("value",&value)!=TIXML_SUCCESS)
							{
								exit(1);
							}

							exists=true;

							return(value);
						}
					}

					exists=false;

					return(0);
				}
			}
		}
	}

	exit(1);

	return(0);
}

const double *gettileset_tile_property_csv   (bool &exists,const char *name,int id,const char *prop_name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("tileset").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("tileset"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			TiXmlElement *tile_element=TiXmlHandle(element).FirstChild("tile").ToElement();

			if (!tile_element)
			{
				exit(1);
			}

			for (;tile_element;tile_element=tile_element->NextSiblingElement("tile"))
			{
				if (!tile_element->Attribute("id"))
				{
					exit(1);
				}

				int tile_id;

				if (tile_element->QueryIntAttribute("id",&tile_id)!=TIXML_SUCCESS)
				{
					exit(1);
				}

				if (tile_id==id)
				{
					TiXmlElement *property_element=TiXmlHandle(tile_element).FirstChild("properties").FirstChild("property").ToElement();

					if (!property_element)
					{
						exit(1);
					}

					for (;property_element;property_element=property_element->NextSiblingElement("property"))
					{
						if (!property_element->Attribute("name") || !property_element->Attribute("value"))
						{
							exit(1);
						}

						if (TiXmlString(prop_name)==property_element->Attribute("name"))
						{
							exists=true;

							return(string_to_csv(property_element->Attribute("value")));
						}
					}

					exists=false;

					return(0);
				}
			}
		}
	}

	exit(1);

	return(0);
}

const char   *gettileset_tile_property_string(bool &exists,const char *name,int id,const char *prop_name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("tileset").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("tileset"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			TiXmlElement *tile_element=TiXmlHandle(element).FirstChild("tile").ToElement();

			if (!tile_element)
			{
				exit(1);
			}

			for (;tile_element;tile_element=tile_element->NextSiblingElement("tile"))
			{
				if (!tile_element->Attribute("id"))
				{
					exit(1);
				}

				int tile_id;

				if (tile_element->QueryIntAttribute("id",&tile_id)!=TIXML_SUCCESS)
				{
					exit(1);
				}

				if (tile_id==id)
				{
					TiXmlElement *property_element=TiXmlHandle(tile_element).FirstChild("properties").FirstChild("property").ToElement();

					if (!property_element)
					{
						exit(1);
					}

					for (;property_element;property_element=property_element->NextSiblingElement("property"))
					{
						if (!property_element->Attribute("name") || !property_element->Attribute("value"))
						{
							exit(1);
						}

						if (TiXmlString(prop_name)==property_element->Attribute("name"))
						{
							exists=true;

							return(property_element->Attribute("value"));
						}
					}

					exists=false;

					return(0);
				}
			}
		}
	}

	exit(1);

	return(0);
}

bool getlayer_exists(const char *name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("layer").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("layer"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			return(true);
		}
	}

	return(false);
}

bool          getlayer_property_bool  (bool &exists,const char *name,const char *prop_name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("layer").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("layer"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			TiXmlElement *property_element=TiXmlHandle(element).FirstChild("properties").FirstChild("property").ToElement();

			if (!property_element)
			{
				exit(1);
			}

			for (;property_element;property_element=property_element->NextSiblingElement("property"))
			{
				if (!property_element->Attribute("name") || !property_element->Attribute("value"))
				{
					exit(1);
				}

				if (TiXmlString(prop_name)==property_element->Attribute("name"))
				{
					if (TiXmlString("true")==property_element->Attribute("value"))
					{
						exists=true;

						return(true);
					}
					else if (TiXmlString("false")==property_element->Attribute("value"))
					{
						exists=true;

						return(false);
					}
					else
					{
						exit(1);
					}
				}
			}

			exists=false;

			return(false);
		}
	}

	exit(1);

	return(false);
}

double        getlayer_property_double(bool &exists,const char *name,const char *prop_name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("layer").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("layer"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			TiXmlElement *property_element=TiXmlHandle(element).FirstChild("properties").FirstChild("property").ToElement();

			if (!property_element)
			{
				exit(1);
			}

			for (;property_element;property_element=property_element->NextSiblingElement("property"))
			{
				if (!property_element->Attribute("name") || !property_element->Attribute("value"))
				{
					exit(1);
				}

				if (TiXmlString(prop_name)==property_element->Attribute("name"))
				{
					double value;

					if (property_element->QueryDoubleAttribute("value",&value)!=TIXML_SUCCESS)
					{
						exit(1);
					}

					exists=true;

					return(value);
				}
			}

			exists=false;

			return(0);
		}
	}

	exit(1);

	return(0);
}

const double *getlayer_property_csv   (bool &exists,const char *name,const char *prop_name,TiXmlDocument &doc)
{
	const char *string=getlayer_property_string(exists,name,prop_name,doc);

	if (exists)
	{
		return(string_to_csv(string));
	}

	return(0);
}

const char   *getlayer_property_string(bool &exists,const char *name,const char *prop_name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("layer").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("layer"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			TiXmlElement *property_element=TiXmlHandle(element).FirstChild("properties").FirstChild("property").ToElement();

			if (!property_element)
			{
				exit(1);
			}

			for (;property_element;property_element=property_element->NextSiblingElement("property"))
			{
				if (!property_element->Attribute("name") || !property_element->Attribute("value"))
				{
					exit(1);
				}

				if (TiXmlString(prop_name)==property_element->Attribute("name"))
				{
					exists=true;

					return(property_element->Attribute("value"));
				}
			}

			exists=false;

			return(0);
		}
	}

	exit(1);

	return(0);
}

const double *getlayer_data_csv(const char *name,TiXmlDocument &doc)
{
	TiXmlElement *element=TiXmlHandle(&doc).FirstChild("map").FirstChild("layer").ToElement();

	if (!element)
	{
		exit(1);
	}

	for (;element;element=element->NextSiblingElement("layer"))
	{
		if (!element->Attribute("name"))
		{
			exit(1);
		}

		if (TiXmlString(name)==element->Attribute("name"))
		{
			TiXmlElement *data_element=TiXmlHandle(element).FirstChild("data").ToElement();

			const char *string=data_element->GetText();

			if (!string)
			{
				exit(1);
			}

			return(string_to_csv(string));
		}
	}

	exit(1);

	return(0);
}

static const double *string_to_csv(const char *string)
{
	int len=0;

	while (string[len]!='\0')
	{
		len++;
	}

	len++;

	if (len==1)
	{
		exit(1);
	}

	int numcommas=0;

	for (int i=0;i<len;i++)
	{
		if (string[i]==',')
		{
			numcommas++;
		}
	}

	double *values=new double[numcommas+1];

	for (int i=0;*string!='\0';i++)
	{
		if (TIXML_SSCANF(string,"%lf",values+i)!=1)
		{
			exit(1);
		}

		while (*string!=',' && *string!='\0')
		{
			string++;
		}

		if (*string!='\0')
		{
			string++;
		}
	}

	return(values);
}
