#include "schemapluginmanager.h"
#include <iostream>

using namespace std::literals;

const auto LOADPLUGIN_SYMBOL_NAME = "loadPlugin"s;

const SchemaPluginManager &SchemaPluginManager::instance()
{
    static SchemaPluginManager instance;
    return instance;
}

std::unique_ptr<SchemaPlugin> SchemaPluginManager::getSchemaPlugin(const std::string &schema) const
{
    // Search for a plugin for a given schema. if found it is returned
    auto iter = schemas_.find(schema);
    if (iter == schemas_.end()) {
        return nullptr;
    }
    return std::unique_ptr<SchemaPlugin>((iter->second)());
}

std::unique_ptr<SchemaPlugin> SchemaPluginManager::getProcessorPlugin(const std::string &filetype) const
{
    // Search for a plugin for a given filetype. if found it is returned.
    auto iter = fileTypes_.find(filetype);
    if (iter == fileTypes_.end()) {
        return nullptr;
    }
    return std::unique_ptr<SchemaPlugin>((iter->second)());

}


SchemaPluginManager::SchemaPluginManager()
{
    try {
        // Get all plugins into the manager
        libraries_.emplace_back("../httpschemaplugin/libhttpschemaplugin.so");
        libraries_.emplace_back("../htmlProcessorPlugin/libhtmlProcessorPlugin.so");
        libraries_.emplace_back("../directoryProcessorPlugin/libdirectoryProcessorPlugin.so");
        libraries_.emplace_back("../fileschemaplugin/libfileschemaplugin.so");
    } catch (std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return;
    }

    // Process each plugin
    for ( auto it = libraries_.begin(); it != libraries_.end(); it++ ) {

        // Load. If not succesfull remove it from the list and finish execution.
        auto loadPlugin = it->resolve<LoadPluginFunctionPointer>(LOADPLUGIN_SYMBOL_NAME);
        if (!loadPlugin) {

            libraries_.pop_back();
            return;                 // Could be changed to continue?
        }

        // Load handler
        auto plugin = (loadPlugin.value())();

        // Emplace plugin into the right category ( schema or processor)
        if (plugin->isSchema()) {
            for (auto schema : plugin->schemaSupported()) {
                schemas_.emplace(schema, loadPlugin.value());
            }
        }
        else {
            for (auto schema : plugin->fileSupported()) {
                fileTypes_.emplace(schema, loadPlugin.value());
            }

        }
    }

}
