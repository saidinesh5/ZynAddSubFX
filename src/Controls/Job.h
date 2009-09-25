#ifndef JOB_H
#define JOB_H

#include <pthread.h>
#include <iostream>
#include <list>

class Job
{
    public:
        Job();

        virtual void exec() = 0;

        /**
         * Initialize Job class. Should be run some time before using any jobs
         */
        static void initialize();

        /**
         * Inform Job class that the thread calling this function is the engine thread (ie, the
         * thread which will call Job::handleJobs)
         */
        static void setEngineThread();

        /**
         * Push an event to the stack. It will then be handled (probably from another thread) when
         * calling handleJobs.
         */
        static void push(Job* event);
        /**
         * Like Job::push, except this function will not return until the function's exec function
         * has been finished.
         */
        static void pushAndWait(Job* job);
        /**
         * Pop an event from the event list. Will return NULL if there is no event available.
         */
        static Job * pop();
        /**
         * Handle all events in the queue, popping and executing them. After executed, the event
         * will be broadcasted to listeners if exec() returns false.
         */
        static void handleJobs();

    private:
        pthread_cond_t jobExecuted;
        bool isWaitingForSignal;
        static pthread_mutex_t mutex;
        static std::list<Job*> jobs;
        static pthread_t engineThread;

};

#endif // JOB_H
