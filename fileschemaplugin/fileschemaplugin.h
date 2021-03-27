#ifndef FILESCHEMAPLUGIN_H
#define FILESCHEMAPLUGIN_H

#include <schemaplugin.h>
#include <QMimeDatabase>
#include <QMimeType>
#include <QFile>

#include <iostream>

// Plugin in charge of retrieving file info from local directory
class  FileSchemaPlugin : public SchemaPlugin
{
public:
    FileSchemaPlugin();

    // Opens file or directory and dumps info into buffer to be processed
    virtual std::optional<FetchInfo> fetchUrl(const std::string& url) override;

    // Indicates schemas supported by this plugin: file
    virtual std::vector<std::string> schemaSupported () override;

    virtual bool isSchema() override { return true; }
};

#endif // FILESCHEMAPLUGIN_H
