#include <actormanager.h>
#include <actor.h>
#include "actorthread.h"
#include "mailbox.h"

#include <QDebug>

#include <QThread>


Actor::Actor(Actor* parent)
    : QObject(parent), done_(false)
{

    mailbox_ = std::make_unique<ActorMailBox>();
    thread_ = new ActorThread(this);
    thread_->start();
}

Actor::~Actor() { //para que el mailbox pueda ser opcaco
    if(done_){
        thread_->wait();
    }
    if (!thread_->isFinished()) {
        qCritical() << "Se ha intentado destruir un actor no detenido: " << this;
    }
}

bool Actor::delivery_from(Actor *sender, const QString& message, const QVariant& arg0,
                          const QVariant& arg1, const QVariant& arg2, const QVariant& arg3,
                          const QVariant& arg4, const QVariant& arg5, const QVariant& arg6,
                          const QVariant& arg7, const QVariant& arg8, const QVariant& arg9)
{

    mailbox_->push([this, sender, message, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9]() {
        sender_ = sender;
        bool success = QMetaObject::invokeMethod(this, message.toLocal8Bit(), Qt::DirectConnection,
                                                  QGenericArgument(arg0.typeName(), arg0.data()),
                                                  QGenericArgument(arg1.typeName(), arg1.data()),
                                                  QGenericArgument(arg2.typeName(), arg2.data()),
                                                  QGenericArgument(arg3.typeName(), arg3.data()),
                                                  QGenericArgument(arg4.typeName(), arg4.data()),
                                                  QGenericArgument(arg5.typeName(), arg5.data()),
                                                  QGenericArgument(arg6.typeName(), arg6.data()),
                                                  QGenericArgument(arg7.typeName(), arg7.data()),
                                                  QGenericArgument(arg8.typeName(), arg8.data()),
                                                  QGenericArgument(arg9.typeName(), arg9.data()));
                if(!success) {
                    QMetaObject::invokeMethod(this, "unknownMessage" , Qt::DirectConnection,
                                  QGenericArgument( "QString" , message.data()));
                }



        sender_ = nullptr;
    });

    return true;
}

void Actor::unknownMessage(const QString &message)
{

}

void Actor::kill()
{
    done_ = true;
}

bool Actor::send(Actor *receiver, const QString& message, const QVariant& arg0,
                 const QVariant& arg1, const QVariant& arg2, const QVariant& arg3,
                 const QVariant& arg4, const QVariant& arg5, const QVariant& arg6,
                 const QVariant& arg7, const QVariant& arg8, const QVariant& arg9)
{
    return receiver->delivery_from(this,message, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

bool Actor::reply(const QString& message, const QVariant& arg0,
                  const QVariant& arg1, const QVariant& arg2, const QVariant& arg3,
                  const QVariant& arg4, const QVariant& arg5, const QVariant& arg6,
                  const QVariant& arg7, const QVariant& arg8, const QVariant& arg9)
{
    if (sender_) {
        return send(sender_, message, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    }
    return false;
}

int Actor::startTimer(const QString& message, qint64 timeout)
{
    // Create a new timer
    ActorTimer timer = {timeout, timeout, message};
    int timerId;
    do {
        // Generate random id for Timer until it is unique for this actor
        timerId = QRandomGenerator::global()->generate();
    } while(currentTimers_.contains(timerId));

    // Add timer to set of timers
    currentTimers_.insert(timerId, timer);

    // Return its id to be able to locate it
    return timerId;

}

bool Actor::restartTimer(int id, qint64 timeout)
{
    // If there is a timer currently running and with the specified id reset it to
    //  new timeout or to max time;
    bool idFound = currentTimers_.contains(id);
    if(idFound){
        currentTimers_[id].timeLeft = (timeout < 0)? currentTimers_[id].maxTime: timeout;
    }

    // Returns true if id was found
    return idFound;

}


// we could either flag we dont want to check this timer or just remove it
bool Actor::stopTimer(int id)
{
    return currentTimers_.remove(id);
}

bool Actor::processMessage()
{
    // Get the time before message is processed
    auto startTime = std::chrono::steady_clock::now();

    // Get the next message in the mailbox and process it in case actor is not done
    auto message = mailbox_->pop(std::chrono::milliseconds(100));
    if(!done_ && message) {
        try {
            std::invoke(message.value());
        }
        catch(...) {
            send(reinterpret_cast<Actor*>(parent()) , "failed");
        }
    }

    // Get time after message id processed
    auto endTime = std::chrono::steady_clock::now();

    // Get time  elapsed processing the message
    std::chrono::duration<double> duration = endTime - startTime;
    std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    // Iterate through all timers and check if the trigger. If a timer is trigered we will
    //  store its id to remove it from the actor. (Timers are single shot)
    QList<int> idsToRemove;
    for(auto timerId: currentTimers_.keys()){
        currentTimers_[timerId].timeLeft -= elapsedTime.count();
        if(currentTimers_[timerId].timeLeft <= 0){
            send(this, currentTimers_[timerId].message, timerId);
            idsToRemove.push_back(timerId);
        }
    }

    // Iterate through all timers that need to be removed and remove them
    for(auto key: idsToRemove){
        currentTimers_.remove(key);
    }
    return !done_;
}


