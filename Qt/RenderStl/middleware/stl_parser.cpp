#include "middleware/stl_parser.h"
#include "middleware/shared_data_handler.h"
#include "middleware/geometry.h"
#include <stdio.h>

extern SharedDataHandler *dataHandler;

uint8_t
stl_datahandler_init(int number_of_facets)
{
    if (dataHandler == NULL) {
        return ERROR_NULL_PTR;
    }

    if (dataHandler->facets != NULL) {
        delete dataHandler->facets;
        dataHandler->facets = NULL;
    }
    dataHandler->facets = new FacetList;
    dataHandler->facets->reserve_list_space(number_of_facets);

    return SUCCESS;
}

uint8_t
process_facets(FacetList *facets)
{
    if (facets == NULL) {
        qDebug() << "Something very wrong.";
        return ERROR_NULL_PTR;
    }

    uint8_t ret = facets->normalize_vertices();
    if (ret != SUCCESS) {
        qDebug() << "Normalizing vertices failed.";
        return ret;
    }

    /* Rotate the object to place the flat side on the bed.
     * Find cross product of max_area_normal and negative y axis.
     * Rotate about this vector by angle between the two */
    if (dataHandler && dataHandler->m_engine) {
        ret = dataHandler->m_engine->backend_place_obj_on_bed(dataHandler->facets);
    }

    if (ret != SUCCESS) {
        qDebug() << "Placing object on bed failed.";
    }

    return ret;
}

uint8_t
stl_read_file_ascii(std::string file_name)
{
    char read_buffer[READ_BUFFER_LEN+1];
    std::ifstream file_ptr;

    file_ptr.open(file_name, std::ios::in);
    if (!file_ptr.is_open())
        return ERROR_FILE_NOT_OPEN;

    uint8_t ret = stl_datahandler_init(ASCII_FACETS_COUNT_DEFAULT);
    if (ret != SUCCESS) {
        qDebug() << "Failure in initializing facets.";
        file_ptr.close();
        return ret;
    }

    while(!file_ptr.eof()) {
        file_ptr.getline(read_buffer, READ_BUFFER_LEN);

        char *normal_string = strstr(read_buffer, "normal");
        if (normal_string != NULL) {
            Vertex c1, c2, c3;
            sscanf(normal_string, "normal %f %f %f", &c1.normal.x,
                   &c1.normal.y, &c1.normal.z);

            file_ptr.getline(read_buffer, READ_BUFFER_LEN);
            if (strstr(read_buffer, "outer loop") == NULL) {
                qDebug() << "Bad file format";
                return ERROR_FACETS_NOT_READ;
            }

            file_ptr.getline(read_buffer, READ_BUFFER_LEN);
            char *vertex_string = strstr(read_buffer, "vertex");
            if (vertex_string == NULL) {
                qDebug() << "Bad file format";
                return ERROR_FACETS_NOT_READ;
            }
            sscanf(vertex_string, "vertex %f %f %f", &c1.position.x, &c1.position.y,
                   &c1.position.z);
            dataHandler->facets->add_vertex(c1);

            file_ptr.getline(read_buffer, READ_BUFFER_LEN);
            vertex_string = strstr(read_buffer, "vertex");
            if (vertex_string == NULL) {
                qDebug() << "Bad file format";
                return ERROR_FACETS_NOT_READ;
            }
            sscanf(vertex_string, "vertex %f %f %f", &c2.position.x, &c2.position.y,
                   &c2.position.z);
            c2.normal.set(c1.normal);
            dataHandler->facets->add_vertex(c2);

            file_ptr.getline(read_buffer, READ_BUFFER_LEN);
            vertex_string = strstr(read_buffer, "vertex");
            if (vertex_string == NULL) {
                qDebug() << "Bad file format";
                return ERROR_FACETS_NOT_READ;
            }
            sscanf(vertex_string, "vertex %f %f %f", &c3.position.x, &c3.position.y,
                   &c3.position.z);
            c3.normal.set(c1.normal);
            dataHandler->facets->add_vertex(c3);
            dataHandler->facets->find_max_area_normal(c1, c2, c3);
        }
    }
    file_ptr.close();
    dataHandler->facets->shrink_list();

    ret = process_facets(dataHandler->facets);

    if (ret != SUCCESS) {
        dataHandler->readyToRender = false;
        return ERROR_FACETS_NOT_READ;
    } else {
        dataHandler->readyToRender = true;
    }

    return SUCCESS;
}

uint8_t
stl_read_file_binary(std::string file_name)
{
    if (dataHandler == NULL) {
        qDebug() << "Something very wrong here."
                    "Call for support.";
        return ERROR_NULL_PTR;
    }

    std::ifstream file_ptr;
    int reported_facets = 0;
    char buffer[80];

    file_ptr.open(file_name, std::ios::in | std::ios::binary);

    if (!file_ptr.is_open())
        return ERROR_FILE_NOT_OPEN;

    /* Get file size */
    file_ptr.seekg(0, file_ptr.end);
    int file_size = file_ptr.tellg();
    file_ptr.seekg(0, file_ptr.beg);

    file_ptr.read(buffer, 80);
    file_ptr.read ((char *)&reported_facets, sizeof(int));

    int number_of_facets = (file_size - 80 - 4) / 50;

    qDebug() << "Number of facets " << number_of_facets << reported_facets;

    if (number_of_facets != reported_facets) {
        qDebug() << "Reported facets count is not what is present in file."
                    " Read as ASCII";
        file_ptr.close();
        return stl_read_file_ascii(file_name);
        //number_of_facets = std::min (reported_facets, number_of_facets);
    }

    uint8_t ret = stl_datahandler_init(number_of_facets);
    if (ret != SUCCESS) {
        qDebug() << "Failure in initializing facets.";
        file_ptr.close();
        return ret;
    }

    for (int i = 0; i < number_of_facets; ++i) {
        Vertex c1, c2, c3;
        uint16_t attr_byte_count;
        file_ptr.read ((char *)&c1.normal.x, sizeof (float));
        file_ptr.read ((char *)&c1.normal.y, sizeof (float));
        file_ptr.read ((char *)&c1.normal.z, sizeof (float));
        file_ptr.read ((char *)&c1.position.x, sizeof (float));
        file_ptr.read ((char *)&c1.position.y, sizeof (float));
        file_ptr.read ((char *)&c1.position.z, sizeof (float));
        dataHandler->facets->add_vertex(c1);
        c2.normal.set(c1.normal);
        file_ptr.read ((char *)&c2.position.x, sizeof (float));
        file_ptr.read ((char *)&c2.position.y, sizeof (float));
        file_ptr.read ((char *)&c2.position.z, sizeof (float));
        dataHandler->facets->add_vertex(c2);
        c3.normal.set(c1.normal);
        file_ptr.read ((char *)&c3.position.x, sizeof (float));
        file_ptr.read ((char *)&c3.position.y, sizeof (float));
        file_ptr.read ((char *)&c3.position.z, sizeof (float));
        dataHandler->facets->add_vertex(c3);
        file_ptr.read ((char *)&attr_byte_count, sizeof (uint16_t));
        dataHandler->facets->find_max_area_normal(c1, c2, c3);
    }

    file_ptr.close();

    ret = process_facets(dataHandler->facets);

    if (ret != SUCCESS) {
        dataHandler->readyToRender = false;
        return ERROR_FACETS_NOT_READ;
    } else {
        dataHandler->readyToRender = true;
    }

    return SUCCESS;
}
