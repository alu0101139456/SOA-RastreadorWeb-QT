#ifndef HTTPSCHEMAPLUGIN_H
#define HTTPSCHEMAPLUGIN_H

#include <schemaplugin.h>
#include <atomic>
#include <iostream>

// Plugin in charge of retrieving http info from html file
class HttpSchemaPlugin : public SchemaPlugin
{
public:
    HttpSchemaPlugin();
    virtual ~HttpSchemaPlugin();

    // Fetch url and dumps file info into buffer to be processed
    virtual std::optional<FetchInfo> fetchUrl(const std::string& url) override;

    // Indicates schemas supported by this plugin: http and https
    virtual std::vector<std::string> schemaSupported () override;


    virtual bool isSchema() override { return true; }

private:
    static std::atomic_int instanceCounter_;

    // Write callback to populate buffer once file is fetched
    static size_t curlWriteCallback(char* ptr, size_t size, size_t nmemb, void *userdata);


};

#endif // HTTPSCHEMAPLUGIN_H
