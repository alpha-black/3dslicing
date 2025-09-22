#include "utilities.h"
#include <QCoreApplication>
#include <iostream>

std::string
get_app_executable_path()
{
   return QCoreApplication::applicationDirPath().toStdString();
}

std::string
get_shaders_path()
{
    std::string app_path = get_app_executable_path();
#ifdef HAVE_OSX
    app_path.append("/../Resources/Shaders/");
#endif
    return app_path;
}

std::string
get_ext_backend_path()
{
    std::string ext_backend_path = get_app_executable_path();

#ifdef HAVE_OSX
    ext_backend_path.append("/../Resources/Ext-backend/");
#endif /* HAVE_OSX */

    return ext_backend_path;
}

std::string
get_ext_backend_run_path()
{
    std::string ext_backend_path = get_app_executable_path();

#ifdef HAVE_OSX
    ext_backend_path.append("/../Resources/Ext-backend/slic3r/");
#endif /* HAVE_OSX */

    return ext_backend_path;
}

std::string
_get_config_file_name(struct ext_backend_args *args)
{
    std::string c_file_name = "config";
    if (args->printer_id == 0)
        c_file_name.append("_A");
    else
        c_file_name.append("_B");

    if (args->support_ == 0)
        c_file_name.append("_SD");
    else
        c_file_name.append("_SA");

    if (args->quality_ == 0)
        c_file_name.append("_QN");
    else
        c_file_name.append("_QH");

    c_file_name.append(".ini");

    return c_file_name;
}

std::string
_get_config_bundle_file_name(struct ext_backend_args *args)
{
    std::string cb_file_name = "Slic3r_";
    cb_file_name.append(_get_config_file_name(args));
    return cb_file_name;
}

std::string
get_abs_config_file_name(struct ext_backend_args *args)
{
    //if (get_ext_backend_path().append(_get_config_file_name(args)).c_str())
    std::cout << "Config file name ";
    std::cout << get_ext_backend_path().append(_get_config_file_name(args));
    std::cout << "\n";
    return get_ext_backend_path().append(_get_config_file_name(args));
}

std::string
get_abs_config_bundle_file_name(struct ext_backend_args *args)
{
    return get_ext_backend_path().append(_get_config_bundle_file_name(args));
}
