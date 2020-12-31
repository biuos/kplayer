#ifndef KT_RUNNABLE_INCLUDE_H
#define KT_RUNNABLE_INCLUDE_H

#include "Namespace.h"
#include "Macro.h"

NS_KT_START

class KT_API Runnable {
public:
    Runnable();
    virtual ~Runnable() = default;

    virtual void run() = 0;

    virtual void abort();

protected:
    bool aborted;
};


inline void Runnable::abort () {
    aborted = true;
}

NS_KT_END

#endif//KT_RUNNABLE_INCLUDE_H
