#include "Condition.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif


#ifdef WIN32
Condition::Condition() :
    Event(nullptr)
{
    Event = CreateEvent(0, FALSE, FALSE, 0);
}

Condition::~Condition()
{
    CloseHandle((HANDLE)Event);
    Event = nullptr;
}

void Condition::Set()
{
    SetEvent((HANDLE)Event);
}

void Condition::Wait()
{
    WaitForSingleObject((HANDLE)Event, INFINITE);
}


#else

Condition::Condition() :
    Mutex(new pthread_mutex_t),
    Event(new pthread_cond_t)
{
    pthread_mutex_init((pthread_mutex_t*)Mutex, 0);
    pthread_cond_init((pthread_cond_t*)Event, 0);
}

Condition::~Condition()
{
    pthread_cond_t* cond = (pthread_cond_t*)Event;
    pthread_mutex_t* mutex = (pthread_mutex_t*)Mutex;
    
    pthread_cond_destroy(cond);
    pthread_mutex_destroy(mutex);
    delete cond;
    delete mutex;
    Event = Mutex = nullptr;
}

void Condition::Set()
{
    pthread_cond_signal((pthread_cond_t*)Event);
}

void Condition::Wait()
{
    pthread_cond_t* cond = (pthread_cond_t*)Event;
    pthread_mutex_t* mutex = (pthread_mutex_t*)Event;
    
    pthread_mutex_lock(mutex);
    pthread_cond_wait(cond, mutex);
    pthread_mutex_unlock(mutex);
}
#endif

