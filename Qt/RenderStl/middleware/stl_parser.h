#ifndef STL_PARSER_H
#define STL_PARSER_H

#include <iostream>
#include "geometry.h"

#define READ_BUFFER_LEN                 256
#define ASCII_FACETS_COUNT_DEFAULT      1024

uint8_t stl_datahandler_init(int number_of_facets);
uint8_t process_facets(FacetList *facets);
uint8_t stl_read_file_ascii(std::string file_name);
uint8_t stl_read_file_binary(std::string file_name);

#endif // STL_PARSER_H
