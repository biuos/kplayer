#ifndef KT_MUTEX_INCLUDE_H
#define KT_MUTEX_INCLUDE_H

#include "Namespace.h"
#include "Macro.h"

#include <pthread.h>
#include <atomic>

NS_KT_START

template <class M>
class ScopedLock {
public:
    explicit ScopedLock(M& mutex): _mutex(mutex) {
        _mutex.lock();
    }

    ~ScopedLock() {
        try {
            _mutex.unlock();
        } catch (...) {
        }
    }

private:
    M& _mutex;

    ScopedLock();
    ScopedLock(const ScopedLock&);
    ScopedLock& operator = (const ScopedLock&);
};


template <class M>
class ScopedLockWithUnlock {
public:
    explicit ScopedLockWithUnlock(M& mutex): _pMutex(&mutex) {
        _pMutex->lock();
    }

    ~ScopedLockWithUnlock() {
        try {
            unlock();
        } catch (...) {
        }
    }

    void unlock() {
        if (_pMutex) {
            _pMutex->unlock();
            _pMutex = nullptr;
        }
    }

private:
    M *_pMutex;

    ScopedLockWithUnlock();
    ScopedLockWithUnlock(const ScopedLockWithUnlock&);
    ScopedLockWithUnlock& operator = (const ScopedLockWithUnlock&);
};



template <class M>
class ScopedUnlock {
public:
    inline ScopedUnlock(M& mutex, bool unlockNow = true): _mutex(mutex) {
        if (unlockNow) {
            _mutex.unlock();
        }
    }
    inline ~ScopedUnlock() {
        try {
            _mutex.lock();
        } catch (...) {
        }
    }

private:
    M& _mutex;

    ScopedUnlock();
    ScopedUnlock(const ScopedUnlock&);
    ScopedUnlock& operator = (const ScopedUnlock&);
};


//class EasyCondition;

class KT_API Mutex {
    //friend class EasyCondition;
public:
    using ScopedLock = ScopedLock<Mutex>;

    enum MutexType {
        MUTEX_NORMAL     = PTHREAD_MUTEX_NORMAL,
        MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK,
        MUTEX_RECURSIVE  = PTHREAD_MUTEX_RECURSIVE,
    };

    Mutex(enum MutexType type = MUTEX_RECURSIVE);
    virtual ~Mutex();

    void lock();
    bool tryLock();
    void unlock();

private:
    pthread_mutex_t _mutex;

//    pthread_mutex_t *get_mutex_t() const {
//        return &_mutex;
//    }

private:
    Mutex(const Mutex&)              = delete;
    Mutex& operator = (const Mutex&) = delete;
};


class KT_API FastMutex {
    //friend class EasyCondition;
public:
    using ScopedLock = ScopedLock<FastMutex>;

    FastMutex();
    virtual ~FastMutex();

    void lock();
    bool tryLock();
    void unlock();

private:
    pthread_mutex_t _mutex;

//    pthread_mutex_t *get_mutex_t() const {
//        return &_mutex;
//    }

private:
    FastMutex(const FastMutex&)              = delete;
    FastMutex& operator = (const FastMutex&) = delete;
};


class KT_API SpinlockMutex {
public:
    using ScopedLock = ScopedLock<SpinlockMutex>;

    SpinlockMutex()  = default;
    ~SpinlockMutex() = default;

    void lock();
    bool tryLock();
    void unlock();
private:
    std::atomic_flag _flag = ATOMIC_FLAG_INIT;
};


NS_KT_END

#endif//KT_MUTEX_INCLUDE_H
