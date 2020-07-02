//Code by Rogier Walraven. Do not distribute.

#include "mapparser.h"

void generate_shapes     (FILE *outfile,TiXmlDocument &doc);
void generate_sounds     (FILE *outfile,TiXmlDocument &doc);
void generate_motions    (FILE *outfile,TiXmlDocument &doc);
void generate_beams      (FILE *outfile,TiXmlDocument &doc);
void generate_pointlights(FILE *outfile,TiXmlDocument &doc);
void generate_indices    (FILE *outfile,TiXmlDocument &doc);
void generate_struct     (FILE *outfile,TiXmlDocument &doc);
void generate_objects    (FILE *outfile,TiXmlDocument &doc);
