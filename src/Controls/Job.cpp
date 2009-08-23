#include "Job.h"

using std::list;
using std::cout;

pthread_mutex_t Job::mutex;
list<Job*> Job::jobs;
pthread_t Job::engineThread;

Job::Job()
        : isWaitingForSignal(false)
{
    pthread_cond_init(&jobExecuted, NULL);
}

void Job::handleJobs()
{
    Job *job = NULL;
    while (job = Job::pop()) {

        //this must be cached, since the job might be deleted
        //during the function that's waiting for the job (ie pushAndWait)
        bool isWaitingForSignal = job->isWaitingForSignal;

        job->exec();
        if (job->isWaitingForSignal) {
            pthread_mutex_lock(&mutex);
            pthread_cond_broadcast(&job->jobExecuted);
            pthread_mutex_unlock(&mutex);
        }
        if (!isWaitingForSignal)
            delete job;
    }
}

Job* Job::pop()
{
    if (jobs.empty()) {
        return NULL;
    }
    Job *jb = NULL;
    pthread_mutex_lock(&mutex);
    jb = jobs.front();
    jobs.pop_front();
    pthread_mutex_unlock(&mutex);

    return jb;
}

void Job::push(Job* event)
{
    pthread_mutex_lock(&mutex);
    jobs.push_back(event);
    pthread_mutex_unlock(&mutex);
}

void Job::pushAndWait(Job* job)
{
    if (pthread_equal(pthread_self(), engineThread)) {
        //this is called from the engine thread. just execute the job directly from here
        job->exec();
        delete job;
        return;
    }

    pthread_mutex_lock(&mutex);

    job->isWaitingForSignal = true;
    jobs.push_back(job);

    pthread_cond_wait(&job->jobExecuted, &mutex);
    //execution now finished
    delete job;

    pthread_mutex_unlock(&mutex);
}

void Job::initialize()
{
    static bool inited = false;
    if (!inited) {
        pthread_mutex_init(&mutex,NULL);
        engineThread = pthread_self();
        inited = true;
    }
}


// vim: sw=4 sts=4 et tw=100
