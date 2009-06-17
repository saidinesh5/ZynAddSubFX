#include "Event.h"

using std::list;

bool Event::mutex_inited = false;
list<Event*> Event::events;
pthread_mutex_t Event::mutex;

Event::Event()
{
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

void Event::initializeMutex()
{
    if (!mutex_inited) {
        pthread_mutex_init(&mutex,NULL);
        mutex_inited = true;
    }
}

// vim: sw=4 sts=4 et tw=100
