#include <iostream>
#include <fstream>
#include <vector>

/* Distance between slices 
#define SLICING_DISTANCE 0.25
*/

struct coordinate {
        float x;
        float y;
        float z;

        coordinate (float xx = 0, float yy = 0, float zz = 0) :
        x(xx), y(yy), z(zz) {}

        void set (const coordinate c) {
                x = c.x; y = c.y; z = c.z;
        }

        float dot_product (const coordinate c) {
                return (c.x * x + c.y * y + c.z * z);
        }

        coordinate subtract (const coordinate c) {
                coordinate d;
                d.x = x - c.x; d.y = y - c.y; d.z = z - c.z;
                return d;
        }

        coordinate add (const coordinate c) {
                coordinate d;
                d.x = x + c.x; d.y = y + c.y; d.z = z + c.z;
                return d;
        }
};

struct line {
        coordinate p1, p2;
        line () {}
        void set_p1 (coordinate c) {
                p1.set (c);
        }
        void set_p2 (coordinate c) {
                p2.set (c);
        }
};

struct facet {
        /* Normal vector, 3 vertices and attribute byte count */
        coordinate normal_vector, v1, v2, v3;
        uint16_t attr_byte_count;
};

class facetList {
        private:
            std::vector<facet> facet_list;
            float min_z, max_z;
        public:
            facetList () : min_z(0), max_z(0) {}

            void set_min_max_z (float z1, float z2, float z3) {
                    min_z = (z1 < min_z) ? z1 : (z2 < min_z) ? z2 : (z3 < min_z) ? z3 : min_z;
                    max_z = (z1 > max_z) ? z1 : (z2 > max_z) ? z2 : (z3 > max_z) ? z3 : max_z;
            }

            float get_min_z () {
                    return min_z;
            }

            float get_max_z () {
                    return max_z;
            }

            void reserve_list_space (int n) {
                    facet_list.reserve (n);
            }

            std::vector<facet>::iterator get_list_begin () {
                    return facet_list.begin ();
            }

            std::vector<facet>::iterator get_list_end () {
                    return facet_list.end ();
            }

            void add_facet (facet f) {
                    facet_list.push_back (f);
            }

            void dump_facet_list () {
                std::cout << "Dumping facet lists from stl" << std::endl;
                for (std::vector<facet>::iterator it = facet_list.begin ();
                                  it != facet_list.end (); ++it) {
                    std::cout << "("<<it->normal_vector.x<<", "<<it->normal_vector.y<<", " \
                              << it->normal_vector.z << "), \t";
                    std::cout << "("<<it->v1.x<<", "<<it->v1.y<<", " << it->v1.z << "), \t";
                    std::cout << "("<<it->v2.x<<", "<<it->v2.y<<", " << it->v2.z << "), \t";
                    std::cout << "("<<it->v3.x<<", "<<it->v3.y<<", " << it->v3.z << ")" << \
                    std::endl;
                }
            }
};

class slicingPlane {
        private:
            float distance;
            coordinate normal_vector;
        public:
            slicingPlane (float _distance = 0) : distance (_distance) {}

            void set_normal_vector (const coordinate n) {
                    normal_vector.set (n);
            }

            void set_distance (float d) {
                    distance = d;
            }

            float get_distance () {
                    return distance;
            }

            coordinate get_normal_vector () {
                    return normal_vector;
            }

            coordinate intersect_line (coordinate c1, coordinate c2) {
                    /* Logic and code from here -
                    http://math.stackexchange.com/questions/83990/line-and-plane-intersection-in-3d
                     */
                    coordinate diff = c1.subtract (c2);
                    float n_dot_c1 = normal_vector.dot_product (c1);
                    float n_dot_diff = normal_vector.dot_product (diff);

                    float temp1 = ((distance - n_dot_c1)/n_dot_diff);
                    coordinate temp2;
                    temp2.x = temp1 * diff.x; temp2.y = temp1 * diff.y;
                    temp2.z = temp1 * diff.z;

                    coordinate ret = c1.add (temp2);

                    /* return c1 + (((distance - n_dot_c1)/n_dot_diff) * diff); */
                    return ret;
            }

            int8_t intersect_facet (facet f, line *line) {
                    /* Distance to the three vertices */
                    float d1 = f.v1.dot_product (normal_vector) - distance;
                    float d2 = f.v2.dot_product (normal_vector) - distance;
                    float d3 = f.v3.dot_product (normal_vector) - distance;

                    if ((d1 < 0 && d2 < 0 && d3 < 0) ||
                        (d1 > 0 && d2 > 0 && d3 > 0)) {
                            return 0;
                    }

                    if (d1 == 0) {
                            if (d2 * d3 < 0) {
                                    line->p1 = d1;
                                    line->p2 = intersect_line (f.v2, f.v3);
                                    return 1;
                            }
                            else {
                                    return 0;
                            }
                    }

                    if (d2 == 0) {
                            if (d1 * d3 < 0) {
                                    line->p1 = d2;
                                    line->p2 = intersect_line (f.v1, f.v3);
                                    return 1;
                            }
                            else {
                                    return 0;
                            }
                    }

                    if (d3 == 0) {
                            if (d1 * d2 < 0) {
                                    line->p1 = d3;
                                    line->p2 = intersect_line (f.v1, f.v2);
                                    return 1;
                            }
                            else {
                                    return 0;
                            }
                    }

                    if (d1 * d2 < 0) {
                            line->p1 = intersect_line (f.v1, f.v2);
                            if (d2 * d3 < 0) {
                                line->p2  = intersect_line (f.v2, f.v3);
                            } else {
                                line->p2 = intersect_line (f.v1, f.v3);
                            }
                            return 1;
                    } else {
                            line->p1 = intersect_line (f.v1, f.v3);
                            line->p2 = intersect_line (f.v2, f.v3);
                            return 1;
                    }
                    return 0;
            }
};

int
read_facets_from_file (std::ifstream *fp, facetList *f_list,
                       size_t number_of_facets)
{
        if (fp == NULL || f_list == NULL) { 
                return 0;
        }

        for (int i = 0; i < number_of_facets; ++i) {
                facet f;
                fp->read ((char *)&f.normal_vector.x, sizeof (float));
                fp->read ((char *)&f.normal_vector.y, sizeof (float));
                fp->read ((char *)&f.normal_vector.z, sizeof (float));
                fp->read ((char *)&f.v1.x, sizeof (float));
                fp->read ((char *)&f.v1.y, sizeof (float));
                fp->read ((char *)&f.v1.z, sizeof (float));
                fp->read ((char *)&f.v2.x, sizeof (float));
                fp->read ((char *)&f.v2.y, sizeof (float));
                fp->read ((char *)&f.v2.z, sizeof (float));
                fp->read ((char *)&f.v3.x, sizeof (float));
                fp->read ((char *)&f.v3.y, sizeof (float));
                fp->read ((char *)&f.v3.z, sizeof (float));
                fp->read ((char *)&f.attr_byte_count, sizeof (uint16_t));
                f_list->add_facet (f);
                f_list->set_min_max_z (f.v1.z, f.v2.z, f.v3.z);
        }
        f_list->dump_facet_list ();
        return 1;
}

void
trivial_slicer (facetList *facet_list, float slicing_distance,
                std::vector<line> *intersecting_lines)
{
        slicingPlane plane;
        //std::vector<std::vector<line> > intersecting_lines;
        //std::vector<line> intersecting_lines;

        /* Normal does not change. Normal is the z axis. */
        coordinate n(0, 0, 1);
        plane.set_normal_vector (n);

        /* Compute number of slices */
        float min_z = facet_list->get_min_z ();
        float max_z = facet_list->get_max_z ();
        //int num_of_slices = (int) ((facet_list->get_max_z() - min_z) / SLICING_DISTANCE);
        int num_of_slices = (int) ((max_z - min_z) / slicing_distance);

        std::cout << "Max Z " << max_z << ", Min Z " << min_z << ", Slicing distance " << slicing_distance << std::endl;
        std::cout << "Number of slices " << num_of_slices << std::endl;

        for (int i = 0; i < num_of_slices; ++i) {
                std::vector<line> lines_per_plane;
                //plane.set_distance ((min_z - SLICING_DISTANCE) + i * SLICING_DISTANCE);
                plane.set_distance ((min_z - slicing_distance) + i * slicing_distance);

                for (std::vector<facet>::iterator it = facet_list->get_list_begin ();
                                           it != facet_list->get_list_end (); ++it) {
                        line int_line;
                        if (plane.intersect_facet (*it, &int_line) != 1) {
                                continue;
                        }
                        lines_per_plane.push_back (int_line);
                        intersecting_lines->push_back (int_line);
                }
                //intersecting_lines.push_back (lines_per_plane);
        }

        std::cout << "Dumping intersecting line segments." << std::endl;
        for (std::vector<line>::iterator it = intersecting_lines->begin ();
                                  it != intersecting_lines->end (); ++it) {
                std::cout << "( " << it->p1.x << ", " << it->p1.y << ", " << it->p1.z << ") \t";
                std::cout << "( " << it->p2.x << ", " << it->p2.y << ", " << it->p2.z << ")" << std::endl;
        }
}

void
generate_g_code (std::vector<line> *lines)
{
        coordinate current (0, 0, 0);

#define FEED_RATE   "F600.00"
#define EXTRUSION   "E0"

        /* Initial std. G code */
        std::cout << "Dumping G code." << std::endl;
        std::cout << "G28\nG21\nG90\nM82\n";
        for (std::vector<line>::iterator it = lines->begin ();
                it != lines->end (); ++it) {
                if (current.z != it->p1.z) {
                        std::cout << "G1 " << "Z" << it->p1.z << " ";
                        std::cout << FEED_RATE << " "<< EXTRUSION << std::endl;
                        current.z = it->p1.z;
                }

                //float move_x = it->p1.x - current.x;
                //float move_y = it->p1.y - current.y;
                float move_x = it->p1.x - current.x + 95;
                float move_y = it->p1.y - current.y + 95;

                current.set (it->p1);
                
                std::cout << "G1 " << "X" << move_x << " Y" << move_y << " ";
                std::cout << FEED_RATE << " "<< EXTRUSION << std::endl;

                //move_x = it->p2.x - current.x;
                //move_y = it->p2.y - current.y;
                move_x = it->p2.x - current.x + 95;
                move_y = it->p2.y - current.y + 95;

                current.set (it->p2);
                
                std::cout << "G1 " << "X" << move_x << " Y" << move_y << " ";
                std::cout << FEED_RATE << " "<< EXTRUSION << std::endl;
        }
        std::cout << "G28\n";
}

int main (int argc, char **argv)
{
        std::ifstream stl_fp;

        if (argc != 3) {
                std::cout <<"Syntax: ./read <filename.stl> <slicing distance>" <<std::endl;
                return 0;
        }

        float slicing_distance = atof (argv[2]);
        if (slicing_distance == 0) {
                std::cout << "Not a distance of 0\n";
                return 0;
        }

        stl_fp.open (argv[1], std::ios::in | std::ios::binary);

        char header[80] = "";
        int number_of_facets = 0;

        stl_fp.read (header, 80);
        stl_fp.read ((char *)&number_of_facets, 4);

        for (int i = 0; i < 80; ++i) {
            std::cout <<header[i];
        }
        std::cout <<std::endl;
        std::cout << "Number of facets "<< number_of_facets<<std::endl;

        //std::vector<facet> facet_list;
        facetList f_list;
        f_list.reserve_list_space (number_of_facets);

        read_facets_from_file (&stl_fp, &f_list, number_of_facets);

        std::vector<line> intersecting_lines;
        trivial_slicer (&f_list, slicing_distance, &intersecting_lines);

        /* G code */
        generate_g_code (&intersecting_lines);

        return 1;
}
