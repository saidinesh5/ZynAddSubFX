
#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <list>

class EventUser
{
    public:
        virtual bool eventFilter(class Event *event) = 0;
};

class Event
{
    public:
        Event();
        virtual bool exec();

        static void initializeMutex();
        static void push(Event* event);
        static Event * pop();
        static void registerUser(EventUser *user);
        static void handleEvents();

    private:
        static std::list<class Event*> events;
        static std::list<EventUser*> users;
        static bool mutex_inited;
        static pthread_mutex_t mutex;

};

#endif // EVENT_H
