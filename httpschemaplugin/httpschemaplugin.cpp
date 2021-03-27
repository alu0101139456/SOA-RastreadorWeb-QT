#include <curl/curl.h>

#include "httpschemaplugin.h"
#include <iostream>
#include <QTextStream>

std::atomic_int HttpSchemaPlugin::instanceCounter_ = 0;

HttpSchemaPlugin::HttpSchemaPlugin()
{
    if(instanceCounter_.fetch_add(1, std::memory_order_acquire) == 0) {
        curl_global_init(CURL_GLOBAL_ALL);
    }
}

HttpSchemaPlugin::~HttpSchemaPlugin()
{
    if(instanceCounter_.fetch_sub(1, std::memory_order_acquire) == 1) {
        curl_global_cleanup();
    }
}

std::optional<SchemaPlugin::FetchInfo> HttpSchemaPlugin::fetchUrl(const std::string &url) {
    // Create necesary data structures
    FetchInfo info;
    info.content = new QBuffer();
    info.content->open(QIODevice::ReadWrite);

    // Initialize curl
    CURL* curl_handle = curl_easy_init();

    // Configure curl request
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());                    // Url to fetch
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);                         // Disabel verbose mode
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);                  // Follow locations
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curlWriteCallback);    // Connect callback
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &info);                    // Set write Data structure


    // Perform the request. If it fails return no data
    auto result = curl_easy_perform(curl_handle);
    if( result != CURLE_OK ) {
        return {};
    }

    // Read content type from curl
    char* contentType = nullptr;
    curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_TYPE, &contentType);
    info.contentType = contentType;

    curl_easy_cleanup(curl_handle);

//    info.content->close();

    return info;
}

std::vector<std::string> HttpSchemaPlugin::schemaSupported() {
    static std::vector<std::string> schemas = {
        "http",
        "https"
    };
    return schemas;
}

size_t HttpSchemaPlugin::curlWriteCallback(char *ptr, size_t, size_t nmemb, void *userdata)
{
    // If there is info to be written dump it on stream
    auto info = static_cast<FetchInfo*>(userdata);
    if(nmemb != 0) {
        QTextStream text(info->content);
        text << ptr;
    }
    return nmemb;
}

SchemaPlugin* loadPlugin() {
    return new HttpSchemaPlugin;
}
