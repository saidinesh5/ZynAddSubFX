
#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <list>

class EventUser
{
    public:
        /**
         * Handle an incoming event. If this function returns true then the event will be noted as
         * handled, and no other EventUsers will receive it.
         */
        virtual bool eventFilter(class Event *event) = 0;
};

class Event
{
    public:
        Event();
        /**
         * The function doing the main work in this event. Will be executed in the main thread.
         * If it returns true, the event will simply be deleted after execution. If it returns false
         * then it will be broadcasted to EventUsers.
         */
        virtual bool exec();

        /**
         * Initialize mutexes. Should be run some time before using any events
         */
        static void initializeMutex();
        /**
         * Push an event to the stack. It will then be handled (probably from another thread) when
         * calling handleEvents.
         */
        static void push(Event* event);
        /**
         * Like Event::push, except this function will not return until the function's exec function
         * has been finished. NOTE: When using this method, the event will NOT be deleted
         * automatically after being executed.
         */
        static bool pushAndWait(Event* event);
        /**
         * Pop an event from the event list. Will return NULL if there is no event available.
         */
        static Event * pop();
        /**
         * Register an EventUser class (ie, a class implementing the virtual eventFilter method)
         * to receive events.
         */
        static void registerUser(EventUser *user);
        /**
         * Handle all events in the queue, popping and executing them. After executed, the event
         * will be broadcasted to listeners if exec() returns false.
         */
        static void handleEvents();

    private:
        pthread_cond_t eventExecuted;
        bool isWaitingForSignal;

        static std::list<class Event*> events;
        static std::list<EventUser*> users;
        static pthread_mutex_t mutex;

};

#endif // EVENT_H
