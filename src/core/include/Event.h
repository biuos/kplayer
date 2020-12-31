#ifndef KT_EVENT_INCLUDE_H
#define KT_EVENT_INCLUDE_H

#include "Namespace.h"
#include "Macro.h"

#include <pthread.h>

NS_KT_START

class KT_API Event {
public:
    enum EventType {
        EVENT_MANUALRESET, // Manual reset event
        EVENT_AUTORESET    // Auto-reset event
    };

    explicit Event(EventType type = EVENT_AUTORESET);
    ~Event();

    void set();
    void reset();

    void wait();
    void wait(long milliseconds);
    bool tryWait(long milliseconds);

private:
    EventType       _type;
    volatile bool   _state;
    pthread_mutex_t _mutex;
    pthread_cond_t  _cond;

private:
    Event(const Event&);
    Event& operator = (const Event&);
};


NS_KT_END

#endif//KT_EVENT_INCLUDE_H

