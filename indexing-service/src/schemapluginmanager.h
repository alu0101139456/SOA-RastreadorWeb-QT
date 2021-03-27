#ifndef SCHEMAPLUGINMANAGER_H
#define SCHEMAPLUGINMANAGER_H

#include <memory>
#include <vector>
#include <map>
#include <schemaplugin.h>
#include "dynamiclibrary.h"
#include <string>


// Loads all available plugins and provides Getter the right plugin depending on
//  url schema and filetype.
class SchemaPluginManager
{
public:
    using LoadPluginFunctionPointer = decltype (&loadPlugin);

    // Returns instance of plugin manager to be used. Prevents user from creating
    //  multiple managers
    static const SchemaPluginManager& instance();

    // Returns plugin for given schema
    std::unique_ptr<SchemaPlugin> getSchemaPlugin(const std::string& schema) const;

    // Returns plugin for given filetype
    std::unique_ptr<SchemaPlugin> getProcessorPlugin(const std::string& filetype) const;

private:

    std::map<std::string, LoadPluginFunctionPointer> schemas_;   // Stores schema plugins
    std::map<std::string, LoadPluginFunctionPointer> fileTypes_; // Stores processor plugins
    std::vector<DynamicLibrary> libraries_; // Stores plugin dinamic libraries

    // Only one manager in execution
    SchemaPluginManager();
    SchemaPluginManager (const SchemaPluginManager&) = delete;
    SchemaPluginManager& operator= (const SchemaPluginManager&) = delete;
};

#endif // SCHEMAPLUGINMANAGER_H
