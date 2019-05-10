#ifndef TREAD_POOL_FUNCTION_WRAPPER_H
#define TREAD_POOL_FUNCTION_WRAPPER_H


#include <memory>

class function_wrapper
    {
private:
    struct impl_base
        {
        virtual void call () = 0;

        virtual ~impl_base () = default;
        };

    std::unique_ptr<impl_base> impl;

    template<typename F>
    struct impl_type : impl_base
        {
        F f;

        explicit impl_type (F &&f_) : f(std::move(f_)) { }

        void call () override { f(); }
        };

public:
    template<typename F>
    function_wrapper (F &&f):
            impl(new impl_type<F>(std::forward<F>(f))) { }

    void operator() () { impl->call(); }

    function_wrapper () = default;

    function_wrapper (function_wrapper &&other) noexcept :
            impl(std::move(other.impl)) { }

    function_wrapper &operator= (function_wrapper &&other) noexcept
    {
        impl = std::move(other.impl);
        return *this;
    }

    function_wrapper (const function_wrapper &) = delete;

    function_wrapper (function_wrapper &) = delete;

    function_wrapper &operator= (const function_wrapper &) = delete;
    };

#endif //TREAD_POOL_FUNCTION_WRAPPER_H
