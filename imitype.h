#ifndef IMTYPE_H
#define IMTYPE_H

#include <dlfcn.h>
#include <string>

namespace _imi_ {

    namespace {
        template<char...> struct _VAR_OBSCURITY_ { using type = void*; };
        template<char... CHAR> inline _VAR_OBSCURITY_<CHAR...> _VAR_EXPLICITLY_;
    }

    struct _data_ {

        uintptr_t base;

        const char* last_symbol;

    } cache;

    static void* g_handle;

    //prevent signal 11
    inline void escape(...){
        std::string error = "Failed to invoke function. Incorrect symbol : " + std::string(_imi_::cache.last_symbol);

        //TODO : Please set your function if you want print error message
    }
}

namespace imitype {
    void initialize(void* handle){
        _imi_::g_handle = handle;
    }

    void initialize(uintptr_t target_base){
        _imi_::cache.base = target_base;
    }

    template<typename NCHAR, NCHAR...CHAR>
    constexpr _imi_::_VAR_OBSCURITY_<CHAR...> const& operator"" _(){ return _imi_::_VAR_EXPLICITLY_<CHAR...>; };

    template<typename RET, auto const& SIGNATURE = ""_>
    class IMTYPE {
    private:
        void* addr = 0x0;
        void* thiz = 0x0;
    public:

        IMTYPE(void* instance, uintptr_t address) : addr(reinterpret_cast<void*>(address + _imi_::cache.base)), thiz(instance){ }

        IMTYPE(void* instance, const char* symbol) :  thiz(instance) {
            _imi_::cache.last_symbol = symbol;
            if (void* addr_fn = dlsym(_imi_::g_handle, symbol)){
                addr = addr_fn;
            } else {
                addr = reinterpret_cast<void*>(&_imi_::escape);
            }
        };

        template<typename FUNC>
        IMTYPE(void* instance, FUNC target){
            thiz = instance;
            if (target != nullptr){
                addr = reinterpret_cast<void*>(target);
            } else {
                addr = reinterpret_cast<void*>(&_imi_::escape);
            }
        }

        template<typename...Params>
        RET operator()(Params...params){
            using func_t = RET (*)(void*, Params...);
            func_t fn = (func_t) addr;
            return fn(thiz, params...);
        }
    };
}

#endif //IMTYPE_H
