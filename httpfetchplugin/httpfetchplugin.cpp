#include "httpfetchplugin.h"
#include "httpfetchpluginimpl.h"

HttpFetchPlugin::HttpFetchPlugin()
{
}

std::vector<std::string> HttpFetchPlugin::supportedSchema()
{

}

std::vector<uint8_t> HttpFetchPlugin::fetchUrl(const std::string &url)
{

}



char **supportedSchema(HttpFetchPluginC *plugin)
{
    auto list = plugin->supportedSchema();
    //TODO Convert list en char**
}

char **fetchUrl(HttpFetchPluginC *plugin, const char *url)
{
    auto bytes = plugin->fetchUrl(std::string(url));
    //TODO Convert bytes to char**
}
HttpFetchPluginC *makeHttpFetchPlugin()
{
    auto C = new HttpFetchPluginC();
    C->fetchUrl = &fetchUrl;
    C->supportedSchema = &supportedSchema;
}
void destroyHttpFetchPlugin(HttpFetchPlugin *plugin)
{
    delete plugin;
}
