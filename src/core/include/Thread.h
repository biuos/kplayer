#ifndef KT_THREAD_INCLUDE_H
#define KT_THREAD_INCLUDE_H

#include "Namespace.h"
#include "Macro.h"
#include "Runnable.h"

#include <stddef.h>
#include <pthread.h>
#include <string>
#include <memory>

NS_KT_START

class KT_API ITerminate {
public:
    ITerminate()          = default;
    virtual ~ITerminate() = default;

    virtual bool terminated() = 0;

private:
    ITerminate(const ITerminate&)            = delete;
    ITerminate& operator=(const ITerminate&) = delete;
};

typedef void (*Callable)(void *data, const ITerminate& abort);

class KT_API Thread {
public:
    enum Priority {
        PRIO_LOWEST,
        PRIO_LOW,
        PRIO_NORMAL,
        PRIO_HIGH,
        PRIO_HIGHEST
    };

    enum Policy {
        POLICY_DEFAULT = SCHED_OTHER, // timesharing
        POLICY_FIFO    = SCHED_FIFO,  // first in, first out
        POLICY_RR      = SCHED_RR,    // round-robin
    };

public:
    Thread();
    explicit Thread(const char *name);
    ~Thread();

    void setStackSize(size_t size);
    void setPriority(enum Priority prio);
    void setPolicy(enum Policy policy);
    void setOSPriority(int prio, enum Policy policy = POLICY_DEFAULT);

    const char *name() const;

    void start(Runnable& target);
    void start(const std::shared_ptr<Runnable>& pTarget);
    void start(Callable target, void *data = nullptr);

    void join();

    bool isRunning() const;

    void terminal();

    /**
     * Yields cpu to other threads.
     */
    static void yield();

private:
    size_t                    _stack_size;
    pthread_t                 _thread{};
    std::string               _thread_name;
    enum Priority             _prio;
    int                       _osPrio;
    enum Policy               _policy;
    std::shared_ptr<Runnable> _runnable;
    bool                      _started;
    bool                      _joined;

private:
    static void *start_routine(void *pthread);
};


inline const char *Thread::name() const {
    return _thread_name.c_str();
}

inline void Thread::yield() {
    sched_yield();
}


NS_KT_END

#endif//KT_THREAD_INCLUDE_H
