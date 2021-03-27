#include "testlinkchecker.h"

TestLinkChecker::TestLinkChecker(Actor* parent): Actor(parent){
    checker_ = spawn<LinkChecker>();
    send(checker_, "request", QUrl("http://www.ull.es"), 2);
}

TestLinkChecker::~TestLinkChecker()
{
}

void TestLinkChecker::result(const QUrl &url, const QVariantList &links)
{
    std::cout << url.toString().toStdString() << "\n";
    foreach(QVariant url, links){
        QString urlString = url.toUrl().toString();
        std::cout << urlString.toStdString() << "\n";
    }
    std::cout << "Numbers of Url's found: " << links.size() << '\n';
    kill();
}
