#ifndef HTTPFETCHPLUGINIMPL_H
#define HTTPFETCHPLUGINIMPL_H

#include <vector>
#include <string>



class HttpFetchPlugin
{
public:
    HttpFetchPlugin();

    std::vector<std::string> supportedSchema();

    std::vector<uint8_t> fetchUrl(const std::string& url);
};

#endif // HTTPFETCHPLUGIN_H
