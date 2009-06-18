#include "Event.h"

using std::list;

bool Event::mutex_inited = false;
list<Event*> Event::events;
list<EventUser*> Event::users;
pthread_mutex_t Event::mutex;

Event::Event()
{
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

void Event::initializeMutex()
{
    if (!mutex_inited) {
        pthread_mutex_init(&mutex,NULL);
        mutex_inited = true;
    }
}
// vim: sw=4 sts=4 et tw=100
