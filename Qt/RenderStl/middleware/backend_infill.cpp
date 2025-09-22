#include "backend_infill.h"
#include "errors.h"
#include "shared_data_handler.h"
#include <QDebug>

extern SharedDataHandler *dataHandler;

/* Useful links
http://stackoverflow.com/questions/15668149/polygon-infill-algorithm
http://wiki.evilmadscientist.com/Creating_filled_regions
*/

const v3
backend_filler::find_loop_centre(std::vector<v3>& loop)
{
#if 0
    float c_x = 0.0f, c_z = 0.0f;
    for (auto i = 0u; i < loop.size(); ++i) {
        c_x = (loop[i].x + c_x)/2.0f;
        c_z = (loop[i].z + c_z)/2.0f;
    }
    return v3(c_x, loop[0].y, c_z);
#endif
    float min_x, max_x; min_x = max_x = loop[0].x;
    float min_z, max_z; min_z = max_z = loop[0].z;

    for (auto i = 0u; i < loop.size(); ++i) {
        BACKEND_MAX(max_x, max_x, loop[i].x);
        BACKEND_MAX(max_z, max_z, loop[i].z);
        BACKEND_MIN(min_x, min_x, loop[i].x);
        BACKEND_MIN(min_z, min_z, loop[i].z);
    }
    return v3((min_x + max_x)*0.5f, loop[0].y, (min_z+max_z)*0.5f);
}

void
backend_filler::fill_loop(std::vector<v3>& loop)
{
    if (!dataHandler || !dataHandler->m_engine)
        return;

    if (loop.size() < 6)
        return;
    //qDebug() << "Input loop"; print_v3_vector(loop);

    std::vector<v3> infill_;

    v3 centre(find_loop_centre(loop));

    //qDebug() << "Centres "; print_v3(centre);

    for (auto i = 0u; i < loop.size() - 1; i+=2) {
        float d1 = std::sqrt((loop[i].x - centre.x)*(loop[i].x - centre.x) + \
                             (loop[i].z - centre.z)*(loop[i].z - centre.z));
        float d2 = std::sqrt((loop[i+1].x - centre.x)*(loop[i+1].x - centre.x) + \
                             (loop[i+1].z - centre.z)*(loop[i+1].z - centre.z));

        /* TODO lots of recalculations */
        //qDebug() << "Line"; print_v3(loop[i]); print_v3(loop[i+1]);
        //std::cout << "depth " << d1 << " " << d2 << " Infill\n";
        v3 n1 = v3::subtract(centre, loop[i]);
        v3 n2 = v3::subtract(centre, loop[i+1]);
        n1.normalize(); n2.normalize();
        for (int j = 1; j < (std::max(d1, d2)/BACKEND_INFILL_DEPTH); ++j) {
            v3 p1, p2;
            float depth = j * BACKEND_INFILL_DEPTH;
            p1.set(v3::add(loop[i], v3::scale(n1, depth)));
            p2.set(v3::add(loop[i+1], v3::scale(n2, depth)));
            //qDebug() << "Adding line "; print_v3(p1); print_v3(p2);
            infill_.push_back(p1); infill_.push_back(p2);
        }
        //print_v3(loop[i]);
    }
    //infill = std::move(infill_);
    infill.push_back(infill_);
}

uint8_t
backend_filler::filler_process_lines()
{
    if (!dataHandler || !dataHandler->m_engine)
        return ERROR_NULL_PTR;

    std::vector<std::vector<v3> >& lines = dataHandler->m_engine->lines;

    for (auto i = 0u; i < lines.size(); ++i) {
        std::vector<v3>& lines_ = lines[i];
        fill_loop(lines_);
        /* if (i == 2)
            break; */
    }

    return SUCCESS;
}
