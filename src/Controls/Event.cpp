#include "Event.h"
#include <pthread.h>

using std::list;

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

        if (event->isWaitingForSignal) {
            pthread_mutex_lock(&mutex);
            pthread_cond_broadcast(&event->eventExecuted);
            pthread_mutex_unlock(&mutex);
            continue; //NOTE: this implies that the event is _not_ deleted
            //automatically if isWaitingForSignal (iow pushAndWait) is used.
            //whether this is good practice or not we will have to decide
        }
        if (finished) {
            delete event;
            continue;
        }

        list<EventUser*>::const_iterator it = users.begin();
        //now, broadcast the event to all eventusers
        while (it != users.end()) {
            if ((*it)->eventFilter(event)) {
                finished = true;
                break;
            }
            it++;
        }
        delete event;
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