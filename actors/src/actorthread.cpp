#include "actorthread.h"

ActorThread::ActorThread(Actor* actor)
    :QThread(actor), actor_(actor)
{
    // We need to connect the signal when the thread is created
    // to ensure that deleteLater is called on thread finished so that destruction order is kept
    connect(this, SIGNAL(finished()), actor, SLOT(deleteLater()));
}

void ActorThread::run()
{
    while (actor_->processMessage());
}
