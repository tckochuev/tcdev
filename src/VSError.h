#ifndef VS_ERROR
#define VS_ERROR

#include <cstddef>
#include <exception>

#include "VSUtils.h"

#include <boost/system/system_error.hpp>

namespace tc::err
{

namespace tag
{

struct Value {};
inline constexpr Value value{};

struct Error {};
inline constexpr Error error{};
}

///@brief Error monad. Holds value(V) or error(E) at any moment of time. Does not throw exceptions.
/// Api resembles one from std::excepted, but implementation and complexity may differ.
template<typename V, typename E>
class Result
{
public:
	using Value = V;
	using Error = E;

	template<typename... Arg>
	Result(tag::Value, Arg&&... arg);

	template<typename... Arg>
	Result(tag::Error, Arg&&... arg);

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
	Result& emplaceValue(Arg&&... arg);

	///@brief assigns error constructed from arguments.
	template<typename... Arg>
	Result& emplaceError(Arg&&... arg);

	bool isError() const;
	explicit operator bool() const;

	/// @pre holds value.
	decltype(auto) value() const&;
	decltype(auto) value() const&&;
	decltype(auto) value()&;
	decltype(auto) value()&&;

	/// @brief Convenience function, invokes value().
	/// @pre holds value.
	decltype(auto) operator*() const&;
	decltype(auto) operator*() const&&;
	decltype(auto) operator*()&;
	decltype(auto) operator*()&&;
	const Value* operator->() const;
	Value* operator->();

	/// @pre holds error.
	const Error& error() const&;
	const Error&& error() const&&;
	Error& error()&;
	Error&& error()&&;

	/// @returns result(R) of std::invoke(f, value) if holds value, or R(fromError, error).
	template<typename F>
	auto andThen(F&& f) const&;
	template<typename F>
	auto andThen(F&& f) const&&;
	template<typename F>
	auto andThen(F&& f)&;
	template<typename F>
	auto andThen(F&& f)&&;

	/// @returns result(R) of std::invoke(f, error) if holds error, or R(fromValue, value)
	template<typename F>
	auto orElse(F&& f) const&;
	template<typename F>
	auto orElse(F&& f) const&&;
	template<typename F>
	auto orElse(F&& f)&;
	template<typename F>
	auto orElse(F&& f)&&;

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

///@brief invokes callable with args and forwards result to handler. Mainly used to handle result with monadic error handling.
template<typename Handler, typename Callable, typename... Args>
decltype(auto) invokeAndHandleResult(Handler&& handler, Callable&& callable, Args&&... args);

/// @brief It is a common situation, when function returns some object or indicator of its absense because of some error.
/// This handler helps to handle such situations with monadic error handling.
struct ObjectObtainHandler
{
	///@returns Result with object, if object is valid, otherwise - Result with error.
	template<typename Object, typename Predicate, typename Error>
	auto operator()(Object&& object, Predicate&& isValid, Error&& error) const;
};

namespace sys
{

using namespace boost::system;

template<typename V>
using Result = tc::err::Result<V, error_code>;

}

namespace exc
{

template<typename V>
using Result = tc::err::Result<V, std::exception_ptr>;

///@brief Monadic allocation error handling.
struct AllocationHandler
{
	///@returns Result with object if object != nullptr, otherwise - Result with std::bad_alloc;
	template<typename Object>
	auto operator()(Object&& object) const;
};

/// @brief Used when operation is not possible in object's current state.
class InvalidState : public std::logic_error
{
	using std::logic_error::logic_error;
};

/// @brief Used when operation can not be performed with argument of some type.
class InvalidArgumentType : public std::invalid_argument
{
	using std::invalid_argument::invalid_argument;
};

/// @brief Used when some operation does not have implementation.
class NotSupported : public std::logic_error
{
	using std::logic_error::logic_error;
};

/// @returns pointer to underlying exception of type T or nullptr if exception can not be casted to T.
/// @pre T is not reference, exception is not empty.
template<typename T>
T* cast(std::exception_ptr exception);

} //namespace tc::err::exc

} //namespace tc::err

#include "VSError.ipp"

#endif
