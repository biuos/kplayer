#include "Mutex.h"
#include "Exception.h"

#include <cerrno>

NS_KT_START

Mutex::Mutex(enum MutexType type) {
    pthread_mutexattr_t     attr;
    pthread_mutexattr_init(&attr);
#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
    pthread_mutexattr_settype_np(&attr, type);
#else
    pthread_mutexattr_settype(&attr, type);
#endif
    if (pthread_mutex_init(&_mutex, &attr)) {
        pthread_mutexattr_destroy(&attr);
        throw SystemException("cannot create mutex");
    }
    pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&_mutex);
}

void Mutex::lock() {
    if (pthread_mutex_lock(&_mutex)) {
        throw SystemException("cannot lock mutex");
    }
}

bool Mutex::tryLock() {
    int rc = pthread_mutex_trylock(&_mutex);
    if (rc == 0) {
        return true;
    } else if (rc == EBUSY) {
        return false;
    } else {
        throw SystemException("cannot tryLock mutex");
    }
}

void Mutex::unlock() {
    if (pthread_mutex_unlock(&_mutex)) {
        throw SystemException("cannot unlock mutex");
    }
}

FastMutex::FastMutex() {
    pthread_mutexattr_t     attr;
    pthread_mutexattr_init(&attr);
#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
    pthread_mutexattr_settype_np(&attr, type);
#else
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
#endif
    if (pthread_mutex_init(&_mutex, &attr)) {
        pthread_mutexattr_destroy(&attr);
        throw SystemException("cannot create mutex");
    }
    pthread_mutexattr_destroy(&attr);
}

FastMutex::~FastMutex() {
    pthread_mutex_destroy(&_mutex);
}

void FastMutex::lock() {
    if (pthread_mutex_lock(&_mutex)) {
        throw SystemException("cannot lock fast mutex");
    }
}

bool FastMutex::tryLock() {
    int rc = pthread_mutex_trylock(&_mutex);
    if (rc == 0) {
        return true;
    } else if (rc == EBUSY) {
        return false;
    } else {
        throw SystemException("cannot tryLock fast mutex");
    }
}

void FastMutex::unlock() {
    if (pthread_mutex_unlock(&_mutex)) {
        throw SystemException("cannot unlock fast mutex");
    }
}


void SpinlockMutex::lock() {
    while (_flag.test_and_set(std::memory_order_acquire));
}

bool SpinlockMutex::tryLock() {
    return !_flag.test_and_set(std::memory_order_acquire);
}

void SpinlockMutex::unlock() {
    _flag.clear(std::memory_order_release);
}


NS_KT_END
