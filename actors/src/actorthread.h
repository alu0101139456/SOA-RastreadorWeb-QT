#ifndef ACTORTHREAD_H
#define ACTORTHREAD_H

#include <QThread>
#include <QObject>
#include <actor.h>

// Thread to exec message
class ActorThread : public QThread
{
    Q_OBJECT
public:
    ActorThread(Actor* actor);

    // Process message
    virtual void run() override;

 private:
    Actor* actor_;
};

#endif // ACTORTHREAD_H
