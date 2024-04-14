#ifndef UTIL_H
#define UTIL_H

#include "pybind11/embed.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <tinyMuduo/Logger.h>
#include <cstdio>
#include <cstdlib>

namespace py = pybind11;

bool ex_py(const std::string& script_base_path, const std::string& script_name, const std::vector<std::string>& args) {
    char cwd[256] = {0};
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        std::cerr << "Error getting current working directory" << std::endl;
        return false;
    }
    if (chdir(script_base_path.c_str()) != 0) {
        std::cerr << "Error changing directory to " << script_base_path << std::endl;
        return false;
    }
    std::string command{"python"};
    for (const auto &s : args) {
        command += (" " + s);
    }
    LOG_INFO("command %s", command.c_str());
    return system(command.c_str()) == 0;
}

#endif