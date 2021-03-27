#ifndef TESTLINKCHECKER_H
#define TESTLINKCHECKER_H

#include <QObject>
#include <actor.h>
#include <actormanager.h>
#include "linkchecker.h"
#include <iostream>

class TestLinkChecker : public Actor
{
    Q_OBJECT
public:
    explicit TestLinkChecker(Actor *parent);
    ~TestLinkChecker();

public Q_SLOTS:
    void result(const QUrl& url, const QVariantList& links);

private:
    LinkChecker* checker_;
};

#endif // TESTLINKCHECKER_H
