#ifndef HTMLPROCESSORPLUGIN_H
#define HTMLPROCESSORPLUGIN_H


#include <schemaplugin.h>
// gumbo-query
#include "Document.h"
#include "Node.h"
#include <iostream>

// Plugin in charge of retrieving url from html file input in buffer
class HtmlProcessorPlugin : public SchemaPlugin
{
public:
    HtmlProcessorPlugin();


    // Parses file and retrieves url from it
    virtual QList<QUrl> extractUrls(const QUrl &url, QIODevice *type) override;

    // Indicates files supported by this plugin <text/html; charset=UTF-8> and <text/html>
    virtual std::vector<std::string> fileSupported () override;

    virtual bool isSchema() override { return false; }

};

#endif // HTMLPROCESSORPLUGIN_H
