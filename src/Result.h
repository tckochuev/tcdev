#ifndef RESULT_H
#define RESULT_H

#include <cstddef>
#include <exception>

#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/mpl/back.hpp>

#include "Utils.h"

namespace tc::err
{

struct FromValue {};
inline constexpr FromValue fromValue{};
struct FromError {};
inline constexpr FromError fromError{};

template<typename V, typename E>
class Result
{
public:
    using Value = V;
    using Error = E;

    template<typename... Arg>
    Result(FromValue, Arg&&... arg);

    template<typename... Arg>
    Result(FromError, Arg&&... arg);

    Result(const Result& other);
    Result(Result&& other) noexcept;

    template<typename V2, typename E2>
    Result(const Result<V2, E2>& other);

    template<typename V2, typename E2>
    Result(Result<V2, E2>&& other);

    Result& operator=(const Result& other);
    Result& operator=(Result&& other) noexcept;

    template<typename V2, typename E2>
    Result& operator=(const Result<V2, E2>& other);

    template<typename V2, typename E2>
    Result& operator=(Result<V2, E2>&& other);

    /// @brief assigns value constructed from arguments.
    template<typename... Arg>
    Result& assignValue(Arg&&... arg);

    ///@brief assigns error constructed from arguments.
    template<typename... Arg>
    Result& assignError(Arg&&... arg);

    bool isError() const;

    /// @pre holds value.
    decltype(auto) value() const &;
    decltype(auto) value() const &&;
    decltype(auto) value() &;
    decltype(auto) value() &&;

    /// @brief Convenience function, invokes value().
    /// @pre holds value.
    decltype(auto) operator*() const&;
    decltype(auto) operator*() const&&;
    decltype(auto) operator*()&;
    decltype(auto) operator*()&&;

    /// @pre holds error.
    const Error& error() const &;
    const Error&& error() const &&;
    Error& error() &;
    Error&& error() &&;

    /// @returns result(R) of std::invoke(f, value) if holds value, or R(fromError, error).
    template<typename F>
    auto andThen(F&& f) const &;
    template<typename F>
    auto andThen(F&& f) const &&;
    template<typename F>
    auto andThen(F&& f) &;
    template<typename F>
    auto andThen(F&& f) &&;

    /// @returns result(R) of std::invoke(f, error) if holds error, or R(fromValue, value)
    template<typename F>
    auto orElse(F&& f) const &;
    template<typename F>
    auto orElse(F&& f) const &&;
    template<typename F>
    auto orElse(F&& f) &;
    template<typename F>
    auto orElse(F&& f) &&;

    ~Result();

private:
    template<typename Val, typename Err>
    struct Storage
    {
        static constexpr size_t alignment = std::max({alignof(Val), alignof(Err)});
        using Type = std::byte[std::max({sizeof(Val), sizeof(Err)})];
    };

    template<typename Err>
    struct Storage<void, Err>
    {
        static constexpr size_t alignment = alignof(Err);
        using Type = std::byte[sizeof(Err)];
    };

    template<typename Res>
    static decltype(auto) doValue(Res&& result);
    template<typename Res>
    static decltype(auto) doError(Res&& result);
    template<typename Res, typename F>
    static auto doAndThen(Res&& result, F&&);
    template<typename Res, typename F>
    static auto doOrElse(Res&& result, F&&);

    template<typename Res>
    Result& constructFromOther(Res&& other);
    template<typename Res>
    Result& assignFromOther(Res&& other);
    Result& destroy();

    alignas(Storage<V, E>::alignment) typename Storage<V, E>::Type m_data;
    bool m_isError;
};

template<typename V, typename E, typename V2, typename E2>
bool operator==(const Result<V, E>& result1, const Result<V2, E2>& result2);

template<typename T>
inline constexpr bool isResult = false;

template<typename V, typename E>
inline constexpr bool isResult<Result<V, E>> = true;

namespace syserr
{

using namespace boost::system;

template<typename V>
using Result = tc::err::Result<V, error_code>;

}

namespace exc
{

template<typename V>
using Result = tc::err::Result<V, std::exception_ptr>;

template<typename V>
struct ResultHandler
{
    ResultHandler(const Result<V>& result, bool handled = false)
        : result(result), handled(handled)
    {}

    template<typename H>
    ResultHandler& operator()(H&& handler)
    {
        using Decayed = std::decay_t<H>;
        using FunctionPointer = std::conditional_t<std::is_pointer_v<Decayed>, Decayed, decltype(&Decayed::operator())>;
        static_assert(boost::function_types::is_callable_builtin<FunctionPointer>::value && "handler must be callable");
        using ParamTypes = boost::function_types::parameter_types<FunctionPointer>;
        static_assert(boost::mpl::is_sequence<ParamTypes>::value && "unable to determine types of function parameters");
        using Exception = typename boost::mpl::back<ParamTypes>::type;

        if (!handled)
        {
            try {
                std::rethrow_exception(result.get().error());
            }
            catch (Exception e) {
                handled = true;
                std::invoke(std::forward<H>(handler), e);
            }
            catch (...) {}
        }
        return *this;
    }

    std::reference_wrapper<const Result<V>> result;
    bool handled;
};

}

}

#include "Result.ipp"

#endif
