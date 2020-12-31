#include "Event.h"
#include "Exception.h"
#include "Platform.h"

#include <cerrno>
#include <ctime>


NS_KT_START

Event::Event(EventType type): _type{type}, _state{false} {
    if (pthread_mutex_init(&_mutex, nullptr)) {
        throw SystemException("cannot create event (mutex)");
    }

    pthread_condattr_t attr;
    if (pthread_condattr_init(&attr)) {
        pthread_mutex_destroy(&_mutex);
        throw SystemException("cannot create event (condition attribute)");
    }
    if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC)) {
        pthread_condattr_destroy(&attr);
        pthread_mutex_destroy   (&_mutex);
        throw SystemException("cannot create event (condition attribute clock)");
    }
    if (pthread_cond_init(&_cond, &attr)) {
        pthread_condattr_destroy(&attr);
        pthread_mutex_destroy   (&_mutex);
        throw SystemException("cannot create event (condition)");
    }
    pthread_condattr_destroy(&attr);
}

Event::~Event() {
    pthread_cond_destroy (&_cond);
    pthread_mutex_destroy(&_mutex);
}

void Event::set() {
    if (pthread_mutex_lock(&_mutex)) {
        throw SystemException("cannot signal event (lock)");
    }
    _state = true;
    if (pthread_cond_broadcast(&_cond)) {
        pthread_mutex_unlock(&_mutex);
        throw SystemException("cannot signal event");
    }
    pthread_mutex_unlock(&_mutex);
}

void Event::reset() {
    if (pthread_mutex_lock(&_mutex)) {
        throw SystemException("cannot reset event");
    }
    _state = false;
    pthread_mutex_unlock(&_mutex);
}

void Event::wait() {
    if (pthread_mutex_lock(&_mutex)) {
        throw SystemException("wait for event failed (lock)");
    }

    while (!_state) {
        if (pthread_cond_wait(&_cond, &_mutex)) {
            pthread_mutex_unlock(&_mutex);
            throw SystemException("wait for event failed");
        }
    }
    if (_type == EVENT_AUTORESET) {
        _state = false;
    }
    pthread_mutex_unlock(&_mutex);
}

bool Event::tryWait(long milliseconds) {
    int rc = 0;
    struct timespec abstime {};

    clock_gettime(CLOCK_MONOTONIC, &abstime);
    abstime.tv_sec  += milliseconds / 1000;
    abstime.tv_nsec += (milliseconds % 1000) * 1000000;
    if (abstime.tv_nsec >= 1000000000) {
        abstime.tv_nsec -= 1000000000;
        abstime.tv_sec++;
    }

    if (pthread_mutex_lock(&_mutex) != 0) {
        throw SystemException("wait for event failed (lock)");
    }
    while (!_state) {
        if ((rc = pthread_cond_timedwait(&_cond, &_mutex, &abstime))) {
            if (rc == ETIMEDOUT){
                break;
            }
            pthread_mutex_unlock(&_mutex);
            throw SystemException("cannot wait for event");
        }
    }
    if (rc == 0 && _type == EVENT_AUTORESET) {
        _state = false;
    }
    pthread_mutex_unlock(&_mutex);
    return rc == 0;
}

void Event::wait(long milliseconds) {
    tryWait(milliseconds);
}


NS_KT_END
