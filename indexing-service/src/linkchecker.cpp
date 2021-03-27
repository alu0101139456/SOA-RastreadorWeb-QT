#include "linkchecker.h"
#include "testlinkchecker.h"

std::atomic<int> LinkChecker::currentTotalGetters_(0);


LinkChecker::LinkChecker(Actor* parent)
    :Actor(parent)
{
}

LinkChecker::~LinkChecker()
{
}

void LinkChecker::request(const QUrl &url, int depth)
{
    // Store original request info
    originalUrl_ = url;
    originalDepth_ = depth;
    originalDomain_ = getDomain(url);

    createGetterAndRequest(url, depth);

    // Set timeout ids to avoid freezes
    timeoutId_ = startTimer("timeout", GETTER_TIMEOUT);
    getterAvId_ = -1;

}

void LinkChecker::checkUrl(const QUrl &url, int depth)
{
    //TODO: Comprobar si la URL ya a sido procesada por un Getter. En caso afirmativo, lo ignoramos.
    //En caso negativo, crear un HttpGetter y pedirle que procese la URL con depth -1.
    //Si depth == 0, guarda la URL pero no procesarla
//    std::cout << "Checking: " << url.toString().toStdString() << '\n';

    // LinkChecker is updated so timer should be restarted
    restartTimer(timeoutId_);

    // Add url to set and check if it needs to be processed (depth is still not zero and url is within the domain)
    foundUrls_.insert(url);
    if (depth > 0){
        QString domain = getDomain(url);
        if(domain == originalDomain_){

            // If there are no available getters, enqueue the request
            // IOC process it
            if(currentTotalGetters_ >= MAX_HTTPGETTER_INSTANCES){
                pendingUrls_.enqueue(QPair<QUrl, int>(url, depth));
            } else {
                createGetterAndRequest(url, depth);
            }
        }
    }

}

void LinkChecker::done()
{
    // LinkChecker is updated so timer should be restarted
    restartTimer(timeoutId_);

    // Acknowledge Getter has finished
    currentTotalGetters_--;
    currentGetters_--;

    // If there are available getters, start processing queued getters
    // If there are still pending urls and no available getters prepare timer to check again later
    if (!pendingUrls_.isEmpty() && currentTotalGetters_ < MAX_HTTPGETTER_INSTANCES) {
        QPair stored = pendingUrls_.dequeue();
        createGetterAndRequest(stored.first, stored.second);
    } else if (!pendingUrls_.isEmpty() && getterAvId_ == -1){
        // We set a timer to try again
        getterAvId_ = startTimer("check_getter_available", GETTER_TIMEOUT);
    }

    // If all Getters finished we return the results and die
    if(currentGetters_ <= 0){
        returnResults();
        stopTimer(timeoutId_);
        kill();
    }
}



QString LinkChecker::getDomain(const QUrl &url)
{
    // Get domain of url by taking the front of host
    QString domain = "";
    QStringList host = url.host().split('.');
    if (!host.isEmpty()){
        host.pop_front();
        domain = host.join('.');
    }
    return domain;
}

void LinkChecker::createGetterAndRequest(const QUrl &url, int depth)
{
    // Create getter and request to process the url in case it has not been checked yet
    if(!checkedUrls_.contains(url)){
        checkedUrls_.insert(url);
        Getter * getter = spawn<Getter>();
        currentTotalGetters_++;
        currentGetters_++;
        send(getter, "request", url, depth - 1);
    }
}

void LinkChecker::timeout(int timerId){
    std::cout << "There has been a timeout for " << timerId << ", returning results now\n";

    // In case LinkChecker times out it will kill all running children, return the results so far and die
    for(auto child: children()){
        ActorManager::kill(reinterpret_cast<Actor*>(child));
    }
    returnResults();
    kill();

}

void LinkChecker::check_getter_available(int timerId)
{
    std::cout << "Checking again for free getters. Id: " << timerId << '\n';

    // if there are pending urls and available getters will request to process url
    // IOC will restart the timer
    if(!pendingUrls_.isEmpty()) {
        currentTotalGetters_++;
        if(currentTotalGetters_ < MAX_HTTPGETTER_INSTANCES) {
            QPair stored = pendingUrls_.dequeue();
            createGetterAndRequest(stored.first, stored.second);
        }  else {
            currentTotalGetters_--;
            restartTimer(timerId);
        }
    } else {
        // At this point Linkchecker would have finished so we stop timeout
        stopTimer(timerId);
        stopTimer(getterAvId_);
        getterAvId_ = -1;
    }
}

void LinkChecker::returnResults(){

    // Iterates found urls and populates variant list to send to requester
    QList<QVariant> variantList;
    foreach(QUrl url, foundUrls_.values()){
        variantList << url;
    }
    send(reinterpret_cast<Actor*>(parent()), "result", originalUrl_,originalDepth_, variantList);
}

