#ifndef KT_EXCEPTION_INCLUDE_H
#define KT_EXCEPTION_INCLUDE_H

#include "Namespace.h"
#include "Macro.h"

#include <stdexcept>
#include <string>

NS_KT_START


class KT_API Exception: public std::exception {
public:
    Exception(const std::string& msg, int code = 0);
    Exception(const std::string& msg, const std::string& arg, int code = 0);
    Exception(const std::string& msg, const Exception& nested, int code = 0);

    ~Exception() noexcept;

    Exception(const Exception& exc);
    Exception& operator = (const Exception& exc);

    virtual const char *name() const noexcept;
    virtual const char *className() const noexcept;
    virtual const char *what() const noexcept;

    const Exception *nested() const;

    const std::string& message() const;

    int code() const;

    std::string displayText() const;

    virtual Exception *clone() const;

    virtual void rethrow() const;

protected:
    Exception(int code = 0);

    void message(const std::string& msg);
    void extendedMessage(const std::string& arg);

private:
    std::string _msg;
    Exception  *_pNested;
    int         _code;
};


inline const Exception *Exception::nested() const {
    return _pNested;
}

inline const std::string& Exception::message() const {
    return _msg;
}

inline void Exception::message(const std::string& msg) {
    _msg = msg;
}

inline int Exception::code() const {
    return _code;
}


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
#define KT_DECLARE_EXCEPTION_CODE(API, CLS, BASE, CODE)                       \
    class API CLS: public BASE                                                  \
    {                                                                           \
    public:                                                                     \
        CLS(int code = CODE);                                                   \
        CLS(const std::string& msg, int code = CODE);                           \
        CLS(const std::string& msg, const std::string& arg, int code = CODE);   \
        CLS(const std::string& msg, const kt::Exception& exc, int code = CODE); \
        CLS(const CLS& exc);                                                    \
        ~CLS() noexcept;                                                        \
        CLS& operator = (const CLS& exc);                                       \
        const char* name() const noexcept;                                      \
        const char* className() const noexcept;                                 \
        kt::Exception* clone() const;                                           \
        void rethrow() const;                                                   \
    };

#define KT_DECLARE_EXCEPTION(API, CLS, BASE) KT_DECLARE_EXCEPTION_CODE(API, CLS, BASE, 0)

#define KT_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)                                                   \
    CLS::CLS(int code): BASE(code) {}                                                             \
    CLS::CLS(const std::string& msg, int code): BASE(msg, code) {}                                \
    CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code) {}   \
    CLS::CLS(const std::string& msg, const kt::Exception& exc, int code): BASE(msg, exc, code) {} \
    CLS::CLS(const CLS& exc): BASE(exc) {}                                                        \
    CLS::~CLS() noexcept {}                                                                       \
    CLS& CLS::operator = (const CLS& exc) {                                                       \
        BASE::operator = (exc);                                                                   \
        return *this;                                                                             \
    }                                                                                             \
    const char* CLS::name() const noexcept {                                                      \
        return NAME;                                                                              \
    }                                                                                             \
    const char* CLS::className() const noexcept {                                                 \
        return typeid(*this).name();                                                              \
    }                                                                                             \
    kt::Exception* CLS::clone() const {                                                           \
        return new CLS(*this);                                                                    \
    }                                                                                             \
    void CLS::rethrow() const {                                                                   \
        throw *this;                                                                              \
    }


//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Standard exception classes
//
KT_DECLARE_EXCEPTION(KT_API, LogicException,       Exception)
KT_DECLARE_EXCEPTION(KT_API, NullPointerException, LogicException)

KT_DECLARE_EXCEPTION(KT_API, RuntimeException,     Exception)
KT_DECLARE_EXCEPTION(KT_API, TimeoutException,     RuntimeException)
KT_DECLARE_EXCEPTION(KT_API, SystemException,      RuntimeException)

NS_KT_END

#endif//KT_EXCEPTION_INCLUDE_H
