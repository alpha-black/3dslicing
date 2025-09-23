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
