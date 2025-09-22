#include "backend_main.h"
#include "errors.h"
#include "shared_data_handler.h"
#include "slicer.h"
#include "gl_utilities.h"

extern SharedDataHandler *dataHandler;

void
backend_main::backend_init()
{
    sPlane = new slicingPlane;
    sPlane->transform_slicing_plane();
    slicing_done = false;
    init_done = true;
}

void
backend_main::backend_cleanup()
{
    qDebug() << "Deleting slicing plane";
    if (sPlane != NULL) {
        delete sPlane;
        sPlane = NULL;
    }

    qDebug() << "Cleanup lines";
    lines.clear();
    lines.shrink_to_fit();
}

void
backend_main::reset_slicer()
{
    if (sPlane != NULL) {
        sPlane->reset_slicing_plane();
    }
    lines.clear();
    lines.shrink_to_fit();
    slicing_done = false;
}

uint8_t
backend_main::backend_place_obj_on_bed(FacetList *facets)
{
    if (!facets) {
        qDebug() << "Something very wrong. Null Pointers!";
        return ERROR_NULL_PTR;
    }

    /* Interchange z and y coordinates. Or rotate about x by 90 */
    float theta = -1.57f;
    v3 rotation_axis(1.0f, 0.0f, 0.0f);

    return rotate_object_on_arb_axis(rotation_axis, theta);

#if 0
    /* Rotate the object to place the flat side on the bed.
     * Find cross product of max_area_normal and negative y axis.
     * Rotate about this vector by angle between the two */
    qDebug() << "Max area and normal " << facets->get_max_area();
    print_v3(facets->get_max_area_normal());
    qDebug() << "Max area triangle ";
    //print_v3(facets->get_vertex_from_index(facets->get_max_area_facet_index()).position);

    /* Normalize the normal */
    facets->normalize_max_area_normal();
    v3 rotation_axis = v3::cross_product(facets->get_max_area_normal(),
                                         v3(0.0f, -1.0f, 0.0f));
    rotation_axis.normalize();
    float dot = facets->get_max_area_normal().y * -1.0f;
    float theta = std::acos(dot);
    qDebug() << "Angle between normal and y " << theta;
    qDebug() << "Rotation axis "; print_v3(rotation_axis);

    if (theta > 0.001f || theta < -0.001f) {
        return rotate_object_on_arb_axis(rotation_axis, theta);
    } else {
        return SUCCESS;
    }
#endif
}


void *
backend_main::get_lines_on_plane_addr(unsigned i)
{
    if (i >= lines.size())
        return NULL;
    return lines[i].data();
}

unsigned
backend_main::get_number_of_lines_on_plane(unsigned i)
{
    if (i >= lines.size())
        return 0u;
    return lines[i].size();
}

unsigned
backend_main::get_slicing_plane_count()
{
    return lines.size();
}

void *
backend_main::get_loop_infill_addr(unsigned i)
{
    std::vector<std::vector<v3> >& infill_ = filler.infill;

    if (i >= infill_.size())
        return NULL;
    return infill_[i].data();
}

unsigned
backend_main::get_loop_infill_count(unsigned i)
{
    std::vector<std::vector<v3> >& infill_ = filler.infill;

    if (i >= infill_.size())
        return 0u;

    return infill_[i].size();
}

unsigned
backend_main::get_infill_count()
{
    std::vector<std::vector<v3> >& infill_ = filler.infill;

    return infill_.size();
}

/* Not used now. */
void
backend_main::backend_remove_duplicate()
{
    std::map<std::pair<v3, v3>, int, lines_compare> duplicate_lines;

    for (unsigned j = 0; j < lines.size(); ++j) {
        std::vector<v3>& lines_ = lines[j];
        for (unsigned i = 0; i < lines_.size() - 1; i+=2) {
            auto key = v3::make_pair(lines_[i], lines_[i+1]);
            if (duplicate_lines[key] == true) {
                lines_.erase(lines_.begin() + i, lines_.begin() + i+2);
                continue;
            }
        duplicate_lines[key] = true;
        }
        lines_.shrink_to_fit();
    }
}

/* Finding loops in a plane */
void
backend_main::backend_process_lines_on_a_plane(std::vector<v3>& lines_)
{
    /* Try to find loops. If npo loops, try to form loops.
     * If not, let them be. */
    unsigned lines_size = lines_.size();
    if (lines_size < 2) {
        return;
    }

    /* Here we temperorily store the loops and add them to "lines" later */
    std::vector<v3> loop;
    /* The algorithm is far from optimum, we need all this to keep a track */
    std::vector<bool> line_visited;
    line_visited.reserve(lines_size/2);
    //qDebug() << "line_visited size " << line_visited.size();

    unsigned lines_visited_count = 0u;

    /* Initialization */
    for (auto i = 0u; i < lines_size/2; ++i) {
        line_visited.push_back(false);
    }

    while (lines_visited_count < lines_size) {
        unsigned i = 0u;
        for (i = 0u; i < lines_size - 1; i += 2) {
            if (line_visited[i/2] == false)
                break;
        }
        v3 start_v(lines_[i]); v3 end_v(lines_[i+1]);
        line_visited[i/2] = true; lines_visited_count += 2;
        loop.push_back(start_v); loop.push_back(end_v);

        for (i = 2u; i < lines_size - 1; i += 2) {
            /* Visited keeps track of lines that have already been added to the
            * loop. We do not need to consider them again */
            if (line_visited[i/2] == true) {
                continue;
            }

            /* i1 has v3 which is equal to end_v. We set end_v
            * to i2 now and search for that in the next iteration.
            * If i2 is start, its the end of the loop */
            unsigned i1 = 0u, i2 = 0u;
            if (v3::equal(lines_[i], end_v)) {
                i1 = i; i2 = i + 1;
            } else if (v3::equal(lines_[i + 1], end_v)) {
                i1 = i + 1; i2 = i;
            } else {
                continue;
            }

            /* end_v was alredy comapred and found equal to i1. My comments here
            * are so good because everytime I read this code, I have no clue what
            * I had done before. Also, I use first person plural most of the time
            * because I am modest. Or mostly because I do not want to take all the
            * blame myself. */
            end_v.set(lines_[i2]); line_visited[i/2] = true;
            lines_visited_count += 2;
            loop.push_back(lines_[i1]); loop.push_back(lines_[i2]);

            /* This is the end.. of the loop */
            if (v3::equal(lines_[i2], start_v)) {
                //qDebug() << "Loop complete";
                lines.push_back(loop);
                loop.clear(); loop.shrink_to_fit();
                break;

                #if 0
                /* We now check for unvisited lines. If there are more, there are
                * continue from top again by resetting i. This is horrendeosuly
                * bad algorithm. Slowly but surely.. */
                bool more_lines = false;
                for (auto j = 0u; j < line_visited.size(); ++j)
                if (line_visited[i] == false) {
                    start_v.set(lines_[j*2]); end_v.set(lines_[(j*2)+1]);
                    more_lines = true; i = j * 2;
                    loop.push_back(start_v); loop.push_back(end_v);
                    break;
                }
                if (!more_lines)
                    break;
                #endif
            } else i = 2u;
        }

        if (loop.size()) {
            #if 0
            /* Here we try to form loops. As promised earlier.
            * Simple algorithm. Could back fire */
            end_v.set(loop[loop.size() -1]);
            if (v3::distance(end_v, start_v) < BACKEND_LOOP_FORM_DIST) {
                qDebug() << "Completing loop";
                loop.push_back(start_v); loop.push_back(end_v);
            }
            #endif
            lines.push_back(loop);
        }
    }
}

uint8_t
backend_main::backend_add_infill()
{
    return filler.filler_process_lines();
}

uint8_t
backend_main::backend_find_overhangs()
{
    return SUCCESS;
}

uint8_t
backend_main::backend_slice()
{
    if (!dataHandler || !dataHandler->facets ||
        !dataHandler->readyToRender) {
        return ERROR_NULL_PTR;
    }

    /* Finding overhangs. An approach similar to slicing. Take the xz plane,
     * transform it. Pass it through (0, 0, 0) to (0, 1, 0) and find all the
     * intersections with the object. Find the local minimum in y and support
     * there.*/
    std::vector<Vertex> vertices = dataHandler->facets->get_vertices();
    std::vector<bool> facet_status;
    facet_status.reserve(vertices.size() / 3);

    for (size_t t = 0; t < facet_status.size(); ++t)
        facet_status[t] = false;

    for (float d = 0.0f; d <= 1.0f; d+= SLICING_DEPTH) {
        /* Keep incrementing the slicing plane distance */
        sPlane->set_distance(d);

        std::vector<v3> lines_;
        /* Loop through all the facets. Check the flag status first */
        for (size_t i = 0; i < vertices.size() - 2; i+=3) {
            //qDebug() << facet_status[i/3];
            if (facet_status[i/3] == true)
                continue;

            v3 line_p1, line_p2;
            uint8_t ret = sPlane->intersect_facet(vertices[i].position,
                          vertices[i+1].position, vertices[i+2].position,
                          &line_p1, &line_p2);

            if (ret == ERROR_TRI_SLICING_DONE) {
                facet_status[i/3] = true;
                continue;
            }

            if (ret != ERROR_NO_INTERSECTION && ret != ERROR_TRI_ON_PLANE) {
                lines_.push_back(line_p1); lines_.push_back(line_p2);
            } else if (ret == ERROR_TRI_ON_PLANE) {
                lines_.push_back(vertices[i].position);
                lines_.push_back(vertices[i+1].position);
                lines_.push_back(vertices[i+1].position);
                lines_.push_back(vertices[i+2].position);
                lines_.push_back(vertices[i+2].position);
                lines_.push_back(vertices[i].position);
            }
        }
        backend_process_lines_on_a_plane(lines_);
    }

    uint8_t ret = backend_add_infill();
    if (ret == SUCCESS) {
        slicing_done = true;
    }
    return ret;
}

uint8_t
backend_main::backend_generate_gcode()
{
    return SUCCESS;
}

uint8_t
backend_main::backend_receive_request(uint8_t request)
{
    if (!dataHandler || !dataHandler->facets ||
        !dataHandler->readyToRender) {
        return ERROR_NULL_PTR;
    }

    if (!init_done) {
        backend_init();
    } else {
        reset_slicer();
    }

    uint8_t ret = ERROR_DEFAULT;
    switch(request) {
        case BACKEND_GENERATE_SUPPORT:
            ret = backend_find_overhangs();
            ret = backend_slice();
            break;
        case BACKEND_SLICE:
            ret = backend_slice();
            break;
        case BACKEND_GENERATE_GCODE:
            ret = backend_generate_gcode();
            break;
        default:
            break;
    }
    return ret;
}
