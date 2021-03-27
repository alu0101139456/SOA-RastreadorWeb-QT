//#include <dlfcn.h>

#include "dynamiclibrary.h"

DynamicLibrary::DynamicLibrary(const std::string &path)
                : handle_(nullptr, dlclose)
{
    handle_.reset(dlopen(path.c_str(), RTLD_LAZY));
    if (!handle_) {
        std::string msg = "DynamicLibrary: ";
        throw std::runtime_error(msg + std::string(dlerror()));
    }
}
