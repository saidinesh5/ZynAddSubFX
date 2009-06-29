#include "Event.h"
#include <pthread.h>

using std::list;
using std::cout;

list<Event*> Event::events;
list<EventUser*> Event::users;
pthread_mutex_t Event::mutex;

Event::Event()
        : isWaitingForSignal(false)
{
    //eventExecuted = PTHREAD_COND_INITIALIZER;
    pthread_cond_init(&eventExecuted, NULL);
}

void Event::registerUser(EventUser *user)
{
    pthread_mutex_lock(&mutex);
    users.push_back(user);
    pthread_mutex_unlock(&mutex);
}

void Event::handleEvents()
{
    Event *event = NULL;
    while (event = Event::pop()) {

        //first, run the events exec function. if it returns true then this event is done and can be
        //deleted.
        bool finished = event->exec();
        bool wasWaitingForSignal = false;

        if (event->isWaitingForSignal) {
            pthread_mutex_lock(&mutex);
            wasWaitingForSignal = true;
            pthread_cond_broadcast(&event->eventExecuted);
            pthread_mutex_unlock(&mutex);
        }
        if (finished) {
            delete event;
            continue;
        }

        if (!wasWaitingForSignal) {

            list<EventUser*>::const_iterator it = users.begin();
            //now, broadcast the event to all eventusers
            while (it != users.end()) {
                if ((*it)->eventFilter(event)) {
                    finished = true;
                    break;
                }
                it++;
            }

            std::cout << event << "Deleting event\n";
            delete event;
        }
    }
}

bool Event::exec()
{
    return false;
}

Event* Event::pop()
{
    if (events.empty()) {
        return NULL;
    }
    Event *ev = NULL;
    pthread_mutex_lock(&mutex);
    ev = events.front();
    events.pop_front();
    pthread_mutex_unlock(&mutex);

    return ev;
}

void Event::push(Event* event)
{
    pthread_mutex_lock(&mutex);
    events.push_back(event);
    pthread_mutex_unlock(&mutex);
}

bool Event::pushAndWait(Event* event)
{
    pthread_mutex_lock(&mutex);

    event->isWaitingForSignal = true;
    std::cout << event << "set to true";
    events.push_back(event);

    pthread_cond_wait(&event->eventExecuted, &mutex);
    //execution now finished

    pthread_mutex_unlock(&mutex);
}

void Event::initializeMutex()
{
    static bool mutex_inited = false;
    if (!mutex_inited) {
        pthread_mutex_init(&mutex,NULL);
        mutex_inited = true;
    }
}

// vim: sw=4 sts=4 et tw=100
