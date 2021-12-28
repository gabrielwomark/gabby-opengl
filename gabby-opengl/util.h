#ifndef _util_h_
#define _util_h_
#include <string>

std::string get_asset_path(std::string sub_path);

inline std::string get_asset_path(std::string sub_path) {
    std::string ROOT_DIR = "/Users/gabrielwomark/dev/GabbyOpenGL";
    return ROOT_DIR + "/" + sub_path;
}

#endif

