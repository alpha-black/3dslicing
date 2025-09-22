#ifndef __OSX_UTILS__
#define __OSX_UTILS__

#include <string>

#ifdef HAVE_EXT_BACKEND
struct ext_backend_args
{
    std::string in_file_name;
    std::string out_file_name;
    float rotate_y;
    float scale;
    uint8_t printer_id;
    int support_, quality_;
};
#endif

std::string get_app_executable_path();
std::string get_shaders_path();
std::string get_ext_backend_path();
std::string get_ext_backend_run_path();
std::string _get_config_file_name(struct ext_backend_args *args);
std::string _get_config_bundle_file_name(struct ext_backend_args *args);
std::string get_abs_config_file_name(struct ext_backend_args *args);
std::string get_abs_config_bundle_file_name(struct ext_backend_args *args);

#endif /* __OSX_UTILS__ */
