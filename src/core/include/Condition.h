#ifndef KT_CONDITION_INCLUDE_H
#define KT_CONDITION_INCLUDE_H

#include "Namespace.h"
#include "Macro.h"
#include "Mutex.h"
#include "Exception.h"
#include "Event.h"

#include <deque>
#include <ctime>
#include <cerrno>

NS_KT_START

class KT_API Condition {
public:
    Condition();
    ~Condition();

    template <class Mtx>
    void wait(Mtx& mutex) {
        ScopedUnlock<Mtx> unlock(mutex, false);
        Event event;
        {
            FastMutex::ScopedLock lock(_mutex);
            mutex.unlock();
            enqueue(event);
        }
        event.wait();
    }

    template <class Mtx>
    void wait(Mtx& mutex, long milliseconds) {
        if (!tryWait(mutex, milliseconds)) {
            throw TimeoutException();
        }
    }

    template <class Mtx>
    bool tryWait(Mtx& mutex, long milliseconds) {
        ScopedUnlock<Mtx> unlock(mutex, false);
        Event event;
        {
            FastMutex::ScopedLock lock(_mutex);
            mutex.unlock();
            enqueue(event);
        }
        if (!event.tryWait(milliseconds)) {
            FastMutex::ScopedLock lock(_mutex);
            dequeue(event);
            return false;
        }
        return true;
    }

    void signal();
    void broadcast();

protected:
    void enqueue(Event& event);
    void dequeue();
    void dequeue(Event& event);

private:
    Condition(const Condition&);
    Condition& operator = (const Condition&);

    typedef std::deque<Event *> WaitQueue;

    FastMutex _mutex;
    WaitQueue _waitQueue;
};


NS_KT_END

#endif//KT_CONDITION_INCLUDE_H
