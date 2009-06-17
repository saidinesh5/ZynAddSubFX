
#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <list>

class Event
{
    public:
        Event();
        virtual void exec() = 0;

        static void initializeMutex();
        static void push(Event* event);
        static Event * pop();

    private:

        static std::list<class Event*> events;
        static bool mutex_inited;
        static pthread_mutex_t mutex;

};

class HelloWorldEvent : public Event
{
    public:
        HelloWorldEvent() : Event() {}
        void exec() {
            std::cout << "Hello world!\n";
        }
};

#endif // EVENT_H
