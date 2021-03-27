#ifndef DYNAMICLIBRARY_H
#define DYNAMICLIBRARY_H

#include <dlfcn.h>
#include <string>
#include <memory>
#include <optional>


// Manages dinamilibrary loading to support plugins
class DynamicLibrary
{
public:
    // Loads dinamic library handler
    DynamicLibrary(const std::string& path);

    template <typename Type>
    std::optional<Type> resolve (const std::string& symbol);

private:
    std::unique_ptr<void, decltype(&dlclose)> handle_;
};

template<typename Type>
std::optional<Type> DynamicLibrary::resolve(const std::string &symbol)
{
    static_assert (std::is_pointer<Type>::value);

    dlerror();
    void* address = dlsym(handle_.get(), symbol.c_str());
    if (address == nullptr && dlerror() != nullptr) {
        return{};
    }
    return reinterpret_cast<Type>(address);
}

#endif // DYNAMICLIBRARY_H
