#include "shared_data_handler.h"
#include "ext_backend.h"
#include "slicer.h"
#include "stl_parser.h"
#include "errors.h"
#include <fstream>
#include <QDebug>


/**************************************************************
 * SharedDataHandler Memberfunctions
**************************************************************/
uint8_t
SharedDataHandler::read_file()
{
    std::ifstream filePtr;

    filePtr.open(fileName, std::ios::in | std::ios::binary);
    if (!filePtr.is_open()) {
        return ERROR_FILE_NOT_OPEN;
    }

    /* Check STL file type */
    char c; uint16_t count = 0;
    bool file_type_ascii = true;
    while (count < 500)  {
        count++;
        filePtr.get(c);
        if (std::isalnum(c) == false) {
            qDebug() << c;
            file_type_ascii = false;
            break;
        }
    }
    filePtr.close();

    if (file_type_ascii == true) {
        qDebug() << "File type ASCII";
        return stl_read_file_ascii(fileName);
    } else {
        qDebug() << "File type BINARY";
        return stl_read_file_binary(fileName);
    }
}

uint8_t
SharedDataHandler::generateGCode(std::string outFileName)
{
    /* Its easier to move all Slic3r related stuff to
     * a new file and remove the file later.
     **/
#ifdef HAVE_EXT_BACKEND
    ext_args.in_file_name = fileName;
    ext_args.out_file_name = outFileName;
    return qt_slic3r_generate_code(&ext_args);
#else
    return SUCCESS;
#endif
}

uint8_t
SharedDataHandler::send_support_request()
{
    return m_engine->backend_receive_request(BACKEND_GENERATE_SUPPORT);
}

void
SharedDataHandler::set_obj_metrics(QMatrix4x4 *o_matrix)
{
    obj_matrix = *o_matrix;
}

void
SharedDataHandler::cleanup()
{
    /* Delete all stuff. Called on GUI quit */
    if (facets) {
        delete facets;
        facets = NULL;
    }
    obj_matrix.setToIdentity();

    if (m_engine != NULL) {
        /* Delete backend instance */
        qDebug() << "Delete engine instance";
        m_engine->backend_cleanup();
        delete m_engine;
        m_engine = NULL;
    }
}
