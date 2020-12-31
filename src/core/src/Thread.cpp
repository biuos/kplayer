#include "Thread.h"
#include "Logger.h"
#include "Exception.h"
#include "Platform.h"

#include <atomic>
#include <string>
#include <sstream>
#include <climits>
#include <csignal>

NS_KT_START

static const char *TAG = "Thread";

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// inner class
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class CallableHolder: public Runnable, ITerminate {
public:
    CallableHolder(Callable callable, void *pData):
        _callable(callable), _pData(pData) {
    }

    ~CallableHolder() override = default;

    void run() override {
        _callable(_pData, *this);
    }

private:
    bool terminated() override {
        return aborted;
    }

private:
    Callable _callable;
    void    *_pData;
};

class RunnableHolder: public Runnable {
public:
    explicit RunnableHolder(Runnable& target):
        _target(target) {
    }

    ~RunnableHolder() override = default;

    void run() override {
        _target.run();
    }

    void abort() override {
        _target.abort();
    }

private:
    Runnable& _target;
};



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// inner static function
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static std::atomic<uint64_t> counter(0);

static std::string make_thread_name(const char *base_name) {
    uint64_t value = ++counter;
    if(value >= LONG_MAX) {
        counter = 0;
    }
    std::ostringstream name;
    name << "#" << value;
    if(base_name) {
        name << ":" << base_name;
    }
    return name.str();
}

static void set_thread_name(pthread_t tid, const std::string& name) {
#if defined(__MACOSX__) || defined(__IPHONEOS__)
    pthread_setname_np(name.c_str());
#else
    if (pthread_setname_np(tid, name.c_str()) == ERANGE && name.size() > 15) {
        std::string threadName(name, 0, 7);
        threadName.append("~");
        threadName.append(name, name.size() - 7, 7);
        pthread_setname_np(tid, threadName.c_str());
    }
#endif
}

static int map_priority(int priority, int policy) {
    int pmin = sched_get_priority_min(policy);
    int pmax = sched_get_priority_max(policy);

    switch(priority) {
    case Thread::PRIO_LOWEST:
        return pmin;

    case Thread::PRIO_LOW:
        return pmin + (pmax - pmin) / 4;

    case Thread::PRIO_HIGH:
        return pmin + 3 * (pmax - pmin) / 4;

    case Thread::PRIO_HIGHEST:
        return pmax;

    case Thread::PRIO_NORMAL:
    default:
        return pmin + (pmax - pmin) / 2;
    }
}

static int reverse_map_priority(int prio, int policy) {
    if (policy == SCHED_OTHER) {
        int pmin = sched_get_priority_min(policy);
        int pmax = sched_get_priority_max(policy);
        int normal = pmin + (pmax - pmin) / 2;
        if (prio == pmax)
            return Thread::PRIO_HIGHEST;
        if (prio > normal)
            return  Thread::PRIO_HIGH;
        else if (prio == normal)
            return Thread::PRIO_NORMAL;
        else if (prio > pmin)
            return Thread::PRIO_LOW;
        else
            return Thread::PRIO_LOWEST;
    }
    return Thread::PRIO_HIGHEST;
}

Thread::Thread():
    _started{false},
    _joined{false},
    _runnable{},
    _stack_size{0},
    _prio{PRIO_NORMAL},
    _osPrio{PRIO_NORMAL},
    _policy{POLICY_DEFAULT},
    _thread_name{make_thread_name(nullptr)} {
}

Thread::Thread(const char *name):
    _started{false},
    _joined{false},
    _runnable{},
    _stack_size{0},
    _prio{PRIO_NORMAL},
    _osPrio{PRIO_NORMAL},
    _policy{POLICY_DEFAULT},
    _thread_name{make_thread_name(name)} {
}

Thread::~Thread() {
    if(nullptr != _runnable) {
        _runnable = nullptr;
    }
}

void Thread::setStackSize(size_t size) {
    _stack_size = size;
}

void Thread::setPriority(enum Priority prio) {
    _prio = prio;
}

void Thread::setPolicy(enum Policy policy) {
    _policy = policy;
}

void Thread::setOSPriority(int prio, enum Policy policy) {
    if(prio != _osPrio || policy != _policy) {
        if(_runnable != nullptr) {
            struct sched_param par {};
            par.sched_priority = prio;
            if (pthread_setschedparam(_thread, policy, &par)) {
                throw SystemException("cannot set thread priority");
            }
        }
        _prio   = Priority(reverse_map_priority(prio, policy));
        _osPrio = prio;
        _policy = policy;
    }
}

bool Thread::isRunning() const {
    return _runnable != nullptr;
}

void Thread::start(Runnable& target) {
    start(std::shared_ptr<RunnableHolder>(new RunnableHolder(target)));
}

void Thread::start(Callable target, void *data ) {
    start(std::shared_ptr<CallableHolder>(new CallableHolder(target, data)));
}

void Thread::start(const std::shared_ptr<Runnable>& pTarget) {
    if(pTarget == nullptr) {
        LOG_WARN(TAG, "start: bad parameter");
        return;
    }

    if(_runnable != nullptr) {
        throw SystemException("thread already running");
    }

    pthread_attr_t     attributes;
    pthread_attr_init(&attributes);

    if(0 != pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE)) {
        pthread_attr_destroy(&attributes);
        throw SystemException("cannot set thread joinable");
    }

    if(_stack_size > 0) {
        if (0 != pthread_attr_setstacksize(&attributes, _stack_size)) {
            pthread_attr_destroy(&attributes);
            throw SystemException("cannot set thread stack size");
        }
    }

    _runnable = pTarget;
    if (pthread_create(&_thread, &attributes, start_routine, this)) {
        _runnable = nullptr;
        pthread_attr_destroy(&attributes);
        throw SystemException("cannot start thread");
    }
    _started = true;
    pthread_attr_destroy(&attributes);

    if(_policy == SCHED_OTHER) {
        if(_prio != PRIO_NORMAL) {
            struct sched_param par {};
            par.sched_priority = map_priority(_prio, SCHED_OTHER);
            if (pthread_setschedparam(_thread, SCHED_OTHER, &par)) {
                throw SystemException("cannot set thread priority");
            }
        }
    } else {
        struct sched_param par {};
        par.sched_priority = map_priority(_osPrio, _policy);
        if (pthread_setschedparam(_thread, _policy, &par)) {
            throw SystemException("cannot set thread os priority");
        }
    }
}

void Thread::join() {
    if(!_started)
        return;
    void *result = nullptr;
    if(pthread_join(_thread, &result)) {
        throw SystemException("cannot join thread");
    }
    _joined = true;
}

void Thread::terminal() {
    if(_runnable != nullptr) {
        _runnable->abort();
    }
}

void *Thread::start_routine(void *pThread) {
    if(!pThread) {
        LOG_WARN(TAG, "start_routine: bad parameter");
        return pThread;
    }

    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset, SIGQUIT);
    sigaddset(&sset, SIGTERM);
    sigaddset(&sset, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &sset, nullptr);

    auto *p = reinterpret_cast<Thread *>(pThread);
    set_thread_name(p->_thread, std::string(p->name()));

    if(!p->_runnable) {
        LOG_WARN(TAG, "start_routine: no runnable in thread %s", p->name());
        return p;
    }

    try {
        reinterpret_cast<Thread *>(pThread)->_runnable->run();
    } catch (Exception& exc) {
        LOG_WARN(TAG, "start_routine[%s]: run error: %d-%s", p->name(), exc.code(), exc.message().c_str());
    } catch(...) {
        LOG_WARN(TAG, "start_routine[%s]: run at unknown error", p->name());
    }

    p->_runnable = nullptr;

    return p;
}



NS_KT_END
