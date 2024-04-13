#ifndef UTIL_H
#define UTIL_H

#include "pybind11/embed.h"
#include <iostream>
#include <string>

namespace py = pybind11;

bool ex_py(const std::string& script_name, const std::vector<std::string>& args) {
    py::scoped_interpreter guard{}; // Start the Python interpreter

    // Convert C++ string vector to Python list
    py::list py_args;
    for (const auto& arg : args) {
        py_args.append(arg);
    }

    // Execute the script with arguments
    py::module::import("sys").attr("argv") = py_args;
    py::eval_file(script_name.c_str());
    std::cout << "Python script executed successfully." << std::endl;

    return true;
}

#endif